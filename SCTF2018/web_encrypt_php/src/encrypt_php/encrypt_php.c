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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_encrypt_php.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <zlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "md5.h"
#include "aes.c"
#include "aes_crypt.c"

/* If you declare any globals in php_encrypt_php.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(encrypt_php)
*/

// PHP_MINIT_FUNCTION(encrypt_php);
// PHP_MSHUTDOWN_FUNCTION(encrypt_php);
// PHP_MINFO_FUNCTION(encrypt_php);

/* True global resources - no need for thread safety here */
static int le_encrypt_php;

/**
 * some function
 */

__attribute__ ((visibility ("hidden")))char *strlwr(char *s){
    char *p;
    for (p=s; *p; p++){
        if (*p >= 'A' && *p <= 'Z')
            *p = *p - 'A' + 'a';
    }
    return s;
}

__attribute__ ((visibility ("hidden")))void xor_enc(char *s){
    char *p;
    for (p=s; *p; p++){
        *p = *p ^ 0x9a;
    }
    return s;
}

__attribute__ ((visibility ("hidden")))int is_php_content(const char *datap){
    char *temp;
    temp = (char*)malloc(maxBytes);
    memset(temp, 0, strlen(datap) + 1);
    memcpy(temp, datap, strlen(datap) + 1);

    // string to lower
    strlwr(temp);

    // check php content
    if(strstr(temp, "<?php") != NULL || strstr(temp, "<?=") != NULL || strstr(temp, "<script") != NULL){
        return 1;
    }

    free(temp);
    return 0;
}

__attribute__ ((visibility ("hidden")))FILE *encrypt_ext_fopen(FILE *fp){
  struct  stat  stat_buf;
  char *cdatap, *udatap;
  char  *datap, *newdatap;
  char lenBuf[16];
  uLong flen, ulen;
  int datalen, newdatalen=0;
  int i;
  uint8_t enTag[16];
  uint8_t key[64];

  memset(key, 0, sizeof(key));
  memcpy(key, md5(CAKEY), 32);
  memcpy(enTag, key, 16);
  memset(lenBuf, 0, 16);
  
  // fstat(fileno(fp), &stat_buf);
  // datalen = stat_buf.st_size;
  fread(&ulen, sizeof(uLong), 1, fp);
  fread(&flen, sizeof(uLong), 1, fp);

  datap = (char*)malloc(maxBytes);
  memset(datap, 0, sizeof(datap));

  fread(datap, sizeof(char), flen, fp);
  fclose(fp);

  if(STRICT_MODE){
    if(is_php_content(datap)){
        fp = tmpfile();
        fwrite(datap, 0, 1, fp);
        free(datap);
        rewind(fp);
        return fp;
    }
  }

  xor_enc(datap);

  char* o2stream = (char *)malloc(maxBytes);
  memset(o2stream, 0, sizeof(o2stream));
  uncompress(o2stream, &ulen, datap, flen);

  aes_crypt(0,o2stream,ulen,key,&ulen);

  fp = tmpfile();
  fwrite(o2stream, sizeof(char), ulen, fp);

  free(datap);
  free(o2stream);
  rewind(fp);
  return fp;
}

ZEND_API zend_op_array *(*org_compile_file)(zend_file_handle *file_handle, int type TSRMLS_DC);

ZEND_API zend_op_array *encrypt_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC){
  FILE  *fp;
  char  fname[32];

  memset(fname, 0, sizeof fname);
  if (zend_is_executing(TSRMLS_C)) {
    if (get_active_function_name(TSRMLS_C)) {
      strncpy(fname, get_active_function_name(TSRMLS_C), sizeof fname - 2);
    }
  }
  if (fname[0]) {
    if ( strcasecmp(fname, "show_source") == 0
      || strcasecmp(fname, "highlight_file") == 0) {
      return NULL;
    }
  }

  // deal with some protocol problem
  // for example: php://filter/convert.base64-encode/resource=/etc/passwd
  if(strstr(file_handle->filename, "://") != NULL){
    return org_compile_file(file_handle, type);
  }

  fp = fopen(file_handle->filename, "rb+");
  if (!fp) {
    fp = zend_fopen(file_handle->filename, &file_handle->opened_path);
    if (!fp) {
      return org_compile_file(file_handle, type);
    }
  }

  if (file_handle->type == ZEND_HANDLE_FP) fclose(file_handle->handle.fp);
  if (file_handle->type == ZEND_HANDLE_FD) close(file_handle->handle.fd);
  
  file_handle->handle.fp = encrypt_ext_fopen(fp);
  //file_handle->handle.fp = fp;

  file_handle->type = ZEND_HANDLE_FP;

  return org_compile_file(file_handle, type);
}


/* {{{ encrypt_php_functions[]
 *
 * Every user visible function must have an entry in encrypt_php_functions[].
 */
const zend_function_entry encrypt_php_functions[] = {
	//PHP_FE(confirm_encrypt_php_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE(welcome_to_sctf2018,	NULL)
	PHP_FE_END	/* Must be the last line in encrypt_php_functions[] */
};
/* }}} */

/* {{{ encrypt_php_module_entry
 */
zend_module_entry encrypt_php_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"encrypt_php",
	encrypt_php_functions,
	PHP_MINIT(encrypt_php),
	PHP_MSHUTDOWN(encrypt_php),
    NULL,
    NULL,
	// PHP_RINIT(encrypt_php),		/* Replace with NULL if there's nothing to do at request start */
	// PHP_RSHUTDOWN(encrypt_php),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(encrypt_php),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_ENCRYPT_PHP_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_ENCRYPT_PHP
ZEND_GET_MODULE(encrypt_php)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("encrypt_php.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_encrypt_php_globals, encrypt_php_globals)
    STD_PHP_INI_ENTRY("encrypt_php.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_encrypt_php_globals, encrypt_php_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_encrypt_php_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_encrypt_php_init_globals(zend_encrypt_php_globals *encrypt_php_globals)
{
	encrypt_php_globals->global_value = 0;
	encrypt_php_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(encrypt_php)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
    CG(compiler_options) |= ZEND_COMPILE_EXTENDED_INFO;
    org_compile_file = zend_compile_file;
    zend_compile_file = encrypt_compile_file;
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(encrypt_php)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
    CG(compiler_options) |= ZEND_COMPILE_EXTENDED_INFO;
    zend_compile_file = org_compile_file;
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(encrypt_php)
{
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(encrypt_php)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(encrypt_php)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "encrypt_php support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_encrypt_php_compiled(string arg)
   Return a string to confirm that the module is compiled in */
// PHP_FUNCTION(confirm_encrypt_php_compiled)
// {
// 	char *arg = NULL;
// 	int arg_len, len;
// 	char *strg;

// 	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
// 		return;
// 	}

// 	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "encrypt_php", arg);
// 	RETURN_STRINGL(strg, len, 0);
// }
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/

/* {{{ proto string welcome_to_sctf2018(string str)
    */
PHP_FUNCTION(welcome_to_sctf2018)
{
	char *str = NULL;
	int argc = ZEND_NUM_ARGS();
	int str_len;

	if (zend_parse_parameters(argc TSRMLS_CC, "s", &str, &str_len) == FAILURE) 
		return;

    php_printf("I am Muhe, Welcome to sctf2018!");
	//php_error(E_WARNING, "welcome_to_sctf2018");
}
/* }}} */


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
