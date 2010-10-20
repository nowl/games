#include "blacklua.h"

static void*
checklightuserdata(lua_State *L, int argn)
{
    luaL_checktype(L, argn, LUA_TLIGHTUSERDATA);
    return lua_topointer(L, argn);
}

static int
kern_new_game_object(lua_State *L)
{
    game_object *obj = make_game_object(0, NULL);
    lua_pushlightuserdata(L, obj);
    return 1;
}

static int
kern_destroy_game_object(lua_State *L)
{
    game_object *obj = checklightuserdata(L, 1);
    destroy_game_object(g_game_manager, obj);
    return 0;
}

static int
kern_game_object_image_set(lua_State *L)
{
    game_object *obj = checklightuserdata(L, 1);
    SDL_Surface *image = checklightuserdata(L, 2);

	obj->image = image;

    return 0;
}

static int
kern_game_object_screen_position_set(lua_State *L)
{
    game_object *obj = checklightuserdata(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);

	obj->screenx = x;
	obj->screeny = y;

    return 0;
}


static int
kern_game_object_append_update(lua_State *L)
{
    game_object *obj = checklightuserdata(L, 1);
    char *func = luaL_checklstring(L, 2, NULL);

    game_object_append_update_callback_script_func(obj, func);

    return 0;
}

static int
append_render(lua_State *L)
{
    game_object *obj = checklightuserdata(L, 1);
    char *func = luaL_checklstring(L, 2, NULL);

    game_object_append_render_callback_script_func(obj, func);

    return 0;
}

static int
collide_polys(lua_State *L)
{
    int res;
    float poly1[64], poly2[64];
    int poly1_num_points, poly2_num_points;

    luaL_checktype(L, 1, LUA_TTABLE);
    luaL_checktype(L, 2, LUA_TTABLE);

    poly1_num_points = 0;
    for(;;)
    {
        lua_rawgeti(L, -2, poly1_num_points+1);
        if(lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            break;
        }
        poly1[poly1_num_points] = lua_tonumber(L, -1);
        lua_pop(L, 1);
        poly1_num_points++;
    }

    poly2_num_points = 0;
    for(;;)
    {
        lua_rawgeti(L, -1, poly2_num_points+1);
        if(lua_isnil(L, -1))
        {
            lua_pop(L, 1);
            break;
        }
        poly2[poly2_num_points] = lua_tonumber(L, -1);
        lua_pop(L, 1);
        poly2_num_points++;
    }

    res = collide_polygon_intersect(poly1, poly1_num_points/2,
                                    poly2, poly2_num_points/2);
    lua_pushboolean(L, res);
    return 1;
}

static int
object_clear_updates(lua_State *L)
{
    game_object *obj = checklightuserdata(L, 1);

    game_object_clear_update_callbacks(obj);

    return 0;
}

static int
remove_render(lua_State *L)
{
    game_object *obj = checklightuserdata(L, 1);
    char *func = luaL_checklstring(L, 2, NULL);

    game_object_remove_render_callback_script_func(obj, func);
    
    return 0;
}

static int
remove_update(lua_State *L)
{
    game_object *obj = checklightuserdata(L, 1);
    char *func = luaL_checklstring(L, 2, NULL);

    game_object_remove_update_callback_script_func(obj, func);
    
    return 0;
}

static const luaL_reg game_object_methods[] = {
    {"collide_polys", collide_polys},
    {"new", kern_new_game_object},
    {"destroy", kern_destroy_game_object},
    {"update_append", kern_game_object_append_update},
    {"update_remove", remove_update},
    {"update_clear", object_clear_updates},
    {"render_append", append_render},
    {"render_remove", remove_render},
    {"image_set", kern_game_object_image_set},
    {"screen_pos_set", kern_game_object_screen_position_set},
    {NULL, NULL}
};

static int
kern_game_manager_quit(lua_State *L)
{
    game_manager_quit(g_game_manager);

    return 0;
}

