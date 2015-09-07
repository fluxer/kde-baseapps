include_directories(
    ${KDE4_KIO_INCLUDES}
    ../..
)

###### kbookmarkmodeltest ######

set(kbookmarkmodeltest_SRCS kbookmarkmodeltest.cpp)
kde4_add_test(kbookmarkmodeltest ${kbookmarkmodeltest_SRCS})
target_link_libraries(kbookmarkmodeltest kbookmarkmodel_private ${KDE4_KIO_LIBS} ${QT_QTTEST_LIBRARY})
