TESTS = test_get_info test_print_info test_read_mbr
check_PROGRAMS = $(TESTS)
test_get_info_SOURCES = test_get_info.c
test_get_info_LDADD = $(top_srcdir)/src/libget_info.la

test_print_info_SOURCES = test_print_info.c
test_print_info_LDADD = $(top_builddir)/src/libprint_info.la

test_read_mbr_SOURCES = test_read_mbr.c
test_read_mbr_LDADD = $(top_builddir)/src/libread_mbr.la

AM_CFLAGS = -I$(top_srcdir)/src -L$(top_src_dir)/src
