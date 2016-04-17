macro_optional_find_package(QJSON)
macro_log_feature(QJSON_FOUND "QJSON" "Json parser for Qt" "")

# bail out without it, Katie has backported JSON support tough
if(NOT QJSON_FOUND AND NOT KATIE_FOUND)
    message(WARNING "Project plugin needs QJSON Library.")
    return()
endif()

if(QJSON_FOUND)
    # Ubuntu 12.10 needs the lower-case qjson
    include_directories(${QJSON_INCLUDE_DIR} ${qjson_INCLUDE_DIR})
endif()

set(kateprojectplugin_PART_SRCS
    kateprojectplugin.cpp
    kateprojectpluginview.cpp
    kateproject.cpp
    kateprojectworker.cpp
    kateprojectitem.cpp
    kateprojectview.cpp
    kateprojectviewtree.cpp
    kateprojecttreeviewcontextmenu.cpp
    kateprojectinfoview.cpp
    kateprojectcompletion.cpp
    kateprojectindex.cpp
    kateprojectinfoviewindex.cpp
    kateprojectinfoviewterminal.cpp
    kateprojectinfoviewcodeanalysis.cpp
    kateprojectinfoviewnotes.cpp
    kateprojectnew.cpp
)

kde4_add_plugin(kateprojectplugin ${kateprojectplugin_PART_SRCS})

# Ubuntu 12.10 needs the lower-case qjson
target_link_libraries(kateprojectplugin
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    kateinterfaces
)

if(QJSON_FOUND)
    target_link_libraries(kateprojectplugin
        ${QJSON_LIBRARIES}
        ${qjson_LIBRARIES}
    )
endif()

########### install files ###############
install(
    TARGETS kateprojectplugin
    DESTINATION ${PLUGIN_INSTALL_DIR}
)
install(
    FILES
    ui.rc
    kateproject.example
    DESTINATION ${DATA_INSTALL_DIR}/kate/plugins/project
)
install(
    FILES kateprojectplugin.desktop
    DESTINATION ${SERVICES_INSTALL_DIR}
)
