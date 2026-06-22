cmake_minimum_required(VERSION 3.19)
project(qt6-tcp-server LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()

qt_add_executable(qt6-tcp-server
    WIN32 MACOSX_BUNDLE
    main.cc
    main_window.cc
    main_window.h
    main_window.ui
    server.h server.cc
    client.h client.cc
    proxy_connections.h proxy_connections.cc
    proxy_data.h proxy_data.cc

)

target_link_libraries(qt6-tcp-server
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
)

install(TARGETS qt6-tcp-server
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET qt6-tcp-server
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
