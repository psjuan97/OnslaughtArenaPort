#include "2dContext.h"
#include "../engineModules/AssetManager.hpp"
#include "../engineModules/EventManager.hpp"
#include "../engineModules/JamEngine.hpp"
#include <regex>

duk_ret_t context_now(duk_context *ctx) {

  JamEngine *gm = JamEngine::Instance();
  duk_push_number(ctx, gm->getTicks());

  return 1;
}

duk_ret_t context_save(duk_context *ctx) {
  JamEngine *gm = JamEngine::Instance();

  gm->saveContext();

  return 1;
}

duk_ret_t context_restore(duk_context *ctx) {
  JamEngine *gm = JamEngine::Instance();
  gm->restoreContext();

  return 1;
}

duk_ret_t is_end(duk_context *ctx) {
  JamEngine *gm = JamEngine::Instance();

  duk_push_number(ctx, !gm->isOpen());

  return 1;
}

duk_ret_t context_FLUSH(duk_context *ctx) {
  JamEngine *gm = JamEngine::Instance();
  gm->Render();

  EventManager *ev = EventManager::Instance();
  ev->launch();

  return 0;
}

duk_ret_t context_clearRect(duk_context *ctx) {
  JamEngine *gm = JamEngine::Instance();
  gm->Clear();

  return 0;
}

duk_ret_t context_fillRect(duk_context *ctx) {
  JamEngine *gm = JamEngine::Instance();

  draw_data data;
  auto dosd_ctx = gm->ctx_stack.top();

  data.d.h = static_cast<int>(duk_get_number(ctx, -1));
  data.d.w = static_cast<int>(duk_get_number(ctx, -2));
  data.d.y = static_cast<int>(duk_get_number(ctx, -3) + dosd_ctx.transform_y);
  data.d.x = static_cast<int>(duk_get_number(ctx, -4) + dosd_ctx.transform_x);

  data.color_r = dosd_ctx.color_r;
  data.color_g = dosd_ctx.color_g;
  data.color_b = dosd_ctx.color_b;

  gm->insertDrawRectPool(data);

  return 0;
}

duk_ret_t context_fillText(duk_context *ctx) {

  // draw text without cache very slow
  return 0;

  if (duk_is_undefined(ctx, -1) || duk_is_undefined(ctx, -2) ||
      duk_is_undefined(ctx, -3)) {
    return 0;
  }

  float y = duk_get_number(ctx, -1);
  float x = duk_get_number(ctx, -2);
  const char *s;
  if (duk_get_type(ctx, -3) == DUK_TYPE_NUMBER) {
    s = std::to_string(duk_get_number(ctx, -3)).c_str();
  } else if (duk_get_type(ctx, -3) == DUK_TYPE_STRING) {
    s = duk_get_string(ctx, -3);
  }else{
      printf("TEXTO EN FORMATO DESCONOCIDO, NI STRING NI NUMERO");
    return 0;
  }

  if (s == NULL) {
    return 0;
  }
  std::string text = std::string(s);

  printf("Texto: %s \n", text.c_str());
  printf("Texto: %s \n", s);

  printf("x: %f \n", x);
  printf("y: %f \n", y);

  JamEngine *gm = JamEngine::Instance();

  text_data data;
  data.text = text;
  data.d.x = static_cast<int>(x);
  data.d.y = static_cast<int>(y);

  gm->insertDrawTextPool(data);

  return 0;
}

duk_ret_t context_rotate(duk_context *ctx) {
  // x, y

  float degree = duk_get_number(ctx, -1);

  printf("degree: %f \n", degree);

  JamEngine *gm = JamEngine::Instance();
  gm->ctx_add_rotation((degree * 180) / 3.141593);

  return 0;
}

duk_ret_t context_translate(duk_context *ctx) {
  // x, y

  float y = duk_get_number(ctx, -1);
  float x = duk_get_number(ctx, -2);

  // printf("sx: %f \n", x);
  // printf("sy: %f \n", y);

  JamEngine *gm = JamEngine::Instance();
  gm->ctx_translate(x, y);

  return 0;
}

bool extract_tokens(std::string expression, std::vector<std::string> &tokens) {
  static const std::regex reg(R"(\d+)");
  std::copy(
      std::sregex_token_iterator(expression.begin(), expression.end(), reg),
      std::sregex_token_iterator(), std::back_inserter(tokens));
  return true;
}

duk_ret_t context_fillStyle(duk_context *ctx) {
  JamEngine *gm = JamEngine::Instance();
  const char *color = static_cast<const char *>(duk_get_string(ctx, -1));
  std::vector<std::string> v;
  extract_tokens(color, v);
  int r = stoi(v[0]);
  int g = stoi(v[1]);
  int b = stoi(v[2]);

  gm->ctx_change_color(r, g, b);
  return 0;
}

