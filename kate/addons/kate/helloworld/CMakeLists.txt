kde4_add_plugin(katehelloworldplugin plugin_katehelloworld.cpp)

target_link_libraries(katehelloworldplugin
    ${KDE4_KDEUI_LIBS}
    kateinterfaces
)

########### install files ###############
install(
    TARGETS katehelloworldplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)
install(
    FILES ui.rc
    DESTINATION ${DATA_INSTALL_DIR}/kate/plugins/katehelloworld
)
install(
    FILES katehelloworld.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
