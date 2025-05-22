// Copyright (c) 2025 Inan Evin

#ifndef VEKT_HPP
#define VEKT_HPP

#include <memory>
#include <assert.h>

#define VEKT_INLINE inline
#define VEKT_API	extern

namespace vekt
{

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
			_capacity				  = _capacity == 0 ? 1 : _capacity * 2;
			T* new_elems			  = reinterpret_cast<T*>(ALIGNED_MALLOC(sizeof(T) * static_cast<size_t>(_capacity), alignof(T)));

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
			_head++;
			return &_elements[_head];
		}

		inline void deallocate(T* ptr)
		{
			ASSERT(ptr);
			ASSERT(ptr->_pool_handle.alive);
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

	class math
	{
	public:
		static inline float max(float a, float b) { return a > b ? a : b; }
		static inline float min(float a, float b) { return a < b ? a : b; }
		static inline float equals(float a, float b, float eps = 0.0001f) { return a > b - eps && a < b + eps; }
	};

	enum widget_flags
	{
		wf_pos_x_relative		 = 1 << 0,
		wf_pos_y_relative		 = 1 << 1,
		wf_size_x_relative		 = 1 << 2,
		wf_size_y_relative		 = 1 << 3,
		wf_size_x_copy_y		 = 1 << 4,
		wf_size_y_copy_x		 = 1 << 5,
		wf_size_x_total_children = 1 << 6,
		wf_size_x_max_children	 = 1 << 7,
		wf_size_y_total_children = 1 << 8,
		wf_size_y_max_children	 = 1 << 9,
		wf_size_x_fill			 = 1 << 10,
		wf_size_y_fill			 = 1 << 11,
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

	struct margins
	{
		float top	 = 0.0f;
		float bottom = 0.0f;
		float left	 = 0.0f;
		float right	 = 0.0f;
	};

	struct row_data
	{
	};

	struct column_data
	{
	};

	struct text_data
	{
	};

	struct button_data
	{
	};

	struct slider_data
	{
	};

	class widget;

	typedef void (*custom_pos_pass)(widget*);
	typedef void (*custom_size_pass)(widget*);

	struct widget_data
	{
		widget*				parent			 = nullptr;
		custom_pos_pass		custom_pos_pass	 = nullptr;
		custom_size_pass	custom_size_pass = nullptr;
		pod_vector<widget*> children		 = {};
		widget_type			type			 = widget_type::plain;
		margins				margins			 = {};
		unsigned int		flags			 = 0;
		float				spacing			 = 0.0f;
		float				x				 = 0.0f;
		float				y				 = 0.0f;
		float				width			 = 0.0f;
		float				height			 = 0.0f;
		float				_final_x		 = 0.0f;
		float				_final_y		 = 0.0f;
		float				_final_width	 = 0.0f;
		float				_final_height	 = 0.0f;
        unsigned short test;

	};

    enum class helper_pos_type
    {
        absolute,
        relative,
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

	class widget
	{
	public:
        
		widget() = default;
		widget(const widget_data& data) : _widget_data(data) {};

		/// <summary>
		///
		/// </summary>
		/// <param name="w"></param>
		void add_child(widget* w);

		/// <summary>
		///
		/// </summary>
		/// <param name="w"></param>
		void remove_child(widget* w);
        
        inline void set_pos_x(float x, helper_pos_type type)
        {
            _widget_data.x = x;
            _widget_data.flags &= ~widget_flags::wf_pos_x_relative;

            switch (type) {
                case helper_pos_type::relative:
                    _widget_data.flags |= widget_flags::wf_pos_x_relative;
                    break;
                default:
                    break;
            }
        }
        
        inline void set_pos_y(float y, helper_pos_type type)
        {
            _widget_data.y = y;
            _widget_data.flags &= ~widget_flags::wf_pos_y_relative;

            switch (type) {
                case helper_pos_type::relative:
                    _widget_data.flags |= widget_flags::wf_pos_y_relative;
                    break;
                default:
                    break;
            }
        }
        
        inline void set_width(float width, helper_size_type type)
        {
            _widget_data.width = width;
            _widget_data.flags &= ~(widget_flags::wf_size_x_relative | widget_flags::wf_size_x_fill | widget_flags::wf_size_x_copy_y | widget_flags::wf_size_x_max_children | widget_flags::wf_size_x_total_children);
            
            switch (type) {
              
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
            _widget_data.flags &= ~(widget_flags::wf_size_y_relative | widget_flags::wf_size_y_fill | widget_flags::wf_size_y_copy_x | widget_flags::wf_size_y_max_children | widget_flags::wf_size_y_total_children);
            
            switch (type) {
              
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

		inline widget_data& get_data_widget() { return _widget_data; }
		inline row_data*	get_data_row() { return &_type_data.row; }
		inline column_data* get_data_column() { return &_type_data.col; }
		inline text_data*	get_data_text() { return &_type_data.text; }
		inline button_data* get_data_button() { return &_type_data.button; }
		inline slider_data* get_data_slider() { return &_type_data.slider; }

	private:
		void size_pass();
		void size_pass_children();
		void size_pass_post();
		void size_copy_check();
		void pos_pass();
		void pos_pass_children();
        void pos_pass_post();

	private:
		friend class pool<widget>;
		friend class builder;

		union {
			row_data	row;
			column_data col;
			text_data	text;
			button_data button;
			slider_data slider;
		} _type_data;

		handle		_pool_handle = {};
		widget_data _widget_data = {};
	};

	class builder
	{
	public:
		builder() {
        };
		~builder();

		/// <summary>
		///
		/// </summary>
		/// <param name="sz"></param>
		void init(unsigned int widget_count);

		/// <summary>
		///
		/// </summary>
		void uninit();

		/// <summary>
		///
		/// </summary>
		void build();

		/// <summary>
		///
		/// </summary>
		/// <param name="root"></param>
		inline void set_root(widget* root) { _root = root; }

		template <typename... Args>
		widget* allocate(Args&&... args)
		{
			widget* w = new (_widget_pool.allocate()) widget(args...);
			return w;
		}

		void deallocate(widget* w) { _widget_pool.deallocate(w); }

	private:
		pool<widget> _widget_pool;
		widget*		 _root = nullptr;
	};
}

#ifdef VEKT_IMPL

namespace vekt
{
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

	void widget::pos_pass()
	{
		_widget_data._final_x = _widget_data.x;
		_widget_data._final_y = _widget_data.y;
		if (_widget_data.parent && _widget_data.flags & widget_flags::wf_pos_x_relative) _widget_data._final_x = _widget_data.parent->_widget_data._final_x + _widget_data.parent->_widget_data._final_width * _widget_data.width;
		if (_widget_data.parent && _widget_data.flags & widget_flags::wf_pos_y_relative) _widget_data._final_y = _widget_data.parent->_widget_data._final_y + _widget_data.parent->_widget_data._final_height * _widget_data.height;

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
        
        switch (_widget_data.type) {
            case widget_type::row:
            {
                for(widget* w : _widget_data.children)
                {
                    
                }
                break;
            }
            default:
                break;
        }
    }

	void widget::size_pass()
	{
		_widget_data._final_width  = _widget_data.width;
		_widget_data._final_height = _widget_data.height;

		if (_widget_data.parent && _widget_data.flags & widget_flags::wf_size_x_relative) _widget_data._final_width = _widget_data.parent->_widget_data._final_width * _widget_data.width;
		if (_widget_data.parent && _widget_data.flags & widget_flags::wf_size_y_relative) _widget_data._final_height = _widget_data.parent->_widget_data._final_height * _widget_data.height;

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
	}

}

#endif

#endif
