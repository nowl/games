#ifndef __BLACKLUA_H__
#define __BLACKLUA_H__

#if defined(__WIN32__) || defined(_WIN32)
# define DLL_INFO __declspec(dllexport)
#else
# define DLL_INFO
#endif

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <black.h>

DLL_INFO int luaopen_libblacklua(lua_State *L);

DLL_INFO int blacklua_init(game_manager *gm);
DLL_INFO int blacklua_load(char *file);
DLL_INFO void blacklua_shutdown();

extern game_manager *g_game_manager;

#endif /* __BLACKLUA_H__ */
