project(konq-plugins)

include_directories (${KDEBASE_APPS_SOURCE_DIR}/lib/konq)

add_subdirectory( kimgalleryplugin )
add_subdirectory( dirfilter )
add_subdirectory( uachanger )
add_subdirectory( babelfish )

#add_subdirectory( quickprint )
add_subdirectory( kttsplugin )

if(UNIX)
      add_subdirectory( shellcmdplugin )
endif(UNIX)

add_subdirectory( imagerotation )
#add_subdirectory( microformat )
add_subdirectory( autorefresh )
add_subdirectory( fsview )
add_subdirectory( searchbar )

add_subdirectory( akregator )
