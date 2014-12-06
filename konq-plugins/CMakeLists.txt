project(konq-plugins)

include_directories (${KDEBASE_APPS_SOURCE_DIR}/lib/konq)


add_subdirectory( akregator )
add_subdirectory( autorefresh )
add_subdirectory( babelfish )
add_subdirectory( dirfilter )
add_subdirectory( fsview )
add_subdirectory( imagerotation )
add_subdirectory( kimgalleryplugin )
add_subdirectory( kttsplugin )
add_subdirectory( searchbar )
add_subdirectory( shellcmdplugin )
add_subdirectory( uachanger )
# FIXME: requires obsolete kprinter, there is a fork of it
# known as kprinter4 at https://github.com/credativ/kprinter4
# add_subdirectory( quickprint )
