########### next target ###############

kde4_add_plugin(katesnippetsplugin katesnippets.cpp)

target_link_libraries(katesnippetsplugin
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KPARTS_LIBS}
    kateinterfaces
)

install(
    TARGETS katesnippetsplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES katesnippetsplugin.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
