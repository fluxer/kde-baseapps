# Andrea Diamantini - adjam7 at gmail dot com
# rekonq project

project(rekonq)

set(REKONQ_VERSION "2.4.2" )
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config-version.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/src/config-version.h
)

## Ensure we have QtWebKit >= 2.2.0
#FIND_PACKAGE(PkgConfig REQUIRED)
#PKG_CHECK_MODULES(PC_QTWEBKIT REQUIRED QtWebKit>=4.8.0)
#IF(NOT PC_QTWEBKIT_FOUND)
#  MESSAGE(FATAL_ERROR "QtWebkit 2.2.0 is required for this version of Rekonq")
#ELSE()
#  MESSAGE(STATUS "Found QtWebKit ${PC_QTWEBKIT_VERSION}")
#ENDIF()

include(MacroOptionalFindPackage)
include(FindPackageHandleStandardArgs)
include(MacroLibrary)


# ==================================================================================
# optional QCA2 & QtOAuth requirements

MACRO_OPTIONAL_FIND_PACKAGE(QCA2)
MACRO_BOOL_TO_01(QCA2_FOUND HAVE_QCA2)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config-qca2.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/src/config-qca2.h
)


MACRO_OPTIONAL_FIND_PACKAGE(QtOAuth)
MACRO_BOOL_TO_01(QTOAUTH_FOUND HAVE_QTOAUTH)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config-qtoauth.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/src/config-qtoauth.h
)

add_subdirectory(src)
add_subdirectory(icons)
