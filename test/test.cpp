// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

#include "logger.hpp"
#include "draw-sdk.hpp"


void test()
{
	if (!draw::begin())
		return;

	// 要画的东西 begin 和 end 中间
	draw::line(200, 200, 500, 500, 222);

	draw::rect(250, 250, 700, 700, 222);

	draw::circle(600, 600, 300, 222);

	draw::text(450, 250, 222, "hello hello hello hello hello");

	draw::end();//end 相当于提交
}


int main()
{
	log("test draw-sdk");

	int ret = EXIT_FAILURE;

	do
	{
		// 初始化 画图库
		if (!draw::init())
		{
			break;
		}

		// 画就完事了
		test();

		ret = EXIT_SUCCESS;
	} while (false);

	return ret;
}

