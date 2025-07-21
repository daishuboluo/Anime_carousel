cmake_minimum_required(VERSION 3.19)
project(LoadingAnime LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)


qt_standard_project_setup()

qt_add_executable(LoadingAnime
    # WIN32 MACOSX_BUNDLE
    main.cpp
    widget.cpp
    widget.h
    ring_loading.h ring_loading.cpp
    bouncing_ball_loading.h bouncing_ball_loading.cpp
    rectangle_loading.h rectangle_loading.cpp

    line_rotating_along_rectangular_border.h
    line_rotating_along_rectangular_border.cpp
    Square_Pseudo_Bounce_Loading.h Square_Pseudo_Bounce_Loading.cpp
    regular_loading.h regular_loading.cpp
    line_loading.h line_loading.cpp
    black_white_ball_loading.h black_white_ball_loading.cpp
    hexagonloaderwidget.h hexagonloaderwidget.cpp
)

target_link_libraries(LoadingAnime
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_link_libraries(LoadingAnime PRIVATE Qt6::Widgets)
target_link_libraries(LoadingAnime PRIVATE Qt6::Widgets)
target_link_libraries(LoadingAnime PRIVATE Qt6::Widgets)


include(GNUInstallDirs)

install(TARGETS LoadingAnime
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET LoadingAnime
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
