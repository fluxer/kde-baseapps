########### A test module (not installed by default) ###############
# TODO use it (uninstalled) from the unit test

kde4_add_plugin(konq_sidebartest konqsidebartest.cpp)

target_link_libraries(konq_sidebartest
    ${KDE4_KPARTS_LIBS}
    konqsidebarplugin
)

#install(TARGETS konq_sidebartest  DESTINATION ${PLUGIN_INSTALL_DIR} )
#install( FILES test.desktop  DESTINATION  ${DATA_INSTALL_DIR}/konqsidebartng/entries )

########### Unit tests ###############

set(modulemanagertest_SRCS
    modulemanagertest.cpp
    ../module_manager.cpp
)

kde4_add_test(sidebar-modulemanagertest ${modulemanagertest_SRCS})
target_link_libraries(sidebar-modulemanagertest
    ${KDE4_KIO_LIBS}
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTTEST_LIBRARY}
)
