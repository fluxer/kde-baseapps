########### next target ###############

set(katexmltoolsplugin_PART_SRCS
    pseudo_dtd.cpp
    plugin_katexmltools.cpp
)

kde4_add_plugin(katexmltoolsplugin ${katexmltoolsplugin_PART_SRCS})

target_link_libraries(katexmltoolsplugin
    ${KDE4_KDECORE_LIBS}
    ${KDE4_KTEXTEDITOR_LIBS}
    kateinterfaces
)

install(
    TARGETS katexmltoolsplugin
    DESTINATION ${KDE4_PLUGIN_INSTALL_DIR}
)

########### install files ###############

install(
    FILES ui.rc
    DESTINATION ${KDE4_DATA_INSTALL_DIR}/kate/plugins/katexmltools
)
install(
    FILES
    html4-loose.dtd.xml
    html4-strict.dtd.xml
    kde-docbook.dtd.xml
    simplify_dtd.xsl
    xhtml1-frameset.dtd.xml
    xhtml1-strict.dtd.xml
    xhtml1-transitional.dtd.xml
    xslt-1.0.dtd.xml
    testcases.xml
    language.dtd.xml
    kpartgui.dtd.xml
    kcfg.dtd.xml
    DESTINATION ${KDE4_DATA_INSTALL_DIR}/katexmltools
)
install(
    FILES katexmltools.desktop
    DESTINATION ${KDE4_SERVICES_INSTALL_DIR}
)
