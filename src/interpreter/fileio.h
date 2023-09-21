#include "../lib/duktape/duktape.h"


 void fileio_register(duk_context *ctx);
 void fileio_push_file_buffer(duk_context *ctx, const char *filename);
 void fileio_push_file_string(duk_context *ctx, const char *filename);
 void eventloop_register(duk_context *ctx);
 duk_ret_t eventloop_run(duk_context *ctx, void *udata);
