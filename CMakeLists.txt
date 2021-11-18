project(kde-baseapps)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(SEND_ERROR "kde-baseapps requires an out of source build")
endif()

find_package(KDE4 4.21.0 REQUIRED)
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

find_program(CTAGS_EXECUTABLE ctags ctags-universal ctags-exuberant uctags)
add_feature_info(ctags
    CTAGS_EXECUTABLE
    "Code indexing support in Kate"
)

find_program(CPPCHECK_EXECUTABLE cppcheck)
add_feature_info(cppcheck
    CPPCHECK_EXECUTABLE
    "Code analysis support in Kate"
)

find_program(XMLLINT_EXECUTABLE xmllint)
add_feature_info(xmllint
    XMLLINT_EXECUTABLE
    "XML linting support in Kate"
)

find_program(RZSZ_EXECUTABLE NAMES rz lrz)
add_feature_info(rzsz
    RZSZ_EXECUTABLE
    "ZModem support in Konsole"
)

find_program(LOCALE_EXECUTABLE NAMES locate)
add_feature_info(locate
    LOCALE_EXECUTABLE
    "Search indexing support in KFind"
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

