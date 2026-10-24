# openvk-purple

Native OpenVK protocol plugin for Pidgin 2 / libpurple 2.

The plugin is intended to expose OpenVK personal dialogs as normal Pidgin IM
conversations and OpenVK group chats as Pidgin multi-user chats. The first
release targets Pidgin 2.14.x and uses only libpurple, GLib, and a small
vendored JSON parser.

## Status

This repository is at **M0**: a loadable protocol skeleton.

What works now:

- the plugin builds and loads in Pidgin 2;
- the OpenVK protocol appears in the account type list;
- account options for the API base, authentication mode, API version, history,
  avatars, and debug logging are registered;
- the Linux `.so` and 32-bit Windows `.dll` builds are set up.

What does **not** work yet:

- login and authentication;
- API requests;
- personal messages and history;
- group chats;
- avatars and presence.

Trying to log in with the M0 build shows a clear "not implemented yet" error.
The real login state machine lands in M1.

## Target

- Pidgin 2.14.x / libpurple 2.14.x.
- Linux development on Arch.
- Windows 7 SP1 x64 with 32-bit Pidgin 2.14.14 for the final check.
- Pidgin 3 is not supported by this repository.

## Dependencies

- C11 compiler
- GLib 2.28-compatible API
- libpurple 2.14
- Meson and Ninja for the Linux build
- i686 MinGW-w64 for the Windows cross-build
- vendored cJSON (added with the JSON layer)

No libsoup3 or json-glib is required.

## Build on Linux

```sh
meson setup build
ninja -C build
```

The plugin is written to `build/openvk.so`.

Install it into the current Pidgin profile:

```sh
scripts/install-dev.sh
pidgin -d 2>&1 | grep -E 'openvk|OpenVK'
```

The plugin log category is `openvk`.

## Build for Windows 7 (32-bit)

The Pidgin 2.14.14 Windows build is 32-bit and bundles GLib 2.28.8. The plugin
must be built as a 32-bit DLL with the same `time_t` ABI (`_USE_32BIT_TIME_T`).

Fetch the build inputs:

```sh
scripts/fetch-win32-deps.sh
```

Build:

```sh
scripts/build-win32.sh
```

The result is `build-win32/openvk.dll`. The script checks that the DLL is PE32
i386 and that it does not import libsoup, json-glib, libgcc, or UCRT.

Copy it to the Windows VM through `win7share`, then place it in:

```
%APPDATA%\.purple\plugins\openvk.dll
```

Run Pidgin with debug output:

```bat
pidgin.exe -d
```

See `docs/windows-build.md` for the full explanation.

## Layout

```
src/          plugin, protocol callbacks, connection state
data/         protocol icon
scripts/      development install, Windows dependency fetch, Windows build
docs/         build and development notes
tests/        unit tests and mock server (added with the API layer)
```

## License

GPL-2.0-or-later. See `LICENSE`.

The OpenVK API is a VK-compatible API. This project is not affiliated with
OpenVK or the Pidgin project.
