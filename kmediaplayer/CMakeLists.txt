project(KMediaPlayer)

add_subdirectory(kcm)

# application
add_executable(kmediaplayer main.cpp kmediawindow.cpp)

target_link_libraries(kmediaplayer
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KDEUI_LIBS} 
    KDE4::kmediaplayer
    KDE4::kcmutils
)

## part
kde4_add_plugin(kmediaplayerpart kmediaplayerpart.cpp)

target_link_libraries(kmediaplayerpart
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KPARTS_LIBS}
    ${KDE4_KDEUI_LIBS}
    KDE4::kmediaplayer
)

## install everything
install(
    TARGETS kmediaplayer
    DESTINATION ${BIN_INSTALL_DIR}
)

install(
    TARGETS kmediaplayerpart
    DESTINATION ${PLUGIN_INSTALL_DIR}
)

install(
    PROGRAMS kmediaplayer.desktop
    DESTINATION ${XDG_APPS_INSTALL_DIR}
)

install(
    PROGRAMS kmediaplayerpart.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
