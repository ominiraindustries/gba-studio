# Source from Git Bash before building: `source tools/env.sh`
# Sets the devkitARM environment Butano expects (see tools/README.md).
GBA_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
export GBA_ROOT
export DEVKITPRO="$GBA_ROOT/toolchain/devkitPro"
export DEVKITARM="$DEVKITPRO/devkitARM"
export BUTANO="$GBA_ROOT/engine/butano/butano"
case ":$PATH:" in
  *":$DEVKITPRO/tools/bin:"*) ;;
  *) export PATH="$GBA_ROOT/toolchain/msys-make/bin:$DEVKITPRO/tools/bin:$DEVKITARM/bin:$PATH" ;;
esac
# devkitARM's rules expect an MSYS make (devkitPro ships one): its recipes hand MSYS paths such as
# the /tmp file of `bin2o` to native gcc, which only works when MSYS translates the arguments.
# toolchain/msys-make holds MSYS2's make.exe; it runs on Git for Windows' msys-2.0.dll.
