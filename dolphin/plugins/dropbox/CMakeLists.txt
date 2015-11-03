project(fileviewdropboxplugin)

kde4_add_plugin(fileviewdropboxplugin fileviewdropboxplugin.cpp)

target_link_libraries(fileviewdropboxplugin ${KDE4_KIO_LIBS} konq)

install(
    FILES fileviewdropboxplugin.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)

install(
    TARGETS fileviewdropboxplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)