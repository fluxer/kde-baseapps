project(kde-baseapps)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(SEND_ERROR "kde-baseapps requires an out of source build")
endif()

find_package(KDE4 REQUIRED)
include(KDE4Defaults)

set(LIBRARY_TYPE SHARED)

find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES
    DESCRIPTION "Support for gzip compressed files and data streams"
    URL "http://www.zlib.net"
    PURPOSE "Required for building Konqueror"
    TYPE REQUIRED
)

include(ConfigureChecks.cmake)
configure_file(
    config-apps.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/config-apps.h
)

include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/konq
    ${CMAKE_BINARY_DIR}
    ${KDE4_INCLUDES}
)

add_definitions(
    ${QT_DEFINITIONS}
    ${KDE4_DEFINITIONS}
    -DQT_USE_FAST_CONCATENATION
    -DQT_USE_FAST_OPERATOR_PLUS
)

add_subdirectory(lib)
add_subdirectory(dolphin)
add_subdirectory(kate)
add_subdirectory(kdialog)
add_subdirectory(keditbookmarks)
add_subdirectory(kfind)
add_subdirectory(settings)
add_subdirectory(konsole)
add_subdirectory(kmediaplayer)

if(Q_WS_X11)
    if(NOT KATIE_FOUND)
        # TODO: fix
        add_subdirectory(nsplugins)
    endif()
    add_subdirectory(kdepasswd)
endif()

if(QT_QTWEBKIT_FOUND)
    add_subdirectory(rekonq)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

