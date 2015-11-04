########### next target ###############

kde4_add_plugin(katefiletemplates filetemplates.cpp)

target_link_libraries(katefiletemplates
    kateinterfaces
    ${KDE4_KTEXTEDITOR_LIBS}
)

install(
    TARGETS katefiletemplates
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES ui.rc
    DESTINATION ${DATA_INSTALL_DIR}/kate/plugins/katefiletemplates
)
install(
    FILES katetemplate.xml
    DESTINATION ${DATA_INSTALL_DIR}/katepart/syntax
)
install(
    FILES katefiletemplates.desktop
    DESTINATION  ${SERVICES_INSTALL_DIR}
)
