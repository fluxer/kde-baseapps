########### kfmclient ###############

set(kfmclient_SRCS kfmclient.cpp )

qt4_add_dbus_interface( kfmclient_SRCS ../src/org.kde.Konqueror.Main.xml konq_main_interface )
qt4_add_dbus_interface( kfmclient_SRCS ../src/org.kde.Konqueror.MainWindow.xml konq_mainwindow_interface )


add_definitions(-DQT_NO_CAST_ASCII)

add_executable( kfmclient ${kfmclient_SRCS})
target_link_libraries(kfmclient  ${KDE4_KIO_LIBS} ${X11_LIBRARIES})
install(TARGETS kfmclient  ${INSTALL_TARGETS_DEFAULT_ARGS} )

install(TARGETS kfmclient ${INSTALL_TARGETS_DEFAULT_ARGS})

