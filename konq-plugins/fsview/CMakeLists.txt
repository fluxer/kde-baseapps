if(ENABLE_TESTING)
    add_subdirectory(tests)
endif()

set(libfsview_SRCS
    treemap.cpp
    fsview.cpp
    scan.cpp
    inode.cpp
)

########### next target ###############

add_executable(fsview main.cpp ${libfsview_SRCS})

target_link_libraries(fsview ${KDE4_KIO_LIBS})

install(
    TARGETS fsview
    ${INSTALL_TARGETS_DEFAULT_ARGS}
)

########### next target ###############

kde4_add_plugin(fsviewpart fsview_part.cpp ${libfsview_SRCS})

target_link_libraries(fsviewpart ${KDE4_KPARTS_LIBS} konq)

install(
    TARGETS fsviewpart
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES fsview_part.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
install(
    FILES fsview_part.rc
    DESTINATION ${DATA_INSTALL_DIR}/fsview
)

kde4_install_icons(${ICON_INSTALL_DIR})
