include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

########### next target ###############

set(libfsview_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/../treemap.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../fsview.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../scan.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../inode.cpp
)

set(scantest_SRCS scantest.cpp ${libfsview_SRCS})

kde4_add_manual_test(scantest ${scantest_SRCS})

target_link_libraries(scantest  ${KDE4_KIO_LIBS})


