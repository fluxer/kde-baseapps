# Kate project
project (kate)

find_package(KDE4 4.14.3 REQUIRED)
include (KDE4Defaults)

# config.h
check_function_exists (fdatasync HAVE_FDATASYNC)
check_function_exists (ctermid HAVE_CTERMID)

configure_file (config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# KDE/QT definitions we want
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions (-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

# global include directories
include_directories (${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES} ${KDE4_KIO_INCLUDES})

# kate part
macro_optional_add_subdirectory (part)

# kate application
macro_optional_add_subdirectory (src)

# addons, e.g. ktexteditor plugins, kate plugins, plasma applets, ...
macro_optional_add_subdirectory (addons)

# tests
if(ENABLE_TESTING)
  add_subdirectory (tests)
endif()