static int
set_ticks_per_sec(lua_State *L)
{
    unsigned int num = luaL_checkinteger(L, 1);

    set_ticks_per_second(num);

    return 0;
}

static int
cur_tick(lua_State *L)
{
    unsigned int ticks = g_game_manager->tick;

    lua_pushinteger(L, ticks);

    return 1;
}

static int
switch_state(lua_State *L)
{
    game_state *state = checklightuserdata(L, 1);
    game_manager_switch_state(g_game_manager, state);
    return 0;
}

static const luaL_reg game_manager_methods[] = {
    {"cur_tick", cur_tick},
    {"set_ticks_per_sec", set_ticks_per_sec},
    {"switch_state", switch_state},
    {"quit", kern_game_manager_quit},
    {NULL, NULL}
};

static int
kern_image_loader_load(lua_State *L)
{
    char *name = luaL_checklstring(L, 1, NULL);
    char *image = luaL_checklstring(L, 2, NULL);
    int x = luaL_checkinteger(L, 3);
    int y = luaL_checkinteger(L, 4);
    int w = luaL_checkinteger(L, 5);
    int h = luaL_checkinteger(L, 6);
    image_loader_load(name, image, x, y, w, h);

    return 0;
}

static int
kern_image_render_set_create(lua_State *L)
{
    char *name = luaL_checklstring(L, 1, NULL);

    image_render_set_create(name);

    return 0;
}

static int
kern_image_render_set_add(lua_State *L)
{
    char *name = luaL_checklstring(L, 1, NULL);
    char *image = luaL_checklstring(L, 2, NULL);
    int num_ticks = luaL_checkinteger(L, 3);

    image_render_set_add(name, image, num_ticks);

    return 0;
}


static int
kern_image_render_set_get(lua_State *L)
{
	char *name = luaL_checklstring(L, 1, NULL);
	int ticks = luaL_checkinteger(L, 2);

	SDL_Surface *image = image_render_set_get_image(name, ticks);

	lua_pushlightuserdata(L, image);

	return 1;
}

static const luaL_reg image_methods[] = {
    {"load", kern_image_loader_load},
    {"set_create", kern_image_render_set_create},
    {"set_add", kern_image_render_set_add},
	{"set_get", kern_image_render_set_get},
    {NULL, NULL}
};

static int
kern_event_peek(lua_State *L)
{
    SDL_Event *event = event_queue_peek();

    if(event)
        lua_pushlightuserdata(L, event);
    else
        lua_pushnil(L);

    return 1;
}

static int
kern_event_pop(lua_State *L)
{
    event_queue_pop();

    return 0;
}


static int
kern_event_get_type(lua_State *L)
{
    SDL_Event *event = checklightuserdata(L, 1);
    lua_pushinteger(L, event->type);

    return 1;
}

static int
kern_event_get_key(lua_State *L)
{
    SDL_Event *event = checklightuserdata(L, 1);
    lua_pushinteger(L, event->key.keysym.sym);

    return 1;
}

static int
kern_event_motion_x(lua_State *L)
{
    SDL_Event *event = checklightuserdata(L, 1);
    lua_pushinteger(L, event->motion.x);

    return 1;
}

static int
kern_event_motion_y(lua_State *L)
{
    SDL_Event *event = checklightuserdata(L, 1);
    lua_pushinteger(L, event->motion.y);

    return 1;
}

static const luaL_reg event_methods[] = {
    {"motion_x", kern_event_motion_x},
    {"motion_y", kern_event_motion_y},
    {"peek", kern_event_peek},
    {"pop", kern_event_pop},
    {"get_type", kern_event_get_type},
    {"get_key", kern_event_get_key},
    {NULL, NULL}
};

static int
kern_new_game_state(lua_State *L)
{
    int state_num = luaL_checkinteger(L, 1);

    game_state *obj = make_game_state(state_num);
    lua_pushlightuserdata(L, obj);

    return 1;
}

static int
kern_game_state_push_object(lua_State *L)
{
    game_state *state = checklightuserdata(L, 1);
    game_object *obj = checklightuserdata(L, 2);

    game_state_append_object(state, obj);

    return 0;
}

