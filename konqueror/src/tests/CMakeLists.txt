include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${KDE4_INCLUDES}
)

########### centralwidget ###############

kde4_add_manual_test(centralwidget centralwidget.cpp)
target_link_libraries(centralwidget
    konq
    ${KDE4_KDECORE_LIBS}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTTEST_LIBRARY}
)

########### historymanagertest ###############

kde4_add_test(konqueror-historymanagertest historymanagertest.cpp)
target_link_libraries(konqueror-historymanagertest
    konq
    konquerorprivate
    ${KDE4_KDEUI_LIBS}
    ${QT_QTCORE_LIBRARY}
    ${QT_QTTEST_LIBRARY}
)

########### undomanagertest ###############

# FIXME: fixup
# kde4_add_test(konqueror-undomanagertest undomanagertest.cpp)
# target_link_libraries(konqueror-undomanagertest konquerorprivate ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})

########### konqviewtest ###############

# FIXME: fixup
# kde4_add_test(konqueror-konqviewtest konqviewtest.cpp)
# target_link_libraries(konqueror-konqviewtest konquerorprivate ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})
