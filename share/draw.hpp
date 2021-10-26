#pragma once
#pragma warning(disable:26812) 

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


	p_draw_array data = nullptr;
	uint32_t size = 32 * 4096;
	static uint32_t index = 0;

	auto initializer() -> bool
	{
		auto ret = false;

		do
		{
			void* buffer = malloc(size);
			if (!buffer)
			{
				break;
			}

			ZeroMemory(buffer, size);

			data = reinterpret_cast<p_draw_array>(buffer);
			data->size = size;
			ret = true;
		} while (false);

		return ret;
	}

	auto release() -> void
	{
		if (data)
		{
			free(data);
			data = nullptr;
		}
		return;
	}

	auto begin() -> bool
	{
		if (!data)
		{
			return false;
		}

		//
		// size 先不设置了，暂时用最大值拷贝
		// 用set设置一个变量，是因为用拷贝最少的数据完成停止和开始绘制的标志
		// 重置index和counts
		//

		index = 0;
		data->counts = index;
		uint32_t offset = offsetof(draw_array, flag);//设置flag停止绘制
		data->flag = draw_flag::exe;
		return msg->set(offset, data->flag);
	}

	auto end() -> bool
	{
		if (!data)
		{
			return false;
		}

		data->counts = index;
		msg->copy(data, data->size);// 拷贝全部数据

		data->counts = index = 0;//每次数据提交后把数量重置
		uint32_t offset = offsetof(draw_array, flag);//设置flag开始绘制
		data->flag = draw_flag::dll;
		return msg->set(offset, data->flag);
	}

	auto line(float x1, float y1, float x2, float y2, int rgb, float t = 1.0f) ->void
	{
		index++;
		draw_info src = { type_line };
		src.u.line_info.x1 = x1;
		src.u.line_info.y1 = y1;
		src.u.line_info.x2 = x2;
		src.u.line_info.y2 = y2;
		src.u.line_info.rgb = rgb;
		src.u.line_info.t = t;
		memcpy(&data->pinfo[index - 1], &src, sizeof(src));
	}
	auto rect(float x, float y, float w, float h, int rgb, float t = 1.0f) ->void
	{
		index++;
		draw_info src = { type_rect };
		src.u.rect_info.x = x;
		src.u.rect_info.y = y;
		src.u.rect_info.w = w;
		src.u.rect_info.h = h;
		src.u.rect_info.rgb = rgb;
		src.u.rect_info.t = t;
		memcpy(&data->pinfo[index - 1], &src, sizeof(src));
	}
	auto circle(float x, float y, float r, int rgb, float t = 1.0f) ->void
	{
		index++;
		draw_info src = { type_circle };
		src.u.circle_info.x = x;
		src.u.circle_info.y = y;
		src.u.circle_info.r = r;
		src.u.circle_info.rgb = rgb;
		src.u.circle_info.t = t;
		memcpy(&data->pinfo[index - 1], &src, sizeof(src));
	}
	auto text(float x, float y, int rgb, const char* txt) ->void
	{
		index++;
		draw_info src = { type_text };
		src.u.text_info.x = x;
		src.u.text_info.y = y;
		src.u.text_info.rgb = rgb;
		memcpy(src.u.text_info.txt, txt, strlen(txt));
		memcpy(&data->pinfo[index - 1], &src, sizeof(src));
	}
}