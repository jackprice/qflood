AC_INIT([qflood], 0.1, [http://www.quetuo.net/qflood])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

AC_CHECK_LIB([pthread], [pthread_create], , AC_MSG_ERROR[Missing -lpthread])

AC_HEADER_STDC
AC_INCLUDES_DEFAULT
AC_OUTPUT
