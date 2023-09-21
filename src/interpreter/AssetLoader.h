#include "../lib/duktape/duktape.h"

duk_ret_t myobject_print_name(duk_context *ctx) ;
/* MyObject */
duk_ret_t Context_constructor(duk_context *ctx) ;

/* Initialize MyObject into global object. */
void myAssetLoader_init(duk_context *ctx) ;