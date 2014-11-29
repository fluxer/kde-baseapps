# Kate project
project (kate)

# shared libs
set (LIBRARY_TYPE SHARED)

# no in source builds
macro_ensure_out_of_source_build("kate requires an out of source build. Please create a separate build directory and run 'cmake path_to_kate [options]' there.")

# config.h
check_function_exists (fdatasync HAVE_FDATASYNC)
check_function_exists (ctermid HAVE_CTERMID)

configure_file (config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# KDE/QT definitions we want
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions (-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS -DKTEXTEDITOR_NO_DEPRECATED)

# global include directories
include_directories (${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES} ${KDE4_KIO_INCLUDES} ${KACTIVITIES_INCLUDE_DIRS})

# kate part
macro_optional_add_subdirectory (part)

# kate application
macro_optional_add_subdirectory (src)

# addons, e.g. ktexteditor plugins, kate plugins, plasma applets, ...
macro_optional_add_subdirectory (addons)

# tests
macro_optional_add_subdirectory (tests)
