########### next target ###############

kde4_add_plugin(kateopenheaderplugin plugin_kateopenheader.cpp)

target_link_libraries(kateopenheaderplugin
    kateinterfaces
    ${KDE4_KIO_LIBS}
    ${KDE4_KPARTS_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
)

install(
    TARGETS kateopenheaderplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES ui.rc
    DESTINATION ${DATA_INSTALL_DIR}/kate/plugins/kateopenheader
)
install(
    FILES kateopenheader.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
