add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${KDEPIMLIBS_INCLUDE_DIR})

macro_optional_add_subdirectory(svn)
macro_optional_add_subdirectory(git)
macro_optional_add_subdirectory(hg)
macro_optional_add_subdirectory(bazaar)
macro_optional_add_subdirectory(dropbox)
