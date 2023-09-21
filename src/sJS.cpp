#include "sJS.hpp"
#include "engineModules/JamEngine.hpp"
#include "engineModules/EventManager.hpp"
#include "engineModules/StateMachine.hpp"
#include "sGame.hpp"
#include "ASSETS_IDs.hpp"



#include <interpreter/fileio.h>
#include <interpreter/2dContext.h>
#include <interpreter/AssetLoader.h>




static int c_evloop = true;

#if !defined(NO_SIGNAL)
static void my_sighandler(int x)
{
	fprintf(stderr, "Got signal %d\n", x);
	fflush(stderr);
}
static void set_sigint_handler(void)
{
	(void)signal(SIGINT, my_sighandler);
}
#endif /* NO_SIGNAL */

/* Print error to stderr and pop error. */
static void print_error(duk_context *ctx, FILE *f)
{
	if (duk_is_object(ctx, -1) && duk_has_prop_string(ctx, -1, "stack"))
	{
		/* XXX: print error objects specially */
		/* XXX: pcall the string coercion */
		duk_get_prop_string(ctx, -1, "stack");
		if (duk_is_string(ctx, -1))
		{
			fprintf(f, "%s\n", duk_get_string(ctx, -1));
			fflush(f);
			duk_pop_2(ctx);
			return;
		}
		else
		{
			duk_pop(ctx);
		}
	}
	duk_to_string(ctx, -1);
	fprintf(f, "%s\n", duk_get_string(ctx, -1));
	fflush(f);
	duk_pop(ctx);
}

static duk_ret_t native_print(duk_context *ctx)
{

	duk_push_string(ctx, " ");
	duk_insert(ctx, 0);
	duk_join(ctx, duk_get_top(ctx) - 1);
	printf("NATIVE: %s\n", duk_safe_to_string(ctx, -1));
	return 0;
}

static void print_register(duk_context *ctx)
{
	duk_push_c_function(ctx, native_print, DUK_VARARGS);
	duk_put_global_string(ctx, "print");
}




static duk_ret_t init_duktape_context(duk_context *ctx, void *udata)
{
	(void)udata;

	print_register(ctx);
	fileio_register(ctx);

	if (c_evloop)
	{
		fprintf(stderr, "Using C based eventloop (omit -c to use ECMAScript based eventloop)\n");
		fflush(stderr);

		/*eventloop_register(ctx);
		fileio_push_file_string(ctx, "js/c_eventloop.js");
		duk_eval(ctx);*/

/*}
	else
	{
		fprintf(stderr, "Using ECMAScript based eventloop (give -c to use C based eventloop)\n");
		fflush(stderr);
*/
		//fileio_push_file_string(ctx, "ecma_eventloop.js");
		//duk_eval(ctx);

		fileio_push_file_string(ctx, "js/base.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/timer.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/sound.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/size.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/vector2.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/rect.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/keyboard.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/mouse.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/waves_full.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/engine.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/object.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/object_types.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/image_loader.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/spawn_point.js");
		duk_eval(ctx);
		fileio_push_file_string(ctx, "js/spawn_wave.js");
		duk_eval(ctx);

		fileio_push_file_string(ctx, "js/run_game.js");
		duk_eval(ctx);

		
	}

	return 0;
}



sJS::sJS()
{

}


sJS::~sJS(){

}


void sJS::Init(){
    
    AssetManager::Instance()->loadInitialAssets();

	duk_context *ctx = NULL;

	ctx = duk_create_heap_default();
	myobject_init(ctx);
	myAssetLoader_init(ctx);
	if (duk_safe_call(ctx, init_duktape_context, NULL, 0 /*nargs*/, 1 /*nrets*/) != 0)
	{
		fprintf(stderr, "Failed to initialize: %s\n", duk_safe_to_string(ctx, -1));
		goto cleanup;
	}
	duk_pop(ctx);






	/* fall through */
cleanup:
	if (ctx)
	{
		duk_destroy_heap(ctx);
	}




}

void sJS::Update(){
    if(EventManager::Instance()->isAnyKeyPressed()){
        StateMachine::Instance()->setState(new sGame);
    }
}

void sJS::Exit(){
    JamEngine::Instance()->clearAllDrawables();
}