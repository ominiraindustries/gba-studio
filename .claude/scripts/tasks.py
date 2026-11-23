"""Small task tracker for the GBA workspace: one JSONL file in git, verifiable states, safe notes.

Why not bd/beads: in the SNES workspace bd needed a wrapper (PowerShell shim blocked, `--notes` erased notes), its
Dolt database lives outside git and `bd init` writes AGENTS.md and hooks. Here one versioned JSONL file is enough.

Data: docs/datos/tareas.jsonl in the MAIN checkout (also when run from a worktree). One JSON object per line.
States: open -> confirmed -> in_progress -> resolved -> closed (see docs/harness/flujo.md).

Usage (always python -I):
  tasks.py add "Title" [-p 0-4] [-l label ...] [--acceptance TEXT] [--parent ID] [--desc TEXT]
  tasks.py ready                      # open/confirmed, unblocked, not needs:user, by priority
  tasks.py list [--status S] [-l label] [--all]
  tasks.py show ID
  tasks.py update ID [--status S] [--claim --actor NAME] [--append-notes TEXT] [--notes TEXT] [--replace-notes TEXT]
                     [-l label ...] [--unlabel label ...] [--acceptance TEXT]
  tasks.py dep ID BLOCKER             # ID waits for BLOCKER
  tasks.py close ID --reason "Verification (YYYY-MM-DD): command: result"  [--discard]
`--notes` never erases: if the task already has notes it is turned into --append-notes with a warning;
overwrite on purpose with --replace-notes.
"""
import argparse, datetime, json, os, pathlib, subprocess, sys, time

STATES = ["open", "confirmed", "in_progress", "resolved", "closed"]


def main_root():
    here = pathlib.Path(__file__).resolve().parent
    p = subprocess.run(["git", "rev-parse", "--path-format=absolute", "--git-common-dir"], cwd=here,
                       capture_output=True, text=True)
    if p.returncode:
        sys.exit("tasks.py: not inside a git repository")
    return pathlib.Path(p.stdout.strip()).resolve().parent


DATA = pathlib.Path(os.environ.get("GBA_TASKS_FILE") or main_root() / "docs" / "datos" / "tareas.jsonl")
LOCK = DATA.with_suffix(".lock")


def now():
    return datetime.datetime.now().isoformat(timespec="seconds")


class Locked:
    def __enter__(self):
        DATA.parent.mkdir(parents=True, exist_ok=True)
        for _ in range(100):
            try:
                self.fd = os.open(LOCK, os.O_CREAT | os.O_EXCL | os.O_WRONLY)
                return self
            except FileExistsError:
                if time.time() - LOCK.stat().st_mtime > 60:      # stale lock from a killed process
                    LOCK.unlink(missing_ok=True)
                time.sleep(0.1)
        sys.exit(f"tasks.py: {LOCK} is held; retry")

    def __exit__(self, *exc):
        os.close(self.fd)
        LOCK.unlink(missing_ok=True)


def load():
    if not DATA.exists():
        return []
    return [json.loads(l) for l in DATA.read_text(encoding="utf-8").splitlines() if l.strip()]


def save(tasks):
    tmp = DATA.with_suffix(".tmp")
    tmp.write_text("".join(json.dumps(t, ensure_ascii=False) + "\n" for t in tasks), encoding="utf-8")
    os.replace(tmp, DATA)


def find(tasks, tid):
    for t in tasks:
        if t["id"] == tid:
            return t
    sys.exit(f"tasks.py: no task {tid}")


def new_id(tasks, parent):
    if parent:
        kids = [t["id"] for t in tasks if t["id"].startswith(parent + ".")]
        return f"{parent}.{1 + max([int(k.rsplit('.', 1)[1]) for k in kids] or [0])}"
    tops = [int(t["id"][2:]) for t in tasks if "." not in t["id"]]
    return f"G-{1 + max(tops or [0])}"


def note(t, text):
    t["notes"] = (t.get("notes", "") + "\n" if t.get("notes") else "") + f"[{now()[:10]}] {text}"


def blocked(t, by_id):
    return [b for b in t.get("deps", []) if by_id.get(b, {}).get("status") != "closed"]


def line(t):
    lab = f" [{', '.join(t['labels'])}]" if t.get("labels") else ""
    who = f" @{t['assignee']}" if t.get("assignee") else ""
    return f"{t['id']:<10} P{t['priority']} {t['status']:<11} {t['title']}{lab}{who}"


def cmd_add(a):
    with Locked():
        tasks = load()
        if a.parent:
            find(tasks, a.parent)
        t = {"id": new_id(tasks, a.parent), "title": a.title, "status": "open", "priority": a.p,
             "labels": a.l or [], "created": now(), "updated": now()}
        for k in ("desc", "acceptance", "parent"):
            if getattr(a, k):
                t[k] = getattr(a, k)
        tasks.append(t)
        save(tasks)
    print(f"created {t['id']}")


