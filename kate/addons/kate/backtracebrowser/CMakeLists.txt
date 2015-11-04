########### next target ###############

set(katebacktracebrowserplugin_PART_SRCS
    katebacktracebrowser.cpp
    btparser.cpp
    btfileindexer.cpp
    btdatabase.cpp
)

kde4_add_plugin(katebacktracebrowserplugin ${katebacktracebrowserplugin_PART_SRCS})

target_link_libraries(katebacktracebrowserplugin
    ${KDE4_KFILE_LIBS}
    kateinterfaces
    ${KDE4_KTEXTEDITOR_LIBS}
)

########### install files ###############

install(
    TARGETS katebacktracebrowserplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)
install(
    FILES katebacktracebrowserplugin.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
