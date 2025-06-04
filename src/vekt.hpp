// Copyright (c) 2025 Inan Evin

#ifndef VEKT_HPP
#define VEKT_HPP

#include <memory>
#include <assert.h>
#include <algorithm>
#include <cmath>
#include <fstream>

#define VEKT_INLINE inline
#define VEKT_API	extern

namespace vekt
{

#define M_PI 3.14159265358979

#ifndef VEKT_USER_DATA_SIZE
#define VEKT_USER_DATA_SIZE 1024
#endif

#if defined _MSC_VER && !__INTEL_COMPILER
#define ALIGNED_MALLOC(SZ, ALIGN) _aligned_malloc(SZ, ALIGN)
#define ALIGNED_FREE(PTR)		  _aligned_free(PTR);
#else
#include <stdlib.h>
#define ALIGNED_MALLOC(SZ, ALIGN) std::aligned_alloc(ALIGN, SZ)
#define ALIGNED_FREE(PTR)		  std::free(PTR)
#endif

#define MALLOC(SZ)	malloc(SZ)
#define MEMCPY(...) memcpy(__VA_ARGS__)
#define FREE(X)		free(X)
#define ASSERT(...) assert(__VA_ARGS__)

	typedef unsigned int id;

	template <typename T>
	class pod_vector
	{
	public:
		using iterator		 = T*;
		using const_iterator = const T*;

		pod_vector() {};
		pod_vector(const pod_vector<T>& other)
		{
			_elements = reinterpret_cast<T*>(ALIGNED_MALLOC(sizeof(T) * static_cast<size_t>(other._capacity), alignof(T)));
			MEMCPY(_elements, other._elements, sizeof(T) * static_cast<size_t>(other._count));
			_count	  = other._count;
			_capacity = other._capacity;
		}
		~pod_vector() { clear(); }

		inline void push_back(const T& elem)
		{
			check_grow();
			new (&_elements[_count]) T(elem);
			_count++;
		}

		inline void remove(unsigned int index)
		{
			ASSERT(index < _count);
			ASSERT(_elements);

			const size_t dest_index		  = static_cast<size_t>(index);
			const size_t move_start_index = static_cast<size_t>(index) + 1;
			const size_t move_count		  = static_cast<size_t>(_count - index - 1);
			if (move_count != 0) MEMCPY(_elements + dest_index, _elements + move_start_index, sizeof(T) * move_count);
			_count--;
		}

		inline void remove(T& elem)
		{
			for (unsigned int i = 0; i < _count; i++)
			{
				if (elem == _elements[i])
				{
					remove(i);
					break;
				}
			}
		}
		inline void clear()
		{
			for (size_t i = 0; i < _count; i++)
				_elements[i].~T();

			if (_elements) ALIGNED_FREE(_elements);
			_capacity = 0;
			_count	  = 0;
			_elements = nullptr;
		}

		inline void resize(unsigned int sz)
		{
			T* new_elems = reinterpret_cast<T*>(ALIGNED_MALLOC(sizeof(T) * static_cast<size_t>(sz), alignof(T)));

			if (_elements)
			{
				MEMCPY(new_elems, _elements, sizeof(T) * static_cast<size_t>(_count > sz ? sz : _count));
				ALIGNED_FREE(_elements);
			}
			_elements = new_elems;

			if (_count > sz) _count = sz;

			_capacity = sz;
		}

		inline T* data() { return _elements; }

		T&					operator[](unsigned int index) { return _elements[index]; }
		const T&			operator[](unsigned int index) const { return _elements[index]; }
		inline bool			empty() const { return _count == 0; }
		inline unsigned int size() const { return _count; }

		iterator	   begin() { return _elements; }
		const_iterator begin() const { return _elements; }
		const_iterator cbegin() const { return _elements; }
		iterator	   end() { return _elements + _count; }
		const_iterator end() const { return _elements + _count; }
		const_iterator cend() const { return _elements + _count; }

	private:
		inline void check_grow()
		{
			if (_count < _capacity) return;
			_capacity	 = _capacity == 0 ? 1 : _capacity * 2;
			T* new_elems = reinterpret_cast<T*>(ALIGNED_MALLOC(sizeof(T) * static_cast<size_t>(_capacity), alignof(T)));

			if (_elements)
			{
				MEMCPY(new_elems, _elements, sizeof(T) * _count);
				ALIGNED_FREE(_elements);
			}

			_elements = new_elems;
		}

	private:
		T*			 _elements = nullptr;
		unsigned int _count	   = 0;
		unsigned int _capacity = 0;
	};

	struct handle
	{
		id	 value = 0;
		bool alive = false;
	};

	template <typename T>
	class pool
	{
	public:
		~pool() { clear(); }

		inline void init(unsigned int size)
		{
			_elements = reinterpret_cast<T*>(ALIGNED_MALLOC(sizeof(T) * size, alignof(T)));
			_size	  = size;
			_head	  = 0;

			for (unsigned int i = 0; i < size; i++)
			{
				_elements[i]._pool_handle.value = i;
				_elements[i]._pool_handle.alive = false;
			}
		}

		inline void clear()
		{
			if (_elements) ALIGNED_FREE(_elements);
			_size = 0;
			_head = 0;
		}

		inline T* allocate()
		{
			if (!_freelist.empty())
			{
				const unsigned int index			= _freelist[_freelist.size() - 1];
				_elements[index]._pool_handle.alive = true;
				_freelist.remove(_freelist.size() - 1);
				return &_elements[index];
			}

			ASSERT(_head < _size);
			_elements[_head]._pool_handle.alive = true;
			const unsigned int index			= _head++;
			return &_elements[index];
		}

		inline void deallocate(T* ptr)
		{
			ASSERT(ptr);
			ASSERT(ptr->_pool_handle.alive);
			ptr->~T();
			_freelist.push_back(ptr->_pool_handle.value);
			ptr->_pool_handle.alive = false;
		}

	public:
		class iterator
		{
		public:
			using iterator_category = std::forward_iterator_tag;
			using value_type		= T;
			using diff_type			= std::ptrdiff_t;
			using pointer			= T*;
			using reference			= T&;

			iterator(pool<T>* p, unsigned int current_index) : _pool(p), _current_index(current_index) {};

			T*	 operator*() const { return &_pool->_elements[_current_index]; }
			T*	 operator->() const { return &_pool->_elements[_current_index]; }
			bool operator==(const iterator& other) const { return _current_index == other._current_index; }
			bool operator!=(const iterator& other) const { return _current_index != other._current_index; }

			iterator& operator++()
			{
				do
				{
					_current_index++;
				} while (_current_index < _pool->_size && !_pool->_elements[_current_index]._pool_handle.alive);
				return *this;
			}

			iterator& operator++(int)
			{
				iterator temp = *this;
				operator++();
				return temp;
			}

		private:
			pool<T>*	 _pool			= nullptr;
			unsigned int _current_index = 0;
		};

		iterator begin() { return iterator(this, 0); }
		iterator end() { return iterator(this, _size); }

	private:
		T*						 _elements = nullptr;
		unsigned int			 _size	   = 0;
		unsigned int			 _head	   = 0;
		pod_vector<unsigned int> _freelist = {};
	};

