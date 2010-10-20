#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include "blacklua.h"

game_manager *g_game_manager;

game_manager *blacklua_get_game_manager()
{
    return g_game_manager;
}

static int
panic_func(lua_State *l)
{
    ERROR("fatal lua error: %s\n", lua_tolstring(l, -1, NULL));
    exit(1);
}

static void
lua_call_update(char *name, game_manager *gm, game_object *obj, unsigned int ticks)
{
    lua_State *l = gm->script_controller;

    lua_getglobal(l, name);
    if(lua_isnil(l, -1))
    {
        char error[512];
        snprintf(error, 512, "function \"%s\" doesn't seem to exist!\n", name);
        lua_pushstring(l, error);
        lua_error(l);
    }

    lua_pushlightuserdata(l, gm);
    lua_pushlightuserdata(l, obj);
    lua_pushinteger(l, ticks);

    int result = lua_pcall(l, 3, 0, 0);

    if(result != 0)
    {
        char *error = lua_tolstring(l, -1, NULL);
        ERROR(error);
        exit(2);
    }
}

static void
lua_call_render(char *name, game_manager *gm, game_object *obj, float interpolation)
{
    lua_State *l = gm->script_controller;

    lua_getglobal(l, name);
    if(lua_isnil(l, -1))
    {
        char error[512];
        snprintf(error, 512, "function \"%s\" doesn't seem to exist!\n", name);
        lua_pushstring(l, error);
        lua_error(l);
    }

    lua_pushlightuserdata(l, gm);
    lua_pushlightuserdata(l, obj);
    lua_pushnumber(l, interpolation);

    int result = lua_pcall(l, 3, 0, 0);

    if(result != 0)
    {
        char *error = lua_tolstring(l, -1, NULL);
        ERROR(error);
        exit(2);
    }
}

int
blacklua_init(game_manager *gm)
{
    lua_State *l;

    l = lua_open();

    gm->script_controller = l;
    gm->script_update_call = lua_call_update;
    gm->script_render_call = lua_call_render;

    luaL_openlibs(l);
    luaopen_libblacklua(l);

    lua_atpanic(l, panic_func);

    g_game_manager = gm;

    return 0;
}

int
blacklua_load(char *file)
{
    lua_State *l = g_game_manager->script_controller;
    int result = luaL_dofile(l, file);

    if(result != 0)
    {
        char *error = lua_tolstring(l, -1, NULL);
        ERROR("error occurred: %s\n\n", error);
        exit(2);
    }

    return 0;
}

void
blacklua_shutdown()
{
    if(g_game_manager)
    {
        if(g_game_manager->script_controller)
            lua_close(g_game_manager->script_controller);
        g_game_manager->script_controller = NULL;
    }
}
