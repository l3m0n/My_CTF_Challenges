dnl $Id$
dnl config.m4 for extension encrypt_php

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(encrypt_php, for encrypt_php support,
dnl Make sure that the comment is aligned:
dnl [  --with-encrypt_php             Include encrypt_php support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(encrypt_php, whether to enable encrypt_php support,
dnl Make sure that the comment is aligned:
[  --enable-encrypt_php           Enable encrypt_php support])

if test "$PHP_ENCRYPT_PHP" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-encrypt_php -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/encrypt_php.h"  # you most likely want to change this
  dnl if test -r $PHP_ENCRYPT_PHP/$SEARCH_FOR; then # path given as parameter
  dnl   ENCRYPT_PHP_DIR=$PHP_ENCRYPT_PHP
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for encrypt_php files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       ENCRYPT_PHP_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$ENCRYPT_PHP_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the encrypt_php distribution])
  dnl fi

  dnl # --with-encrypt_php -> add include path
  dnl PHP_ADD_INCLUDE($ENCRYPT_PHP_DIR/include)

  dnl # --with-encrypt_php -> check for lib and symbol presence
  dnl LIBNAME=encrypt_php # you may want to change this
  dnl LIBSYMBOL=encrypt_php # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $ENCRYPT_PHP_DIR/lib, ENCRYPT_PHP_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_ENCRYPT_PHPLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong encrypt_php lib version or lib not found])
  dnl ],[
  dnl   -L$ENCRYPT_PHP_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(ENCRYPT_PHP_SHARED_LIBADD)

  PHP_NEW_EXTENSION(encrypt_php, encrypt_php.c, $ext_shared)
fi