	enum class log_verbosity
	{
		info,
		warning,
		error
	};

	typedef void (*log_callback)(log_verbosity, const char*, ...);

	struct config_data
	{
		log_callback on_log		  = nullptr;
		unsigned int atlas_width  = 1024;
		unsigned int atlas_height = 1024;
	};

	extern config_data config;

#define V_LOG(...)                                                                                                                                                                                                                                                 \
	if (config.on_log) config.on_log(log_verbosity::info, __VA_ARGS__)
#define V_ERR(...)                                                                                                                                                                                                                                                 \
	if (config.on_log) config.on_log(log_verbosity::error, __VA_ARGS__)
#define V_WARN(...)                                                                                                                                                                                                                                                \
	if (config.on_log) config.on_log(log_verbosity::warning, __VA_ARGS__)

	struct vec2
	{
		float x = 0.0f;
		float y = 0.0f;

		vec2 operator+(const vec2& other) const
		{
			vec2 v = {};
			v.x	   = x + other.x;
			v.y	   = y + other.y;
			return v;
		}

		vec2 operator*(float f) const
		{
			vec2 v = *this;
			v.x *= f;
			v.y *= f;
			return v;
		}

		vec2 operator-(const vec2& other) const
		{
			vec2 v = *this;
			v.x -= other.x;
			v.y -= other.y;
			return v;
		}

		vec2 operator/=(const vec2& other)
		{
			vec2 v = *this;
			v.x /= other.x;
			v.y /= other.y;
			return v;
		}

		vec2 operator/=(float f)
		{
			vec2 v = *this;
			v.x /= f;
			v.y /= f;
			return v;
		}

		vec2 operator*=(float f)
		{
			vec2 v = *this;
			v.x *= f;
			v.y *= f;
			return v;
		}

		vec2 operator+=(const vec2& other) { return *this + other; }

		inline void normalize()
		{
			const float s = mag();
			x /= s;
			y /= s;
		}

		inline vec2 normalized()
		{
			vec2 v = *this;
			v.normalize();
			return v;
		}

		inline float mag() { return sqrt(x * x + y * y); }
		inline float mag2() { return x * x + y * y; }
	};

