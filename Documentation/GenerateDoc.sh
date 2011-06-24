#!/bin/sh

usage='./GenerateDoc [all, Nc-LibName]'

if [ $# -eq 0 ]; then
    action='--help'
else
    action=$1
fi

cd Build/
case $action in
    --help)
	echo $usage
	exit;;
    all)
	doxygen "doxy.conf";;
    *)
	doxygen "doxy-$action.conf";;
esac

cd ../
firefox html/index.html &