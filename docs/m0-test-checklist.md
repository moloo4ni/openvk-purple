# M0 test checklist

M0 is the loadable-protocol skeleton. It does not perform any network requests.

## Linux

```sh
meson setup build
ninja -C build
scripts/install-dev.sh
pidgin -d
```

Expected:

- the plugin log contains `openvk: plugin initialized (version 0.1.0)`;
- `Accounts -> Manage Accounts -> Add` lists **OpenVK**;
- selecting OpenVK and clicking Add shows:
  `OpenVK login is not implemented yet. This is the M0 skeleton plugin.`
- the protocol icon is visible. Pidgin 2 reads protocol icons from its compiled-in
  data directory; if the icon is missing, run the `doas install` commands printed by
  `scripts/install-dev.sh`.

## Windows 7

Inputs:

- `pidgin-2.14.14-offline.exe`;
- `openvk.dll` (32-bit, built by `scripts/build-win32.sh`);
- optional `openvk.png` and `install-openvk-test.cmd` from the test share.

Steps:

1. Install Pidgin 2.14.14.
2. Copy `openvk.dll` to `%APPDATA%\.purple\plugins\openvk.dll`.
3. Start Pidgin.
4. Open `Accounts -> Manage Accounts -> Add`.
5. Confirm that **OpenVK** appears in the protocol list.
6. Select OpenVK and click Add. For M0, any non-empty placeholder values are
   enough (for example `testuser` / `testpass`); no network request is made.
7. Confirm the expected M0 error appears.

Useful checks on the DLL:

```sh
file openvk.dll
# PE32 executable for MS Windows 4.00 (DLL), Intel i386

objdump -p openvk.dll | grep 'DLL Name'
# libglib-2.0-0.dll, libpurple.dll, KERNEL32.dll, msvcrt.dll

objdump -p openvk.dll | grep purple_init_plugin
```

The DLL must be 32-bit and must not import libsoup, json-glib, libgcc, UCRT, or
`api-ms-win-crt-*`.

## Reporting

A screenshot of the protocol list and the error dialog is enough. Do not paste
credentials or message contents into the chat or logs.
