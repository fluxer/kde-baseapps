project(babelfish)

########### babelfishplugin ###############

kde4_add_plugin(babelfishplugin plugin_babelfish.cpp)

target_link_libraries(babelfishplugin  ${KDE4_KPARTS_LIBS} )

install(
    TARGETS babelfishplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES
    plugin_babelfish.rc
    plugin_translator.desktop
    DESTINATION ${DATA_INSTALL_DIR}/kwebkitpart/kpartplugins
)

kde4_install_icons(${ICON_INSTALL_DIR})
