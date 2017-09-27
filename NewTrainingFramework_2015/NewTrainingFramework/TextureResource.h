#pragma once
#include <string>
#include <GLES2/gl2.h>
#include <EGL/egl.h>

enum TextureType {
	_2d,
	_3d,
	cubemap
};

struct TextureResource {
	TextureType type;
	std::string path;
	GLenum min_filter;
	GLenum mag_filter;
	GLenum wrap_s;
	GLenum wrap_t;
};