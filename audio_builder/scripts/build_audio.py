#!/usr/bin/env python3
# Author: Michael Garcia, M&E Design
# License: MIT
# Date: 2026-04-17
# Contact: michael@mandedesign.studio
# Website: https://mandedesign.studio

"""Generate GeneratedSoundData.cpp from JSON sound definitions."""

from __future__ import annotations

import json
from pathlib import Path


def load_json(path: Path):
    with path.open("r", encoding="utf-8") as handle:
        return json.load(handle)


def format_note_event(note_map: dict[str, int], item: dict[str, object]) -> str:
    note_name = item.get("note")
    duration_ms = int(item["duration_ms"])
    frequency = 0 if note_name is None else int(note_map[str(note_name)])
    return f"    {{{frequency}, {duration_ms}}},"


def main() -> None:
    root = Path(__file__).resolve().parents[2]
    data_dir = root / "audio_builder" / "data"
    output_path = root / "src" / "audio" / "GeneratedSoundData.cpp"

    note_map = load_json(data_dir / "note_map.json")
    melodies = load_json(data_dir / "melodies.json")
    sounds = load_json(data_dir / "sounds.json")

    lines: list[str] = [
        '#include "audio/GeneratedSoundData.h"',
        "",
        "namespace audio {",
        "namespace {",
        "",
    ]

    for melody_name, notes in melodies.items():
        symbol = "".join(part.capitalize() for part in melody_name.split("_"))
        lines.append(f"constexpr NoteEvent k{symbol}Notes[] = {{")
        lines.extend(format_note_event(note_map, item) for item in notes)
        lines.append("};")
        lines.append("")

    lines.append("constexpr SoundDef kSoundTable[] = {")
    for sound in sounds:
        pattern_symbol = "".join(part.capitalize() for part in sound["pattern"].split("_"))
        lines.append(
            "    {SoundId::%s, k%sNotes, sizeof(k%sNotes) / sizeof(k%sNotes[0]), %d, %d, %s},"
            % (
                sound["id"],
                pattern_symbol,
                pattern_symbol,
                pattern_symbol,
                int(sound["channel"]),
                int(sound["priority"]),
                "true" if sound["loop"] else "false",
            )
        )
    lines.append("};")
    lines.append("")
    lines.append("}  // namespace")
    lines.append("")
    lines.append("const SoundDef* findSoundDef(SoundId id)")
    lines.append("{")
    lines.append("    for (const SoundDef& sound : kSoundTable) {")
    lines.append("        if (sound.id == id) {")
    lines.append("            return &sound;")
    lines.append("        }")
    lines.append("    }")
    lines.append("")
    lines.append("    return nullptr;")
    lines.append("}")
    lines.append("")
    lines.append("}  // namespace audio")
    lines.append("")

    output_path.write_text("\n".join(lines), encoding="utf-8")


if __name__ == "__main__":
    main()
