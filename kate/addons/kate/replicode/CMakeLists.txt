set(katereplicode_SRCS
    replicodeplugin.cpp
    replicodeconfigpage.cpp
    replicodeconfig.cpp
    replicodesettings.cpp
)

kde4_add_plugin(katereplicodeplugin
    ${katereplicode_SRCS}
  )

target_link_libraries(katereplicodeplugin
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    kateinterfaces
)

install(
    TARGETS katereplicodeplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)
install(
    FILES ui.rc
    DESTINATION ${DATA_INSTALL_DIR}/kate/plugins/katereplicodeplugin
)
install(
    FILES katereplicodeplugin.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