static int
kern_game_state_remove_object(lua_State *L)
{
    game_state *state = checklightuserdata(L, 1);
    game_object *obj = checklightuserdata(L, 2);

    game_state_remove_object(state, obj);

    return 0;
}

static const luaL_reg state_methods[] = {
    {"new", kern_new_game_state},
    {"push", kern_game_state_push_object},
    {"remove", kern_game_state_remove_object},
    {NULL, NULL}
};

static int
draw_text(lua_State *L)
{
    char * name = luaL_checklstring(L, 1, NULL);
    int size = luaL_checkinteger(L, 2);
    char * text = luaL_checklstring(L, 3, NULL);
    int x = luaL_checkinteger(L, 4);
    int y = luaL_checkinteger(L, 5);
    int r = luaL_checkinteger(L, 6);
    int g = luaL_checkinteger(L, 7);
    int b = luaL_checkinteger(L, 8);

    SDL_Surface *surf = sdl_font_get_surf(name, size, text, r, g, b);
    black_sdl_draw_image(g_game_manager, surf, x, y);

    return 0;
}

static int
draw_image(lua_State *L)
{
    SDL_Surface *image = checklightuserdata(L, 1);
    int x = luaL_checkinteger(L, 2);
    int y = luaL_checkinteger(L, 3);

    black_sdl_draw_image(g_game_manager, image, x, y);

    return 0;
}

static int
draw_rect(lua_State *L)
{
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int w = luaL_checkinteger(L, 3);
    int h = luaL_checkinteger(L, 4);
    int r = luaL_checkinteger(L, 5);
    int g = luaL_checkinteger(L, 6);
    int b = luaL_checkinteger(L, 7);

    black_sdl_fill_rect(g_game_manager, x, y, w, h, r, g, b);
    return 0;
}

static const luaL_reg draw_methods[] = {
    {"text", draw_text},
    {"image", draw_image},
    {"rect", draw_rect},
    {NULL, NULL}
};

static int
get_fps(lua_State *L)
{
	lua_pushinteger(L, g_game_manager->fps);

	return 1;
}

static const luaL_reg info_methods[] = {
    {"fps", get_fps},
    {NULL, NULL}
};

static int
load_sound(lua_State *L)
{
    char *file = luaL_checklstring(L, 1, NULL);
    char *alias = luaL_checklstring(L, 2, NULL);

    sdl_sound_load(file, alias);

	return 0;
}

static int
play_sound(lua_State *L)
{
    char *alias = luaL_checklstring(L, 1, NULL);

    sdl_sound_play(alias);

	return 0;
}


static const luaL_reg sound_methods[] = {
    {"load", load_sound},
    {"play", play_sound},
    {NULL, NULL}
};

#define register_in_table(key, value)    lua_pushinteger(L, value); lua_setfield(L, -2, #key);

