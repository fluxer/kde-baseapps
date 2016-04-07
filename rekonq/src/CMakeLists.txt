### ------- sub dirs -------

add_subdirectory(data)

### ------- SETTING REKONQ FILES..
add_definitions(-DQT_STATICPLUGIN)
qt4_wrap_cpp(wk_HEADERS_MOC qwebkitplatformplugin.h)

# src files
set(rekonq_SRCS
    #----------------------------------------
    application.cpp
    autosaver.cpp
    searchengine.cpp
    sessionmanager.cpp
    sessionwidget.cpp
    urlresolver.cpp
    websnap.cpp
    #----------------------------------------
    adblock/adblockelementhiding.cpp
    adblock/adblockhostmatcher.cpp
    adblock/adblockmanager.cpp
    adblock/adblockrule.cpp
    adblock/adblockrulefallbackimpl.cpp
    adblock/adblockrulenullimpl.cpp
    adblock/adblockruletextmatchimpl.cpp
    adblock/adblocksettingwidget.cpp
    #----------------------------------------
    bookmarks/bookmarkmanager.cpp
    bookmarks/bookmarkscontextmenu.cpp
    bookmarks/bookmarksmenu.cpp
    bookmarks/bookmarkstoolbar.cpp
    bookmarks/bookmarkstreemodel.cpp
    bookmarks/bookmarkowner.cpp
    #----------------------------------------
    download/downloaditem.cpp
    download/downloadmanager.cpp
    #----------------------------------------
    history/historymanager.cpp
    history/historymodels.cpp
    #----------------------------------------
    icons/icondownloader.cpp
    icons/iconmanager.cpp
    icons/webicon.cpp
    #----------------------------------------
    panels/bookmarkspanel.cpp
    panels/historypanel.cpp
    panels/paneltreeview.cpp
    panels/urlfilterproxymodel.cpp
    panels/urlpanel.cpp
    #----------------------------------------
    rekonqpage/newtabpage.cpp
    rekonqpage/thumbupdater.cpp
    #----------------------------------------
    settings/settingsdialog.cpp
    settings/appearancewidget.cpp
    settings/advancedwidget.cpp
    settings/generalwidget.cpp
    settings/privacywidget.cpp
    settings/tabswidget.cpp
    settings/webkitwidget.cpp
    settings/passexceptionswidget.cpp
    #----------------------------------------
    sync/ftpsynchandler.cpp
    sync/googlesynchandler.cpp
    sync/syncassistant.cpp
    sync/synchandler.cpp
    sync/syncmanager.cpp
    #
    sync/synccheckwidget.cpp
    sync/syncdatawidget.cpp
    sync/synchosttypewidget.cpp
    sync/syncftpsettingswidget.cpp
    sync/sshsynchandler.cpp
    sync/syncgooglesettingswidget.cpp
    sync/syncoperasettingswidget.cpp
    sync/syncsshsettingswidget.cpp
    #----------------------------------------
    tabwindow/rwindow.cpp
    tabwindow/rekonqwindow.cpp
    tabwindow/tabbar.cpp
    tabwindow/tabhighlighteffect.cpp
    tabwindow/tabpreviewpopup.cpp
    tabwindow/tabwidget.cpp
    #----------------------------------------
    urlbar/urlbar.cpp
    urlbar/completionwidget.cpp
    urlbar/listitem.cpp
    urlbar/adblockwidget.cpp
    urlbar/bookmarkwidget.cpp
    urlbar/rsswidget.cpp
    urlbar/sslwidget.cpp
    urlbar/urlsuggester.cpp
    #----------------------------------------
    useragent/useragentinfo.cpp
    useragent/useragentmanager.cpp
    useragent/useragentwidget.cpp
    #----------------------------------------
    webtab/clicktoflash.cpp
    webtab/crashmessagebar.cpp
    webtab/networkaccessmanager.cpp
    webtab/previewselectorbar.cpp
    webtab/protocolhandler.cpp
    webtab/knetworkaccessmanager.cpp
    webtab/searchenginebar.cpp
    webtab/sslinfodialog.cpp
    webtab/walletbar.cpp
    webtab/webpage.cpp
    webtab/webpluginfactory.cpp
    webtab/websslinfo.cpp
    webtab/webview.cpp
    webtab/webtab.cpp
    #----------------------------------------
    webwindow/findbar.cpp
    webwindow/maintoolbar.cpp
    webwindow/rekonqfactory.cpp
    webwindow/rekonqmenu.cpp
    webwindow/webwindow.cpp
    #----------------------------------------
    kspellplugin.cpp
    ${wk_HEADERS_MOC}
)

# optional sync source files
if(HAVE_QCA2 AND HAVE_QTOAUTH)
    set(rekonq_SRCS
        ${rekonq_SRCS}
        sync/operasynchandler.cpp
    )
endif()

### ------------- INCLUDING DIRECTORIES...

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/adblock
    ${CMAKE_CURRENT_SOURCE_DIR}/bookmarks
    ${CMAKE_CURRENT_SOURCE_DIR}/download
    ${CMAKE_CURRENT_SOURCE_DIR}/history
    ${CMAKE_CURRENT_SOURCE_DIR}/icons
    ${CMAKE_CURRENT_SOURCE_DIR}/panels
    ${CMAKE_CURRENT_SOURCE_DIR}/rekonqpage
    ${CMAKE_CURRENT_SOURCE_DIR}/settings
    ${CMAKE_CURRENT_SOURCE_DIR}/sync
    ${CMAKE_CURRENT_SOURCE_DIR}/urlbar
    ${CMAKE_CURRENT_SOURCE_DIR}/tabwindow
    ${CMAKE_CURRENT_SOURCE_DIR}/useragent
    ${CMAKE_CURRENT_SOURCE_DIR}/webtab
    ${CMAKE_CURRENT_SOURCE_DIR}/webwindow
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Opera sync optional include directories
if(HAVE_QCA2 AND HAVE_QTOAUTH)
    include_directories(
        ${QCA2_INCLUDE_DIR}
        ${QTOAUTH_INCLUDE_DIR}
    )
endif()

### -------------- ADDING APPLICATION KCFG FILES...

KDE4_ADD_KCFG_FILES(rekonq_SRCS rekonq.kcfgc)

### --------------- ADDING EXECUTABLE...

add_executable(rekonq ${rekonq_SRCS} main.cpp)

### --------------- TARGETTING LINK LIBRARIES...

target_link_libraries(rekonq
    ${QT_LIBRARIES}
    ${QT_QTSCRIPT_LIBRARY}
    ${QT_QTWEBKIT_LIBRARY}
    ${KDE4_KDEWEBKIT_LIBS}
    ${KDE4_KCMUTILS_LIBS}
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KPARTS_LIBS}
    ${KDE4_SOLID_LIBS}
)

# optional sync link libraries
if(HAVE_QCA2 AND HAVE_QTOAUTH)
    target_link_libraries(rekonq
        ${QCA2_LIBRARY}
        ${QTOAUTH_LIBRARY}
    )
endif()


### ------------ INSTALL FILES...

install(TARGETS rekonq ${INSTALL_TARGETS_DEFAULT_ARGS})

# kwebapp script install (provided for retro-compatibility...)
#FILE( COPY data/kwebapp DESTINATION ${BIN_INSTALL_DIR}
#        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE )

install(
    FILES rekonq.kcfg
    DESTINATION ${KCFG_INSTALL_DIR}
)

install(
    FILES webwindow/rekonqui.rc
    DESTINATION ${DATA_INSTALL_DIR}/rekonq
)

