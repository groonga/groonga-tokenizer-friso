AM_CFLAGS =					\
	$(GROONGA_CFLAGS)			\
	$(FRISO_CFLAGS)

AM_LDFLAGS =					\
	-avoid-version				\
	-module					\
	-no-undefined				\
	$(FRISO_LDFLAGS)

LIBS =						\
	$(GROONGA_LIBS)				\
	$(FRISO_LIBS)

tokenizers_plugins_LTLIBRARIES =
tokenizers_plugins_LTLIBRARIES += friso.la

tokenizers_plugins_SOURCE =			\
	friso.c
