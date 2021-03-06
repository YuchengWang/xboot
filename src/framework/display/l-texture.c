/*
 * framework/display/l-texture.c
 *
 * Copyright(c) 2007-2016 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <cairo.h>
#include <cairoint.h>
#include <xfs/xfs.h>
#include <framework/display/l-display.h>

static cairo_status_t xfs_read_func(void * closure, unsigned char * data, unsigned int size)
{
	struct xfs_file_t * file = closure;
	size_t ret;

    while(size)
    {
    	ret = xfs_read(file, data, 1, size);
    	size -= ret;
    	data += ret;
    	if(size && xfs_eof(file))
    		return _cairo_error(CAIRO_STATUS_READ_ERROR);
    }
    return CAIRO_STATUS_SUCCESS;
}

static cairo_surface_t * cairo_image_surface_create_from_png_xfs(const char * filename)
{
	struct xfs_file_t * file;
	cairo_surface_t * surface;

	file = xfs_open_read(filename);
	if(!file)
		return _cairo_surface_create_in_error(_cairo_error(CAIRO_STATUS_FILE_NOT_FOUND));
	surface = cairo_image_surface_create_from_png_stream(xfs_read_func, file);
	xfs_close(file);
    return surface;
}

static int l_texture_new(lua_State * L)
{
	const char * filename = luaL_checkstring(L, 1);
	struct ltexture_t * texture = lua_newuserdata(L, sizeof(struct ltexture_t));
	texture->surface = cairo_image_surface_create_from_png_xfs(filename);
	if(cairo_surface_status(texture->surface) != CAIRO_STATUS_SUCCESS)
		return 0;
	luaL_setmetatable(L, MT_TEXTURE);
	return 1;
}

static const luaL_Reg l_texture[] = {
	{"new",	l_texture_new},
	{NULL,	NULL}
};

static int m_texture_gc(lua_State * L)
{
	struct ltexture_t * texture = luaL_checkudata(L, 1, MT_TEXTURE);
	cairo_surface_destroy(texture->surface);
	return 0;
}

static int m_texture_size(lua_State * L)
{
	struct ltexture_t * texture = luaL_checkudata(L, 1, MT_TEXTURE);
	int w = cairo_image_surface_get_width(texture->surface);
	int h = cairo_image_surface_get_height(texture->surface);
	lua_pushnumber(L, w);
	lua_pushnumber(L, h);
	return 2;
}

static int m_texture_region(lua_State * L)
{
	struct ltexture_t * texture = luaL_checkudata(L, 1, MT_TEXTURE);
	int x = luaL_optinteger(L, 2, 0);
	int y = luaL_optinteger(L, 3, 0);
	int w = luaL_optinteger(L, 4, cairo_image_surface_get_width(texture->surface));
	int h = luaL_optinteger(L, 5, cairo_image_surface_get_height(texture->surface));
	struct ltexture_t * tex = lua_newuserdata(L, sizeof(struct ltexture_t));
	tex->surface = cairo_surface_create_similar(texture->surface, cairo_surface_get_content(texture->surface), w, h);
	cairo_t * cr = cairo_create(tex->surface);
	cairo_set_source_surface(cr, texture->surface, -x, -y);
	cairo_paint(cr);
	cairo_destroy(cr);
	luaL_setmetatable(L, MT_TEXTURE);
	return 1;
}

static const luaL_Reg m_texture[] = {
	{"__gc",		m_texture_gc},
	{"size",		m_texture_size},
	{"region",		m_texture_region},
	{NULL,			NULL}
};

int luaopen_texture(lua_State * L)
{
	luaL_newlib(L, l_texture);
	luahelper_create_metatable(L, MT_TEXTURE, m_texture);
	return 1;
}
