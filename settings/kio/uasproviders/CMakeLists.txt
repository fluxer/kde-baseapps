FILE(GLOB useragents *.desktop)
install(
    FILES ${useragents}
    DESTINATION ${KDE4_SERVICES_INSTALL_DIR}/useragentstrings
)
