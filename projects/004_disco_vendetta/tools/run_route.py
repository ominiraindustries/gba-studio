"""Play a recorded route on 004_disco_vendetta headless (gbarun.py) and check the block 1 acceptance.

usage (from the repo root): python -I projects/004_disco_vendetta/tools/run_route.py OUT_DIR [ROUTE]
ROUTE lines are "<frames> <keys|->" (keys joined with '+', '-' = no keys, '#' comments); default route_floor1.txt.
Checks: floor clear (test_state 5) with 0 deaths, every enemy shot preceded by its "!" on screen, no frame over the
CPU budget. Prints the frame each step starts on, to recalibrate after tuning changes.
"""
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent.parent


def main():
    out = sys.argv[1]
    route = Path(sys.argv[2]) if len(sys.argv) > 2 else HERE / "route_floor1.txt"
    frame = 1
    parts = []
    for line in route.read_text().splitlines():
        line = line.split("#")[0].strip()
        if not line:
            continue
        count, keys = line.split()[:2]
        count = int(count)
        print(frame, line)
        if keys != "-":
            parts.append(f"{frame}-{frame + count - 1}:{keys}" if count > 1 else f"{frame}:{keys}")
        frame += count
    checks = "test_state.32==5,test_deaths.32==0,test_unwarned_shots.32==0,test_cpu_over.32==0," \
             "test_floor_frames.32<=5400"
    args = [sys.executable, "-I", str(ROOT / "tools/scripts/gbarun.py"),
            str(ROOT / "projects/004_disco_vendetta/004_disco_vendetta.gba"), "--frames", str(frame + 5),
            "--input", ";".join(parts), "--check", checks, "--nonblank", "--out", out, "--timeout", "300",
            "--shot", ",".join(str(f) for f in range(150, frame, 150))]
    sys.exit(subprocess.run(args).returncode)


if __name__ == "__main__":
    main()