static void
register_sdl_keys(lua_State *L)
{
    lua_newtable(L);

    /* register sdl key syms */

    register_in_table(SDLK_UNKNOWN, 0);
    register_in_table(SDLK_FIRST, 0);
    register_in_table(SDLK_BACKSPACE, 8);
    register_in_table(SDLK_TAB, 9);
    register_in_table(SDLK_CLEAR, 12);
    register_in_table(SDLK_RETURN, 13);
    register_in_table(SDLK_PAUSE, 19);
    register_in_table(SDLK_ESCAPE, 27);
    register_in_table(SDLK_SPACE, 32);
    register_in_table(SDLK_EXCLAIM, 33);
    register_in_table(SDLK_QUOTEDBL, 34);
    register_in_table(SDLK_HASH, 35);
    register_in_table(SDLK_DOLLAR, 36);
    register_in_table(SDLK_AMPERSAND, 38);
    register_in_table(SDLK_QUOTE, 39);
    register_in_table(SDLK_LEFTPAREN, 40);
    register_in_table(SDLK_RIGHTPAREN, 41);
    register_in_table(SDLK_ASTERISK, 42);
    register_in_table(SDLK_PLUS, 43);
    register_in_table(SDLK_COMMA, 44);
    register_in_table(SDLK_MINUS, 45);
    register_in_table(SDLK_PERIOD, 46);
    register_in_table(SDLK_SLASH, 47);
    register_in_table(SDLK_0, 48);
    register_in_table(SDLK_1, 49);
    register_in_table(SDLK_2, 50);
    register_in_table(SDLK_3, 51);
    register_in_table(SDLK_4, 52);
    register_in_table(SDLK_5, 53);
    register_in_table(SDLK_6, 54);
    register_in_table(SDLK_7, 55);
    register_in_table(SDLK_8, 56);
    register_in_table(SDLK_9, 57);
    register_in_table(SDLK_COLON, 58);
    register_in_table(SDLK_SEMICOLON, 59);
    register_in_table(SDLK_LESS, 60);
    register_in_table(SDLK_EQUALS, 61);
    register_in_table(SDLK_GREATER, 62);
    register_in_table(SDLK_QUESTION, 63);
    register_in_table(SDLK_AT, 64);
    register_in_table(SDLK_LEFTBRACKET, 91);
    register_in_table(SDLK_BACKSLASH, 92);
    register_in_table(SDLK_RIGHTBRACKET, 93);
    register_in_table(SDLK_CARET, 94);
    register_in_table(SDLK_UNDERSCORE, 95);
    register_in_table(SDLK_BACKQUOTE, 96);
    register_in_table(SDLK_a, 97);
    register_in_table(SDLK_b, 98);
    register_in_table(SDLK_c, 99);
    register_in_table(SDLK_d, 100);
    register_in_table(SDLK_e, 101);
    register_in_table(SDLK_f, 102);
    register_in_table(SDLK_g, 103);
    register_in_table(SDLK_h, 104);
    register_in_table(SDLK_i, 105);
    register_in_table(SDLK_j, 106);
    register_in_table(SDLK_k, 107);
    register_in_table(SDLK_l, 108);
    register_in_table(SDLK_m, 109);
    register_in_table(SDLK_n, 110);
    register_in_table(SDLK_o, 111);
    register_in_table(SDLK_p, 112);
    register_in_table(SDLK_q, 113);
    register_in_table(SDLK_r, 114);
    register_in_table(SDLK_s, 115);
    register_in_table(SDLK_t, 116);
    register_in_table(SDLK_u, 117);
    register_in_table(SDLK_v, 118);
    register_in_table(SDLK_w, 119);
    register_in_table(SDLK_x, 120);
    register_in_table(SDLK_y, 121);
    register_in_table(SDLK_z, 122);
    register_in_table(SDLK_DELETE, 127);
    register_in_table(SDLK_WORLD_0, 160);
    register_in_table(SDLK_WORLD_1, 161);
    register_in_table(SDLK_WORLD_2, 162);
    register_in_table(SDLK_WORLD_3, 163);
    register_in_table(SDLK_WORLD_4, 164);
    register_in_table(SDLK_WORLD_5, 165);
    register_in_table(SDLK_WORLD_6, 166);
    register_in_table(SDLK_WORLD_7, 167);
    register_in_table(SDLK_WORLD_8, 168);
    register_in_table(SDLK_WORLD_9, 169);
    register_in_table(SDLK_WORLD_10, 170);
    register_in_table(SDLK_WORLD_11, 171);
    register_in_table(SDLK_WORLD_12, 172);
    register_in_table(SDLK_WORLD_13, 173);
    register_in_table(SDLK_WORLD_14, 174);
    register_in_table(SDLK_WORLD_15, 175);
    register_in_table(SDLK_WORLD_16, 176);
    register_in_table(SDLK_WORLD_17, 177);
    register_in_table(SDLK_WORLD_18, 178);
    register_in_table(SDLK_WORLD_19, 179);
    register_in_table(SDLK_WORLD_20, 180);
    register_in_table(SDLK_WORLD_21, 181);
    register_in_table(SDLK_WORLD_22, 182);
    register_in_table(SDLK_WORLD_23, 183);
    register_in_table(SDLK_WORLD_24, 184);
    register_in_table(SDLK_WORLD_25, 185);
    register_in_table(SDLK_WORLD_26, 186);
    register_in_table(SDLK_WORLD_27, 187);
    register_in_table(SDLK_WORLD_28, 188);
    register_in_table(SDLK_WORLD_29, 189);
    register_in_table(SDLK_WORLD_30, 190);
    register_in_table(SDLK_WORLD_31, 191);
    register_in_table(SDLK_WORLD_32, 192);
    register_in_table(SDLK_WORLD_33, 193);
    register_in_table(SDLK_WORLD_34, 194);
    register_in_table(SDLK_WORLD_35, 195);
    register_in_table(SDLK_WORLD_36, 196);
    register_in_table(SDLK_WORLD_37, 197);
    register_in_table(SDLK_WORLD_38, 198);
    register_in_table(SDLK_WORLD_39, 199);
    register_in_table(SDLK_WORLD_40, 200);
    register_in_table(SDLK_WORLD_41, 201);
    register_in_table(SDLK_WORLD_42, 202);
    register_in_table(SDLK_WORLD_43, 203);
    register_in_table(SDLK_WORLD_44, 204);
    register_in_table(SDLK_WORLD_45, 205);
    register_in_table(SDLK_WORLD_46, 206);
    register_in_table(SDLK_WORLD_47, 207);
    register_in_table(SDLK_WORLD_48, 208);
    register_in_table(SDLK_WORLD_49, 209);
    register_in_table(SDLK_WORLD_50, 210);
    register_in_table(SDLK_WORLD_51, 211);
    register_in_table(SDLK_WORLD_52, 212);
    register_in_table(SDLK_WORLD_53, 213);
    register_in_table(SDLK_WORLD_54, 214);
    register_in_table(SDLK_WORLD_55, 215);
    register_in_table(SDLK_WORLD_56, 216);
    register_in_table(SDLK_WORLD_57, 217);
    register_in_table(SDLK_WORLD_58, 218);
    register_in_table(SDLK_WORLD_59, 219);
    register_in_table(SDLK_WORLD_60, 220);
    register_in_table(SDLK_WORLD_61, 221);
    register_in_table(SDLK_WORLD_62, 222);
    register_in_table(SDLK_WORLD_63, 223);
    register_in_table(SDLK_WORLD_64, 224);
    register_in_table(SDLK_WORLD_65, 225);
    register_in_table(SDLK_WORLD_66, 226);
    register_in_table(SDLK_WORLD_67, 227);
    register_in_table(SDLK_WORLD_68, 228);
    register_in_table(SDLK_WORLD_69, 229);
    register_in_table(SDLK_WORLD_70, 230);
    register_in_table(SDLK_WORLD_71, 231);
    register_in_table(SDLK_WORLD_72, 232);
    register_in_table(SDLK_WORLD_73, 233);
    register_in_table(SDLK_WORLD_74, 234);
    register_in_table(SDLK_WORLD_75, 235);
    register_in_table(SDLK_WORLD_76, 236);
    register_in_table(SDLK_WORLD_77, 237);
    register_in_table(SDLK_WORLD_78, 238);
    register_in_table(SDLK_WORLD_79, 239);
    register_in_table(SDLK_WORLD_80, 240);
    register_in_table(SDLK_WORLD_81, 241);
    register_in_table(SDLK_WORLD_82, 242);
    register_in_table(SDLK_WORLD_83, 243);
    register_in_table(SDLK_WORLD_84, 244);
    register_in_table(SDLK_WORLD_85, 245);
    register_in_table(SDLK_WORLD_86, 246);
    register_in_table(SDLK_WORLD_87, 247);
    register_in_table(SDLK_WORLD_88, 248);
    register_in_table(SDLK_WORLD_89, 249);
    register_in_table(SDLK_WORLD_90, 250);
    register_in_table(SDLK_WORLD_91, 251);
    register_in_table(SDLK_WORLD_92, 252);
    register_in_table(SDLK_WORLD_93, 253);
    register_in_table(SDLK_WORLD_94, 254);
    register_in_table(SDLK_WORLD_95, 255);
    register_in_table(SDLK_KP0, 256);
    register_in_table(SDLK_KP1, 257);
    register_in_table(SDLK_KP2, 258);
    register_in_table(SDLK_KP3, 259);
    register_in_table(SDLK_KP4, 260);
    register_in_table(SDLK_KP5, 261);
    register_in_table(SDLK_KP6, 262);
    register_in_table(SDLK_KP7, 263);
    register_in_table(SDLK_KP8, 264);
    register_in_table(SDLK_KP9, 265);
    register_in_table(SDLK_KP_PERIOD, 266);
    register_in_table(SDLK_KP_DIVIDE, 267);
    register_in_table(SDLK_KP_MULTIPLY, 268);
    register_in_table(SDLK_KP_MINUS, 269);
    register_in_table(SDLK_KP_PLUS, 270);
    register_in_table(SDLK_KP_ENTER, 271);
    register_in_table(SDLK_KP_EQUALS, 272);
    register_in_table(SDLK_UP, 273);
    register_in_table(SDLK_DOWN, 274);
    register_in_table(SDLK_RIGHT, 275);
    register_in_table(SDLK_LEFT, 276);
    register_in_table(SDLK_INSERT, 277);
    register_in_table(SDLK_HOME, 278);
    register_in_table(SDLK_END, 279);
    register_in_table(SDLK_PAGEUP, 280);
    register_in_table(SDLK_PAGEDOWN, 281);
    register_in_table(SDLK_F1, 282);
    register_in_table(SDLK_F2, 283);
    register_in_table(SDLK_F3, 284);
    register_in_table(SDLK_F4, 285);
    register_in_table(SDLK_F5, 286);
    register_in_table(SDLK_F6, 287);
    register_in_table(SDLK_F7, 288);
    register_in_table(SDLK_F8, 289);
    register_in_table(SDLK_F9, 290);
    register_in_table(SDLK_F10, 291);
    register_in_table(SDLK_F11, 292);
    register_in_table(SDLK_F12, 293);
    register_in_table(SDLK_F13, 294);
    register_in_table(SDLK_F14, 295);
    register_in_table(SDLK_F15, 296);
    register_in_table(SDLK_NUMLOCK, 300);
    register_in_table(SDLK_CAPSLOCK, 301);
    register_in_table(SDLK_SCROLLOCK, 302);
    register_in_table(SDLK_RSHIFT, 303);
    register_in_table(SDLK_LSHIFT, 304);
    register_in_table(SDLK_RCTRL, 305);
    register_in_table(SDLK_LCTRL, 306);
    register_in_table(SDLK_RALT, 307);
    register_in_table(SDLK_LALT, 308);
    register_in_table(SDLK_RMETA, 309);
    register_in_table(SDLK_LMETA, 310);
    register_in_table(SDLK_LSUPER, 311);
    register_in_table(SDLK_RSUPER, 312);
    register_in_table(SDLK_MODE, 313);
    register_in_table(SDLK_COMPOSE, 314);
    register_in_table(SDLK_HELP, 315);
    register_in_table(SDLK_PRINT, 316);
    register_in_table(SDLK_SYSREQ, 317);
    register_in_table(SDLK_BREAK, 318);
    register_in_table(SDLK_MENU, 319);
    register_in_table(SDLK_POWER, 320);
    register_in_table(SDLK_EURO, 321);
    register_in_table(SDLK_UNDO, 322);

    lua_setfield(L, -2, "key");
}

