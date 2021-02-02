#!/usr/bin/env bash

##
# This script install the man pages splitting the 
# file names by "_", appending the string "prioqueue_"
# and copying it to $PREFIX/man/man3 with extension ".3".
# All files copied are compressed.
#
# When --uninstall flag is passed as argument the man files
# are deleted.
##

proj="prioqueue"
ext=".3"

function show_usage_and_exit {
    echo >&2 "usage: $0 --prefix PREFIX [--install|--uninstall]"
    exit 1
}

function install {
    INSTALL=$1
    PREFIX=$2
    for f in `ls man3/*.3`;
    do
	    for m in `basename $f ${ext} | awk -F "_" '{print $1" "$2" "$3}'`;
	    do
            fman3="${PREFIX}/man/man3/${proj}_${m}${ext}"
            if [ $INSTALL == true ];
            then
                install -d ${PREFIX}/man/man3
		        install ${f} ${fman3}
                gzip ${fman3}
            else
                rm -f "${fman3}.gz"
            fi
	    done
    done
}

if [ $# -ne 3 ];
then
    show_usage_and_exit
fi

while [[ $# -gt 0 ]] && [[ "$1" == "--"* ]] ;
do
    opt="$1";
    shift;
    case "$opt" in
        "--" ) break 2;;
        "--prefix" )
            PREFIX="$1"; shift;;
        "--install" )
            INSTALL=true; UNINSTALL=false;;
        "--uninstall" )
            INSTALL=false; UNINSTALL=true;;
        * ) show_usage_and_exit;;
    esac
done

install $INSTALL $PREFIX

exit 0
