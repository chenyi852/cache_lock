#!/bin/bash

exename=$(basename $0)

echo $exename

case $1 in
	copy)

	;;

	clone)

	;;

	add)
	git add -u .; git commit -m "`date`"

	;;
	*)
	;;
esac
