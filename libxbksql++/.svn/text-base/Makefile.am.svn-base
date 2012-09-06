
# fuer make dist-all
AUTOMAKE_OPTIONS = dist-bzip2 

SUBDIRS = xbksql++

# zusaetzliche Files fuer make dist-all

EXTRA_DIST = xbksql++ 

testmich:
	if test "$$use_debug" = "yes"; then
	echo "DEBUG!"   
	fi


ChangeLog:
	cvs2cl --utc --fsf -t -S --prune

