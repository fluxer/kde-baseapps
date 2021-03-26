# Kate project
project (kate)

find_program(CTAGS_EXECUTABLE ctags)
add_feature_info(ctags
    CTAGS_EXECUTABLE
    "Code indexing support in Kate"
)

find_program(CPPCHECK_EXECUTABLE cppcheck)
add_feature_info(cppcheck
    CPPCHECK_EXECUTABLE
    "Code analysis support in Kate"
)

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
    ${CMAKE_CURRENT_BINARY_DIR}/src/app
    ${KDE4_KIO_INCLUDES}
)

# kate part
add_subdirectory(part)

# kate application
add_subdirectory(src)

# addons, e.g. ktexteditor plugins, kate plugins, plasma applets, ...
add_subdirectory(addons)

# tests
if(ENABLE_TESTING)
    add_subdirectory(tests)
endif()
