# Dungeon Crawler

A tile-based, top-down dungeon crawler written in **modern C++** with a **Qt** GUI.
Built as a multi-part project for the *Programmieren 2* course at Hochschule Darmstadt,
with a focus on clean object-oriented design, safe resource management (RAII), and a
strict separation of game logic from presentation (MVC).

<!-- Add a screenshot or a short GIF of the game — this is the first thing a recruiter looks at.
     Put the image in a docs/ folder and update the path below. -->
![Gameplay](docs/screenshot.png)

<!-- After you set up CI (see TESTING_CI_PLAN.md), add the build badge here:
[![CI](https://github.com/USERNAME/DungeonCrawler/actions/workflows/ci.yml/badge.svg)](https://github.com/USERNAME/DungeonCrawler/actions/workflows/ci.yml)
-->

---

## Features

- Tile-based world with several interactive tile types: floor, wall, portal, door, switch, pit, ramp, loot chest
- Rule-based movement (e.g. a pit can only be left toward another pit or a ramp)
- Paired **portals** that teleport the character on entry
- **Switches and doors** wired via the Observer pattern — a switch can open several doors, and a door can be driven by several switches
- **NPCs (enemies)** and turn-based **combat** between characters
- Win / lose conditions (player death, reaching the loot chest)
- Levels loaded from external files
- (optional) Real-time enemy movement on a separate thread
- Two interchangeable frontends: a text-based Terminal UI and a graphical Qt UI

> ⚠️ Trim this list to what you actually implemented before publishing.

## Architecture

Game logic (the *model*) is fully separated from presentation (the *view*), so the same
logic runs behind either the terminal or the Qt interface.

| Component | Responsibility |
|---|---|
| `Tile` (abstract) | Base for all tiles: `Floor`, `Wall`, `Portal`, `Door`, `Switch`, `Pit`, `Ramp`, `Lootchest` |
| `Character` | A figure (player or NPC) with strength, stamina and hitpoints |
| `Level` | **Owns** all tiles and characters via `std::unique_ptr`; builds the world from a level string / file |
| `DungeonCrawler` | Game manager; drives each turn and the movement protocol |
| `AbstractUI` | Interface implemented by `TerminalUI` and the Qt-based `GraphicalUI` |
| `Active` / `Passive` | Observer-pattern base classes connecting switches to doors |

**Key design decisions**

- **RAII & smart pointers** — ownership is explicit; no manual `new` / `delete` in the game logic
- **Polymorphism & RTTI** — movement uses `dynamic_cast` to react to special tile types
- **MVC** — the GUI holds no game state; it only projects the model
- **Signal–Slot** — Qt input and UI updates are decoupled from the logic

## Tech Stack

C++ (C++`[17/20/23 — điền chuẩn bạn dùng]`) · Qt 6 · `[CMake / qmake]` · Git

## Getting Started

### Prerequisites
- Qt 6 and a C++ compiler (`g++` / `clang`)
- `[CMake ≥ 3.16]` **or** Qt Creator

### Build & Run
Easiest path: open the project in **Qt Creator** and press **Run**.

Command line (if you use CMake):
```bash
cmake -B build
cmake --build build
./build/DungeonCrawler
```
> `[Adjust the commands / binary name to your build system.]`

### Controls
`[Describe your controls — e.g. arrow keys / WASD / on-screen buttons]`

## Project Structure
```
[paste your actual folder layout here, e.g.:]
src/        game logic (Tile, Character, Level, DungeonCrawler, ...)
ui/         TerminalUI, GraphicalUI, MainWindow
levels/     level files
tests/      unit tests
```

## Testing
Unit tests cover the UI-independent game logic (HP formula, combat, level parsing,
movement rules). See [`TESTING_CI_PLAN.md`](TESTING_CI_PLAN.md).
```bash
[test run command]
```

## What this project demonstrates
Object-oriented design, deep inheritance hierarchies and polymorphism, the Observer
pattern, RAII and smart-pointer ownership, RTTI via `dynamic_cast`, MVC separation,
Qt Signal–Slot, `[multithreading,]` and file I/O.

## License
`[optional — e.g. MIT. If you add one, create a LICENSE file.]`
