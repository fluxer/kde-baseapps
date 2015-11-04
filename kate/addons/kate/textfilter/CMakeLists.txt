########### next target ###############

kde4_add_plugin(katetextfilterplugin plugin_katetextfilter.cpp)

target_link_libraries(katetextfilterplugin
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    kateinterfaces
)

install(
    TARGETS katetextfilterplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES ui.rc
    DESTINATION ${DATA_INSTALL_DIR}/kate/plugins/katetextfilter
)
install(
    FILES katetextfilter.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
