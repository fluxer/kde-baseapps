########### konqsidebar_places module ###############

kde4_add_plugin(konqsidebar_places places_module.cpp)

target_link_libraries(konqsidebar_places
    ${KDE4_KFILE_LIBS}
    ${KDE4_KPARTS_LIBRARY}
    konqsidebarplugin
)

install(
    TARGETS konqsidebar_places
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES konqsidebar_places.desktop
    DESTINATION ${DATA_INSTALL_DIR}/konqsidebartng/plugins
)
