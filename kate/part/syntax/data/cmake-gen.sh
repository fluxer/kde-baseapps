#!/bin/bash
# Copyright 2008, 2009 Matthew Woehlke (mw_triad@users.sourceforge.net)
# Copyright 2013, Alex Turbov (i.zaufi@gmail.com)
#
# ATTENTION Since CMake 3.x this generator is broken! So last time
# (making version 1.31 of cmake.xml syntax) I had to use `kdiff3`
# to manually merge changes!
#
# TODO CMake 2 and 3 has some difference in syntax... So, this
# generator do not feasible anymore if we are trying to keep a single
# syntax files for both CMake generations. It must be fixed! (somehow)

export LC_ALL=C

# need cmake
CMAKE="$(type -P cmake)"
[ -x "$CMAKE" ] || exit 1
echo "found cmake... $CMAKE"

t=.tmp_cmake$$

# Get cmake version
CMAKE_VERSION="$("$CMAKE" --help | sed -n '1p')"

count() {
    wc -l $t.$1 | awk '{print $1}'
}

# Extract before/after command list
sed -n -e '/<list.*"commands"/{p;q}' -e 'p' cmake.xml > $t.1
sed -e '/<\/list\s*>/ba' -e 'd' -e ':a' -e 'n;ba' cmake.xml > $t._2
sed -n -e '/<list.*"special_args"/{p;q}' -e 'p' $t._2 > $t.2
sed "1,$(wc -l < $t.2)d" $t._2 | sed -e '/<\/list\s*>/ba' -e 'd' -e ':a' -e 'n;ba' > $t._3
sed -n -e '/<list.*"properties"/{p;q}' -e 'p' $t._3 > $t.3
sed "1,$(wc -l < $t.3)d" $t._3 | sed -e '/<\/list\s*>/ba' -e 'd' -e ':a' -e 'n;ba' > $t._4
sed -n -e '/<list.*"cmake_vars"/{p;q}' -e 'p' $t._4 > $t.4
sed "1,$(wc -l < $t.4)d" $t._4 | sed -e '/<\/list\s*>/ba' -e 'd' -e ':a' -e 'n;ba' > $t._5
sed -n -e '/<context.*"Detect More Builtin Variables"/{p;q}' -e 'p' $t._5 > $t.5
sed "1,$(wc -l < $t.5)d" $t._5 | sed -e '/<\/context\s*>/ba' -e 'd' -e ':a' -e 'n;ba' > $t.6

$CMAKE --help-command-list | sed '1d' | sort > $t.commands
echo "$(count commands) commands"

extract_args() {
    sed -e '/^\s\+'"$1"'(/ba' \
        -e 'd' \
        -e ':a' \
        -e '/)/{s/^.*(\(.*\)).*$/\1/p;d}' \
        -e 'N;s/\n/ /;ba' | \
    sed -e 's/[][]//g' -e 's/|\| \+/\n/g' -e 's/<[[:upper:][:digit:]_]\+>//g' -e 's/[<>]//g' | \
    sed -n '/^[[:upper:][:digit:]_]\+$/p' | \
    # NOTE Remove some false-positives:
    # 0) one-letter-commands -- found from usage examples
    # 1) CMP<NNN> from cmake_policy will be handled individually
    # 2) fix incorrect parsing of separate_arguments(): append UNIX_COMMAND
    # 3) 'VAR' actually is not a part of any command!
    sed -e '/^[A-Z]$/d' -e '/^CMP$/d' -e 's/\(WINDOWS_COMMAND\)/\1\nUNIX_COMMAND/' -e '/^VAR[0-9]*$/d' \
    >> $t.args
}

while read COMMAND ; do
    echo "# Getting args of '$COMMAND' command" >>$t.args
    "$CMAKE" --help-command $COMMAND | extract_args $COMMAND
done < $t.commands
sed '/^#/d' $t.args | sort -u > $t.argsu
echo "$(count args) arguments, $(count argsu) unique"
$CMAKE --help-property-list | sed -e '1d' -e '/[<>]/d' | sort -u > $t.props
echo "$(count props) properties"

# Get builtin CMake variables list
$CMAKE --help-variable-list | sed -e '1d' | grep -v 'Project name' > $t.all_vars
grep '^[A-Za-z_0-9]\+\s*$' $t.all_vars | sort -u > $t.vars
grep -v '^[A-Za-z_0-9]\+\s*$' $t.all_vars \
  | sed 's,<LANG>,[A-Za-z_][A-Za-z_0-9]*,' \
  | sed 's,<CONFIG>,[A-Za-z_][A-Za-z_0-9]*,' \
  | sed 's,<PackageName>,[A-Za-z_][A-Za-z_0-9]*,' \
  | sed 's,<PROJECT-NAME>,[A-Za-z_][A-Za-z_0-9]*,' \
  | sed 's,CMP<NNNN>,CMP[0-9]+,' \
  | sed 's,\[CMAKE_BUILD_TYPE\],[A-Za-z_][A-Za-z_0-9]*,' \
  | sort -u \
  > $t.varsrr
echo "$(count all_vars) builtin variables"


# Generate new .xml
{
    sed '/<!-- generated for/s/^.*$/<!-- generated for "'"$($CMAKE --version)"'" -->/' $t.1
    echo "      <!-- generated list -->"
    sed 's!.*!      <item> & </item>!' $t.commands
    cat $t.2
    echo "      <!-- generated list -->"
    sed 's!.*!      <item> & </item>!' $t.argsu
    cat $t.3
    echo "      <!-- generated list -->"
    sed 's!.*!      <item> & </item>!' $t.props
    cat $t.4
    echo "      <!-- generated list -->"
    sed 's!.*!      <item> & </item>!' $t.vars
    cat $t.5
    echo "        <!-- generated rules -->"
    sed 's|.*|        <RegExpr attribute="Builtin CMake Variable" context="#stay" String="\\b&\\b" />|' $t.varsrr
    cat $t.6
} > cmake.xml

rm -f $t.*
echo "Remember to update the version!"

# kate: tab-width 4; indent-mode normal; indent-width 4;
