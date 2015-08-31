project(KDEBASE_APPS)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(SEND_ERROR "kde-baseapps requires an out of source build")
endif()

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules )

include(CTest)
include(CTestConfig.cmake)

#search packages used by KDE
find_package(KDE4 4.14.3 REQUIRED)
include (KDE4Defaults)

set(LIBRARY_TYPE SHARED)

set(QT_MIN_VERSION "4.8.2")

find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES DESCRIPTION "Support for gzip compressed files and data streams"
                       URL "http://www.zlib.net"
                       TYPE REQUIRED
                       PURPOSE "Required for building Konqueror"
                      )

include(ConfigureChecks.cmake)
configure_file (config-apps.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-apps.h )
include_directories (${CMAKE_CURRENT_BINARY_DIR})

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions (-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/lib/konq ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

add_subdirectory( lib )
add_subdirectory( dolphin )
add_subdirectory( kate )
add_subdirectory( kdialog )
add_subdirectory( keditbookmarks )
add_subdirectory( kfind )
add_subdirectory( konqueror )
add_subdirectory( konq-plugins )
add_subdirectory( konsole )

if ( Q_WS_X11 )
  add_subdirectory( nsplugins )
  add_subdirectory( kdepasswd )
endif ( Q_WS_X11 )

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

