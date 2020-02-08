# install default libraries files
install(
    DIRECTORY libraries
    DESTINATION ${KDE4_DATA_INSTALL_DIR}/katepart/script/libraries
    FILES_MATCHING PATTERN "*.js"
)

# install default commands files
install(
    DIRECTORY commands
    DESTINATION ${KDE4_DATA_INSTALL_DIR}/katepart/script/commands
    FILES_MATCHING PATTERN "*.js"
)

# install default indentation files
install(
    DIRECTORY indentation
    DESTINATION ${KDE4_DATA_INSTALL_DIR}/katepart/script/indentation
    FILES_MATCHING PATTERN "*.js"
)

# install default files files
# this is a place where scripts can store additional data
install(
    DIRECTORY files
    DESTINATION ${KDE4_DATA_INSTALL_DIR}/katepart/script/files
)
