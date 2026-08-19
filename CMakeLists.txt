cmake_minimum_required(VERSION 3.19)
project(p5 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(p5
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    dungeoncrawler.h dungeoncrawler.cpp
    level.h level.cpp
    tile.h tile.cpp
    character.h character.cpp
    floor.h floor.cpp
    wall.h wall.cpp
    door.h door.cpp
    portal.h portal.cpp
    active.h active.cpp
    passive.h passive.cpp
    pit.h pit.cpp
    ramp.h ramp.cpp
    switch.h switch.cpp
    input.h input.cpp
    Images.qrc
    levelportal.h levelportal.cpp
    monster.h monster.cpp
    player.h player.cpp
    lootchest.h lootchest.cpp
    npcworker.h npcworker.cpp




)

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