static void
register_sdl_event_types(lua_State *L)
{
    lua_newtable(L);

    register_in_table(SDL_NOEVENT, SDL_NOEVENT);
    register_in_table(SDL_ACTIVEEVENT, SDL_ACTIVEEVENT);
    register_in_table(SDL_KEYDOWN, SDL_KEYDOWN);
    register_in_table(SDL_KEYUP, SDL_KEYUP);
    register_in_table(SDL_MOUSEMOTION, SDL_MOUSEMOTION);
    register_in_table(SDL_MOUSEBUTTONDOWN, SDL_MOUSEBUTTONDOWN);
    register_in_table(SDL_MOUSEBUTTONUP, SDL_MOUSEBUTTONUP);
    register_in_table(SDL_JOYAXISMOTION, SDL_JOYAXISMOTION);
    register_in_table(SDL_JOYBALLMOTION, SDL_JOYBALLMOTION);
    register_in_table(SDL_JOYHATMOTION,	SDL_JOYHATMOTION);
    register_in_table(SDL_JOYBUTTONDOWN, SDL_JOYBUTTONDOWN);
    register_in_table(SDL_JOYBUTTONUP, SDL_JOYBUTTONUP);
    register_in_table(SDL_QUIT, SDL_QUIT);
    register_in_table(SDL_SYSWMEVENT, SDL_SYSWMEVENT);
    register_in_table(SDL_EVENT_RESERVEDA, SDL_EVENT_RESERVEDA);
    register_in_table(SDL_EVENT_RESERVEDB, SDL_EVENT_RESERVEDB);
    register_in_table(SDL_VIDEORESIZE, SDL_VIDEORESIZE);
    register_in_table(SDL_VIDEOEXPOSE, SDL_VIDEOEXPOSE);
    register_in_table(SDL_EVENT_RESERVED2, SDL_EVENT_RESERVED2);
    register_in_table(SDL_EVENT_RESERVED3, SDL_EVENT_RESERVED3);
    register_in_table(SDL_EVENT_RESERVED4, SDL_EVENT_RESERVED4);
    register_in_table(SDL_EVENT_RESERVED5, SDL_EVENT_RESERVED5);
    register_in_table(SDL_EVENT_RESERVED6, SDL_EVENT_RESERVED6);
    register_in_table(SDL_EVENT_RESERVED7, SDL_EVENT_RESERVED7);
    register_in_table(SDL_USEREVENT, SDL_USEREVENT);
    register_in_table(SDL_NUMEVENTS, SDL_NUMEVENTS);

    lua_setfield(L, -2, "type");
}

