#pragma once
#include <stdint.h>
#include <windows.h>

#pragma warning(disable:26812) 

#define FILE_MAP_NAME "file-map-name-msgs"
#define FILE_MAP_SIZE 32*4096

//
// read only msg
//

namespace draw
{
	typedef enum _draw_flag
	{
		null,
		exe,
		dll
	}draw_flag, * p_draw_flag;

	typedef enum _draw_type
	{
		type_line,
		type_rect,
		type_circle,
		type_text
	}draw_type, * p_draw_type;

	typedef struct _draw_line
	{
		float x1;
		float y1;
		float x2;
		float y2;
		int rgb;
		float t;
	}draw_line, * p_draw_line;

	typedef struct _draw_rect
	{
		float x;
		float y;
		float w;
		float h;
		int rgb;
		float t;
	}draw_rect, * p_draw_rect;

	typedef struct _draw_circle
	{
		float x;
		float y;
		float r;
		int rgb;
		float t;
	}draw_circle, * p_draw_circle;

	typedef struct _draw_text
	{
		float x;
		float y;
		int rgb;
		char txt[256];
	}draw_text, * p_draw_text;

	typedef struct _draw_info
	{
		draw_type type;
		union info
		{
			draw_line line_info;
			draw_rect rect_info;
			draw_circle circle_info;
			draw_text text_info;
		}u;
	}draw_info, * p_draw_info;

	typedef struct _draw_array
	{
		uint32_t size;				// buffer size
		uint32_t conneted;		// reserved
		draw_flag flag;				// draw begin or end flag
		uint32_t counts;			// draw info count
		draw_info pinfo[1];		// draw info ptr
	}draw_array, * p_draw_array;
}

class msgs
{

public:
	static auto instance() -> msgs*
	{
		static msgs obj;
		return &obj;
	}

	auto initializer() -> bool
	{
		if (mapping != nullptr)
		{
			return true;
		}

		bool ret = false;

		HANDLE handle = nullptr;
		LPVOID buffer = nullptr;

		do
		{
			handle = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, FILE_MAP_SIZE, FILE_MAP_NAME);
			if (!handle)
			{
				break;
			}

			buffer = MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, FILE_MAP_SIZE);
			if (!buffer)
			{
				break;
			}

			mapping = buffer;
			ret = true;
		} while (false);

		if (ret == false)
		{
			if (buffer)
			{
				UnmapViewOfFile(buffer);
			}
			if (handle)
			{
				CloseHandle(handle);
			}
			mapping = nullptr;
		}

		return ret;
	}

	auto connet() -> bool
	{
		return true;
	}

	//
	// 实现获取绘制数据的接口
	//

	auto get_draw_data() -> draw::p_draw_array
	{
		return reinterpret_cast<draw::p_draw_array>(mapping);
	}

	auto get_draw_count() -> uint32_t
	{
		return get_draw_data()->counts;
	}

	auto get_draw_flag() -> uint32_t
	{
		return get_draw_data()->flag == draw::draw_flag::dll;
	}

	auto get_draw_type(uint32_t index) ->draw::draw_type
	{
		return get_draw_data()->pinfo[index].type;
	}

	template <typename type>
	auto get_draw_info(uint32_t index) ->type
	{
		return *reinterpret_cast<type*>(&(get_draw_data()->pinfo[index].u.line_info));
	}

protected:
	explicit msgs() {}

	template <typename type>
	auto read(uint64_t address) -> type
	{
		return *reinterpret_cast<type*>(address);
	}

private:
	void* mapping = nullptr;
};

msgs* msg = msgs::instance();

