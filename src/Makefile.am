bin_PROGRAMS = diskmbr
noinst_LTLIBRARIES = libget_info.la libprint_info.la libread_mbr.la

diskmbr_SOURCES = diskmbr.c #read_mbr.c print_info.c
diskmbr_LDADD = libget_info.la libprint_info.la libread_mbr.la

libget_info_la_SOURCES = get_info.c get_info.h
libprint_info_la_SOURCES = print_info.c print_info.h
libread_mbr_la_SOURCES = read_mbr.c read_mbr.h
