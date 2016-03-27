
########### next target ###############

kde4_add_plugin(kcm_kmediaplayer kcmplayer.cpp)

target_link_libraries(kcm_kmediaplayer
    ${KDE4_KIO_LIBS}
    KDE4::kmediaplayer
)

install(TARGETS kcm_kmediaplayer  DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kcmplayer.desktop DESTINATION ${SERVICES_INSTALL_DIR})


