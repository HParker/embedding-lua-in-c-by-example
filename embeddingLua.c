#include <stdio.h>
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

// TUTORIAL: https://www.oreilly.com/library/view/creating-solid-apis/9781491986301/ch01.html

int basicCFunc(lua_State * L) {
  printf("This is my c function called from lua\n");
  return 0; // number of results
}

int returningCFunc(lua_State * L) {
  lua_pushnumber(L, 678); // result
  return 1; // number of results
}

int argumentCFunc(lua_State * L) {
  lua_pushnumber(L, lua_tonumber(L, 1) + 1); // result
  return 1; // number of results
}

int variableArgumentCFunc(lua_State * L) {
  int n = lua_gettop(L); // number of arguments
  float sum = 0.0;
  int i;
  for (i = 1; i <= n; i++) {
    if (!lua_isnumber(L, i)) {
      lua_pushliteral(L, "incorrect argument");
      lua_error(L);
    }
    sum += lua_tonumber(L, i);
  }
  lua_pushnumber(L, sum); // result
  return 1; // number of results
}

int main() {
  // setup lua
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  // load a file
  // luaL_dofile(L, "hello.lua");

  // Run a lua string
  luaL_dostring(L, "print(\"hello from lua\")");

  // get number
  luaL_dostring(L, "x = 10");
  lua_getglobal(L, "x");
  int x = (int)lua_tonumber(L, -1);
  printf("x = %i\n", x);

  // get string
  luaL_dostring(L, "string = 'hi there'");
  lua_getglobal(L, "string");
  const char * string = lua_tolstring(L, -1, NULL);
  printf("string = '%s'\n", string);

  // get table field
  luaL_dostring(L, "table = { key = 123 }");
  lua_getglobal(L, "table");
  lua_getfield(L, -1, "key");
  int key = (int)lua_tonumber(L, -1);
  printf("key = %i\n", key);

  // call lua function
  luaL_dostring(L, "function foo() print(\"Hello from foo method\") end");
  lua_getglobal(L, "foo");
  if (lua_isfunction(L, -1)) {
    //                        V number of arguments
    //                        |  V number of results
    int status = lua_pcall(L, 0, 0, 0);
    // status can be LUA_OK, LUA_ERRRUN, LUA_ERRMEM, or LUA_ERRERR
    if (status != LUA_OK) {
      printf("Something went wrong when calling the lua function\n");
    } else {
      int result = (int)lua_tonumber(L, -1);
      printf("result = %i\n", result);
    }
  } else {
    printf("function `foo` was not found or was not a function\n");
  }

  // call lua function with a return value
  luaL_dostring(L, "function foo() return 456 end");
  lua_getglobal(L, "foo");
  if (lua_isfunction(L, -1)) {
    //                        V number of arguments
    //                        |  V number of results
    int status = lua_pcall(L, 0, 1, 0);
    // status can be LUA_OK, LUA_ERRRUN, LUA_ERRMEM, or LUA_ERRERR
    if (status != LUA_OK) {
      printf("Something went wrong when calling the lua function\n");
    } else {
      int result = (int)lua_tonumber(L, -1);
      printf("result = %i\n", result);
    }
  } else {
    printf("function `foo` was not found or was not a function\n");
  }


  // call a function with arguments
  luaL_dostring(L, "function square(x) return x * x end");
  lua_getglobal(L, "square");
  if (lua_isfunction(L, -1)) {
    //                        V number of arguments
    //                        |  V number of results
    lua_pushnumber(L, (lua_Number)5);
    int status = lua_pcall(L, 1, 1, 0);
    // status can be LUA_OK, LUA_ERRRUN, LUA_ERRMEM, or LUA_ERRERR
    if (status != LUA_OK) {
      printf("Something went wrong when calling the lua function\n");
      printf("Error: %s \n", lua_tostring(L, -1));
      lua_pop(L, 1);
    } else {
      int doubled = (int)lua_tonumber(L, -1);
      printf("doubled = %i\n", doubled);
    }
  } else {
    printf("function `square` was not found or was not a function\n");
  }

  // expose a function to lua
  lua_register(L, "basicCFunc", basicCFunc);
  luaL_dostring(L, "basicCFunc()");

  // expose a function to lua that returns a value
  lua_register(L, "returningCFunc", returningCFunc);
  luaL_dostring(L, "returnedFromC = returningCFunc()");
  lua_getglobal(L, "returnedFromC");
  int returnedFromC = (int)lua_tonumber(L, -1);
  printf("returned from c = %i\n", returnedFromC);

  // expose a function to lua that takes an argument
  lua_register(L, "argumentCFunc", argumentCFunc);
  luaL_dostring(L, "returnedFromC = argumentCFunc(1)");
  lua_getglobal(L, "returnedFromC");
  returnedFromC = (int)lua_tonumber(L, -1);
  printf("returned from c = %i\n", returnedFromC);

  // expose a function to lua that takes any number of arguments
  lua_register(L, "variableArgumentCFunc", variableArgumentCFunc);
  luaL_dostring(L, "returnedFromC = variableArgumentCFunc(1, 2, 3)");
  lua_getglobal(L, "returnedFromC");
  returnedFromC = (int)lua_tonumber(L, -1);
  printf("returned from c = %i\n", returnedFromC);

  return 0;
}
