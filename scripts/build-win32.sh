#!/usr/bin/env bash
#
# Cross-build the 32-bit Windows plugin with MinGW-w64.
#
# Run scripts/fetch-win32-deps.sh first, or provide the three path variables:
#   OPENVK_PIDGIN_WIN  directory containing libpurple.dll
#   OPENVK_GTK         GTK+ 2.24 bundle root (contains include/ and lib/)
#   OPENVK_PIDGIN_SRC  Pidgin 2.14.14 source tree
#
# The resulting openvk.dll is written to build-win32/ by default.

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DEPS="${OPENVK_WIN_DEPS:-$ROOT/.deps/win32}"
PREFIX="$DEPS/prefix"

PIDGIN_WIN="${OPENVK_PIDGIN_WIN:-$PREFIX/pidgin/pidgin-2.14.14-win32bin}"
GTK="${OPENVK_GTK:-$PREFIX/gtk}"
PIDGIN_SRC="${OPENVK_PIDGIN_SRC:-$PREFIX/pidgin-src/pidgin-2.14.14}"
OUT="${OPENVK_WIN_BUILD:-$ROOT/build-win32}"

CC="${CC_WIN32:-i686-w64-mingw32-gcc}"
DLLTOOL="${DLLTOOL_WIN32:-i686-w64-mingw32-dlltool}"

for command in "$CC" "$DLLTOOL" python3 objdump file; do
	if ! command -v "$command" >/dev/null 2>&1; then
		echo "error: required command not found: $command" >&2
		exit 1
	fi
done

if [[ ! -f "$PIDGIN_WIN/libpurple.dll" ]]; then
	echo "error: libpurple.dll not found in $PIDGIN_WIN" >&2
	echo "run scripts/fetch-win32-deps.sh or set OPENVK_PIDGIN_WIN" >&2
	exit 1
fi

if [[ ! -f "$GTK/include/glib-2.0/glib.h" ]]; then
	echo "error: GLib headers not found in $GTK" >&2
	echo "run scripts/fetch-win32-deps.sh or set OPENVK_GTK" >&2
	exit 1
fi

if [[ ! -f "$PIDGIN_SRC/libpurple/prpl.h" ]]; then
	echo "error: libpurple headers not found in $PIDGIN_SRC" >&2
	echo "run scripts/fetch-win32-deps.sh or set OPENVK_PIDGIN_SRC" >&2
	exit 1
fi

mkdir -p "$OUT/import"

echo "generating libpurple import library"
python3 "$ROOT/scripts/gen-def.py" "$PIDGIN_WIN/libpurple.dll" "$OUT/libpurple.def"
"$DLLTOOL" -d "$OUT/libpurple.def" -D libpurple.dll \
	-l "$OUT/import/libpurple.dll.a"

mapfile -t SOURCES < <(find "$ROOT/src" -maxdepth 1 -name '*.c' -print | sort)
if [[ ${#SOURCES[@]} -eq 0 ]]; then
	echo "error: no sources found in $ROOT/src" >&2
	exit 1
fi

CFLAGS=(
	-O2
	-g
	-std=gnu11
	-Wall
	-Wextra
	-Wno-unused-parameter
	-DPURPLE_PLUGINS
	-D_USE_32BIT_TIME_T
	-D_WIN32_WINNT=0x0601
	-DWIN32_LEAN_AND_MEAN
	-I"$ROOT/src"
	-I"$PIDGIN_SRC"
	-I"$PIDGIN_SRC/libpurple"
	-I"$GTK/include/glib-2.0"
	-I"$GTK/lib/glib-2.0/include"
)

# Arch's mingw-w64 defaults to the UCRT import library.  Pidgin 2.14.14 and
# Windows 7 use msvcrt.dll, so select the matching MinGW CRT explicitly.
LDFLAGS=(
	-shared
	-static-libgcc
	-mcrtdll=msvcrt-os
	-L"$OUT/import"
	-L"$GTK/lib"
)

echo "compiling ${#SOURCES[@]} source files"
"$CC" "${CFLAGS[@]}" "${LDFLAGS[@]}" \
	-o "$OUT/openvk.dll" \
	"${SOURCES[@]}" \
	-lpurple -lglib-2.0 -lgobject-2.0 -lgmodule-2.0

echo
file "$OUT/openvk.dll"
echo
echo "imported DLLs:"
LC_ALL=C objdump -p "$OUT/openvk.dll" | sed -n 's/^[[:space:]]*DLL Name: //p' | sort -u

BAD_IMPORTS="$(
	LC_ALL=C objdump -p "$OUT/openvk.dll" |
		sed -n 's/^[[:space:]]*DLL Name: //p' |
		grep -Ei 'soup|json-glib|libgcc|ucrt|api-ms-win-crt' || true
)"
if [[ -n "$BAD_IMPORTS" ]]; then
	echo "error: unexpected runtime dependency:" >&2
	echo "$BAD_IMPORTS" >&2
	exit 1
fi

if [[ -n "${OPENVK_WIN7SHARE:-}" ]]; then
	mkdir -p "$OPENVK_WIN7SHARE"
	cp "$OUT/openvk.dll" "$OPENVK_WIN7SHARE/openvk.dll"
	echo "copied to $OPENVK_WIN7SHARE/openvk.dll"
fi

echo
echo "built: $OUT/openvk.dll"
