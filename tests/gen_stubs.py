#!/usr/bin/env python3
"""Создаёт тест-заглушку для каждого content/**/code.hpp, которого ещё нет.

Уже существующие tests/<тот же путь>/test.cpp не перезаписывает.
Запуск из любого каталога: python3 tests/gen_stubs.py
"""

from __future__ import annotations

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONTENT = ROOT / "content"
TESTS = ROOT / "tests"

TEMPLATE = """\
#include <gtest/gtest.h>

// Заглушка: {path}
TEST({slug}, Stub) {{
    SUCCEED();
}}
"""


# Латиница, не кириллица: CTest читает список тестов не в UTF-8, если локаль
# не собрана, и подставляет в --gtest_filter испорченное имя. gtest тогда
# запускает 0 тестов и всё равно завершается с кодом 0.
_RU = {
    "а": "a",
    "б": "b",
    "в": "v",
    "г": "g",
    "д": "d",
    "е": "e",
    "ё": "yo",
    "ж": "zh",
    "з": "z",
    "и": "i",
    "й": "y",
    "к": "k",
    "л": "l",
    "м": "m",
    "н": "n",
    "о": "o",
    "п": "p",
    "р": "r",
    "с": "s",
    "т": "t",
    "у": "u",
    "ф": "f",
    "х": "h",
    "ц": "ts",
    "ч": "ch",
    "ш": "sh",
    "щ": "sch",
    "ъ": "",
    "ы": "y",
    "ь": "",
    "э": "e",
    "ю": "yu",
    "я": "ya",
}


def _is_cyr(ch: str) -> bool:
    return ch.lower() in _RU


def transliterate(text: str) -> str:
    out: list[str] = []
    for i, ch in enumerate(text):
        mapped = _RU.get(ch.lower())
        if mapped is None:
            out.append(ch)
            continue
        if ch.isupper() and mapped:
            nxt = text[i + 1] if i + 1 < len(text) else ""
            if _is_cyr(nxt) and nxt.islower():
                mapped = mapped[0].upper() + mapped[1:]
            else:
                mapped = mapped.upper()
        out.append(mapped)
    return "".join(out)


def is_ident_char(ch: str) -> bool:
    return ch == "_" or ("A" <= ch <= "Z") or ("a" <= ch <= "z") or ("0" <= ch <= "9")


def slug_from(rel_dir: Path) -> str:
    text = transliterate(rel_dir.as_posix())
    leftover = [ch for ch in text if "\u0400" <= ch <= "\u04ff"]
    if leftover:
        raise SystemExit(f"unmapped cyrillic in {rel_dir}: {''.join(leftover)}")
    parts: list[str] = []
    prev_underscore = False
    for ch in text:
        if is_ident_char(ch):
            parts.append(ch)
            prev_underscore = False
        elif parts and not prev_underscore:
            parts.append("_")
            prev_underscore = True
    slug = "".join(parts).strip("_")
    if not slug or slug[0].isdigit():
        slug = "a_" + slug
    return slug


def algorithms() -> list[Path]:
    return sorted(p for p in CONTENT.rglob("code.hpp") if p.is_file())


def main() -> int:
    headers = algorithms()
    if not headers:
        print(f"no code.hpp under {CONTENT}", file=sys.stderr)
        return 1

    planned: list[tuple[Path, Path, str, str]] = []
    seen: dict[str, str] = {}
    for header in headers:
        rel_dir = header.parent.relative_to(CONTENT)
        rel_header = header.relative_to(ROOT).as_posix()
        slug = slug_from(rel_dir)
        if slug in seen:
            print(
                f"slug collision: {slug}\n  {seen[slug]}\n  {rel_header}",
                file=sys.stderr,
            )
            return 1
        seen[slug] = rel_header
        test_path = TESTS / rel_dir / "test.cpp"
        planned.append((header, test_path, slug, rel_header))

    created = 0
    for _, test_path, slug, rel_header in planned:
        if test_path.exists():
            continue
        test_path.parent.mkdir(parents=True, exist_ok=True)
        test_path.write_text(
            TEMPLATE.format(path=rel_header, slug=slug),
            encoding="utf-8",
            newline="\n",
        )
        created += 1
        print(f"created {test_path.relative_to(ROOT).as_posix()}")

    header_dirs = {header.parent.relative_to(CONTENT) for header, _, _, _ in planned}
    orphans = []
    for test_path in sorted(TESTS.rglob("test.cpp")):
        rel_dir = test_path.parent.relative_to(TESTS)
        if rel_dir not in header_dirs:
            orphans.append(test_path.relative_to(ROOT).as_posix())
    for orphan in orphans:
        print(f"orphan stub (no code.hpp): {orphan}", file=sys.stderr)

    print(f"algorithms: {len(planned)}, new stubs: {created}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
