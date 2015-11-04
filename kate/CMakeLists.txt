# Kate project
project (kate)

# config.h
check_function_exists(fdatasync HAVE_FDATASYNC)
check_function_exists(ctermid HAVE_CTERMID)

configure_file(
  config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

# global include directories
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${KDE4_KIO_INCLUDES}
)

# kate part
macro_optional_add_subdirectory(part)

# kate application
macro_optional_add_subdirectory(src)

# addons, e.g. ktexteditor plugins, kate plugins, plasma applets, ...
macro_optional_add_subdirectory(addons)

# tests
if(ENABLE_TESTING)
    add_subdirectory(tests)
endif()
