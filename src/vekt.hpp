// Copyright (c) 2025 Inan Evin

#ifndef VEKT_HPP
#define VEKT_HPP

#include <memory>
#include <assert.h>

#define VEKT_INLINE inline
#define VEKT_API	extern

namespace vekt
{

#define MALLOC(SZ)	malloc(SZ)
#define MEMCPY(...) memcpy(__VA_ARGS__)
#define FREE(X)		free(X)
#define ASSERT(...) assert(__VA_ARGS__)

	typedef unsigned int id;

	template <typename T>
	class pod_vector
	{
	public:
		~pod_vector() { clear(); }

		inline void push_back(const T& elem)
		{
			check_grow();
			_elements[_count] = elem;
			_count++;
		}

		inline void remove(unsigned int index)
		{
			ASSERT(index < _count, "");
			ASSERT(_elements);

			const size_t dest_index		  = static_cast<size_t>(index);
			const size_t move_start_index = static_cast<size_t>(index) + 1;
			const size_t move_count		  = static_cast<size_t>(_count - index - 1);
			if (move_count != 0) MEMCPY(_elements + sizeof(T) * dest_index, _elements + sizeof(T) * move_start_index, sizeof(T) * move_count);
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
			T* new_elems = reinterpret_cast<T*>(MALLOC(sizeof(T) * static_cast<size_t>(sz)));

			if (_elements)
			{
				MEMCPY(new_elems, _elements, sizeof(T) * static_cast<size_t>(_count > sz ? sz : _count));
				FREE(_elements);
			}
			_elements = new_elems;

			if (_count > sz) _count = sz;

			_capacity = sz;
		}

		T&		 operator[](unsigned int index) { return _elements[index]; }
		const T& operator[](unsigned int index) const { return _elements[index]; }

		inline bool empty() const { return _count == 0; }

		inline unsigned int size() const { return _count; }

	private:
		inline void check_grow()
		{
			if (_count < _capacity) return;
			const size_t old_capacity = _capacity;
			_capacity				  = _capacity == 0 ? 1 : _capacity * 2;
			T* new_elems			  = reinterpret_cast<T*>(MALLOC(sizeof(T) * static_cast<size_t>(_capacity)));
			MEMCPY(new_elems, _elements, sizeof(T*) * _count);
			FREE(_elements);
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
			_elements = reinterpret_cast<T*>(MALLOC(sizeof(T) * size));
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

			ASSERT(_head < _size, "");
			_elements[_head]._pool_handle.alive = true;
			return &_elements[_head];
		}

		inline void deallocate(T* ptr)
		{
			_freelist.push_back(ptr->_pool_handle.value);
			ptr->_pool_handle.alive = false;
		}

	private:
		T*						 _elements = nullptr;
		unsigned int			 _size	   = 0;
		unsigned int			 _head	   = 0;
		pod_vector<unsigned int> _freelist = {};
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
		wf_owns_size			 = 1 << 10,
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

	struct widget_data
	{
		widget_flags		flags	 = {};
		widget_type			type	 = widget_type::plain;
		margins				margins	 = {};
		pod_vector<widget*> children = {};
		float				x		 = 0.0f;
		float				y		 = 0.0f;
		float				width	 = 0.0f;
		float				height	 = 0.0f;
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

		inline row_data* get_data_row() { return &_type_data.row; }

		inline column_data* get_data_column() { return &_type_data.col; }

		inline text_data* get_data_text() { return &_type_data.text; }

		inline button_data* get_data_button() { return &_type_data.button; }

		inline slider_data* get_data_slider() { return &_type_data.slider; }

	private:
		friend class pool<widget>;

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
		builder() {};
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

		template <typename... Args>
		widget* allocate(Args&&... args)
		{
			widget* w = new (_widget_pool.allocate()) widget(args);
			return w;
		}

		void deallocate(widget* w) { _widget_pool.deallocate(w); }

	private:
		pool<widget> _widget_pool;
	};

}

#ifdef VEKT_IMPL

namespace vekt
{

	builder::~builder() {}
	void builder::init(unsigned int widget_count) { _widget_pool.init(widget_count); }
	void builder::uninit() { _widget_pool.clear(); }

	void widget::add_child(widget* w) { _widget_data.children.push_back(w); }

	void widget::remove_child(widget* w) { _widget_data.children.remove(w); }
}

#endif

#endif