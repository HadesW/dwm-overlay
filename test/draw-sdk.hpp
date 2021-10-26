#pragma once
#include <windows.h>

//
// draw sdk
//


namespace draw
{
	//
	// 定义函数指针这样兼容更好一点
	//
	typedef bool (*init_t)(void);
	typedef bool (*begin_t)(void);
	typedef bool (*end_t)(void);
	typedef void (*line_t)(float x1, float y1, float x2, float y2, int rgb, float t);
	typedef void (*rect_t)(float x, float y, float w, float h, int rgb, float t);
	typedef void (*circle_t)(float x, float y, float r, int rgb, float t);
	typedef void (*text_t)(float x, float y, int rgb, const char* txt);

	init_t pfninit = nullptr;
	begin_t pfnbegin = nullptr;
	end_t pfnend = nullptr;
	line_t pfnline = nullptr;
	rect_t pfnrect = nullptr;
	circle_t pfncircle = nullptr;
	text_t pfntext = nullptr;

	// 初始化
	bool init()
	{
		bool ret = false;

		do
		{
			// 加载模块
			HMODULE module = LoadLibraryA("share.dll");
			if (!module)
			{
				break;
			}

			// 获取函数
			pfninit = reinterpret_cast<init_t>(GetProcAddress(module, "init"));
			pfnbegin = reinterpret_cast<begin_t>(GetProcAddress(module, "begin"));
			pfnend = reinterpret_cast<end_t>(GetProcAddress(module, "end"));
			pfnline = reinterpret_cast<line_t>(GetProcAddress(module, "line"));
			pfnrect = reinterpret_cast<rect_t>(GetProcAddress(module, "rect"));
			pfncircle = reinterpret_cast<circle_t>(GetProcAddress(module, "circle"));
			pfntext = reinterpret_cast<text_t>(GetProcAddress(module, "text"));
			if (!pfninit || !pfnbegin || !pfnend || !pfnline || !pfnrect || !pfncircle || !pfntext)
			{
				break;
			}

			// 初始化
			if (!pfninit())
			{
				break;
			}

			ret = true;
		} while (false);

		return ret;
	}

	bool begin()
	{
		if (!pfnbegin)
		{
			return false;
		}
		return pfnbegin();
	}

	bool end()
	{
		if (!pfnend)
		{
			return false;
		}
		return pfnend();
	}

	void line(float x1, float y1, float x2, float y2, int rgb, float t = 1.0f)
	{
		if (!pfnline)
		{
			return;
		}
		return pfnline(x1, y1, x2, y2, rgb, t);
	}

	void rect(float x, float y, float w, float h, int rgb, float t = 1.0f)
	{
		if (!pfnrect)
		{
			return;
		}
		return pfnrect(x, y, w, h, rgb, t);
	}

	void circle(float x, float y, float r, int rgb, float t = 1.0f)
	{
		if (!pfncircle)
		{
			return;
		}
		return pfncircle(x, y, r, rgb, t);
	}

	void text(float x, float y, int rgb, const char* txt)
	{
		if (!pfntext)
		{
			return;
		}
		return pfntext(x, y, rgb, txt);
	}
}
