# Kate interfaces used for the plugins
include_directories(${CMAKE_SOURCE_DIR}/kate/src/interfaces)

# default debug area
add_definitions(-DKDE_DEFAULT_DEBUG_AREA=13040)

add_subdirectory(gdbplugin)

add_subdirectory(textfilter)
add_subdirectory(konsole)

add_subdirectory(filebrowser)
add_subdirectory(mailfiles)

add_subdirectory(tabbarextension)
add_subdirectory(symbolviewer)
add_subdirectory(backtracebrowser)

add_subdirectory(filetemplates)

add_subdirectory(kate-ctags)
add_subdirectory(katebuild-plugin)
add_subdirectory(search)

add_subdirectory(kttsd)

add_subdirectory(openheader)
add_subdirectory(xmltools)
add_subdirectory(xmlcheck)

add_subdirectory(snippets)

add_subdirectory(tabify)

add_subdirectory(close-except-like)

add_subdirectory(project)