static luaL_Reg null_lauL_Reg[] = {
    {NULL, NULL}};

int
luaopen_libblacklua(lua_State *L)
{
    /* set up the global bl table */
    luaL_register(L, "bl", null_lauL_Reg);

    /* game object table */
    lua_createtable(L, 0, sizeof(game_object_methods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, game_object_methods);
    lua_setfield(L, -2, "object");

    /* game manager table */
    lua_createtable(L, 0, sizeof(game_manager_methods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, game_manager_methods);
    lua_setfield(L, -2, "manager");

    /* set up event table */
    lua_createtable(L, 0, sizeof(event_methods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, event_methods);
    register_sdl_keys(L);
    register_sdl_event_types(L);
    lua_setfield(L, -2, "event");

    /* set up image table */
    lua_createtable(L, 0, sizeof(image_methods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, image_methods);
    lua_setfield(L, -2, "image");

    /* set up state table */
    lua_createtable(L, 0, sizeof(state_methods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, state_methods);
    lua_setfield(L, -2, "state");

    /* set up draw table */
    lua_createtable(L, 0, sizeof(draw_methods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, draw_methods);
    lua_setfield(L, -2, "draw");

	/* set up info table */
    lua_createtable(L, 0, sizeof(info_methods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, info_methods);
    lua_setfield(L, -2, "info");

    /* set up sound table */
    lua_createtable(L, 0, sizeof(sound_methods) / sizeof(luaL_reg) - 1);
    luaL_register(L, NULL, sound_methods);
    lua_setfield(L, -2, "sound");

    return 0;
}