duk_ret_t context_drawImage(duk_context *ctx) {
  JamEngine *gm = JamEngine::Instance();

  // image, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight)
  draw_data data;
  auto dosd_ctx = gm->ctx_stack.top();

  data.angle = dosd_ctx.angle;
  data.d.h = static_cast<int>(duk_get_number(ctx, -1));
  data.d.w = static_cast<int>(duk_get_number(ctx, -2));
  data.d.y = static_cast<int>(duk_get_number(ctx, -3) + dosd_ctx.transform_y);
  data.d.x = static_cast<int>(duk_get_number(ctx, -4) + dosd_ctx.transform_x);

  data.s.h = static_cast<int>(duk_get_number(ctx, -5));
  data.s.w = static_cast<int>(duk_get_number(ctx, -6));
  data.s.y = static_cast<int>(duk_get_number(ctx, -7));
  data.s.x = static_cast<int>(duk_get_number(ctx, -8));

  float imageID = duk_get_number(ctx, -9);

  gm->insertDrawPool(imageID, data);

  return 0;
}

/* 2dContext */
duk_ret_t Context_constructor(duk_context *ctx) {
  /* Stack at entry is: [ name ] */

  if (!duk_is_constructor_call(ctx)) {
    return DUK_RET_TYPE_ERROR;
  }

  /* Push explicitly created instance and set its prototype. */
  duk_push_object(ctx);
  duk_get_global_string(ctx, "Context_prototype");
  duk_set_prototype(ctx, -2); /* -> stack: [ name result ] */

  /* Set result.name to name. */
  // duk_dup(ctx, 0);
  // duk_put_prop_string(ctx, -2, "name");  /* -> stack: [ name result ] */

  /* Return the 'result' object: replaces the default instance. */
  return 1;
}

duk_ret_t context_keyDown(duk_context *ctx) {
  EventManager *ev = EventManager::Instance();
  int k = duk_get_number(ctx, -1);
  PspCtrlButtons btn = (PspCtrlButtons)k;

  int status = ev->getAndResetButtonState(btn);
  // printf("skey: %i \n", k);
  //  printf("sbtn: %i \n", btn);

  // printf("status: %i \n", status);

  duk_push_number(ctx, status);

  return 1;
}

duk_ret_t context_keyUp(duk_context *ctx) {
  JamEngine *gm = JamEngine::Instance();
  // int key = gm->keyUp();
  // duk_push_number(ctx, key);

  return 1;
}

/* Initialize MyObject into global object. */
void myobject_init(duk_context *ctx) {
  /* Register MyObject_prototype. */
  duk_push_object(ctx);

  // drawImage
  duk_push_c_function(ctx, context_drawImage, 9 /*nargs*/);
  duk_put_prop_string(ctx, -2, "drawImage");

  // drawImage
  duk_push_c_function(ctx, context_fillStyle, 1 /*nargs*/);
  duk_put_prop_string(ctx, -2, "fillStyle");

  //  Flush
  duk_push_c_function(ctx, context_FLUSH, 0 /*nargs*/);
  duk_put_prop_string(ctx, -2, "NATIVEFLUSH");

  //  clearRect
  duk_push_c_function(ctx, context_clearRect, 4 /*nargs*/);
  duk_put_prop_string(ctx, -2, "clearRect");

  // FILL RECT
  duk_push_c_function(ctx, context_fillRect, 4 /*nargs*/);
  duk_put_prop_string(ctx, -2, "fillRect");

  // FILL TEXT
  duk_push_c_function(ctx, context_fillText, 3 /*nargs*/);
  duk_put_prop_string(ctx, -2, "fillText");

  // KEY DOWN
  duk_push_c_function(ctx, context_keyDown, 1 /*nargs*/);
  duk_put_prop_string(ctx, -2, "keyDown");

  // KEY DOWN
  duk_push_c_function(ctx, context_keyDown, 1 /*nargs*/);
  duk_put_prop_string(ctx, -2, "keyUp");

  // TRANSALTE
  duk_push_c_function(ctx, context_translate, 2 /*nargs*/);
  duk_put_prop_string(ctx, -2, "translate");

  // TRANSALTE
  duk_push_c_function(ctx, context_rotate, 1 /*nargs*/);
  duk_put_prop_string(ctx, -2, "rotate");

  // NOW
  duk_push_c_function(ctx, context_now, 0 /*nargs*/);
  duk_put_prop_string(ctx, -2, "now");

  // SAVE
  duk_push_c_function(ctx, context_save, 0 /*nargs*/);
  duk_put_prop_string(ctx, -2, "save");

  // RESTORE
  duk_push_c_function(ctx, context_restore, 0 /*nargs*/);
  duk_put_prop_string(ctx, -2, "restore");

  // SAVE
  duk_push_c_function(ctx, is_end, 0 /*nargs*/);
  duk_put_prop_string(ctx, -2, "IsEnd");

  duk_put_global_string(ctx, "Context_prototype");

  /* Register MyObject. */
  duk_push_c_function(ctx, Context_constructor, 0 /*nargs*/);
  duk_put_global_string(ctx, "Context"); /* -> stack: [ ] */
}