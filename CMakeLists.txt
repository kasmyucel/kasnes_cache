cmake_minimum_required(VERSION 3.16)
project(kasnes_cache LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)


if(APPLE)
    # Check if brew exists and get its prefix
    execute_process(COMMAND brew --prefix libpqxx
        OUTPUT_VARIABLE BREW_PQXX_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET)

    if(BREW_PQXX_PREFIX)
        set(ENV{PKG_CONFIG_PATH} "${BREW_PQXX_PREFIX}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
    endif()
endif()

pkg_check_modules(LIBPQXX REQUIRED IMPORTED_TARGET libpqxx)

add_executable(kasnes_cache
    main.cpp
    base/lrucache.h base/lrucache.cpp
    base/threadpool.h base/threadpool.cpp
    network/tcpserver.h network/tcpserver.cpp
    network/protocol.h network/protocol.cpp
    base/data.h base/data.cpp
    base/node.h base/node.cpp
    settings.h
    network/dbconnection.h network/dbconnection.cpp
    network/dbpool.h network/dbpool.cpp
    base/dataservice.h base/dataservice.cpp
    network/connectionpool.h network/connectionpool.cpp
)

target_link_libraries(kasnes_cache PRIVATE PkgConfig::LIBPQXX)

target_include_directories(kasnes_cache PRIVATE ${CMAKE_SOURCE_DIR})

include(GNUInstallDirs)
install(TARGETS kasnes_cache
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
