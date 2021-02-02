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

PROJ="prioqueue"
EXT=".3"
SUDO=""

if [ -n `which sudo` ];
then
    SUDO="sudo"
fi

function show_usage_and_exit {
    echo >&2 "usage: $0 --prefix PREFIX [--install|--uninstall]"
    exit 1
}

function run {
    cmd="${SUDO} $1"
    echo "$cmd"
    eval "$cmd"
}

function install {
    INSTALL=$1
    PREFIX=$2
    MANDIR="${PREFIX}/man/man3"
    for f in `ls man3/*.troff`;
    do
	    for m in `basename ${f} .troff | awk -F "_" '{print $1" "$2" "$3}'`;
	    do
            if [ $INSTALL == true ];
            then    
                local manpage="${MANDIR}/${PROJ}_${m}${EXT}"            
                run "install -d ${MANDIR}"
		        run "install -m 644 ./${f} ${manpage}"
                run "gzip -f ${manpage}"
            else                
                local manpage="${MANDIR}/${PROJ}_${m}${EXT}.gz"
                run "rm -f ${manpage}"
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
