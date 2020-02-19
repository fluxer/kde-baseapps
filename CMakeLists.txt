project(kde-baseapps)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(SEND_ERROR "kde-baseapps requires an out of source build")
endif()

find_package(KDE4 4.19.0 REQUIRED)
include(KDE4Defaults)
include(GenerateExportHeader)

set(LIBRARY_TYPE SHARED)

find_package(Sudo)
set_package_properties(Sudo PROPERTIES
    DESCRIPTION "Sudo allows a system administrator to delegate authority to give certain users"
    URL "http://www.sudo.ws/"
    TYPE RUNTIME
    PURPOSE "Needed for kdesudo to operate"
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
add_subdirectory(kdesudo)

if(Q_WS_X11)
    add_subdirectory(kdepasswd)
endif()

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

