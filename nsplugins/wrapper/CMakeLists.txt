include_directories(${CMAKE_CURRENT_SOURCE_DIR})

########### next target ###############

kde4_add_library(nswrapper SHARED wrapper.c)

target_link_libraries(nswrapper ${KDE4_KDECORE_LIBS})

set_target_properties(nswrapper PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION}
)

install(TARGETS nswrapper ${INSTALL_TARGETS_DEFAULT_ARGS})

