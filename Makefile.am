ACLOCAL_AMFLAGS=-I config

#
# Library 
#
lib_LTLIBRARIES = src/libhybridspi.la

include_HEADERS = \
    include/hybridspi/programmes.h \
    include/hybridspi/groups.h \
    include/hybridspi/services.h \
    include/hybridspi/marshall.h \
    include/hybridspi/binary.h \
    include/hybridspi/xml.h \
    include/hybridspi/common.h

src_libhybridspi_la_SOURCES = \
    src/hybridspi/common.cpp \
    src/hybridspi/xml.cpp \
    src/hybridspi/binary.cpp \
    src/hybridspi/mot.cpp \
    src/hybridspi/programmes.cpp \
    src/hybridspi/groups.cpp \
    src/hybridspi/services.cpp 

src_libhybridspi_la_CPPFLAGS = -Iinclude -I${LIBMOT_CFLAGS} -Wno-shift-count-overflow
src_libhybridspi_la_CXXFLAGS = @LIBHYBRIDSPI_EXTRA_CXXFLAGS@
src_libhybridspi_la_LIBADD = -L${LIBMOT_LIBS}

# 
# Binaries
#
bin_PROGRAMS = src/write_binary

src_write_binary_SOURCES = \
    src/write_binary.cpp

src_write_binary_CPPFLAGS = -Iinclude
src_write_binary_LDADD = -Lsrc/.libs -lhybridspi


#
# Tests
#
test_apps = \
    tests/test_encode_timepoint \
    tests/test_encode_cdata \
    tests/test_build_name \
    tests/test_build_bearer \
    tests/test_serviceinfo \
    tests/serviceinfo_minimal_xml \
    tests/serviceinfo_minimal_binary

tests_test_encode_timepoint_SOURCES = tests/test_encode_timepoint.cpp 
tests_test_encode_timepoint_CPPFLAGS = -Iinclude
tests_test_encode_timepoint_LDADD = -Lsrc/.libs -lhybridspi

tests_test_encode_cdata_SOURCES = tests/test_encode_cdata.cpp 
tests_test_encode_cdata_CPPFLAGS = -Iinclude
tests_test_encode_cdata_LDADD = -Lsrc/.libs -lhybridspi

tests_test_build_name_SOURCES = tests/test_build_name.cpp 
tests_test_build_name_CPPFLAGS = -Iinclude
tests_test_build_name_LDADD = -Lsrc/.libs -lhybridspi

tests_test_build_bearer_SOURCES = tests/test_build_bearer.cpp 
tests_test_build_bearer_CPPFLAGS = -Iinclude
tests_test_build_bearer_LDADD = -Lsrc/.libs -lhybridspi

tests_test_serviceinfo_SOURCES = tests/test_serviceinfo.cpp 
tests_test_serviceinfo_CPPFLAGS = -Iinclude
tests_test_serviceinfo_LDADD = -Lsrc/.libs -lhybridspi

tests_serviceinfo_minimal_xml_SOURCES = tests/serviceinfo_minimal_xml.cpp
tests_serviceinfo_minimal_xml_CPPFLAGS = -Iinclude
tests_serviceinfo_minimal_xml_LDADD = -Lsrc/.libs -lhybridspi

tests_serviceinfo_minimal_binary_SOURCES = tests/serviceinfo_minimal_binary.cpp
tests_serviceinfo_minimal_binary_CPPFLAGS = -Iinclude
tests_serviceinfo_minimal_binary_LDADD = -Lsrc/.libs -lhybridspi

check_PROGRAMS = ${test_apps}

#  Run the test cases
TESTS = $(test_apps)
XFAIL_TESTS =

EXTRA_DIST	= COPYING NEWS README.md INSTALL.md LICENCE AUTHORS ChangeLog TODO doc
