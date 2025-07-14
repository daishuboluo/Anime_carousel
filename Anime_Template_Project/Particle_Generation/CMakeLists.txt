cmake_minimum_required(VERSION 3.19)
project(Particle_Generation LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
)
qt_add_executable(Particle_Generation
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    particlewidget.h particlewidget.cpp
)

target_link_libraries(Particle_Generation
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(Particle_Generation
    PRIVATE
        # opencv_world470d.lib
        opencv_world470.lib

)
target_link_libraries(Particle_Generation PRIVATE Qt6::Widgets)

include(GNUInstallDirs)

install(TARGETS Particle_Generation
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Particle_Generation
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
