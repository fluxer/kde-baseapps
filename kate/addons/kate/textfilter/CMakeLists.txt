########### next target ###############

set(katetextfilterplugin_PART_SRCS
    plugin_katetextfilter.cpp
    textfilterwidget.ui
)

kde4_add_plugin(katetextfilterplugin ${katetextfilterplugin_PART_SRCS})

target_link_libraries(katetextfilterplugin  ${KDE4_KDEUI_LIBS} ${KDE4_KTEXTEDITOR_LIBS} kateinterfaces )

install(TARGETS katetextfilterplugin  DESTINATION ${PLUGIN_INSTALL_DIR} )

########### install files ###############

install(FILES ui.rc DESTINATION ${DATA_INSTALL_DIR}/kate/plugins/katetextfilter)
install(FILES katetextfilter.desktop DESTINATION ${SERVICES_INSTALL_DIR})

