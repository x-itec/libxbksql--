
LTVERSION = $(MEINELIB_MAJOR_VERSION):$(MEINELIB_MINOR_VERSION):$(MEINELIB_MICRO_VERSION)

quellen = \
	xbkdbq.cpp

include_HEADERS = \
	xbkdbq.h 


lib_LTLIBRARIES = \
	libxbkdbq.la

libxbkdbq_la_SOURCES = $(quellen)
libxbkdbq_la_LDFLAGS = -version-info $(LTVERSION)

