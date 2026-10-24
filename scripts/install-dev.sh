#!/usr/bin/env bash
#
# Install the development build into the current user's Pidgin profile.
#
# Pidgin 2 loads protocol plugins from ~/.purple/plugins.  It loads protocol
# icons from the compiled-in DATADIR (usually /usr/share), not from ~/.purple.
# If the system icon directory is not writable, this script prints the
# commands needed to install the icon with doas.

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
BUILD="${OPENVK_BUILD_DIR:-$ROOT/build}"
PLUGIN_DIR="${PURPLE_PLUGIN_DIR:-$HOME/.purple/plugins}"

if [[ ! -f "$BUILD/openvk.so" ]]; then
	echo "error: $BUILD/openvk.so not found; run ninja -C build first" >&2
	exit 1
fi

mkdir -p "$PLUGIN_DIR"
cp "$BUILD/openvk.so" "$PLUGIN_DIR/openvk.so"

PIDGIN_DATADIR="$(pkg-config --variable=datadir pidgin 2>/dev/null || echo /usr/share)"
SYSTEM_ICON_DIR="${OPENVK_ICON_DIR:-$PIDGIN_DATADIR/pixmaps/pidgin/protocols}"
USER_ICON_DIR="$HOME/.purple/pixmaps/pidgin/protocols"

# Keep a copy in the profile too.  This is harmless, and some third-party
# themes/plugins look there.
for size in 16 22 48; do
	mkdir -p "$USER_ICON_DIR/$size"
	cp "$ROOT/data/openvk.png" "$USER_ICON_DIR/$size/openvk.png"
done

if [[ -w "$SYSTEM_ICON_DIR" ]]; then
	for size in 16 22 48; do
		mkdir -p "$SYSTEM_ICON_DIR/$size"
		cp "$ROOT/data/openvk.png" "$SYSTEM_ICON_DIR/$size/openvk.png"
	done
	echo "installed system protocol icons to $SYSTEM_ICON_DIR"
else
	echo
	echo "note: Pidgin 2 reads protocol icons from:"
	echo "      $SYSTEM_ICON_DIR"
	echo "      That directory is not writable. To install the icon, run:"
	for size in 16 22 48; do
		echo "      doas install -Dm644 '$ROOT/data/openvk.png' '$SYSTEM_ICON_DIR/$size/openvk.png'"
	done
fi

cat <<EOF

installed plugin:
  $PLUGIN_DIR/openvk.so

Start Pidgin with debug output:
  pidgin -d 2>&1 | grep -E 'openvk|OpenVK'
EOF
