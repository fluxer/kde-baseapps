########### next target ###############

set(kateinterfaces_LIB_SRCS kateappadaptor.cpp
   ../interfaces/kate/application.cpp
   ../interfaces/kate/mainwindow.cpp
   ../interfaces/kate/documentmanager.cpp
   ../interfaces/kate/pluginmanager.cpp
   ../interfaces/kate/plugin.cpp
   ../interfaces/kate/pluginconfigpageinterface.cpp
   kateapp.cpp
   kateconfigdialog.cpp
   kateconfigplugindialogpage.cpp
   katedocmanager.cpp
   katemainwindow.cpp
   katepluginmanager.cpp
   kateviewmanager.cpp
   kateviewspace.cpp
   katesavemodifieddialog.cpp
   katemwmodonhddialog.cpp
   katesession.cpp
   katemdi.cpp
   katecontainer.cpp
   katerunninginstanceinfo.cpp
   kateappcommands.cpp
   katequickopen.cpp
   )



kde4_add_library(kateinterfaces SHARED ${kateinterfaces_LIB_SRCS})

target_link_libraries(kateinterfaces PRIVATE ${QT_QTXML_LIBRARY} ${KDE4_KTEXTEDITOR_LIBS} ${KDE4_KPARTS_LIBS} )
target_link_libraries(kateinterfaces PUBLIC "${KDE4_KPARTS_LIBS}" )


set_target_properties(kateinterfaces PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION})

install(TARGETS kateinterfaces ${INSTALL_TARGETS_DEFAULT_ARGS} )


########### next target ###############

set(kate_KDEINIT_SRCS katemain.cpp)

kde4_add_executable( kate ${kate_KDEINIT_SRCS})

target_link_libraries(kate ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS} kateinterfaces )

install(TARGETS kate ${INSTALL_TARGETS_DEFAULT_ARGS})

