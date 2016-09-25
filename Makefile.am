ACLOCAL_AMFLAGS = -I config

SUBDIRS = doc include

pkgconfig_DATA = libhybridspi.pc

#
# Library 
#
lib_LTLIBRARIES = src/libhybridspi.la

src_libhybridspi_la_SOURCES = \
    src/hybridspi/common.cpp \
    src/hybridspi/xml.cpp \
    src/hybridspi/binary.cpp \
    src/hybridspi/mot.cpp \
    src/hybridspi/programmes.cpp \
    src/hybridspi/groups.cpp \
    src/hybridspi/services.cpp 

src_libhybridspi_la_CPPFLAGS = -Iinclude ${LIBMOT_CFLAGS} ${TINYXML2_CFLAGS}
src_libhybridspi_la_CXXFLAGS = @LIBHYBRIDSPI_EXTRA_CXXFLAGS@
src_libhybridspi_la_LIBADD = ${LIBMOT_LIBS} ${TINYXML2_LIBS}

# 
# Binaries
#
bin_PROGRAMS = \
    src/write_binary \
    src/decode_si_xml \
    examples/marshall_serviceinfo_minimal_xml \
    examples/marshall_serviceinfo_minimal_binary \
    examples/marshall_programmeinfo_minimal_xml \
    examples/marshall_programmeinfo_spec_xml \
    examples/unmarshall_serviceinfo_spec_xml \
    examples/unmarshall_programmeinfo_xml 

src_write_binary_SOURCES = src/write_binary.cpp
src_write_binary_CPPFLAGS = -Iinclude
src_write_binary_LDADD = -Lsrc/.libs -lhybridspi

src_decode_si_xml_SOURCES = src/decode_si_xml.cpp
src_decode_si_xml_CPPFLAGS =  -Iinclude ${TINYXML2_CFLAGS}
src_decode_si_xml_LDADD = -Lsrc/.libs -lhybridspi ${TINYXML2_LIBS}

examples_marshall_serviceinfo_minimal_xml_SOURCES = examples/marshall_serviceinfo_minimal_xml.cpp
examples_marshall_serviceinfo_minimal_xml_CPPFLAGS = -Iinclude ${TINYXML2_CFLAGS}
examples_marshall_serviceinfo_minimal_xml_LDADD = -Lsrc/.libs -lhybridspi

examples_marshall_serviceinfo_minimal_binary_SOURCES = examples/marshall_serviceinfo_minimal_binary.cpp
examples_marshall_serviceinfo_minimal_binary_CPPFLAGS = -Iinclude
examples_marshall_serviceinfo_minimal_binary_LDADD = -Lsrc/.libs -lhybridspi

examples_marshall_programmeinfo_minimal_xml_SOURCES = examples/marshall_programmeinfo_minimal_xml.cpp
examples_marshall_programmeinfo_minimal_xml_CPPFLAGS = -Iinclude ${TINYXML2_CFLAGS}
examples_marshall_programmeinfo_minimal_xml_LDADD = -Lsrc/.libs -lhybridspi

examples_marshall_programmeinfo_spec_xml_SOURCES = examples/marshall_programmeinfo_spec_xml.cpp
examples_marshall_programmeinfo_spec_xml_CPPFLAGS = -Iinclude ${TINYXML2_CFLAGS}
examples_marshall_programmeinfo_spec_xml_LDADD = -Lsrc/.libs -lhybridspi

examples_unmarshall_serviceinfo_spec_xml_SOURCES = examples/unmarshall_serviceinfo_spec_xml.cpp
examples_unmarshall_serviceinfo_spec_xml_CPPFLAGS = -Iinclude ${TINYXML2_CFLAGS}
examples_unmarshall_serviceinfo_spec_xml_LDADD = -Lsrc/.libs -lhybridspi

