project(fileviewbazaarplugin)

kde4_add_plugin(fileviewbazaarplugin fileviewbazaarplugin.cpp)

target_link_libraries(fileviewbazaarplugin ${KDE4_KIO_LIBS} konq)

install(
    FILES
    fileviewbazaarplugin.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)

install(
    TARGETS fileviewbazaarplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)
