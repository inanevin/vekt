#pragma once

#include <string>
#include <unordered_map>

namespace vekt
{
	struct basic_draw_buffer;
	struct text_draw_buffer;
	class builder;
	class atlas;
}

class gl_backend
{
public:
	struct shader_data
	{
		unsigned int								  handle = 0;
		std::unordered_map<std::string, unsigned int> uniforms;
	};
	void init(vekt::builder& builder);
	void uninit();
	void start_frame();
	void end_frame();

	inline void set_display(unsigned int width, unsigned int height)
	{
		_width	= width;
		_height = height;
	}

private:
	void create_shader(shader_data& data, const char* vert, const char* frag);
	void set_scissors(float x, float y, float w, float h);
	void create_font_texture(unsigned int width, unsigned int height);
	void draw_basic(const vekt::basic_draw_buffer& db);
	void draw_text(const vekt::text_draw_buffer& db);
	void atlas_created(vekt::atlas* atlas);
	void atlas_updated(vekt::atlas* atlas);
	void atlas_destroyed(vekt::atlas* atlas);

private:
	unsigned int _vao		   = 0;
	unsigned int _vbo		   = 0;
	unsigned int _ebo		   = 0;
	unsigned int _width		   = 0;
	unsigned int _height	   = 0;
	float		 _proj[4][4]   = {0};
	unsigned int _font_texture = 0;
	bool		 _skipped_draw = false;
	shader_data	 _basic_shader;
	shader_data	 _text_shader;
};