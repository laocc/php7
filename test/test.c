#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_test.h"

#include "ext/standard/md5.h"
//#include "zend_string.h"

/* 声明全局变量:
 ZEND_DECLARE_MODULE_GLOBALS(test)
 */

/* True global resources - no need for thread safety here */
static int le_test;

char *md(zend_string *str) {

	char md5str[33];
	PHP_MD5_CTX context;
	unsigned char digest[16];

	md5str[0] = '\0';
	PHP_MD5Init(&context);
	PHP_MD5Update(&context, ZSTR_VAL(str), ZSTR_LEN(str));
	PHP_MD5Final(digest, &context);
	make_digest_ex(md5str, digest, 16);

	return md5str;
}

PHP_FUNCTION(md6) {
	zend_string *arg;
	char *str;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "S", &arg) == FAILURE) {
		return;
	}
	str = md(arg);

	RETURN_STR(str);
}

PHP_FUNCTION(md7) {
	zend_string *arg;
	zend_string *md;
	zend_bool raw_output = 0;
	zend_bool isok = 0;
	char md5str[33];
	PHP_MD5_CTX context;
	unsigned char digest[16];
	char *code;
	size_t arg_len;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sS|b", &code, &arg_len, &arg,
			&raw_output) == FAILURE) {
		return;
	}

	md5str[0] = '\0';
	PHP_MD5Init(&context);
	PHP_MD5Update(&context, ZSTR_VAL(arg), ZSTR_LEN(arg));
	PHP_MD5Final(digest, &context);
	if (raw_output) {
		md = ((char *) digest, 16);
	} else {
		make_digest_ex(md5str, digest, 16);
		md = (md5str);
	}

	php_printf("%s\n%s\n", md, code);
	isok = 0 == strcmp(code, md);
	RETURN_BOOL(isok);
	return;
}

PHP_FUNCTION(str_test) {

	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}
	//	arg=md5(arg);

	strg = strpprintf(0, "%s\n", arg);
	RETURN_STR(strg);
}

/*
 * 本扩展所实现的所有函数
 */
const zend_function_entry
		test_functions[] = { PHP_FE(str_test, NULL) PHP_FE(md6, NULL)
		PHP_FE(md7, NULL)
		PHP_FE_END
	};

//
//zend_class_entry *cz_cz_ce;
//
//const zend_function_entry cz_class_functions[]={
//    PHP_ME(test, __construct,NULL,ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
//    PHP_ME(test,createApp,NULL,ZEND_ACC_PUBLIC)
//    PHP_FE_END
//};
//
//PHP_MINIT_FUNCTION(cz)
//{
//    zend_class_entry ce;
//    memset(&ce, 0, sizeof(zend_class_entry));
//    INIT_CLASS_ENTRY(ce,"test",cz_class_functions);
//    cz_cz_ce = zend_register_internal_class_ex(&ce,NULL,NULL TSRMLS_CC);
//    /*
//    cz_cz_ce->ce_flags |= ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;
//     * */
//    zend_declare_property_null(cz_cz_ce,ZEND_STRL("_test"),ZEND_ACC_PUBLIC TSRMLS_CC);
//}
//
//PHP_METHOD(test,__construct)
//{
//    zend_printf("The is Cz class __construct!!</br>");
//}
//PHP_METHOD(test,createApp)
//{
//    zend_printf("The is Cz lcass createApp !</br>");
//}
//


/* {{{ php_test_init_globals
 */
/* Uncomment this function if you have INI entries
 static void php_test_init_globals(zend_test_globals *test_globals)
 {
 test_globals->global_value = 0;
 test_globals->global_string = NULL;
 }
 */
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(test) {
	/* If you have INI entries, uncomment these lines
	 REGISTER_INI_ENTRIES();
	 */
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(test) {
	/* uncomment this line if you have INI entries
	 UNREGISTER_INI_ENTRIES();
	 */
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(test) {
#if defined(COMPILE_DL_TEST) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(test) {
	return SUCCESS;
}
/* }}} */

/**
 * 调用phpinfo()时模块信息函数被呼叫，从而打印出模块信息。
 */
PHP_MINFO_FUNCTION(test) {
	php_info_print_table_start();
	php_info_print_table_header(2, "test support", "enabled");
	php_info_print_table_row(2, "Version", PHP_TEST_VERSION);
	php_info_print_table_row(2, "Author", "Fazo(fazo@qq.com)");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	 DISPLAY_INI_ENTRIES();
	 */
}

//const zend_function_entry vtest_functions[] = {
//	PHP_FE(test, has,NULL)
//	PHP_FE_END
//};

/* }}} */

/* {{{ test_module_entry
 */
zend_module_entry test_module_entry = { STANDARD_MODULE_HEADER, "test",
test_functions,
PHP_MINIT(test),
PHP_MSHUTDOWN(test),
PHP_RINIT(test), /* Replace with NULL if there's nothing to do at request start */
PHP_RSHUTDOWN(test), /* Replace with NULL if there's nothing to do at request end */
PHP_MINFO(test),
PHP_TEST_VERSION,
STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_TEST
#ifdef ZTS
		ZEND_TSRMLS_CACHE_DEFINE()
#endif
		ZEND_GET_MODULE(test)
#endif