	struct vec4
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;
	};
	class math
	{
	public:
		template <typename T>
		static inline T max(T a, T b)
		{
			return a > b ? a : b;
		}
		template <typename T>
		static inline T min(T a, T b)
		{
			return a < b ? a : b;
		}
		static inline float equals(float a, float b, float eps = 0.0001f) { return a > b - eps && a < b + eps; }
		static inline float cos(float x) { return std::cos(x); }
		static inline float sin(float x) { return std::sin(x); }
		static inline float lerp(float a, float b, float t) { return a + (b - a) * t; }
		static inline float ceilf(float f) { return std::ceilf(f); }
	};

	enum class input_event_type
	{
		pressed,
		released,
		repeated,
	};

	enum class input_event_result
	{
		handled,
		not_handled,
	};

	enum class input_event_phase
	{
		tunneling,
		bubbling,
	};

	enum widget_flags
	{
		wf_pos_x_relative		 = 1 << 0,
		wf_pos_y_relative		 = 1 << 1,
		wf_pos_x_absolute		 = 1 << 2,
		wf_pos_y_absolute		 = 1 << 3,
		wf_size_x_relative		 = 1 << 4,
		wf_size_y_relative		 = 1 << 5,
		wf_size_x_absolute		 = 1 << 6,
		wf_size_y_absolute		 = 1 << 7,
		wf_size_x_copy_y		 = 1 << 9,
		wf_size_y_copy_x		 = 1 << 9,
		wf_size_x_total_children = 1 << 10,
		wf_size_x_max_children	 = 1 << 11,
		wf_size_y_total_children = 1 << 12,
		wf_size_y_max_children	 = 1 << 13,
		wf_size_x_fill			 = 1 << 14,
		wf_size_y_fill			 = 1 << 15,
		wf_visible				 = 1 << 16,
		wf_pos_anchor_x_mid		 = 1 << 17,
		wf_pos_anchor_x_end		 = 1 << 18,
		wf_pos_anchor_y_mid		 = 1 << 19,
		wf_pos_anchor_y_end		 = 1 << 20,
	};

	enum class widget_type
	{
		plain,
		row,
		column,
		text,
		button,
		slider,
		dropdown,
		input,
		custom,
	};

	enum class helper_pos_type
	{
		absolute,
		relative,
	};

	enum class child_positioning
	{
		none,
		row,
		column,
	};

	enum class helper_size_type
	{
		absolute,
		relative,
		fill,
		max_children,
		total_children,
		copy_other,
	};

	enum class helper_anchor_type
	{
		start,
		middle,
		end
	};

	struct mouse_event
	{
		input_event_type type	= input_event_type::pressed;
		int				 button = 0;
		unsigned int	 x		= 0;
		unsigned int	 y		= 0;
	};

	struct mouse_wheel_event
	{
		float amount = 0.0f;
	};

	struct key_event
	{
		input_event_type type	   = input_event_type::pressed;
		int				 key	   = 0;
		int				 scan_code = 0;
	};

	struct margins
	{
		float top	 = 0.0f;
		float bottom = 0.0f;
		float left	 = 0.0f;
		float right	 = 0.0f;
	};

	enum class direction
	{
		horizontal,
		vertical
	};

	struct gfx_rect
	{
		vec4		 color_start	 = {};
		vec4		 color_end		 = {};
		float		 rounding		 = 0.0f;
		unsigned int thickness		 = 0;
		unsigned int aa_thickness	 = 0;
		direction	 color_direction = direction::horizontal;
	};

	struct gfx_text
	{
		const char* text = nullptr;
		id			font = 0;
	};

	enum class gfx_type
	{
		none,
		rect,
		text,
	};

	class widget;

	typedef void (*custom_pos_pass)(widget*);
	typedef void (*custom_size_pass)(widget*);
	typedef input_event_result (*custom_mouse_event)(widget* w, const mouse_event& ev, input_event_phase& phase, widget*& last_widget);
	typedef input_event_result (*custom_key_event)(widget* w, const key_event& ev, input_event_phase& phase, widget*& last_widget);
	typedef input_event_result (*custom_mouse_wheel_event)(widget* w, const mouse_wheel_event& ev, input_event_phase& phase, widget*& last_widget);

	struct widget_data
	{
		widget*					 parent			   = nullptr;
		custom_mouse_event		 on_mouse		   = nullptr;
		custom_mouse_wheel_event on_mouse_wheel	   = nullptr;
		custom_key_event		 on_key			   = nullptr;
		custom_pos_pass			 custom_pos_pass   = nullptr;
		custom_size_pass		 custom_size_pass  = nullptr;
		pod_vector<widget*>		 children		   = {};
		widget_type				 type			   = widget_type::plain;
		child_positioning		 child_positioning = child_positioning::none;
		margins					 margins		   = {};
		vec2					 pos			   = {};
		vec2					 size			   = {};
		vec2					 final_pos		   = {};
		vec2					 final_size		   = {};
		unsigned int			 flags			   = 0;
		float					 spacing		   = 0.0f;
	};

	struct widget_gfx
	{
		union gfx {
			gfx_text text;
			gfx_rect rect;
		} gfx;

		void*		 user_data	= nullptr;
		gfx_type	 type		= gfx_type::none;
		unsigned int draw_order = 0;

		template <typename T>
		T& get_data()
		{
			return reinterpret_cast<T&>(gfx);
		}
	};

	class widget
	{
	public:
		widget() = default;
		widget(const widget_data& data) : _widget_data(data) {};

		void add_child(widget* w);
		void remove_child(widget* w);
		void set_visible(bool is_visible, bool recursive);

		inline void set_pos_x(float x, helper_pos_type type, helper_anchor_type anchor = helper_anchor_type::start)
		{
			_widget_data.pos.x = x;
			_widget_data.flags &= ~(widget_flags::wf_pos_x_relative | widget_flags::wf_pos_x_absolute | widget_flags::wf_pos_anchor_x_mid | widget_flags::wf_pos_anchor_x_end);

			switch (type)
			{
			case helper_pos_type::relative:
				_widget_data.flags |= widget_flags::wf_pos_x_relative;
				break;
			case helper_pos_type::absolute:
				_widget_data.flags |= widget_flags::wf_pos_x_absolute;
				break;
			default:
				break;
			}

			switch (anchor)
			{
			case helper_anchor_type::middle:
				_widget_data.flags |= widget_flags::wf_pos_anchor_x_mid;
				break;
			case helper_anchor_type::end:
				_widget_data.flags |= widget_flags::wf_pos_anchor_x_end;
				break;
			default:
				break;
			}
		}

		inline void set_pos_y(float y, helper_pos_type type, helper_anchor_type anchor = helper_anchor_type::start)
		{
			_widget_data.pos.y = y;
			_widget_data.flags &= ~(widget_flags::wf_pos_y_relative | widget_flags::wf_pos_y_absolute | widget_flags::wf_pos_anchor_y_mid | widget_flags::wf_pos_anchor_y_end);

			switch (type)
			{
			case helper_pos_type::relative:
				_widget_data.flags |= widget_flags::wf_pos_y_relative;
				break;
			case helper_pos_type::absolute:
				_widget_data.flags |= widget_flags::wf_pos_y_absolute;
				break;
			default:
				break;
			}

			switch (anchor)
			{
			case helper_anchor_type::middle:
				_widget_data.flags |= widget_flags::wf_pos_anchor_y_mid;
				break;
			case helper_anchor_type::end:
				_widget_data.flags |= widget_flags::wf_pos_anchor_y_end;
				break;
			default:
				break;
			}
		}

		inline void set_width(float width, helper_size_type type)
		{
			_widget_data.size.x = width;
			_widget_data.flags &= ~(widget_flags::wf_size_x_absolute | widget_flags::wf_size_x_relative | widget_flags::wf_size_x_fill | widget_flags::wf_size_x_copy_y | widget_flags::wf_size_x_max_children | widget_flags::wf_size_x_total_children);

			switch (type)
			{
			case helper_size_type::absolute:
				_widget_data.flags |= widget_flags::wf_size_x_absolute;
				break;
			case helper_size_type::relative:
				_widget_data.flags |= widget_flags::wf_size_x_relative;
				break;
			case helper_size_type::fill:
				_widget_data.flags |= widget_flags::wf_size_x_fill;
				break;
			case helper_size_type::max_children:
				_widget_data.flags |= widget_flags::wf_size_x_max_children;
				break;
			case helper_size_type::total_children:
				_widget_data.flags |= widget_flags::wf_size_x_total_children;
				break;
			case helper_size_type::copy_other:
				_widget_data.flags |= widget_flags::wf_size_x_copy_y;
				break;
			default:
				break;
			}
		}

		inline void set_height(float height, helper_size_type type)
		{
			_widget_data.size.y = height;
			_widget_data.flags &= ~(widget_flags::wf_size_y_absolute | widget_flags::wf_size_y_relative | widget_flags::wf_size_y_fill | widget_flags::wf_size_y_copy_x | widget_flags::wf_size_y_max_children | widget_flags::wf_size_y_total_children);

			switch (type)
			{
			case helper_size_type::absolute:
				_widget_data.flags |= widget_flags::wf_size_y_absolute;
				break;
			case helper_size_type::relative:
				_widget_data.flags |= widget_flags::wf_size_y_relative;
				break;
			case helper_size_type::fill:
				_widget_data.flags |= widget_flags::wf_size_y_fill;
				break;
			case helper_size_type::max_children:
				_widget_data.flags |= widget_flags::wf_size_y_max_children;
				break;
			case helper_size_type::total_children:
				_widget_data.flags |= widget_flags::wf_size_y_total_children;
				break;
			case helper_size_type::copy_other:
				_widget_data.flags |= widget_flags::wf_size_y_copy_x;
				break;
			default:
				break;
			}
		}

		inline void set_draw_order(bool draw_order) { _widget_gfx.draw_order = draw_order; }
		inline bool get_is_visible() const { return _widget_data.flags & widget_flags::wf_visible; }
		inline bool is_point_in_bounds(unsigned int x, unsigned int y)
		{
			return x >= _widget_data.final_pos.x && x <= _widget_data.final_pos.x + _widget_data.final_size.x && y >= _widget_data.final_pos.y && y <= _widget_data.final_pos.y + _widget_data.final_size.y;
		}
		inline widget_data& get_data_widget() { return _widget_data; }
		inline widget_gfx&	get_data_gfx() { return _widget_gfx; }

		template <typename T>
		inline T* get_data_user() const
		{
			static_assert(sizeof(T) < VEKT_USER_DATA_SIZE);
			return static_cast<T*>(&_user_data);
		}

	private:
		void size_pass();
		void size_pass_children();
		void size_pass_post();
		void size_copy_check();
		void pos_pass();
		void pos_pass_children();
		void pos_pass_post();
		void draw_pass(class builder& builder);
		void draw_pass_children(class builder& builder);

		template <typename EventType>
		input_event_result on_event_internal(const EventType& ev, input_event_phase phase, widget*& out_last_widget)
		{
			if constexpr (std::is_same_v<EventType, mouse_event>)
			{
				if (_widget_data.on_mouse)
				{
					const input_event_result res = _widget_data.on_mouse(this, ev, phase, out_last_widget);
					if (res == input_event_result::handled) return res;
				}
			}
			else if constexpr (std::is_same_v<EventType, mouse_wheel_event>)
			{
				if (_widget_data.on_key)
				{
					const input_event_result res = _widget_data.on_mouse_wheel(this, ev, phase, out_last_widget);
					if (res == input_event_result::handled) return res;
				}
			}
			else if constexpr (std::is_same_v<EventType, key_event>)
			{
				if (_widget_data.on_mouse_wheel)
				{
					const input_event_result res = _widget_data.on_key(this, ev, phase, out_last_widget);
					if (res == input_event_result::handled) return res;
				}
			}
			else
				static_assert(false);

			out_last_widget = this;

			if (phase == input_event_phase::tunneling)
			{
				for (widget* w : _widget_data.children)
				{
					return w->on_event_internal(ev, phase, out_last_widget);
				}
			}
			else
			{
				if (_widget_data.parent)
				{
					out_last_widget = this;

					widget* sibling = nullptr;
					for (widget* w : _widget_data.parent->_widget_data.children)
					{
						if (w == this)
						{
							if (sibling) return sibling->on_event_internal(ev, phase, out_last_widget);
						}
						sibling = w;
					}

					if (sibling == nullptr) return _widget_data.parent->on_event_internal(ev, phase, out_last_widget);
				}
			}

			return input_event_result::not_handled;
		}

	private:
		friend class pool<widget>;
		friend class builder;

		handle		  _pool_handle = {};
		widget_data	  _widget_data = {};
		widget_gfx	  _widget_gfx  = {};
		unsigned char _user_data[VEKT_USER_DATA_SIZE];
	};

	struct vertex
	{
		vec2 pos;
		vec4 color;
	};

	typedef unsigned short index;

	class builder
	{
	public:
		struct draw_buffer
		{
			pod_vector<vertex> vertices;
			pod_vector<index>  indices;
			unsigned int	   draw_order = 0;
			void*			   user_data  = nullptr;
		};

		struct input_layer
		{
			unsigned int priority = 0;
			widget*		 root	  = nullptr;
			bool		 operator==(const input_layer& other) const { return priority == other.priority && root == other.root; }
		};

		struct init_config
		{
			size_t widget_buffer_sz = 0;
		};

		builder()					  = default;
		builder(const builder& other) = delete;
		~builder() {}

		void			   init(const init_config& conf);
		void			   uninit();
		void			   build();
		input_event_result on_mouse_event(const mouse_event& ev);
		input_event_result on_mouse_wheel_event(const mouse_wheel_event& ev);
		input_event_result on_key_event(const key_event& ev);
		void			   add_input_layer(unsigned int priority, widget* root);
		void			   remove_input_layer(unsigned int priority);
		void			   add_rect(const gfx_rect& rect, const vec2& min, const vec2& max, unsigned int draw_order, void* user_data);
		void			   add_text(const gfx_text& text, unsigned int draw_order, void* user_data);
		draw_buffer*	   get_draw_buffer_temp(unsigned int draw_order, void* user_data);

		template <typename... Args>
		widget* allocate(Args&&... args)
		{
			widget* w = new (_widget_pool.allocate()) widget(args...);
			return w;
		}

		inline void deallocate(widget* w) { _widget_pool.deallocate(w); }
		inline void set_root(widget* root) { _root = root; }

	private:
		void generate_rounded_rect(pod_vector<vec2>& out_path, const vec2& min, const vec2& max, float rounding, int segments);
		void generate_sharp_rect(pod_vector<vec2>& out_path, const vec2& min, const vec2& max);
		void generate_offset_path(pod_vector<vec2>& out_path, const pod_vector<vec2>& base_path, float amount);
		void add_strip(draw_buffer* db, unsigned int outer_start, unsigned int inner_start, unsigned int size);
		void add_filled_rect(draw_buffer* db, unsigned int start, unsigned int size);
		void add_vertices(draw_buffer* db, const pod_vector<vec2>& path, const vec4& color_start, const vec4& color_end, direction direction, const vec2& min, const vec2& max);
		void add_vertices_aa(draw_buffer* db, const pod_vector<vec2>& path, unsigned int original_vertices_idx, float alpha);

	private:
		pool<widget>			_widget_pool;
		pod_vector<input_layer> _input_layers;
		pod_vector<draw_buffer> _draw_buffers;
		widget*					_root = nullptr;
	};

	class atlas;

	struct glyph
	{
		int	  width		   = 0;
		int	  height	   = 0;
		int	  advance_x	   = 0;
		int	  left_bearing = 0;
		float x_offset	   = 0.0f;
		float y_offset	   = 0.0f;
		int	  atlas_x	   = 0;
		int	  atlas_y	   = 0;
		float u0		   = 0.0f;
		float v0		   = 0.0f;
		float u1		   = 0.0f;
		float v1		   = 0.0f;
	};

	struct font
	{
		glyph		 glyph_info[128];
		atlas*		 _atlas					= nullptr;
		unsigned int _atlas_required_height = 0;
		unsigned int _atlas_pos				= 0;
		float		 _scale					= 0.0f;
		int			 ascent					= 0;
		int			 descent				= 0;
		int			 line_gap				= 0;
		unsigned int size					= 0;
	};

	class atlas
	{
	public:
		struct slice
		{
			slice(unsigned int pos_y, unsigned int h) : pos(pos_y), height(h) {};
			unsigned int pos	= 0;
			unsigned int height = 0;
		};

		atlas(unsigned int width, unsigned int height);
		~atlas();

		bool				  add_font(font* font);
		void				  remove_font(font* font);
		bool				  empty() { return _fonts.empty(); }
		inline unsigned int	  get_width() const { return _width; }
		inline unsigned int	  get_height() const { return _height; }
		inline unsigned char* get_data() const { return _data; }
		inline unsigned int	  get_data_size() const { return _data_size; }

	private:
		unsigned int	   _width			 = 0;
		unsigned int	   _height			 = 0;
		pod_vector<slice*> _available_slices = {};
		pod_vector<font*>  _fonts			 = {};
		unsigned char*	   _data			 = nullptr;
		unsigned int	   _data_size		 = 0;
	};

	class font_manager
	{
	public:
		typedef void (*atlas_cb)(atlas* atl);

		font_manager() {};
		~font_manager() { uninit(); };

		static inline font_manager& get()
		{
			static font_manager fm;
			return fm;
		}

		font* load_font(const char* file, unsigned int size);
		void  unload_font(font* fnt);

		inline void init() {};
		inline void uninit()
		{
			for (atlas* atl : _atlases)
			{
				if (_atlas_destroyed_cb) _atlas_destroyed_cb(atl);
				delete atl;
			}

			for (font* fnt : _fonts)
				delete fnt;

			_atlases.clear();
			_fonts.clear();
		}

		inline void set_atlas_created_callback(atlas_cb cb) { _atlas_created_cb = cb; }
		inline void set_atlas_updated_callback(atlas_cb cb) { _atlas_destroyed_cb = cb; }
		inline void set_atlas_destroyed_callback(atlas_cb cb) { _atlas_destroyed_cb = cb; }

	private:
		void find_atlas(font* fnt);

	private:
		pod_vector<atlas*> _atlases;
		pod_vector<font*>  _fonts;
		atlas_cb		   _atlas_created_cb   = nullptr;
		atlas_cb		   _atlas_updated_cb   = nullptr;
		atlas_cb		   _atlas_destroyed_cb = nullptr;
	};

}

