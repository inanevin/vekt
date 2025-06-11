#include "gl_backend.hpp"
#include "glad/glad.h"
#include "vekt.hpp"
#include <functional>

void gl_backend::init(vekt::builder& builder)
{

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	glBindVertexArray(_vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	// glEnable(GL_DEPTH_TEST);
	//  glDisable(GL_DEPTH_TEST);
	// glDisable(GL_STENCIL_TEST);
	 glEnable(GL_SCISSOR_TEST);
	// glDepthMask(GL_FALSE);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glBlendEquation(static_cast<GLenum>(true));
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	const char* BASIC_VERT = "#version 330 core\n"
							 "layout (location = 0) in vec2 pos;\n"
							 "layout (location = 1) in vec2 uv;\n"
							 "layout (location = 2) in vec4 col;\n"
							 "uniform mat4 proj; \n"
							 "out vec4 fCol;\n"
							 "out vec2 fUV;\n"
							 "void main()\n"
							 "{\n"
							 "   fCol = col;\n"
							 "   fUV = uv;\n"
							 "   gl_Position = proj * vec4(pos.x, pos.y, 0.0f, 1.0);\n"
							 "}\0";

	const char* BASIC_FRAG = "#version 330 core\n"
							 "out vec4 fragColor;\n"
							 "in vec2 fUV;\n"
							 "in vec4 fCol;\n"
							 "void main()\n"
							 "{\n"
							 "   fragColor = fCol ; \n"
							 "}\0";
	const char* TEXT_FRAG  = "#version 330 core\n"
							 "out vec4 fragColor;\n"
							 "in vec2 fUV;\n"
							 "in vec4 fCol;\n"
							 "uniform sampler2D diffuse;\n"
							 "void main()\n"
							 "{\n"
							 "fragColor = vec4(fCol.rgb, texture(diffuse, fUV).r * fCol.a);\n"
							 "}\0";

	create_shader(_basic_shader, BASIC_VERT, BASIC_FRAG);
	create_shader(_text_shader, BASIC_VERT, TEXT_FRAG);

	builder.set_on_draw_basic(std::bind(&gl_backend::draw_basic, this, std::placeholders::_1));
	builder.set_on_draw_text(std::bind(&gl_backend::draw_text, this, std::placeholders::_1));
	vekt::font_manager::get().set_atlas_created_callback(std::bind(&gl_backend::atlas_created, this, std::placeholders::_1));
	vekt::font_manager::get().set_atlas_updated_callback(std::bind(&gl_backend::atlas_updated, this, std::placeholders::_1));
	vekt::font_manager::get().set_atlas_destroyed_callback(std::bind(&gl_backend::atlas_destroyed, this, std::placeholders::_1));
}

void gl_backend::uninit()
{
	glDeleteShader(_basic_shader.handle);
	glDeleteShader(_text_shader.handle);
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void gl_backend::start_frame()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor((GLfloat)0.8f, (GLfloat)0.8f, (GLfloat)0.8f, (GLfloat)1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glViewport(0, 0, (GLsizei)_width, (GLsizei)_height);

	int fb_width  = (int)(_width);
	int fb_height = (int)(_height);

	if (fb_width <= 0 || fb_height <= 0)
	{
		_skipped_draw = true;
		return;
	}
	_skipped_draw = false;

	glPolygonMode(GL_FRONT_AND_BACK, _is_wireframe ? GL_LINE : GL_FILL);

	float L = static_cast<float>(0.0f);
	float R = static_cast<float>(0.0f + _width);
	float T = static_cast<float>(0.0f);
	float B = static_cast<float>(0.0f + _height);

	L *= _zoom;
	R *= _zoom;
	T *= _zoom;
	B *= _zoom;

	// debug offsets.
	L += _debug_offsets[0];
	R += _debug_offsets[0];
	T += _debug_offsets[1];
	B += _debug_offsets[1];

	_proj[0][0] = 2.0f / (R - L);
	_proj[0][1] = 0.0f;
	_proj[0][2] = 0.0f;
	_proj[0][3] = 0.0f;

	_proj[1][0] = 0.0f;
	_proj[1][1] = 2.0f / (T - B);
	_proj[1][2] = 0.0f;
	_proj[1][3] = 0.0f;

	_proj[2][0] = 0.0f;
	_proj[2][1] = 0.0f;
	_proj[2][2] = -1.0f;
	_proj[2][3] = 0.0f;

	_proj[3][0] = (R + L) / (L - R);
	_proj[3][1] = (T + B) / (B - T);
	_proj[3][2] = 0.0f;
	_proj[3][3] = 1.0f;

	glBindVertexArray(_vao);
}

void gl_backend::end_frame()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void gl_backend::create_shader(shader_data& data, const char* vert, const char* frag)
{
	unsigned int vertex, fragment;
	int			 success;
	char		 infoLog[512];

	// VTX
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vert, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		V_ERR("vekt::gl_backend::create_shader -> Shader vertex compilation failed!");
		return;
	}

	// FRAG
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &frag, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		V_ERR("vekt::gl_backend::create_shader -> Shader fragment compilation failed!");
		return;
	}

	GLuint handle = glCreateProgram();
	glAttachShader(handle, vertex);
	glAttachShader(handle, fragment);
	glLinkProgram(handle);

	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(handle, 512, NULL, infoLog);
		V_ERR("vekt::gl_backend::create_shader -> Shader linking failed!");
		return;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	data.handle = handle;

	// Load uniforms.
	GLint numUniforms = 0;
	glGetProgramiv(data.handle, GL_ACTIVE_UNIFORMS, &numUniforms);

	// Iterate through uniforms.
	GLchar chars[256];
	for (GLint uniform = 0; uniform < numUniforms; ++uniform)
	{
		GLint	arraySize	 = 0;
		GLenum	type		 = 0;
		GLsizei actualLength = 0;

		// Get sampler uniform data & store it on our sampler map.
		glGetActiveUniform(data.handle, uniform, (GLsizei)256, &actualLength, &arraySize, &type, &chars[0]);

		GLint loc				 = glGetUniformLocation(data.handle, (char*)&chars[0]);
		data.uniforms[&chars[0]] = loc;
	}
}

