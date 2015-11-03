########### next target ###############

kde4_add_plugin(dirfilterplugin dirfilterplugin.cpp)

target_link_libraries(dirfilterplugin ${KDE4_KPARTS_LIBS})

install(
    TARGETS dirfilterplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES
    dirfilterplugin.rc
    dirfilterplugin.desktop
    DESTINATION ${DATA_INSTALL_DIR}/dolphinpart/kpartplugins
)
