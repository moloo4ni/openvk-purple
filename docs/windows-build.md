# Windows 32-bit build

Pidgin 2.14.14 for Windows is a 32-bit PE32 application, even on 64-bit
Windows. Its offline installer bundles GTK+ 2.24.10 with GLib 2.28.8. That
bundle does not contain libsoup3 or json-glib, so the plugin deliberately uses
libpurple's built-in HTTP client and a vendored copy of cJSON instead.

The Windows build is cross-compiled from Linux with MinGW-w64.

## Why `_USE_32BIT_TIME_T` is required

`libpurple.dll` in the Pidgin 2.14.14 Windows build imports the 32-bit
`localtime` and `mktime` functions from `msvcrt.dll`. The plugin passes
`time_t` values to libpurple callbacks such as `serv_got_im`. If the plugin
were built with 64-bit `time_t`, the stack/ABI layout would not match and
Pidgin could crash. Always compile the Windows plugin with:

```
-D_USE_32BIT_TIME_T
```

## Inputs

`scripts/fetch-win32-deps.sh` downloads and extracts:

- `pidgin-2.14.14-win32-bin.zip` — `libpurple.dll` and Pidgin binaries;
- `gtk+-bundle_2.24.10-20120208_win32.zip` — GLib 2.28.8 headers and import
  libraries;
- `pidgin-2.14.14.tar.bz2` — libpurple headers.

They are stored in `.deps/win32/` by default. Override the location with
`OPENVK_WIN_DEPS`.

## Build

```sh
scripts/fetch-win32-deps.sh
scripts/build-win32.sh
```

The build script:

1. generates `libpurple.def` from the export table of `libpurple.dll`;
2. creates `libpurple.dll.a` with `i686-w64-mingw32-dlltool`;
3. compiles the plugin with `-D_USE_32BIT_TIME_T -D_WIN32_WINNT=0x0601`;
4. links with `-static-libgcc -mcrtdll=msvcrt-os` (Arch's MinGW defaults to
   UCRT, while Pidgin 2.14.14 and Windows 7 use `msvcrt.dll`);
5. verifies the PE architecture and runtime imports.

The output is `build-win32/openvk.dll`.

## Install in the VM

1. Install Pidgin 2.14.14 offline in the Windows 7 VM.
2. Copy `openvk.dll` through `win7share`.
3. Put it in `%APPDATA%\.purple\plugins\openvk.dll`.
4. Start Pidgin from a command prompt:

   ```bat
   pidgin.exe -d
   ```

5. Check that the OpenVK protocol appears in the account type list and that
   the debug log contains the `openvk` lines.

The plugin icon is copied to the Pidgin profile by `scripts/install-dev.sh`
on Linux. On Windows, copy `data/openvk.png` to the matching
`pixmaps\pidgin\protocols\<size>\openvk.png` directories under
`%APPDATA%\.purple` if the icon is missing.

## Troubleshooting

- **"The specified module could not be found"** — the DLL was linked against a
  runtime DLL that is not present. Check the import table with
  `objdump -p openvk.dll` and compare it with Pidgin's own DLLs.
- **Pidgin crashes on login** — verify `_USE_32BIT_TIME_T` and that the plugin
  is 32-bit (`file openvk.dll`).
- **Plugin is not listed** — check that the file is in the `plugins`
  directory, has a `.dll` extension, and that Pidgin was restarted.
- **No `openvk` debug lines** — start Pidgin with `-d` from a command prompt;
  the shortcut may not pass arguments.
