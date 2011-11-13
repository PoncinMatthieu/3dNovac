#!/bin/sh

usage='./GenerateDoc [-f, clean] [all, Nc-LibName]'

if [ $# -eq 0 ]; then
    action='--help'
else
    action=$1
fi

# firefox
# clean
launchFirefox=0;
case $action in
    -f)
	launchFirefox=1;
	action=$2;;
    clean)
	rm -Rf ./html/*;
	action=$2;;
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
if [ $launchFirefox -eq 1 ]; then
    firefox ./html/index.html &
fi
