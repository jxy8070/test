// lua.hpp
// Lua header files for C++
// <<extern "C">> not supplied automatically because Lua also compiles as C++

#include <windows.h>
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define LUA_QL(x)	"'" x "'"
#define LUA_QS		LUA_QL("%s")

#ifdef WIN32
# ifdef libqtlua_EXPORTS
#  define QTLUAAPI __declspec(dllexport)
# else
#  define QTLUAAPI /*__declspec(dllimport)*/
# endif
#else
# define QTLUAAPI
#endif

#ifdef __cplusplus
# define QTLUA_EXTERNC extern "C"
#else
# define QTLUA_EXTERNC extern
#endif



