cmake_minimum_required(VERSION 3.19)
project(p5 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

qt_add_executable(p5
    WIN32 MACOSX_BUNDLE
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.h
    src/mainwindow.ui
    src/dungeoncrawler.h src/dungeoncrawler.cpp
    src/level.h src/level.cpp
    src/tile.h src/tile.cpp
    src/character.h src/character.cpp
    src/floor.h src/floor.cpp
    src/wall.h src/wall.cpp
    src/door.h src/door.cpp
    src/portal.h src/portal.cpp
    src/active.h src/active.cpp
    src/passive.h src/passive.cpp
    src/pit.h src/pit.cpp
    src/ramp.h src/ramp.cpp
    src/switch.h src/switch.cpp
    src/input.h src/input.cpp
    assets/Images.qrc
    src/levelportal.h src/levelportal.cpp
    src/monster.h src/monster.cpp
    src/player.h src/player.cpp
    src/lootchest.h src/lootchest.cpp
    src/npcworker.h src/npcworker.cpp
)

# Damit die MOC-generierten Dateien und die Header untereinander gefunden werden:
target_include_directories(p5 PRIVATE src)

configure_file(
    levels/level1.txt
    ${CMAKE_BINARY_DIR}/level1.txt
    COPYONLY
)
configure_file(
    levels/level2.txt
    ${CMAKE_BINARY_DIR}/level2.txt
    COPYONLY
)

target_link_libraries(p5
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS p5
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET p5
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
