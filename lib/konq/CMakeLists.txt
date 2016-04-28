add_subdirectory( pics )
add_subdirectory( favicons )
add_subdirectory( Templates )
if(ENABLE_TESTING)
    add_subdirectory( tests )
endif()

########### libkonq ###############

# For crc32 in konq_historyloader.cpp
include_directories( ${ZLIB_INCLUDE_DIR} )

set(konq_LIB_SRCS
   konq_popupmenu.cpp       # used by konqueror, kfind, folderview, kickoff
   konq_popupmenuplugin.cpp # for KonqPopupMenu and its plugins
   konq_dndpopupmenuplugin.cpp # for KonqDndPopupMenu and its plugins
   konq_copytomenu.cpp         # used by dolphin, KonqPopupMenu
   konq_operations.cpp         # used by dolphin and konqueror
   konq_statusbarmessagelabel.cpp  # used by dolphin and konqueror
   konq_events.cpp
   konqmimedata.cpp         # used by dolphin, KonqOperations, some filemanagement konqueror modules.
   konq_historyentry.cpp
   konq_historyloader.cpp
   konq_historyprovider.cpp
   kversioncontrolplugin.cpp  # used by dolphin and its version control plugins (deprecated)
   kversioncontrolplugin2.cpp # used by dolphin and its version control plugins
)

add_library(konq ${LIBRARY_TYPE} ${konq_LIB_SRCS})

target_link_libraries(konq PRIVATE ${KDE4_KFILE_LIBS} ${ZLIB_LIBRARY} ${X11_X11_LIB})
target_link_libraries(konq PUBLIC ${KDE4_KPARTS_LIBS})

set_target_properties(konq PROPERTIES
    VERSION ${GENERIC_LIB_VERSION}
    SOVERSION ${GENERIC_LIB_SOVERSION}
)

install(TARGETS konq ${INSTALL_TARGETS_DEFAULT_ARGS})

########### konq_sound ###############

set(konq_sound_LIB_SRCS konq_sound.cc)
kde4_add_plugin(konq_sound ${konq_sound_LIB_SRCS})
target_link_libraries(konq_sound ${KDE4_KDECORE_LIBS} kmediaplayer)
install(TARGETS konq_sound DESTINATION ${PLUGIN_INSTALL_DIR} )

########### install files ###############

install(FILES
    directory_bookmarkbar.desktop
    DESTINATION ${DATA_INSTALL_DIR}/kbookmark
)

generate_export_header(konq)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/konq_export.h
    konq_popupmenu.h # used by folderview
    konq_popupmenuplugin.h
    konq_dndpopupmenuplugin.h
    # konq_copytomenu.h - anyone needs it?
    konq_operations.h
    konq_events.h
    konqmimedata.h
    kversioncontrolplugin.h
    kversioncontrolplugin2.h
    konq_historyprovider.h
    konq_historyentry.h
    DESTINATION ${INCLUDE_INSTALL_DIR}
    COMPONENT Devel
)
install(FILES
    konqpopupmenuplugin.desktop
    konqdndpopupmenuplugin.desktop
    DESTINATION ${SERVICETYPES_INSTALL_DIR}
)
