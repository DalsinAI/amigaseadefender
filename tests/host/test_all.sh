#!/bin/sh
# Host tests of the Sea Defender port. Needs a C compiler and the GL
# headers; for the 68k half, m68k-linux-gnu-gcc and qemu-m68k (skipped, and
# said so, without them).
#   sh tests/host/test_all.sh [WORKDIR]
# 1. amiga/sd_glutess.c (GLU's tessellator on libtess2, at the lock's
#    commit): its checks on the PC and on 68k under qemu-m68k -cpu m68040;
#    the two must print the same.
# 2. tests/host/test_*.py.
# MIT, Copyright (c) 2026 AmiCradle Team.
set -eu
HERE=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
ROOT=$(CDPATH= cd -- "$HERE/../.." && pwd)
. "$ROOT/package/seadefender.lock"
W=${1:-"${TMPDIR:-/tmp}/amigaseadefender-test"}
mkdir -p "$W"
T="$W/libtess2"
if [ "$(git -C "$T" rev-parse HEAD 2>/dev/null)" != "$LIBTESS2_COMMIT" ]; then
    rm -rf "$T"; mkdir -p "$T"; git -C "$T" init -q
    git -C "$T" fetch -q --depth 1 "$LIBTESS2_URL" "$LIBTESS2_COMMIT"
    git -C "$T" checkout -q FETCH_HEAD
fi
SRC="$HERE/test_sd_glutess.c $ROOT/amiga/sd_glutess.c $T/Source/*.c"
INC="-I$ROOT/amiga/include -I$T/Include"

# the GL/gl.h of the host; amiga/include's glu.h before any host GLU
cc -O2 -Wall $INC -o "$W/test_glutess-pc" $SRC -lm
"$W/test_glutess-pc" > "$W/glutess-pc.txt" || { cat "$W/glutess-pc.txt"; exit 1; }
cat "$W/glutess-pc.txt"
if command -v m68k-linux-gnu-gcc >/dev/null && command -v qemu-m68k >/dev/null; then
    # the host's GL headers, without the rest of /usr/include
    mkdir -p "$W/glinc"; cp -R /usr/include/GL "$W/glinc/"
    [ -d /usr/include/KHR ] && cp -R /usr/include/KHR "$W/glinc/"
    m68k-linux-gnu-gcc -m68040 -O2 -static $INC -I"$W/glinc" -o "$W/test_glutess-68k" $SRC -lm
    qemu-m68k -cpu m68040 "$W/test_glutess-68k" > "$W/glutess-68k.txt" || { cat "$W/glutess-68k.txt"; exit 1; }
    if diff -u "$W/glutess-pc.txt" "$W/glutess-68k.txt"; then
        echo "PASS tessellator: PC and 68k the same"
    else
        echo "FAIL tessellator: PC and 68k differ"; exit 1
    fi
else
    echo "SKIP tessellator on 68k: no m68k-linux-gnu-gcc or qemu-m68k"
fi

for t in "$HERE"/test_*.py; do
    python3 "$t"
done
echo "ALL PASS"
