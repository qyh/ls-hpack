#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lshpack.h"

typedef struct _hpack_t {
    struct lshpack_dec hdec;
    struct lshpack_enc henc;
}hpack_t;

static int newpack(lua_State *L) {
    hpack_t *p = (hpack_t*)lua_newuserdata(L, sizeof(hpack_t));
  
    lshpack_enc_init(&(p->henc));
    lshpack_dec_init(&(p->hdec));
    luaL_getmetatable(L, "ls-hpack");
    lua_setmetatable(L, -2);
    return 1;
}

static int _encode(lua_State *L) {
    void *ud = (void*)luaL_checkudata(L, 1, "ls-hpack");
    luaL_argcheck(L, ud != NULL, 1, "hpack_t expected");
    hpack_t *p = (hpack_t*)ud;
    unsigned char tmp_buf[0x100];
    size_t nlen = 0;
    size_t vlen = 0;
    const char* name = luaL_checklstring(L, 2, &nlen);
    const char* value = luaL_checklstring(L, 3, &vlen);
    unsigned char *end = lshpack_enc_encode(&(p->henc), tmp_buf, tmp_buf + sizeof(tmp_buf), name, nlen, value, vlen, 0);

    lua_pushlstring(L, tmp_buf, end-tmp_buf);
    lua_pushnumber(L, end-tmp_buf);
    return 2; 
}

static int _decode(lua_State *L) {
    void *ud = (void*)luaL_checkudata(L, 1, "ls-hpack");
    luaL_argcheck(L, ud != NULL, 1, "hpack_t expected");
    hpack_t *p = (hpack_t*)ud;
    char out[0x1000];
    size_t len = 0;
    unsigned char* tx = (unsigned char*)luaL_checklstring(L, 2, &len);
    uint16_t nlen = 0; 
    uint16_t vlen = 0;
    unsigned char* end = tx + len;
    const unsigned char* com = tx;
    lshpack_dec_decode(&(p->hdec), &com, end, out, out + sizeof(out), &nlen, &vlen);

    lua_pushlstring(L, out, nlen);
    lua_pushlstring(L, out+nlen, vlen);
    return 2;
}

static const struct luaL_Reg hpacklib_f[] = {
    {"new", newpack},
    {NULL, NULL}
};

static const struct luaL_Reg hpacklib_m[] = {
    {"encode", _encode},
    {"decode", _decode},
    {NULL, NULL}
};

static int _gc(lua_State *L) {
    hpack_t *p = (hpack_t*)lua_touserdata(L, 1);
    if (p) {
	lshpack_dec_cleanup(&(p->hdec));
	lshpack_enc_cleanup(&(p->henc));
    }
    return 0;
}

int luaopen_hpack(lua_State *L) {
    luaL_newmetatable(L, "ls-hpack");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    lua_pushstring(L, "__gc");
    lua_pushcfunction(L, _gc);
    lua_settable(L, -3);
    /*luaL_openlib(L, "hpack", hpacklib_m, 0);*/
    /*luaL_openlib(L, "hpack", hpacklib_f, 0);*/
    luaL_setfuncs(L, hpacklib_m, 0);
    luaL_newlib(L, hpacklib_f);
    return 1;
}
