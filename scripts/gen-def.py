#!/usr/bin/env python3
"""Generate a MinGW .def file from the export table of a PE DLL."""

from __future__ import annotations

import os
import re
import subprocess
import sys
from pathlib import Path

EXPORT_RE = re.compile(
    r"^\s*\[\s*\d+\]\s+\+base\[\s*\d+\]\s+[0-9a-fA-F]+\s+(\S+)\s*$"
)


def main() -> int:
    if len(sys.argv) != 3:
        print(f"usage: {Path(sys.argv[0]).name} DLL OUTPUT.def", file=sys.stderr)
        return 2

    dll = Path(sys.argv[1])
    output = Path(sys.argv[2])
    if not dll.is_file():
        print(f"error: DLL not found: {dll}", file=sys.stderr)
        return 1

    env = dict(os.environ, LC_ALL="C")
    try:
        text = subprocess.check_output(
            ["objdump", "-p", str(dll)],
            text=True,
            errors="replace",
            env=env,
        )
    except (OSError, subprocess.CalledProcessError) as exc:
        print(f"error: objdump failed: {exc}", file=sys.stderr)
        return 1

    marker = "[Ordinal/Name Pointer] Table"
    marker_pos = text.find(marker)
    if marker_pos < 0:
        print(f"error: export name table not found in {dll}", file=sys.stderr)
        return 1

    names: list[str] = []
    seen: set[str] = set()
    for line in text[marker_pos:].splitlines():
        match = EXPORT_RE.match(line)
        if not match:
            continue
        name = match.group(1)
        if name in seen:
            continue
        seen.add(name)
        names.append(name)

    if not names:
        print(f"error: no exports found in {dll}", file=sys.stderr)
        return 1

    output.parent.mkdir(parents=True, exist_ok=True)
    with output.open("w", encoding="utf-8", newline="\n") as handle:
        handle.write(f"LIBRARY {dll.name}\n")
        handle.write("EXPORTS\n")
        for name in names:
            handle.write(f"    {name}\n")

    print(f"wrote {len(names)} exports to {output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
