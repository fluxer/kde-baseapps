########### next target ###############

set(kcm_kio_PART_SRCS
    main.cpp
    smbrodlg.cpp
    kproxydlg.cpp
    netpref.cpp
    bookmarks.cpp
    ksaveioconfig.cpp
)

kde4_add_plugin(kcm_kio ${kcm_kio_PART_SRCS})

target_link_libraries(kcm_kio
    ${KDE4_KCMUTILS_LIBS}
    ${KDE4_KIO_LIBS}
)

install(
    TARGETS kcm_kio
    DESTINATION ${KDE4_PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES
    smb.desktop
    bookmarks.desktop
    netpref.desktop
    proxy.desktop
    DESTINATION ${KDE4_SERVICES_INSTALL_DIR}
)
