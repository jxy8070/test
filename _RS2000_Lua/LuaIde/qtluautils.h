// -*- C -*-

#ifndef QTLUAUTILS_H
#define QTLUAUTILS_H

#include "lua.hpp"




#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM==501
# define lua_pushglobaltable(L) lua_pushvalue(L, LUA_GLOBALSINDEX)
# define lua_setuservalue lua_setfenv
# define lua_getuservalue lua_getfenv
# define luaQ_typerror luaL_typerror
static void luaL_setfuncs (lua_State *L, const luaL_Reg *l, int nup) {
  luaL_checkstack(L, nup+1, "too many upvalues");
  for (; l->name != NULL; l++) {  /* fill the table with given functions */
    int i;
    lua_pushstring(L, l->name);
    for (i = 0; i < nup; i++)  /* copy upvalues to the top */
      lua_pushvalue(L, -(nup+1));
    lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
    lua_settable(L, -(nup + 3));
  }
  lua_pop(L, nup);  /* remove upvalues */
}
#else
# define lua_objlen lua_rawlen
# define LUA_PATHSEP     ";"

static int luaQ_typerror(lua_State *L, int narg, const char *tname)
{
  return luaL_error(L, "%s expected, got %s", tname, luaL_typename(L, narg));
}
#endif

#ifdef __cplusplus
# define QTLUA_EXTERNC extern "C"
#else
# define QTLUA_EXTERNC extern
#endif

QTLUA_EXTERNC QTLUAAPI void luaQ_getfield(lua_State *L, int index, const char *name);
QTLUA_EXTERNC QTLUAAPI int  luaQ_tracebackskip(lua_State *L, int skip);
QTLUA_EXTERNC QTLUAAPI int  luaQ_traceback(lua_State *L);
QTLUA_EXTERNC QTLUAAPI int  luaQ_complete(lua_State *L);
QTLUA_EXTERNC QTLUAAPI int  luaQ_print(lua_State *L, int nr);
QTLUA_EXTERNC QTLUAAPI int  luaQ_pcall(lua_State *L, int na, int nr, int eh, int oh);
QTLUA_EXTERNC QTLUAAPI void luaQ_doevents(lua_State *L);
QTLUA_EXTERNC QTLUAAPI int  luaopen_qt(lua_State *L);

#endif




/* -------------------------------------------------------------
   Local Variables:
   c++-font-lock-extra-types: ( "\\sw+_t" "lua_[A-Z]\\sw*[a-z]\\sw*" )
   c-font-lock-extra-types: ( "\\sw+_t" "lua_[A-Z]\\sw*[a-z]\\sw*" )
   End:
   ------------------------------------------------------------- */