def cmd_ready(a):
    tasks = load()
    by_id = {t["id"]: t for t in tasks}
    rdy = [t for t in tasks if t["status"] in ("open", "confirmed") and not blocked(t, by_id)
           and "needs:user" not in t.get("labels", [])]
    for t in sorted(rdy, key=lambda t: (t["priority"], t["created"])):
        print(line(t))


def cmd_list(a):
    for t in load():
        if a.status and t["status"] != a.status:
            continue
        if not a.status and not a.all and t["status"] == "closed":
            continue
        if a.l and a.l not in t.get("labels", []):
            continue
        print(line(t))


def cmd_show(a):
    tasks = load()
    t = find(tasks, a.id)
    by_id = {x["id"]: x for x in tasks}
    print(line(t))
    for k in ("desc", "acceptance", "parent", "deps", "created", "updated", "close_reason"):
        if t.get(k):
            print(f"  {k}: {t[k]}")
    b = blocked(t, by_id)
    if b:
        print(f"  blocked by: {', '.join(b)}")
    if t.get("notes"):
        print("  notes:\n    " + t["notes"].replace("\n", "\n    "))
    kids = [x for x in tasks if x.get("parent") == t["id"]]
    for k in kids:
        print("  child " + line(k))


def cmd_update(a):
    with Locked():
        tasks = load()
        t = find(tasks, a.id)
        if a.claim:
            if t["status"] == "closed":
                sys.exit(f"{a.id} is closed")
            if t.get("assignee") and t["assignee"] != a.actor and t["status"] == "in_progress":
                sys.exit(f"{a.id} already claimed by {t['assignee']}")
            t["assignee"], t["status"] = a.actor, "in_progress"
        if a.status:
            if a.status == "closed":
                sys.exit("use: tasks.py close ID --reason ...")
            t["status"] = a.status
        if a.replace_notes is not None:
            t["notes"] = f"[{now()[:10]}] {a.replace_notes}"
        if a.notes is not None:
            if t.get("notes"):
                print("warning: --notes would erase existing notes; appended instead (--replace-notes overwrites)",
                      file=sys.stderr)
            note(t, a.notes)
        if a.append_notes:
            note(t, a.append_notes)
        for lab in a.l or []:
            if lab not in t.setdefault("labels", []):
                t["labels"].append(lab)
        for lab in a.unlabel or []:
            if lab in t.get("labels", []):
                t["labels"].remove(lab)
        if a.acceptance:
            t["acceptance"] = a.acceptance
        t["updated"] = now()
        save(tasks)
    print(line(t))


def cmd_dep(a):
    with Locked():
        tasks = load()
        t = find(tasks, a.id)
        find(tasks, a.blocker)
        if a.blocker not in t.setdefault("deps", []):
            t["deps"].append(a.blocker)
        t["updated"] = now()
        save(tasks)
    print(f"{a.id} waits for {a.blocker}")


def cmd_close(a):
    if len(a.reason.strip()) < 15:
        sys.exit("close needs a real reason: 'Verification (date): command: result' or why it is discarded")
    with Locked():
        tasks = load()
        t = find(tasks, a.id)
        t["status"], t["close_reason"], t["updated"] = "closed", a.reason, now()
        if a.discard and "outcome:discarded" not in t.setdefault("labels", []):
            t["labels"].append("outcome:discarded")
        save(tasks)
    print(line(t))


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("add"); p.add_argument("title"); p.add_argument("-p", type=int, default=2, choices=range(5))
    p.add_argument("-l", action="append"); p.add_argument("--acceptance"); p.add_argument("--parent")
    p.add_argument("--desc"); p.set_defaults(f=cmd_add)
    p = sub.add_parser("ready"); p.set_defaults(f=cmd_ready)
    p = sub.add_parser("list"); p.add_argument("--status", choices=STATES); p.add_argument("-l")
    p.add_argument("--all", action="store_true"); p.set_defaults(f=cmd_list)
    p = sub.add_parser("show"); p.add_argument("id"); p.set_defaults(f=cmd_show)
    p = sub.add_parser("update"); p.add_argument("id"); p.add_argument("--status", choices=STATES)
    p.add_argument("--claim", action="store_true"); p.add_argument("--actor", default=os.environ.get("GBA_AGENT", "coord"))
    p.add_argument("--notes"); p.add_argument("--append-notes"); p.add_argument("--replace-notes")
    p.add_argument("-l", action="append"); p.add_argument("--unlabel", action="append"); p.add_argument("--acceptance")
    p.set_defaults(f=cmd_update)
    p = sub.add_parser("dep"); p.add_argument("id"); p.add_argument("blocker"); p.set_defaults(f=cmd_dep)
    p = sub.add_parser("close"); p.add_argument("id"); p.add_argument("--reason", required=True)
    p.add_argument("--discard", action="store_true"); p.set_defaults(f=cmd_close)
    a = ap.parse_args()
    a.f(a)


if __name__ == "__main__":
    main()
