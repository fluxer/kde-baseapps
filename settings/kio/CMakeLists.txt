add_subdirectory(uasproviders)

########### next target ###############

set(kcm_kio_PART_SRCS
    main.cpp
    kcookiesmain.cpp
    kcookiespolicies.cpp
    kcookiesmanagement.cpp
    kcookiespolicyselectiondlg.cpp
    smbrodlg.cpp
    useragentdlg.cpp
    kproxydlg.cpp
    useragentinfo.cpp
    useragentselectordlg.cpp
    netpref.cpp
    cache.cpp
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
    cookies.desktop
    useragent.desktop
    bookmarks.desktop
    cache.desktop
    netpref.desktop
    proxy.desktop
    DESTINATION ${KDE4_SERVICES_INSTALL_DIR}
)

install(
    FILES uasprovider.desktop
    DESTINATION ${KDE4_SERVICETYPES_INSTALL_DIR}
)
