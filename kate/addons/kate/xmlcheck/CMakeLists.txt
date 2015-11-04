########### next target ###############

kde4_add_plugin(katexmlcheckplugin plugin_katexmlcheck.cpp)

target_link_libraries(katexmlcheckplugin
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    kateinterfaces
)

install(
    TARGETS katexmlcheckplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES ui.rc
    DESTINATION ${DATA_INSTALL_DIR}/kate/plugins/katexmlcheck
)
install(
    FILES katexmlcheck.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
