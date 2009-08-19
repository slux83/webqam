#!/bin/bash

#
# A simple svn cleaner (remove all .svn dir from the ./ recursivelly)
#

for dir in `find . | grep .svn$`
do
	echo "* deleting $dir"
#	rm -rf $dir
	echo "  DONE!"
	echo
done

#rm -f svn_clear.sh