#ifdef VEKT_IMPL

#ifndef VEKT_NO_STB_IMPL
#define STB_TRUETYPE_IMPLEMENTATION
#endif

#include "stb_truetype.h"

namespace vekt
{
	config_data config = {};

	void widget::add_child(widget* w)
	{
		_widget_data.children.push_back(w);
		w->_widget_data.parent = this;
	}

	void widget::remove_child(widget* w)
	{
		_widget_data.children.remove(w);
		w->_widget_data.parent = nullptr;
	}

	void widget::set_visible(bool is_visible, bool recursive)
	{
		if (is_visible)
			_widget_data.flags |= widget_flags::wf_visible;
		else
			_widget_data.flags &= ~widget_flags::wf_visible;

		if (recursive)
		{
			for (widget* w : _widget_data.children)
				w->set_visible(is_visible, recursive);
		}
	}

	void widget::pos_pass()
	{
		if (_widget_data.flags & widget_flags::wf_pos_x_absolute)
			_widget_data.final_pos.x = _widget_data.pos.x;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_pos_x_relative)
		{
			const float parent_width = _widget_data.parent->_widget_data.final_size.x - _widget_data.parent->_widget_data.margins.left - _widget_data.parent->_widget_data.margins.right;

			if (_widget_data.flags & widget_flags::wf_pos_anchor_x_end)
				_widget_data.final_pos.x = (_widget_data.parent->_widget_data.final_pos.x + _widget_data.parent->_widget_data.margins.left) + (parent_width * _widget_data.pos.x) - _widget_data.final_size.x;
			else if (_widget_data.flags & widget_flags::wf_pos_anchor_x_mid)
				_widget_data.final_pos.x = (_widget_data.parent->_widget_data.final_pos.x + _widget_data.parent->_widget_data.margins.left) + (parent_width * _widget_data.pos.x) - _widget_data.final_size.x * 0.5f;
			else
				_widget_data.final_pos.x = (_widget_data.parent->_widget_data.final_pos.x + _widget_data.parent->_widget_data.margins.left) + (parent_width * _widget_data.pos.x);
		}

