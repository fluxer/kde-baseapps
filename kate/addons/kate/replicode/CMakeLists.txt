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
    DESTINATION ${KDE4_PLUGIN_INSTALL_DIR}
)
install(
    FILES ui.rc
    DESTINATION ${KDE4_DATA_INSTALL_DIR}/kate/plugins/katereplicodeplugin
)
install(
    FILES katereplicodeplugin.desktop
    DESTINATION ${KDE4_SERVICES_INSTALL_DIR}
)
