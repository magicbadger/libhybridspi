#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.61)

AC_INIT([libhybridspi], [0.0.1], [magicbadger@gmail.com])

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([config])
AC_CONFIG_HEADERS([src/config.h])
AM_INIT_AUTOMAKE(foreign subdir-objects tar-ustar dist-zip)

AX_CREATE_PKGCONFIG_INFO

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# This lets us use PACKAGE_VERSION in Makefiles
AC_SUBST(PACKAGE_VERSION)

# Libtool -version-info (ABI version)
LTVER="5:0:0"
AC_SUBST(LTVER)

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
AC_PROG_SED
AC_PROG_AWK
PKG_PROG_PKG_CONFIG
AM_PATH_CPPUNIT(1.9.6)
m4_pattern_forbid([^PKG_[A-Z_]+$], [missing some pkg-config macros (pkg-config package)])

# Libtool configuration for different targets. See acinclude.m4
AC_ARG_VAR([XMLTO], [Path to xmlto command])
AC_PATH_PROG([XMLTO], [xmlto])
AC_ARG_VAR([ASCIIDOC], [Path to asciidoc command])
AC_PATH_PROG([ASCIIDOC], [asciidoc])
LIBHYBRIDSPI_CONFIG_LIBTOOL
AC_LIBTOOL_WIN32_DLL
AC_PROG_LIBTOOL

# Check whether to build a with debug symbols
LIBHYBRIDSPI_CHECK_ENABLE_DEBUG

# By default compiling with -Werror except OSX.
libhybridspi_werror="yes"

# By default use DSO visibility
libhybridspi_dso_visibility="yes"

# Platform specific checks
libhybridspi_on_mingw32="no"
libhybridspi_on_cygwin="no"
libhybridspi_on_android="no"
libhybridspi_on_linux="no"

# Set some default features required
CPPFLAGS="$CPPFLAGS"

# For host type checks
AC_CANONICAL_HOST

# OS-specific tests
case "${host_os}" in
    *linux*)
        # Define on Linux to enable all library features. Define if using a gnu compiler
        if test "x$GXX" = "xyes"; then
            CPPFLAGS="-D_GNU_SOURCE $CPPFLAGS"
        fi
        AC_DEFINE(HYBRIDSPI_HAVE_LINUX, 1, [Have Linux OS])
        libhybridspi_on_linux="yes"

        case "${host_os}" in
            *android*)
                AC_DEFINE(HYBRIDSPI_HAVE_ANDROID, 1, [Have Android OS])
                libhybridspi_on_android="yes"
            ;;
        esac
        ;;
    *freebsd*)
        # Define on FreeBSD to enable all library features
        CPPFLAGS="-D__BSD_VISIBLE $CPPFLAGS"
        AC_DEFINE(HYBRIDSPI_HAVE_FREEBSD, 1, [Have FreeBSD OS])
        ;;
    *darwin*)
        # Define on Darwin to enable all library features
        CPPFLAGS="-D_DARWIN_C_SOURCE $CPPFLAGS"
        libhybridspi_pedantic="no"
        libhybridspi_werror="no"
        AC_DEFINE(HYBRIDSPI_HAVE_OSX, 1, [Have DarwinOSX OS])
        ;;
    *mingw*)
        AC_DEFINE(HYBRIDSPI_HAVE_WINDOWS, 1, [Have Windows OS])
        AC_DEFINE(HYBRIDSPI_HAVE_MINGW32, 1, [Have MinGW32])
        AC_CHECK_HEADERS(windows.h)
        AC_CHECK_LIB(ws2_32, main, ,
            [AC_MSG_ERROR([cannot link with ws2_32.dll.])])
        AC_CHECK_LIB(rpcrt4, main, ,
            [AC_MSG_ERROR([cannot link with rpcrt4.dll.])])
        AC_CHECK_LIB(iphlpapi, main, ,
            [AC_MSG_ERROR([cannot link with iphlpapi.dll.])])
        # mingw32 defines __int64_t as long long
        libhybridspi_on_mingw32="yes"
        libhybridspi_dso_visibility="no"

        if test "x$enable_static" = "xyes"; then
            AC_MSG_ERROR([Building static libraries is not supported under MinGW32])
        fi
        
	# Set FD_SETSIZE to 1024
	CPPFLAGS=" -DFD_SETSIZE=1024 $CPPFLAGS"
        ;;
    *cygwin*)
        # Define on Cygwin to enable all library features
        CPPFLAGS="-D_GNU_SOURCE $CPPFLAGS"
        AC_DEFINE(HYBRIDSPI_HAVE_CYGWIN, 1, [Have Cygwin])
        libhybridspi_on_cygwin="yes"
        libhybridspi_dso_visibility="no"
        if test "x$enable_static" = "xyes"; then
            AC_MSG_ERROR([Building static libraries is not supported under Cygwin])
        fi
        ;;
    *)
        AC_MSG_ERROR([unsupported system: ${host_os}.])
        ;;
esac

# Checks for libraries
PKG_CHECK_MODULES([LIBMOT], [libmot]) # libmot
PKG_CHECK_MODULES([TINYXML2], [tinyxml2 >= 3.0.0]) # tinyxml2

# Check whether to build docs / install man pages
LIBHYBRIDSPI_CHECK_DOC_BUILD

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(\
    errno.h \
    time.h \
    unistd.h \
    limits.h \
    stddef.h \
    stdlib.h \
    string.h \
    arpa/inet.h \
    netinet/tcp.h \
    netinet/in.h \
    sys/socket.h \
    sys/time.h)

AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_HEADER_TIME
AC_TYPE_UINT32_T
AC_C_VOLATILE

AM_CONDITIONAL(ON_MINGW, test "x$libhybridspi_on_mingw32" = "xyes")
AM_CONDITIONAL(ON_CYGWIN, test "x$libhybridspi_on_cygwin" = "xyes")
AM_CONDITIONAL(ON_ANDROID, test "x$libhybridspi_on_android" = "xyes")
AM_CONDITIONAL(ON_LINUX, test "x$libhybridspi_on_linux" = "xyes")

# Checks for library functions.
AC_TYPE_SIGNAL
AC_CHECK_FUNCS(perror gettimeofday clock_gettime memset socket getifaddrs freeifaddrs fork posix_memalign)
AC_CHECK_HEADERS([alloca.h])

AM_CONDITIONAL(HAVE_FORK, test "x$ac_cv_func_fork" = "xyes")

# Subst LIBHYBRIDSPI_EXTRA_CFLAGS & CXXFLAGS & LDFLAGS
AC_SUBST(LIBHYBRIDSPI_EXTRA_CFLAGS)
AC_SUBST(LIBHYBRIDSPI_EXTRA_CXXFLAGS)
AC_SUBST(LIBHYBRIDSPI_EXTRA_LDFLAGS)

# set pkgconfigdir, allow override
AC_ARG_WITH([pkgconfigdir],
    AS_HELP_STRING([--with-pkgconfigdir=PATH],
        [Path to the pkgconfig directory [[LIBDIR/pkgconfig]]]),
    [pkgconfigdir="$withval"],
    [pkgconfigdir='${libdir}/pkgconfig'])
AC_SUBST([pkgconfigdir])

AC_CONFIG_FILES([ \
    Makefile \
    include/Makefile \
    libhybridspi.pc \
    doc/Makefile])

AC_OUTPUT