		if (_widget_data.flags & widget_flags::wf_pos_y_absolute)
			_widget_data.final_pos.y = _widget_data.pos.y;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_pos_y_relative)
		{
			const float parent_height = _widget_data.parent->_widget_data.final_size.y - _widget_data.parent->_widget_data.margins.top - _widget_data.parent->_widget_data.margins.bottom;

			if (_widget_data.flags & widget_flags::wf_pos_anchor_x_end)
				_widget_data.final_pos.x = (_widget_data.parent->_widget_data.final_pos.y + _widget_data.parent->_widget_data.margins.top) + (parent_height * _widget_data.pos.y) - _widget_data.final_size.y;
			else if (_widget_data.flags & widget_flags::wf_pos_anchor_x_mid)
				_widget_data.final_pos.x = (_widget_data.parent->_widget_data.final_pos.y + _widget_data.parent->_widget_data.margins.top) + (parent_height * _widget_data.pos.y) - _widget_data.final_size.y * 0.5f;
			else
				_widget_data.final_pos.x = (_widget_data.parent->_widget_data.final_pos.y + _widget_data.parent->_widget_data.margins.top) + (parent_height * _widget_data.pos.y);
		}

		if (_widget_data.custom_pos_pass) _widget_data.custom_pos_pass(this);
	}

	void widget::pos_pass_children()
	{
		for (widget* w : _widget_data.children)
		{
			w->pos_pass();
			w->pos_pass_children();
		}
	}

	void widget::pos_pass_post()
	{
		if (_widget_data.child_positioning == child_positioning::row)
		{
			float child_x = _widget_data.final_pos.x + _widget_data.margins.left;

			for (widget* w : _widget_data.children)
			{
				w->_widget_data.final_pos.x = child_x;
				child_x += _widget_data.spacing;
			}
		}
		else if (_widget_data.child_positioning == child_positioning::column)
		{
			float child_y = _widget_data.final_pos.y + _widget_data.margins.top;

			for (widget* w : _widget_data.children)
			{
				w->_widget_data.final_pos.y = child_y;
				child_y += _widget_data.spacing;
			}
		}
	}

	void widget::size_pass()
	{
		if (_widget_data.flags & widget_flags::wf_size_x_absolute)
			_widget_data.final_size.x = _widget_data.size.x;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_size_x_relative)
			_widget_data.final_size.x = (_widget_data.parent->_widget_data.final_size.x - _widget_data.parent->_widget_data.margins.left - _widget_data.parent->_widget_data.margins.right) * _widget_data.size.x;

		if (_widget_data.flags & widget_flags::wf_size_y_absolute)
			_widget_data.final_size.y = _widget_data.size.y;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_size_y_relative)
			_widget_data.final_size.y = (_widget_data.parent->_widget_data.final_size.y - _widget_data.parent->_widget_data.margins.top - _widget_data.parent->_widget_data.margins.bottom) * _widget_data.size.y;

		size_copy_check();

		if (_widget_data.custom_size_pass) _widget_data.custom_size_pass(this);
	}

	void widget::size_pass_children()
	{
		for (widget* w : _widget_data.children)
		{
			w->size_pass();
			w->size_pass_children();
			w->size_pass_post();
		}
	}

	void widget::size_pass_post()
	{
		/*
			Max/Total behaviour.
		*/
		if (_widget_data.flags & widget_flags::wf_size_x_max_children)
		{
			_widget_data.final_size.x = _widget_data.margins.left + _widget_data.margins.right;

			float max = 0.0f;

			for (widget* w : _widget_data.children)
				max = math::max(w->_widget_data.final_size.x, max);

			_widget_data.final_size.x += max;
		}
		else if (_widget_data.flags & widget_flags::wf_size_x_total_children)
		{
			_widget_data.final_size.x = _widget_data.margins.left + _widget_data.margins.right;
			for (widget* w : _widget_data.children)
				_widget_data.final_size.x += w->_widget_data.final_size.x + _widget_data.spacing;
			if (!_widget_data.children.empty()) _widget_data.final_size.x -= _widget_data.spacing;
		}

		if (_widget_data.flags & widget_flags::wf_size_y_max_children)
		{
			_widget_data.final_size.y = _widget_data.margins.top + _widget_data.margins.bottom;

			float max = 0.0f;

			for (widget* w : _widget_data.children)
				max = math::max(w->_widget_data.final_size.y, max);

			_widget_data.final_size.y += max;
		}
		else if (_widget_data.flags & widget_flags::wf_size_y_total_children)
		{
			_widget_data.final_size.y = _widget_data.margins.top + _widget_data.margins.bottom;
			for (widget* w : _widget_data.children)
				_widget_data.final_size.y += w->_widget_data.final_size.y + _widget_data.spacing;
			if (!_widget_data.children.empty()) _widget_data.final_size.y -= _widget_data.spacing;
		}

		size_copy_check();

		/*
			Expand/Fill behaviour.
		*/
		pod_vector<widget*> fill_x, fill_y;
		float				non_fill_total_x = -_widget_data.spacing, non_fill_total_y = -_widget_data.spacing;

		for (widget* w : _widget_data.children)
		{
			if (w->_widget_data.flags & widget_flags::wf_size_x_fill) { fill_x.push_back(w); }
			else { non_fill_total_x += w->_widget_data.final_size.x + _widget_data.spacing; }

			if (w->_widget_data.flags & widget_flags::wf_size_y_fill) { fill_y.push_back(w); }
			else { non_fill_total_y += w->_widget_data.final_size.y + _widget_data.spacing; }
		}

		if (!fill_x.empty())
		{
			const float diff  = _widget_data.final_size.x - non_fill_total_x;
			const float share = diff / static_cast<float>(fill_x.size());
			for (widget* w : fill_x)
				w->_widget_data.final_size.x = share;
		}

		if (!fill_y.empty())
		{
			const float diff  = _widget_data.final_size.y - non_fill_total_y;
			const float share = diff / static_cast<float>(fill_y.size());
			for (widget* w : fill_y)
				w->_widget_data.final_size.y = share;
		}
	}

	void widget::size_copy_check()
	{
		if (_widget_data.flags & widget_flags::wf_size_x_copy_y)
			_widget_data.final_size.x = _widget_data.final_size.y * (_widget_data.flags & widget_flags::wf_size_x_relative ? _widget_data.size.x : 1.0f);
		else if (_widget_data.flags & widget_flags::wf_size_y_copy_x)
			_widget_data.final_size.y = _widget_data.final_size.x * (_widget_data.flags & widget_flags::wf_size_y_relative ? _widget_data.size.y : 1.0f);
	}

	void widget::draw_pass(builder& builder)
	{
		if (_widget_gfx.type == gfx_type::rect) { builder.add_rect(_widget_gfx.get_data<gfx_rect>(), _widget_data.final_pos, _widget_data.final_pos + _widget_data.final_size, _widget_gfx.draw_order, _widget_gfx.user_data); }
		else if (_widget_gfx.type == gfx_type::text) { builder.add_text(_widget_gfx.get_data<gfx_text>(), _widget_gfx.draw_order, _widget_gfx.user_data); }
	}

	void widget::draw_pass_children(builder& builder)
	{
		for (widget* w : _widget_data.children)
		{
			w->draw_pass(builder);
			w->draw_pass_children(builder);
		}
	}

	void builder::init(const init_config& conf)
	{
		const unsigned int widget_count = static_cast<unsigned int>(conf.widget_buffer_sz / sizeof(widget));
		_widget_pool.init(widget_count);
	}

	void builder::uninit() { _widget_pool.clear(); }

	void builder::build()
	{
		ASSERT(_root);

		for (draw_buffer& db : _draw_buffers)
		{
			db.vertices.resize(0);
			db.indices.resize(0);
		}
		_draw_buffers.resize(0);

		_root->size_pass();
		_root->size_pass_children();
		_root->size_pass_post();
		_root->pos_pass();
		_root->pos_pass_children();
		_root->pos_pass_post();
		_root->draw_pass(*this);
		_root->draw_pass_children(*this);
	}

	input_event_result builder::on_mouse_event(const mouse_event& ev)
	{
		ASSERT(_root);

		if (_input_layers.empty())
		{
			V_ERR("vekt::builder::on_mouse_event -> No input layers are added to the builder!");
			return input_event_result::not_handled;
		}

		widget* last_widget = nullptr;
		for (const input_layer& layer : _input_layers)
		{
			input_event_result res = layer.root->on_event_internal(ev, input_event_phase::tunneling, last_widget);
			if (res == input_event_result::not_handled && last_widget) res = last_widget->on_event_internal(ev, input_event_phase::bubbling, last_widget);
			if (res == input_event_result::handled) return res;
		}

		return input_event_result::not_handled;
	}

	input_event_result builder::on_mouse_wheel_event(const mouse_wheel_event& ev)
	{
		ASSERT(_root);

		if (_input_layers.empty())
		{
			V_ERR("vekt::builder::on_mouse_wheel_event -> No input layers are added to the builder!");
			return input_event_result::not_handled;
		}

		widget* last_widget = nullptr;
		for (const input_layer& layer : _input_layers)
		{
			input_event_result res = layer.root->on_event_internal(ev, input_event_phase::tunneling, last_widget);
			if (res == input_event_result::not_handled && last_widget) res = last_widget->on_event_internal(ev, input_event_phase::bubbling, last_widget);
			if (res == input_event_result::handled) return res;
		}

		return input_event_result::not_handled;
	}

	input_event_result builder::on_key_event(const key_event& ev)
	{
		ASSERT(_root);

		if (_input_layers.empty())
		{
			V_ERR("vekt::builder::on_key_event -> No input layers are added to the builder!");
			return input_event_result::not_handled;
		}

		widget* last_widget = nullptr;
		for (const input_layer& layer : _input_layers)
		{
			input_event_result res = layer.root->on_event_internal(ev, input_event_phase::tunneling, last_widget);
			if (res == input_event_result::not_handled && last_widget) res = last_widget->on_event_internal(ev, input_event_phase::bubbling, last_widget);
			if (res == input_event_result::handled) return res;
		}

		return input_event_result::not_handled;
	}

	void builder::add_input_layer(unsigned int priority, widget* root)
	{
		for (input_layer& layer : _input_layers)
		{
			if (layer.priority == priority)
			{
				V_WARN("vekt::builder::add_input_layer -> Input layer with this priority already exists, overriding it's root! priority: %d", priority);
				layer.root = root;
				return;
			}
		}

		_input_layers.push_back({.priority = priority, .root = root});

		std::sort(_input_layers.begin(), _input_layers.end(), [](const input_layer& layer0, const input_layer& layer1) -> bool { return layer0.priority < layer1.priority; });
	}

	void builder::remove_input_layer(unsigned int priority)
	{
		for (input_layer& layer : _input_layers)
		{
			if (layer.priority == priority)
			{
				_input_layers.remove(layer);
				std::sort(_input_layers.begin(), _input_layers.end(), [](const input_layer& layer0, const input_layer& layer1) -> bool { return layer0.priority < layer1.priority; });
				return;
			}
		}
		V_ERR("vekt::remove_input_layer -> No input layer with the given priority exists! priority: %d", priority);
	}

	void builder::add_rect(const gfx_rect& rect, const vec2& min, const vec2& max, unsigned int draw_order, void* user_data)
	{
		builder::draw_buffer* db = get_draw_buffer_temp(draw_order, user_data);

		pod_vector<vec2> outer_path;
		pod_vector<vec2> inner_path;

		if (rect.rounding > 0.0f)
			generate_rounded_rect(outer_path, min, max, rect.rounding, 36);
		else
			generate_sharp_rect(outer_path, min, max);

		const bool has_stroke = rect.thickness > 0;
		const bool has_aa	  = rect.aa_thickness > 0;

		if (has_stroke) generate_offset_path(inner_path, outer_path, -static_cast<float>(rect.thickness));

		pod_vector<vec2> aa_outermost_path;
		pod_vector<vec2> aa_innermost_path;

		if (has_aa)
		{
			generate_offset_path(aa_outermost_path, outer_path, static_cast<float>(rect.aa_thickness));
			if (has_stroke && !inner_path.empty()) { generate_offset_path(aa_innermost_path, inner_path, -static_cast<float>(rect.aa_thickness)); }
		}

		if (has_stroke)
		{
			// Original stroke
			const unsigned int out_start = db->vertices.size();
			add_vertices(db, outer_path, rect.color_start, rect.color_end, rect.color_direction, min, max);
			const unsigned int in_start = db->vertices.size();
			add_vertices(db, inner_path, rect.color_start, rect.color_end, rect.color_direction, min, max);
			add_strip(db, out_start, in_start, outer_path.size());

			if (has_aa)
			{
				// outer aa
				const unsigned int out_aa_start = db->vertices.size();
				add_vertices_aa(db, aa_outermost_path, out_start, 0.0f);
				add_strip(db, out_aa_start, out_start, aa_outermost_path.size());

				// inner aa
				const unsigned int in_aa_start = db->vertices.size();
				add_vertices_aa(db, aa_innermost_path, in_start, 0.0f);
				add_strip(db, in_start, in_aa_start, aa_innermost_path.size());
			}
		}
		else
		{
			const unsigned int out_start = db->vertices.size();
			add_vertices(db, outer_path, rect.color_start, rect.color_end, rect.color_direction, min, max);
			add_filled_rect(db, out_start, outer_path.size());

			if (has_aa)
			{
				const unsigned int out_aa_start = db->vertices.size();
				add_vertices_aa(db, aa_outermost_path, out_start, 0.0f);
				add_strip(db, out_aa_start, out_start, aa_outermost_path.size());
			}
		}
	}

	void builder::add_text(const gfx_text& text, unsigned int draw_order, void* user_data) {}

	builder::draw_buffer* builder::get_draw_buffer_temp(unsigned int draw_order, void* user_data)
	{
		for (builder::draw_buffer& db : _draw_buffers)
		{
			if (db.draw_order == draw_order && db.user_data == user_data) { return &db; }
		}

		_draw_buffers.push_back({});
		return &_draw_buffers[_draw_buffers.size() - 1];
	}

	void builder::generate_offset_path(pod_vector<vec2>& out_path, const pod_vector<vec2>& base_path, float amount)
	{
		if (base_path.empty()) return;
		out_path.clear();
		out_path.resize(base_path.size());

		vec2 center = {};
		for (const vec2& p : base_path)
			center += p;
		center /= static_cast<float>(base_path.size());

		for (unsigned int i = 0; i = base_path.size(); i++)
		{
			const vec2 p	  = base_path[i];
			const vec2 normal = (p - center).normalized();
			out_path[i]		  = (p + normal) * amount;
		}
	}

	void builder::add_strip(draw_buffer* db, unsigned int outer_start, unsigned int inner_start, unsigned int size)
	{
		for (unsigned int i = 0; i < size - 1; i++)
		{
			const unsigned int p1_curr = outer_start + i;
			const unsigned int p1_next = outer_start + (i + 1) % size;
			const unsigned int p2_curr = inner_start + i;
			const unsigned int p2_next = inner_start + (i + 1) % size;
			db->indices.push_back(p1_curr);
			db->indices.push_back(p1_next);
			db->indices.push_back(p2_curr);

			db->indices.push_back(p1_next);
			db->indices.push_back(p2_next);
			db->indices.push_back(p2_curr);
		}
	}

	void builder::add_filled_rect(draw_buffer* db, unsigned int start, unsigned int size)
	{
		if (size != 4) return;

		db->indices.push_back(start);
		db->indices.push_back(start + 1);
		db->indices.push_back(start + 3);

		db->indices.push_back(start + 1);
		db->indices.push_back(start + 2);
		db->indices.push_back(start + 3);
	}

	void builder::add_vertices_aa(draw_buffer* db, const pod_vector<vec2>& path, unsigned int original_vertices_idx, float alpha)
	{
		const unsigned int start_vtx_idx = db->vertices.size();
		db->vertices.resize(db->vertices.size() + path.size());

		for (unsigned int i = 0; i < path.size(); i++)
		{
			vertex& vtx = db->vertices[start_vtx_idx + i];
			vtx.pos		= path[i];
			vtx.color	= db->vertices[original_vertices_idx + i].color;
			vtx.color.w = alpha;
		}
	}

	void builder::add_vertices(draw_buffer* db, const pod_vector<vec2>& path, const vec4& color_start, const vec4& color_end, direction direction, const vec2& min, const vec2& max)
	{
		const unsigned int start_vtx_idx = db->vertices.size();
		db->vertices.resize(db->vertices.size() + path.size());

		for (unsigned int i = 0; i < path.size(); i++)
		{
			vertex& vtx		  = db->vertices[start_vtx_idx + i];
			vtx.pos			  = path[i];
			const float ratio = direction == direction::horizontal ? math::lerp(min.x, max.x, vtx.pos.x) : math::lerp(min.y, max.y, vtx.pos.y);
			vtx.color.x		  = math::lerp(color_start.x, color_end.x, ratio);
			vtx.color.y		  = math::lerp(color_start.y, color_end.y, ratio);
			vtx.color.z		  = math::lerp(color_start.z, color_end.z, ratio);
			vtx.color.w		  = math::lerp(color_start.w, color_end.w, ratio);
		}
	}

	void builder::generate_rounded_rect(pod_vector<vec2>& out_path, const vec2& min, const vec2& max, float r, int segments)
	{
		r = math::min(r, math::min((max.x - min.x) * 0.5f, (max.y - min.y) * 0.5f)); // Clamp radius

		// Top-right corner (center: max.x-r, min.y+r)
		for (int i = 0; i <= segments; ++i)
		{
			float angle = (float)M_PI / 2.0f * ((float)i / segments); // 0 to PI/2
			out_path.push_back({max.x - r + cos(angle) * r, min.y + r - sin(angle) * r});
		}
		// Bottom-right corner (center: max.x-r, max.y-r)
		for (int i = 0; i <= segments; ++i)
		{
			float angle = (float)M_PI / 2.0f + (float)M_PI / 2.0f * ((float)i / segments); // PI/2 to PI
			out_path.push_back({max.x - r + cos(angle) * r, max.y - r - sin(angle) * r});
		}
		// Bottom-left corner (center: min.x+r, max.y-r)
		for (int i = 0; i <= segments; ++i)
		{
			float angle = (float)M_PI + (float)M_PI / 2.0f * ((float)i / segments); // PI to 3PI/2
			out_path.push_back({min.x + r + cos(angle) * r, max.y - r - sin(angle) * r});
		}
		// Top-left corner (center: min.x+r, min.y+r)
		for (int i = 0; i <= segments; ++i)
		{
			float angle = (float)M_PI * 3.0f / 2.0f + (float)M_PI / 2.0f * ((float)i / segments); // 3PI/2 to 2PI
			out_path.push_back({min.x + r + cos(angle) * r, min.y + r - sin(angle) * r});
		}
	}
	void builder::generate_sharp_rect(pod_vector<vec2>& out_path, const vec2& min, const vec2& max)
	{
		out_path.clear();
		out_path.push_back({min.x, min.y}); // Top-left
		out_path.push_back({max.x, min.y}); // Top-right
		out_path.push_back({max.x, max.y}); // Bottom-right
		out_path.push_back({min.x, max.y}); // Bottom-left
	}

	atlas::atlas(unsigned int width, unsigned int height)
	{
		_width	= width;
		_height = height;
		_available_slices.push_back(new atlas::slice(0, _height));
		_data	   = reinterpret_cast<unsigned char*>(MALLOC(static_cast<size_t>(width * height)));
		_data_size = width * height;
	}

	atlas::~atlas()
	{
		for (slice* slc : _available_slices)
			delete slc;
		_available_slices.clear();

		FREE(_data);
	}

	bool atlas::add_font(font* font)
	{
		if (font->_atlas_required_height > _height) return false;

		unsigned int best_slice_diff = _height;
		slice*		 best_slice		 = nullptr;

		for (slice* slc : _available_slices)
		{
			if (slc->height < font->_atlas_required_height) continue;

			const unsigned int diff = slc->height - font->_atlas_required_height;
			if (diff < best_slice_diff)
			{
				best_slice_diff = diff;
				best_slice		= slc;
			}
		}

		if (best_slice == nullptr) return false;

		font->_atlas	 = this;
		font->_atlas_pos = best_slice->pos;
		_fonts.push_back(font);

		best_slice->pos += font->_atlas_required_height;
		best_slice->height -= font->_atlas_required_height;

		if (best_slice->height == 0)
		{
			_available_slices.remove(best_slice);
			delete best_slice;
		}

		return true;
	}

	void atlas::remove_font(font* fnt)
	{
		slice* slc = new slice(fnt->_atlas_pos, fnt->_atlas_pos);
		_available_slices.push_back(slc);
		_fonts.remove(fnt);
	}

	void font_manager::find_atlas(font* fnt)
	{
		for (atlas* atl : _atlases)
		{
			if (atl->add_font(fnt)) { return; }
		}

		atlas* atl = new atlas(config.atlas_width, config.atlas_height);
		if (_atlas_created_cb) _atlas_created_cb(atl);
		const bool ok = atl->add_font(fnt);
		ASSERT(ok);
	}

	font* font_manager::load_font(const char* filename, unsigned int size)
	{
		std::ifstream file(filename, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			V_ERR("vekt::font_manager::load_font -> Failed opening font file! %s", filename);
			return nullptr;
		}

		std::streamsize file_size = file.tellg();
		file.seekg(0, std::ios::beg);

		pod_vector<unsigned char> ttf_buffer;
		ttf_buffer.resize(static_cast<unsigned int>(file_size));
		if (!file.read(reinterpret_cast<char*>(ttf_buffer.data()), file_size))
		{
			V_ERR("vekt::font_manager::load_font -> Failed reading font buffer! %s", filename);
			return nullptr;
		}

		stbtt_fontinfo stb_font;
		stbtt_InitFont(&stb_font, ttf_buffer.data(), stbtt_GetFontOffsetForIndex(ttf_buffer.data(), 0));

		font* fnt	= new font();
		fnt->_scale = stbtt_ScaleForPixelHeight(&stb_font, static_cast<float>(size));

		stbtt_GetFontVMetrics(&stb_font, &fnt->ascent, &fnt->descent, &fnt->line_gap);
		fnt->size = size;

		int total_width = 0;
		int max_height	= 0;

		for (int i = 0; i < 128; i++)
		{
			int ix0 = 0, iy0 = 0, ix1 = 0, iy1 = 0;
			stbtt_GetCodepointBitmapBox(&stb_font, i, fnt->_scale, fnt->_scale, &ix0, &iy0, &ix1, &iy1);

			glyph& glyph_info = fnt->glyph_info[i];

			glyph_info.width	= ix1 - ix0;
			glyph_info.height	= iy1 - iy0;
			glyph_info.x_offset = static_cast<float>(ix0);
			glyph_info.y_offset = static_cast<float>(iy0);

			if (glyph_info.width > 1) total_width += glyph_info.width + 1;
			max_height = static_cast<int>(math::max(max_height, glyph_info.height));

			stbtt_GetCodepointHMetrics(&stb_font, i, &glyph_info.advance_x, &glyph_info.left_bearing);
		}

		const int required_rows		= static_cast<int>(math::ceilf(static_cast<float>(total_width) / static_cast<float>(config.atlas_width)));
		const int required_height	= max_height;
		fnt->_atlas_required_height = required_rows * required_height;
		find_atlas(fnt);

		if (fnt->_atlas == nullptr)
		{
			delete fnt;
			V_ERR("vekt::font_manager::load_font -> Failed finding an atlas for the font! %s", filename);
			return nullptr;
		}

		_fonts.push_back(fnt);

		int current_atlas_pen_x = 0;
		int current_atlas_pen_y = 0;

		for (int i = 0; i < 128; i++)
		{
			glyph& glyph_info = fnt->glyph_info[i];

			if (glyph_info.width <= 0 || glyph_info.height <= 0)
			{
				glyph_info.atlas_x = 0;
				glyph_info.atlas_y = 0;
				continue;
			}

			if (current_atlas_pen_x + glyph_info.width > static_cast<int>(config.atlas_width))
			{
				current_atlas_pen_x = 0;
				current_atlas_pen_y += max_height;
			}

			if (current_atlas_pen_y + glyph_info.height > static_cast<int>(fnt->_atlas_required_height)) { ASSERT(false); }

			glyph_info.atlas_x = current_atlas_pen_x;
			glyph_info.atlas_y = fnt->_atlas_pos + current_atlas_pen_y;
			glyph_info.u0	   = static_cast<float>(glyph_info.atlas_x) / static_cast<float>(fnt->_atlas->get_width());
			glyph_info.v0	   = static_cast<float>(glyph_info.atlas_y) / static_cast<float>(fnt->_atlas->get_height());
			glyph_info.u1	   = static_cast<float>(glyph_info.width) / static_cast<float>(fnt->_atlas->get_width());
			glyph_info.v1	   = static_cast<float>(glyph_info.height) / static_cast<float>(fnt->_atlas->get_height());

			unsigned char* dest_pixel_ptr = fnt->_atlas->get_data() + (glyph_info.atlas_y * fnt->_atlas->get_width()) + glyph_info.atlas_x;

			stbtt_MakeCodepointBitmap(&stb_font,
									  dest_pixel_ptr,
									  glyph_info.width,			// Output bitmap width
									  glyph_info.height,		// Output bitmap height
									  fnt->_atlas->get_width(), // Atlas stride/pitch
									  fnt->_scale,				// Horizontal scale
									  fnt->_scale,				// Vertical scale
									  i);						// Codepoint

			current_atlas_pen_x += glyph_info.width + 1;
		}

		if (_atlas_updated_cb) _atlas_updated_cb(fnt->_atlas);
		return fnt;
	}

	void font_manager::unload_font(font* fnt)
	{
		fnt->_atlas->remove_font(fnt);

		if (fnt->_atlas->empty())
		{
			_atlases.remove(fnt->_atlas);

			if (_atlas_destroyed_cb) _atlas_destroyed_cb(fnt->_atlas);

			delete fnt->_atlas;
		}

		_fonts.remove(fnt);
		delete fnt;
	}
}

#endif

#endif
