project(Konsole)

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/lib/konq
)

add_definitions(
    ${QT_QTDBUS_DEFINITIONS}
    -DQT_USE_FAST_CONCATENATION
    -DQT_USE_FAST_OPERATOR_PLUS
    -DKDE_DEFAULT_DEBUG_AREA=1211
)

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(desktop)
