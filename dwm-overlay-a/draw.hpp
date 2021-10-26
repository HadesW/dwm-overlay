#pragma once
#include <iostream>
#include <thread>
#include <mutex>
#include <dxgi.h>
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"



namespace global
{
	static ID3D11Device* device = nullptr;
	static ID3D11DeviceContext* context = nullptr;
	static ID3D11RenderTargetView* view = nullptr;
}

namespace draw
{
	void line(float x1, float y1, float x2, float y2, int rgb, float t = 1.0f)
	{
		ImGui::GetForegroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImAlphaBlendColors(rgb, 255), t);
	}
	void rect(float x, float y, float w, float h, int rgb, float t = 1.0f)
	{
		ImGui::GetForegroundDrawList()->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), ImAlphaBlendColors(rgb, 255), 0.0f, 0, t);
	}
	void circle(float x, float y, float r, int rgb, float t = 1.0f)
	{
		ImVec2 center = ImVec2(x, y);
		ImGui::GetForegroundDrawList()->AddCircle(center, r, ImAlphaBlendColors(rgb, 255), 0, t);
	}
	void text(float x, float y, int rgb, const char* text)
	{
		ImGui::GetForegroundDrawList()->AddText(ImVec2(x, y), ImAlphaBlendColors(rgb, 255), text);
	}

	void drawx()
	{
		for (uint32_t i = 0; i < msg->get_draw_count(); i++)
		{
			uint32_t type = msg->get_draw_type(i);
			switch (type)
			{
			case draw::draw_type::type_line:
			{
				auto tmp = msg->get_draw_info<draw_line>(i);
				line(tmp.x1, tmp.y1, tmp.x2, tmp.y2, tmp.rgb, tmp.t);
				break;
			}
			case draw::draw_type::type_rect:
			{
				auto tmp = msg->get_draw_info<draw_rect>(i);
				rect(tmp.x, tmp.y, tmp.w, tmp.h, tmp.rgb, tmp.t);
				break;
			}
			case draw::draw_type::type_circle:
			{
				auto tmp = msg->get_draw_info<draw_circle>(i);
				circle(tmp.x, tmp.y, tmp.r, tmp.rgb, tmp.t);
				break;
			}
			case draw::draw_type::type_text:
			{
				auto tmp = msg->get_draw_info<draw_text>(i);
				text(tmp.x, tmp.y, tmp.rgb, tmp.txt);
				break;
			}
			default:
				break;
			}
		}

		return;
	}

	void render()
	{
		// draw
		do
		{
			if (!msg->connet())
			{
				break;
			}

			if (!msg->get_draw_flag())
			{
				break;
			}

			if (msg->get_draw_count() == 0)
			{
				break;
			}

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			// all
			drawx();
			//draw::line(400, 200, 500, 500, 222);

			const float clear_color_with_alpha[4] = {0};
			ImGui::Render();
			global::context->OMSetRenderTargets(1, &global::view, nullptr);
			//global::context->ClearRenderTargetView(global::view, clear_color_with_alpha);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		} while (false);

		return;
	}

	bool setup_imgui(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context)
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.IniFilename = nullptr;
		io.LogFilename = nullptr;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device, context);
		return true;
	}

	void present(IDXGISwapChain* swapchain)
	{
		bool inited = false;
		static std::once_flag once;
		std::call_once(once, [swapchain, &inited]() {

			ID3D11Device* device = nullptr;
			ID3D11DeviceContext* context = nullptr;
			ID3D11Texture2D* texture = nullptr;
			ID3D11RenderTargetView* view = nullptr;

			do
			{
				if (FAILED(swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&device)) || device == nullptr)
				{
					break;
				}

				device->GetImmediateContext(&context);
				if (context == nullptr)
				{
					break;
				}

				if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&texture))))
				{
					break;
				}

				if (FAILED(device->CreateRenderTargetView(texture, NULL, &view)))
				{
					break;
				}

				HWND hwnd = GetDesktopWindow();
				if (!hwnd)
				{
					break;
				}

				if (!setup_imgui(hwnd, device, context))
				{
					break;
				}

				// global::device == nullptr
				global::device = device;
				global::context = context;
				global::view = view;

				inited = true;
			} while (false);

			if (texture)
			{
				texture->Release();
				texture = nullptr;
			}
			});

		//
		if (global::device)
		{
			render();
		}
	}
}