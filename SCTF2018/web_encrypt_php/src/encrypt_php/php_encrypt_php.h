/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2013 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_ENCRYPT_PHP_H
#define PHP_ENCRYPT_PHP_H

extern zend_module_entry encrypt_php_module_entry;
#define phpext_encrypt_php_ptr &encrypt_php_module_entry

#define PHP_ENCRYPT_PHP_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_ENCRYPT_PHP_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_ENCRYPT_PHP_API __attribute__ ((visibility("default")))
#else
#	define PHP_ENCRYPT_PHP_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(encrypt_php);
PHP_MSHUTDOWN_FUNCTION(encrypt_php);
PHP_RINIT_FUNCTION(encrypt_php);
PHP_RSHUTDOWN_FUNCTION(encrypt_php);
PHP_MINFO_FUNCTION(encrypt_php);

//PHP_FUNCTION(confirm_encrypt_php_compiled);	/* For testing, remove later. */
PHP_FUNCTION(welcome_to_sctf2018);

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(encrypt_php)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(encrypt_php)
*/

/* In every utility function you add that needs to use variables 
   in php_encrypt_php_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as ENCRYPT_PHP_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define ENCRYPT_PHP_G(v) TSRMG(encrypt_php_globals_id, zend_encrypt_php_globals *, v)
#else
#define ENCRYPT_PHP_G(v) (encrypt_php_globals.v)
#endif

#endif	/* PHP_ENCRYPT_PHP_H */

#define CAKEY  "YP68y3FsMDc6TvRgghq"
//如果只允许执行加过密的php文件 设置STRICT_MODE为1
//set STRICT_MODE to 1 if you only want the crypted php files to be executed
#define STRICT_MODE 1
const int maxBytes = 1024*1024*2;

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