void gl_backend::set_scissors(float x, float y, float w, float h)
{
	int a = 5;
	glScissor(x, static_cast<GLint>(_height - (y + h)), static_cast<GLint>(w), static_cast<GLint>(h));
}

void gl_backend::create_font_texture(unsigned int width, unsigned int height) {}

void gl_backend::draw_basic(const vekt::basic_draw_buffer& db)
{
	if (_skipped_draw) return;
	set_scissors(db.clip.x, db.clip.y, db.clip.z, db.clip.w);

	shader_data& data = _basic_shader;

	glUseProgram(data.handle);
	glUniformMatrix4fv(data.uniforms["proj"], 1, GL_FALSE, &_proj[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, db.vertices.size() * sizeof(vekt::vertex_basic), (const GLvoid*)db.vertices.begin(), GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, db.indices.size() * sizeof(vekt::index), (const GLvoid*)db.indices.begin(), GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES, (GLsizei)db.indices.size(), sizeof(vekt::index) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0);
}

void gl_backend::draw_text(const vekt::text_draw_buffer& db)
{
	if (_skipped_draw) return;
	set_scissors(db.clip.x, db.clip.y, db.clip.z, db.clip.w);

	shader_data& data = _text_shader;

	glUseProgram(data.handle);
	glUniformMatrix4fv(data.uniforms["proj"], 1, GL_FALSE, &_proj[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _font_texture);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, db.vertices.size() * sizeof(vekt::vertex_basic), (const GLvoid*)db.vertices.begin(), GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, db.indices.size() * sizeof(vekt::index), (const GLvoid*)db.indices.begin(), GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES, (GLsizei)db.indices.size(), sizeof(vekt::index) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, 0);
}

void gl_backend::atlas_created(vekt::atlas* atlas)
{
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas->get_width(), atlas->get_height(), 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_font_texture = tex;
}

void gl_backend::atlas_updated(vekt::atlas* atlas)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, atlas->get_width(), atlas->get_height(), GL_RED, GL_UNSIGNED_BYTE, atlas->get_data());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 0);
}

void gl_backend::atlas_destroyed(vekt::atlas* atlas) { glDeleteTextures(1, &_font_texture); }
