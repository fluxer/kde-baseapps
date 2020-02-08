########### next target ###############

kde4_add_plugin(katesnippetsplugin katesnippets.cpp)

target_link_libraries(katesnippetsplugin
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KPARTS_LIBS}
    kateinterfaces
)

install(
    TARGETS katesnippetsplugin
    DESTINATION ${KDE4_PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES katesnippetsplugin.desktop
    DESTINATION ${KDE4_SERVICES_INSTALL_DIR}
)
