#include "share.hpp"
#include "loader.hpp"
#include "msgs.hpp"
#include "draw.hpp"

bool init()
{
	bool ret = false;

	do
	{
		// memory loader
		if (!loader::memload("dwm-overlay-a.dll"))
		{
			break;
		}

		// alloc local draw memory
		if (!draw::initializer())
		{
			break;
		}

		// open share memory
		if (!msg->initializer())
		{
			break;
		}

		ret = true;
	} while (false);

	return ret;
}

bool begin()
{
	return draw::begin();
}

bool end()
{
	return draw::end();
}

void line(float x1, float y1, float x2, float y2, int rgb, float t /*= 1.0f*/)
{
	return draw::line(x1, y1, x2, y2, rgb, t);
}

void rect(float x, float y, float w, float h, int rgb, float t /*= 1.0f*/)
{
	return draw::rect(x, y, w, h, rgb, t);;
}

void circle(float x, float y, float r, int rgb, float t /*= 1.0f*/)
{
	return draw::circle(x, y, r, rgb, t);;
}

void text(float x, float y, int rgb, const char* txt)
{
	return draw::text(x, y, rgb, txt);;
}