examples_unmarshall_programmeinfo_xml_SOURCES = examples/unmarshall_programmeinfo_xml.cpp
examples_unmarshall_programmeinfo_xml_CPPFLAGS = -Iinclude ${TINYXML2_CFLAGS}
examples_unmarshall_programmeinfo_xml_LDADD = -Lsrc/.libs -lhybridspi

#
# Tests
#
test_apps = \
    tests/test_encode_number \
    tests/test_encode_string \
    tests/test_encode_timepoint \
    tests/test_encode_cdata \
    tests/test_encode_attribute \
    tests/test_encode_bearer \
    tests/test_encode_name \
    tests/test_encode_header_parameters \
    tests/test_parse_duration

tests_test_encode_attribute_SOURCES = tests/test_encode_attribute.cpp 
tests_test_encode_attribute_CPPFLAGS = -Iinclude ${CPPUNIT_CFLAGS}
tests_test_encode_attribute_LDADD = -Lsrc/.libs -lhybridspi ${CPPUNIT_LIBS}

tests_test_encode_cdata_SOURCES = tests/test_encode_cdata.cpp 
tests_test_encode_cdata_CPPFLAGS = -Iinclude ${CPPUNIT_CFLAGS}
tests_test_encode_cdata_LDADD = -Lsrc/.libs -lhybridspi ${CPPUNIT_LIBS}

tests_test_encode_string_SOURCES = tests/test_encode_string.cpp 
tests_test_encode_string_CPPFLAGS = -Iinclude ${CPPUNIT_CFLAGS}
tests_test_encode_string_LDADD = -Lsrc/.libs -lhybridspi ${CPPUNIT_LIBS}

tests_test_encode_number_SOURCES = tests/test_encode_number.cpp 
tests_test_encode_number_CPPFLAGS = -Iinclude ${CPPUNIT_CFLAGS}
tests_test_encode_number_LDADD = -Lsrc/.libs -lhybridspi ${CPPUNIT_LIBS}

tests_test_encode_timepoint_SOURCES = tests/test_encode_timepoint.cpp 
tests_test_encode_timepoint_CPPFLAGS = -Iinclude ${CPPUNIT_CFLAGS}
tests_test_encode_timepoint_LDADD = -Lsrc/.libs -lhybridspi ${CPPUNIT_LIBS}

tests_test_encode_name_SOURCES = tests/test_encode_name.cpp 
tests_test_encode_name_CPPFLAGS = -Iinclude ${CPPUNIT_CFLAGS}
tests_test_encode_name_LDADD =  -Lsrc/.libs -lhybridspi ${CPPUNIT_LIBS}

tests_test_encode_bearer_SOURCES = tests/test_encode_bearer.cpp 
tests_test_encode_bearer_CPPFLAGS = -Iinclude ${CPPUNIT_CFLAGS}
tests_test_encode_bearer_LDADD = -Lsrc/.libs -lhybridspi ${CPPUNIT_LIBS}

tests_test_encode_header_parameters_SOURCES = tests/test_encode_header_parameters.cpp 
tests_test_encode_header_parameters_CPPFLAGS = -Iinclude ${LIBMOT_CFLAGS} ${CPPUNIT_CFLAGS}
tests_test_encode_header_parameters_LDADD = -Lsrc/.libs -lhybridspi ${LIBMOT_LIBS} ${CPPUNIT_LIBS}

tests_test_parse_duration_SOURCES = tests/test_parse_duration.cpp 
tests_test_parse_duration_CPPFLAGS = -Iinclude ${LIBMOT_CFLAGS} ${CPPUNIT_CFLAGS}
tests_test_parse_duration_LDADD = -Lsrc/.libs -lhybridspi ${LIBMOT_LIBS} ${CPPUNIT_LIBS}

check_PROGRAMS = ${test_apps}

#  Run the test cases
TESTS = $(test_apps)
XFAIL_TESTS =

EXTRA_DIST	= COPYING NEWS README.md INSTALL.md LICENCE AUTHORS ChangeLog TODO doc
