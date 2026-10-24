#!/usr/bin/env bash
#
# Download the 32-bit Windows build inputs for the OpenVK plugin.
#
# The files are kept outside the repository in .deps/win32 by default.
# Set OPENVK_WIN_DEPS to use a different directory.

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
DEPS="${OPENVK_WIN_DEPS:-$ROOT/.deps/win32}"
DOWNLOADS="$DEPS/downloads"
PREFIX="$DEPS/prefix"

PIDGIN_VERSION="2.14.14"
GTK_BUNDLE="gtk+-bundle_2.24.10-20120208_win32.zip"

PIDGIN_ZIP="$DOWNLOADS/pidgin-$PIDGIN_VERSION-win32-bin.zip"
GTK_ZIP="$DOWNLOADS/$GTK_BUNDLE"
PIDGIN_TAR="$DOWNLOADS/pidgin-$PIDGIN_VERSION.tar.bz2"

PIDGIN_ZIP_URL="https://sourceforge.net/projects/pidgin/files/Pidgin/$PIDGIN_VERSION/pidgin-$PIDGIN_VERSION-win32-bin.zip/download"
GTK_ZIP_URL="https://download.gnome.org/binaries/win32/gtk+/2.24/$GTK_BUNDLE"
PIDGIN_TAR_URL="https://sourceforge.net/projects/pidgin/files/Pidgin/$PIDGIN_VERSION/pidgin-$PIDGIN_VERSION.tar.bz2/download"

download() {
	local url="$1"
	local output="$2"

	if [[ -s "$output" ]]; then
		echo "already present: $output"
		return
	fi

	echo "downloading: $url"
	curl --fail --location --retry 3 --output "$output.part" "$url"
	mv "$output.part" "$output"
}

mkdir -p "$DOWNLOADS" "$PREFIX"

download "$PIDGIN_ZIP_URL" "$PIDGIN_ZIP"
download "$GTK_ZIP_URL" "$GTK_ZIP"
download "$PIDGIN_TAR_URL" "$PIDGIN_TAR"

if [[ ! -d "$PREFIX/pidgin/pidgin-$PIDGIN_VERSION-win32bin" ]]; then
	echo "extracting Pidgin Windows binaries"
	mkdir -p "$PREFIX/pidgin"
	7z x -y -o"$PREFIX/pidgin" "$PIDGIN_ZIP" >/dev/null
fi

if [[ ! -d "$PREFIX/gtk/include/glib-2.0" ]]; then
	echo "extracting GTK+ 2.24 bundle"
	mkdir -p "$PREFIX/gtk"
	7z x -y -o"$PREFIX/gtk" "$GTK_ZIP" >/dev/null
fi

if [[ ! -d "$PREFIX/pidgin-src/pidgin-$PIDGIN_VERSION" ]]; then
	echo "extracting Pidgin source headers"
	mkdir -p "$PREFIX/pidgin-src"
	tar -xjf "$PIDGIN_TAR" -C "$PREFIX/pidgin-src"
fi

cat <<EOF
Windows dependencies are ready:

  Pidgin binaries: $PREFIX/pidgin/pidgin-$PIDGIN_VERSION-win32bin
  GTK+ bundle:     $PREFIX/gtk
  Pidgin source:   $PREFIX/pidgin-src/pidgin-$PIDGIN_VERSION

Next: scripts/build-win32.sh
EOF
