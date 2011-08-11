#!/bin/sh

usage='./GenerateDoc [clean] [all, Nc-LibName]'

if [ $# -eq 0 ]; then
    action='--help'
else
    action=$1
fi

# clean
case $action in
    clean)
	rm -Rf ./html/*;
	action=$2;
esac

# execute doxygen
cd ./Build/
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

# copy les images
cp ./Build/Images/* ./html/

# execute firefox
firefox ./html/index.html &
