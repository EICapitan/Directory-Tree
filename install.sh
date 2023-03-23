#! /usr/bin/env bash
echo "DirectoryTree Show ver. atdt-1.0.0"

REQUIRE="cmake make"
perm=${1:-"perm"}
NL=$'\n'

function jmp
{
    label=$1
    cmd=$(sed -n "/$label:/{:a;n;p;ba};" $0 | grep -v ':$')
    eval "$cmd"
    exit
}

echo -n "Checking permissions..."
if [ "$EUID" -ne 0 ]
  then echo " error: permission denied. Maybe your not a root?"
  exit
fi
echo " OK"

jmp $perm

perm:
echo -n "Checking dependencies... "
for x in $REQUIRE; do
    INSTALLED=$(pacman -Q $x)

    set -- $INSTALLED
    if [ "$1" != "$x" ]
    then
        echo -n "$x package not installed, install? [y/n]: "
        read input

        if [ "$input" == "y" ]
        then
            yes | pacman -Sy $x
        fi
        jmp $perm
    fi
done
echo " OK"

echo "Running CMAKE..."
cmake .
echo "Running MAKE..."
make

echo -n "Copy executable file "ds" to /usr/bin folder? [y/n]: "
read input

if [ "$input" == "y" ] 
then
    cp dtree /usr/bin/ds
fi
echo "Done!"