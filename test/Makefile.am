bin_PROGRAMS = encode_directory test_header_parameters

encode_directory_SOURCES = encode_directory.cpp
encode_directory_CPPFLAGS = -I${libmot_CFLAGS} -I${libmsc_CFLAGS}
encode_directory_LDADD = -L../src -lhybridspi -L${libmot_LIBS} -lmot -L${libmsc_LIBS} -lmsc

test_header_parameters_SOURCES = test_header_parameters.cpp
test_header_parameters_CPPFLAGS = -I../src -I${libmot_CFLAGS} 
test_header_parameters_LDADD = -L../src -lhybridspi -L${libmot_LIBS} -lmot
