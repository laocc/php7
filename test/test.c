#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_test.h"

/* 声明全局变量:
ZEND_DECLARE_MODULE_GLOBALS(test)
*/

/* True global resources - no need for thread safety here */
static int le_test;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("test.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_test_globals, test_globals)
    STD_PHP_INI_ENTRY("test.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_test_globals, test_globals)
PHP_INI_END()
*/
/* }}} */

PHP_FUNCTION(str_test)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Hello %.78s\n", arg);

	RETURN_STR(strg);
}
/* }}} */

zend_class_entry *cz_cz_ce;

const zend_function_entry cz_class_functions[]={
    PHP_ME(test, __construct,NULL,ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
    PHP_ME(test,createApp,NULL,ZEND_ACC_PUBLIC)
    PHP_FE_END
};
PHP_MINIT_FUNCTION(cz)
{
    zend_class_entry ce;
    memset(&ce, 0, sizeof(zend_class_entry));
    INIT_CLASS_ENTRY(ce,"test",cz_class_functions);
    cz_cz_ce = zend_register_internal_class_ex(&ce,NULL,NULL TSRMLS_CC);
    /*
    cz_cz_ce->ce_flags |= ZEND_ACC_IMPLICIT_ABSTRACT_CLASS;
     * */
    zend_declare_property_null(cz_cz_ce,ZEND_STRL("_test"),ZEND_ACC_PUBLIC TSRMLS_CC);
}
PHP_METHOD(test,__construct)
{
    zend_printf("The is Cz class __construct!!</br>");
}
PHP_METHOD(test,createApp)
{
    zend_printf("The is Cz lcass createApp !</br>");
}




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
PHP_MINIT_FUNCTION(test)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(test)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(test)
{
#if defined(COMPILE_DL_TEST) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(test)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(test)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "test support", "enabled");
    php_info_print_table_row(2, "Version", PHP_TEST_VERSION);
    php_info_print_table_row(2, "Author", "Fazo(fazo@qq.com)");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ test_functions[]
 *
 * 本扩展所实现的所有函数
 */
const zend_function_entry test_functions[] = {
	PHP_FE(str_test,	NULL)
	PHP_FE_END
};

const zend_function_entry vtest_functions[] = {
	PHP_FE(test, has,NULL)
	PHP_FE_END
};

/* }}} */

/* {{{ test_module_entry
 */
zend_module_entry test_module_entry = {
	STANDARD_MODULE_HEADER,
	"test",
	test_functions,
	PHP_MINIT(test),
	PHP_MSHUTDOWN(test),
	PHP_RINIT(test),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(test),	/* Replace with NULL if there's nothing to do at request end */
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
