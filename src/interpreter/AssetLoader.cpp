#include "2dContext.h"
#include "../engineModules/JamEngine.hpp"
#include "../engineModules/AssetManager.hpp"
#include "../engineModules/EventManager.hpp"




duk_ret_t loader_loadImage(duk_context *ctx) {
  const char *imageName = duk_get_string(ctx, -1);
  float id = duk_get_number(ctx, -2);
  AssetManager *Assets = AssetManager::Instance();
  Assets->loadTexture(id,imageName );

  return 0;
}

/* loader */
duk_ret_t loader_constructor(duk_context *ctx) {
  /* Stack at entry is: [ name ] */

  if (!duk_is_constructor_call(ctx)) {
    return DUK_RET_TYPE_ERROR;
  }

  /* Push explicitly created instance and set its prototype. */
  duk_push_object(ctx);
  duk_get_global_string(ctx, "AssetLoader_prototype");
  duk_set_prototype(ctx, -2); /* -> stack: [ name result ] */

  /* Set result.name to name. */
  // duk_dup(ctx, 0);
  // duk_put_prop_string(ctx, -2, "name");  /* -> stack: [ name result ] */

  /* Return the 'result' object: replaces the default instance. */
  return 1;
}


/* Initialize MyObject into global object. */
void myAssetLoader_init(duk_context *ctx) {
  /* Register MyObject_prototype. */
  duk_push_object(ctx);

  // drawImage
  duk_push_c_function(ctx, loader_loadImage, 2 /*nargs*/);
  duk_put_prop_string(ctx, -2, "loadImage");


  duk_put_global_string(ctx, "AssetLoader_prototype");

  /* Register MyObject. */
  duk_push_c_function(ctx, loader_constructor, 0 /*nargs*/);
  duk_put_global_string(ctx, "AssetLoader"); /* -> stack: [ ] */
}