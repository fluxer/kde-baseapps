set(kfind_SRCS
    main.cpp
    kfinddlg.cpp
    kftabdlg.cpp
    kquery.cpp
    kdatecombo.cpp
    kfindtreeview.cpp
)

add_executable(kfind ${kfind_SRCS})

target_link_libraries(kfind konq ${KDE4_KFILE_LIBS})

install(
    TARGETS kfind
    ${INSTALL_TARGETS_DEFAULT_ARGS}
)

########### install files ###############

install(
    PROGRAMS kfind.desktop
    DESTINATION ${XDG_APPS_INSTALL_DIR}
)

kde4_install_icons(${ICON_INSTALL_DIR})
