// Copyright (c) 2025 Inan Evin

#ifndef VEKT_HPP
#define VEKT_HPP

#include <memory>
#include <assert.h>
#include <algorithm>

#define VEKT_INLINE inline
#define VEKT_API	extern

namespace vekt
{

#ifndef VEKT_USER_DATA_SIZE
#define VEKT_USER_DATA_SIZE 1024
#endif

#if defined _MSC_VER && !__INTEL_COMPILER
#define ALIGNED_MALLOC(SZ, ALIGN) _aligned_malloc(SZ, ALIGN)
#else
#include <stdlib.h>
#define ALIGNED_MALLOC(SZ, ALIGN) std::aligned_alloc(ALIGN, SZ)
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
			if (_elements) FREE(_elements);
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
				FREE(_elements);
			}
			_elements = new_elems;

			if (_count > sz) _count = sz;

			_capacity = sz;
		}

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
				FREE(_elements);
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
			if (_elements) FREE(_elements);
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
		log_callback on_log = nullptr;
	};

	extern config_data config;

#define V_LOG(...)                                                                                                                                                                                                                                                 \
	if (config.on_log) config.on_log(log_verbosity::info, __VA_ARGS__)
#define V_ERR(...)                                                                                                                                                                                                                                                 \
	if (config.on_log) config.on_log(log_verbosity::error, __VA_ARGS__)
#define V_WARN(...)                                                                                                                                                                                                                                                \
	if (config.on_log) config.on_log(log_verbosity::warning, __VA_ARGS__)

	class math
	{
	public:
		static inline float max(float a, float b) { return a > b ? a : b; }
		static inline float min(float a, float b) { return a < b ? a : b; }
		static inline float equals(float a, float b, float eps = 0.0001f) { return a > b - eps && a < b + eps; }
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

	struct margins
	{
		float top	 = 0.0f;
		float bottom = 0.0f;
		float left	 = 0.0f;
		float right	 = 0.0f;
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
		unsigned int			 draw_order		   = 0;
		unsigned int			 flags			   = 0;
		float					 spacing		   = 0.0f;
		float					 x				   = 0.0f;
		float					 y				   = 0.0f;
		float					 width			   = 0.0f;
		float					 height			   = 0.0f;
		float					 _final_x		   = 0.0f;
		float					 _final_y		   = 0.0f;
		float					 _final_width	   = 0.0f;
		float					 _final_height	   = 0.0f;
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
			_widget_data.x = x;
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
			_widget_data.y = y;
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
			_widget_data.width = width;
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
			_widget_data.height = height;
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

		inline void set_draw_order(bool draw_order) { _widget_data.draw_order = draw_order; }
		inline bool get_is_visible() const { return _widget_data.flags & widget_flags::wf_visible; }
		inline bool is_point_in_bounds(unsigned int x, unsigned int y) { return x >= _widget_data._final_x && x <= _widget_data._final_x + _widget_data._final_width && y >= _widget_data._final_y && y <= _widget_data._final_y + _widget_data._final_height; }
		inline widget_data& get_data_widget() { return _widget_data; }

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
		unsigned char _user_data[VEKT_USER_DATA_SIZE];
	};

	class builder
	{
	public:
		struct input_layer
		{
			unsigned int priority = 0;
			widget*		 root	  = nullptr;

			bool operator==(const input_layer& other) const { return priority == other.priority && root == other.root; }
		};

		builder() {};
		~builder();

		void			   init(unsigned int widget_count);
		void			   uninit();
		void			   build();
		input_event_result on_mouse_event(const mouse_event& ev);
		input_event_result on_mouse_wheel_event(const mouse_wheel_event& ev);
		input_event_result on_key_event(const key_event& ev);
		void			   add_input_layer(unsigned int priority, widget* root);
		void			   remove_input_layer(unsigned int priority);

		inline void set_root(widget* root) { _root = root; }

		template <typename... Args>
		widget* allocate(Args&&... args)
		{
			widget* w = new (_widget_pool.allocate()) widget(args...);
			return w;
		}
		inline void deallocate(widget* w) { _widget_pool.deallocate(w); }

	private:
		pool<widget>			_widget_pool;
		pod_vector<input_layer> _input_layers;
		widget*					_root = nullptr;
	};
}

