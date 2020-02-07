set(kateprojectplugin_PART_SRCS
    kateprojectplugin.cpp
    kateprojectpluginview.cpp
    kateproject.cpp
    kateprojectworker.cpp
    kateprojectitem.cpp
    kateprojectview.cpp
    kateprojectviewtree.cpp
    kateprojecttreeviewcontextmenu.cpp
    kateprojectinfoview.cpp
    kateprojectcompletion.cpp
    kateprojectindex.cpp
    kateprojectinfoviewindex.cpp
    kateprojectinfoviewterminal.cpp
    kateprojectinfoviewcodeanalysis.cpp
    kateprojectinfoviewnotes.cpp
    kateprojectnew.cpp
)

kde4_add_plugin(kateprojectplugin ${kateprojectplugin_PART_SRCS})

target_link_libraries(kateprojectplugin
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    kateinterfaces
)

########### install files ###############
install(
    TARGETS kateprojectplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)
install(
    FILES
    ui.rc
    kateproject.example
    DESTINATION ${DATA_INSTALL_DIR}/kate/plugins/project
)
install(
    FILES kateprojectplugin.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
