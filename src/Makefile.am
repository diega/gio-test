bin_PROGRAMS = giotest

giotest_CFLAGS = \
	$(GLIB_CFLAGS) \
	$(GIO_CFLAGS) \
	$(PROTOBUF_C_CFLAGS)
giotest_LDFLAGS = \
	$(GLIB_LIBS) \
	$(GIO_LIBS) \
	$(PROTOBUF_C_LIBS)
giotest_SOURCES = cast_channel.pb-c.h \
                  cast_channel.pb-c.c \
                  client.c

