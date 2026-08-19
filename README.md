# Dungeon Crawler

A tile-based, top-down dungeon crawler written in **modern C++** with a **Qt 6** GUI.
Built as a multi-part project for the *Programmieren 2* course at Hochschule Darmstadt,
with a focus on clean object-oriented design, safe resource management (RAII), the
Observer pattern, and a separation of game logic from presentation.

<img width="2550" height="1558" alt="image" src="https://github.com/user-attachments/assets/78de784e-fbaf-4c20-9e1e-66ffca3e2fb4" />

<!-- After you set up CI (see TESTING_CI_PLAN.md), add the build badge here:
[![CI](https://github.com/mkdang194/Dungeon-Crawler/actions/workflows/ci.yml/badge.svg)](https://github.com/mkdang194/Dungeon-Crawler/actions/workflows/ci.yml)
-->

---

## Features

- Tile-based world with many interactive tile types: floor, wall, door, portal, switch, pit, ramp, loot chest
- Rule-based movement (e.g. a pit can only be left toward another pit or a ramp)
- Paired **portals** that teleport the character, plus **level portals** that switch between levels
- **Switches and doors** wired via the Observer pattern — a switch can open several doors, and a door can be driven by several switches
- **Enemies (monsters)** and turn-based **combat** between characters
- **Real-time enemy movement** driven from a separate worker thread, with thread-safe GUI updates via Qt's signal–slot mechanism
- Win / lose conditions (player death; reaching the loot chest)
- Multiple levels loaded from external files (`levels/level1.txt`, `levels/level2.txt`)
- Graphical interface built with Qt 6

## Architecture

Game logic (the *model*) is kept separate from the Qt presentation (the *view*), so the
UI only projects the state held by the model.

| Component | Responsibility |
|---|---|
| `Tile` (abstract) | Base for all tiles: `Floor`, `Wall`, `Door`, `Portal`, `LevelPortal`, `Switch`, `Pit`, `Ramp`, `Lootchest` |
| `Character` (abstract) | Base for figures: `Player` and `Monster`, with strength, stamina and hitpoints |
| `Active` / `Passive` | Observer-pattern base classes connecting switches to doors |
| `Level` | **Owns** all tiles and characters via smart pointers; builds the world from a level file |
| `DungeonCrawler` | Game manager; drives each turn and the movement protocol |
| `Input` | Translates user input into movement commands |
| `MainWindow` | Qt view: renders the grid and forwards input to the logic |
| `NpcWorker` | Runs enemy turns on a separate thread; communicates with the GUI via signals/slots |

**Key design decisions**

- **RAII & smart pointers** — ownership is explicit; no manual `new` / `delete` in the game logic
- **Polymorphism & RTTI** — movement uses `dynamic_cast` to react to special tile types
- **Observer pattern** — switches (`Active`) notify doors (`Passive`) without tight coupling
- **Model / view separation** — the Qt UI holds no game state; the `Level` is the single source of truth
- **Multithreading** — enemy logic runs off the UI thread; GUI is only touched via Qt signals/slots

## Tech Stack

C++23 · Qt 6.5+ · CMake · Git

## Getting Started

### Prerequisites
- **Qt 6.5+** with the **Core** and **Widgets** modules
- **CMake ≥ 3.19** and a C++ compiler (MinGW / g++ / clang)

### Build & Run
Easiest path: open `CMakeLists.txt` in **Qt Creator**, let it configure, and press **Run**.

Command line (CMake):
```bash
cmake -B build -DCMAKE_PREFIX_PATH="<path-to-Qt>/6.x.x/<compiler>"
cmake --build build
./build/p5           # on Windows: build\p5.exe
```
The level files are copied next to the executable automatically during the build.

### Controls
WASD / on-screen buttons

## Project Structure
```
├── main.cpp
├── dungeoncrawler.*      game manager / turn logic
├── level.*               owns the world, loads levels from files
├── tile.* + floor/wall/door/portal/levelportal/switch/pit/ramp/lootchest
├── character.* + player.* / monster.*
├── active.* / passive.*  Observer pattern
├── input.*               input handling
├── mainwindow.*          Qt GUI
├── npcworker.*           enemy movement on a worker thread
├── Images.qrc            textures
└── levels/               level1.txt, level2.txt
```

## What this project demonstrates
Object-oriented design, deep inheritance hierarchies and polymorphism, the Observer
pattern, RAII and smart-pointer ownership, RTTI via `dynamic_cast`, model/view
separation, Qt signal–slot, multithreading, and file I/O.

## License
`[optional — e.g. MIT. If you add one, create a LICENSE file.]`