#ifdef VEKT_IMPL

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
			_widget_data._final_x = _widget_data.x;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_pos_x_relative)
		{
			const float parent_width = _widget_data.parent->_widget_data._final_width - _widget_data.parent->_widget_data.margins.left - _widget_data.parent->_widget_data.margins.right;

			if (_widget_data.flags & widget_flags::wf_pos_anchor_x_end)
				_widget_data._final_x = (_widget_data.parent->_widget_data._final_x + _widget_data.parent->_widget_data.margins.left) + (parent_width * _widget_data.x) - _widget_data._final_width;
			else if (_widget_data.flags & widget_flags::wf_pos_anchor_x_mid)
				_widget_data._final_x = (_widget_data.parent->_widget_data._final_x + _widget_data.parent->_widget_data.margins.left) + (parent_width * _widget_data.x) - _widget_data._final_width * 0.5f;
			else
				_widget_data._final_x = (_widget_data.parent->_widget_data._final_x + _widget_data.parent->_widget_data.margins.left) + (parent_width * _widget_data.x);
		}

		if (_widget_data.flags & widget_flags::wf_pos_y_absolute)
			_widget_data._final_y = _widget_data.y;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_pos_y_relative)
		{
			const float parent_height = _widget_data.parent->_widget_data._final_height - _widget_data.parent->_widget_data.margins.top - _widget_data.parent->_widget_data.margins.bottom;

			if (_widget_data.flags & widget_flags::wf_pos_anchor_x_end)
				_widget_data._final_x = (_widget_data.parent->_widget_data._final_y + _widget_data.parent->_widget_data.margins.top) + (parent_height * _widget_data.y) - _widget_data._final_height;
			else if (_widget_data.flags & widget_flags::wf_pos_anchor_x_mid)
				_widget_data._final_x = (_widget_data.parent->_widget_data._final_y + _widget_data.parent->_widget_data.margins.top) + (parent_height * _widget_data.y) - _widget_data._final_height * 0.5f;
			else
				_widget_data._final_x = (_widget_data.parent->_widget_data._final_y + _widget_data.parent->_widget_data.margins.top) + (parent_height * _widget_data.y);
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
			float child_x = _widget_data._final_x + _widget_data.margins.left;

			for (widget* w : _widget_data.children)
			{
				w->_widget_data._final_x = child_x;
				child_x += _widget_data.spacing;
			}
		}
		else if (_widget_data.child_positioning == child_positioning::column)
		{
			float child_y = _widget_data._final_y + _widget_data.margins.top;

			for (widget* w : _widget_data.children)
			{
				w->_widget_data._final_y = child_y;
				child_y += _widget_data.spacing;
			}
		}
	}

	void widget::size_pass()
	{
		if (_widget_data.flags & widget_flags::wf_size_x_absolute)
			_widget_data._final_width = _widget_data.width;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_size_x_relative)
			_widget_data._final_width = (_widget_data.parent->_widget_data._final_width - _widget_data.parent->_widget_data.margins.left - _widget_data.parent->_widget_data.margins.right) * _widget_data.width;

		if (_widget_data.flags & widget_flags::wf_size_y_absolute)
			_widget_data._final_height = _widget_data.height;
		else if (_widget_data.parent && _widget_data.flags & widget_flags::wf_size_y_relative)
			_widget_data._final_height = (_widget_data.parent->_widget_data._final_height - _widget_data.parent->_widget_data.margins.top - _widget_data.parent->_widget_data.margins.bottom) * _widget_data.height;

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
			_widget_data._final_width = _widget_data.margins.left + _widget_data.margins.right;

			float max = 0.0f;

			for (widget* w : _widget_data.children)
				max = math::max(w->_widget_data._final_width, max);

			_widget_data._final_width += max;
		}
		else if (_widget_data.flags & widget_flags::wf_size_x_total_children)
		{
			_widget_data._final_width = _widget_data.margins.left + _widget_data.margins.right;
			for (widget* w : _widget_data.children)
				_widget_data._final_width += w->_widget_data._final_width + _widget_data.spacing;
			if (!_widget_data.children.empty()) _widget_data._final_width -= _widget_data.spacing;
		}

		if (_widget_data.flags & widget_flags::wf_size_y_max_children)
		{
			_widget_data._final_height = _widget_data.margins.top + _widget_data.margins.bottom;

			float max = 0.0f;

			for (widget* w : _widget_data.children)
				max = math::max(w->_widget_data._final_height, max);

			_widget_data._final_height += max;
		}
		else if (_widget_data.flags & widget_flags::wf_size_y_total_children)
		{
			_widget_data._final_height = _widget_data.margins.top + _widget_data.margins.bottom;
			for (widget* w : _widget_data.children)
				_widget_data._final_height += w->_widget_data._final_height + _widget_data.spacing;
			if (!_widget_data.children.empty()) _widget_data._final_height -= _widget_data.spacing;
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
			else { non_fill_total_x += w->_widget_data._final_width + _widget_data.spacing; }

			if (w->_widget_data.flags & widget_flags::wf_size_y_fill) { fill_y.push_back(w); }
			else { non_fill_total_y += w->_widget_data._final_height + _widget_data.spacing; }
		}

		if (!fill_x.empty())
		{
			const float diff  = _widget_data._final_width - non_fill_total_x;
			const float share = diff / static_cast<float>(fill_x.size());
			for (widget* w : fill_x)
				w->_widget_data._final_width = share;
		}

		if (!fill_y.empty())
		{
			const float diff  = _widget_data._final_height - non_fill_total_y;
			const float share = diff / static_cast<float>(fill_y.size());
			for (widget* w : fill_y)
				w->_widget_data._final_height = share;
		}
	}

	void widget::size_copy_check()
	{
		if (_widget_data.flags & widget_flags::wf_size_x_copy_y)
			_widget_data._final_width = _widget_data._final_height * (_widget_data.flags & widget_flags::wf_size_x_relative ? _widget_data.width : 1.0f);
		else if (_widget_data.flags & widget_flags::wf_size_y_copy_x)
			_widget_data._final_height = _widget_data._final_width * (_widget_data.flags & widget_flags::wf_size_y_relative ? _widget_data.height : 1.0f);
	}

	builder::~builder() {}
	void builder::init(unsigned int widget_count) { _widget_pool.init(widget_count); }
	void builder::uninit() { _widget_pool.clear(); }

	void builder::build()
	{
		ASSERT(_root);
		_root->size_pass();
		_root->size_pass_children();
		_root->size_pass_post();
		_root->pos_pass();
		_root->pos_pass_children();
		_root->pos_pass_post();
	}

	input_event_result builder::on_mouse_event(const mouse_event& ev)
	{
		ASSERT(_root);

		if (_input_layers.empty())
		{
			V_ERR("vekt::on_mouse_event error! No input layers are added to the builder!");
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
			V_ERR("vekt::on_mouse_wheel_event error! No input layers are added to the builder!");
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
			V_ERR("vekt::on_key_event error! No input layers are added to the builder!");
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
				V_WARN("Input layer with this priority already exists, overriding it's root! priority: %d", priority);
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
		V_ERR("vekt::remove_input_layer error! No input layer with the given priority exists! priority: %d", priority);
	}
}

#endif

#endif
