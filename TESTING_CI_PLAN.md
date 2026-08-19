# Testing & CI Plan — Dungeon Crawler

Goal: add automated **unit tests** for the game logic and a **CI pipeline** that builds
the project and runs those tests on every push. This is what turns coursework into a
portfolio piece a recruiter trusts.

Do the phases in order. Don't start Phase 3 until Phase 2 passes locally.

---

## Phase 1 — Get the project on GitHub  ✅ (do this first)

1. Copy `README.md`, `.gitignore` and this file into the **root** of your project folder.
2. Follow the step-by-step guide (in chat) to push to GitHub.
3. Add a screenshot/GIF of the game to a `docs/` folder and link it in the README.

That alone already makes your CV much stronger. Phases 2 & 3 are the follow-up over the
next days.

---

## Phase 2 — Unit tests (Catch2)

### What to test
Test the **game logic only** — the classes that don't depend on the GUI. This is exactly
what your MVC separation was for. Do **not** try to unit-test the Qt UI.

Good targets (pick what you implemented):

| What | Example assertion |
|---|---|
| HP formula | `Character(str, sta).getMaxHP() == 20 + sta * 5` |
| Constructor | new character's `hitpoints == getMaxHP()` |
| Combat | attacker deals `strength` damage; a dead defender is removed and the attacker moves in |
| Combat (same type) | two players / two NPCs do **not** fight |
| Level parsing | a level string produces the right tile types at the right positions |
| Portal | entering a portal returns/teleports to its paired portal |
| Pit / Ramp | leaving a `Pit` succeeds only if the target is a `Pit` or a `Ramp` |

> ✅ Tip: if your logic classes use `std::string` and are **free of Qt types**, tests (and CI)
> become trivial — no Qt needed to compile them. Keeping the model Qt-independent is good
> design *and* makes this whole step easy.

### Set up Catch2 (single-header — simplest)
1. Download `catch_amalgamated.hpp` (Catch2 v3, "amalgamated" single header) from the
   Catch2 releases page and put it in `tests/`.
2. Create `tests/test_main.cpp`:

```cpp
#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

#include "character.h"   // adjust include paths to your project
#include "level.h"

TEST_CASE("getMaxHP follows the formula") {
    Character c(/*strength*/ 5, /*stamina*/ 4);
    REQUIRE(c.getMaxHP() == 20 + 4 * 5);   // 40
}

TEST_CASE("a new character starts at full HP") {
    Character c(5, 4);
    REQUIRE(c.getHitpoints() == c.getMaxHP());
}

TEST_CASE("attacker deals damage equal to its strength") {
    Character attacker(/*str*/ 10, /*sta*/ 3);
    Character defender(/*str*/ 2,  /*sta*/ 5);   // maxHP = 45
    int before = defender.getHitpoints();
    // call your combat function here, e.g. attacker.attack(defender);
    // REQUIRE(defender.getHitpoints() == before - 10);
}

TEST_CASE("level string is parsed into the right tiles") {
    Level level("###\n#O#\n###");             // adjust to your format/API
    // REQUIRE(dynamic_cast<Wall*>(level.getTile(0, 0)) != nullptr);
    // REQUIRE(dynamic_cast<Portal*>(level.getTile(1, 1)) != nullptr);
}
```

> Replace the method names with your real API and uncomment as you wire each one up.
> Start with **one** passing test, then add more.

### Build & run the tests
If your logic files are Qt-free, a plain compile is enough:
```bash
g++ -std=c++20 tests/test_main.cpp src/character.cpp src/level.cpp \
    -Isrc -Itests -o run_tests
./run_tests
```
Adjust the list of `.cpp` files and the `-I` include paths. **Do not** include `main.cpp`
or any Qt/GUI file here.

If your logic *does* need Qt, build the tests as a second target in Qt Creator / CMake
instead (ask me and I'll give you a `CMakeLists.txt`).

---

## Phase 3 — Continuous Integration (GitHub Actions)

CI runs your build + tests automatically on every push, and shows a green ✅ / red ❌ badge
on the repo. Put the file below at `.github/workflows/ci.yml` (a ready copy is provided).

### Version A — logic is Qt-free (simple, fast)
```yaml
name: CI
on: [push, pull_request]
jobs:
  build-and-test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Install compiler
        run: sudo apt-get update && sudo apt-get install -y g++
      - name: Build tests
        run: |
          g++ -std=c++20 tests/test_main.cpp src/character.cpp src/level.cpp \
              -Isrc -Itests -o run_tests
      - name: Run tests
        run: ./run_tests
```
> Edit the `.cpp` list / include paths to match your project. This is the recommended start.

### Version B — tests need Qt
Add a Qt install step before building (e.g. the `jurplel/install-qt-action`) and build with
CMake. This is more involved — ping me and I'll write the full workflow + `CMakeLists.txt`.

### After CI is green
Add the badge line (shown commented at the top of `README.md`) so it appears on your repo.

---

## Suggested order (a few short sessions)
1. Push to GitHub with README + screenshot. ✅
2. Add Catch2 + **one** passing test locally.
3. Add 4–6 more tests for combat / level parsing / movement.
4. Add `ci.yml` (Version A) → get the green badge.
5. Link the badge in the README.
