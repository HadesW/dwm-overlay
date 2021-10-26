#pragma once
#include <dxgi.h>
#include "utils.hpp"
#include "logger.hpp"
#include "draw.hpp"
#include "minhook\inc\MinHook.h"


namespace hooks
{
	using present_mpo_t = __int64(__fastcall*)(void*, IDXGISwapChain*, unsigned int, unsigned int, int, __int64, __int64, int);
	using present_dwm_t = __int64(__fastcall*)(void*, IDXGISwapChain*, unsigned int, unsigned int, const struct tagRECT*, unsigned int, const struct DXGI_SCROLL_RECT*, unsigned int, struct IDXGIResource*, unsigned int);

	present_mpo_t fn_present_mpo = nullptr;
	present_dwm_t fn_present_dwm = nullptr;

	//
	// 
	//

	// DrawingContext::PresentDWM(IDXGISwapChainDWM1 *,uint,uint,tagRECT const *,uint,DXGI_SCROLL_RECT const *,uint,IDXGIResource *,uint)
	// DrawingContext::PresentMultiplaneOverlay(IDXGISwapChainDWM1 *,uint,uint,DXGI_HDR_METADATA_TYPE,void const *,_DXGI_PRESENT_MULTIPLANE_OVERLAY const *,uint)

	__int64 __fastcall hk_present_mpo(void* thisptr, IDXGISwapChain* swapchain, unsigned int a3, unsigned int a4, int a5, __int64 a6, __int64 a7, int a8)
	{
		//logx("hk_present_mpo call");
		draw::present(swapchain);
		return fn_present_mpo(thisptr, swapchain, a3, a4, a5, a6, a7, a8);
	}


	__int64 __fastcall hk_present_dwm(void* thisptr, IDXGISwapChain* swapchain, unsigned int a3, unsigned int a4, const struct tagRECT* a5, unsigned int a6, const struct DXGI_SCROLL_RECT* a7, unsigned int a8, struct IDXGIResource* a9, unsigned int a10)
	{
		//logx("hk_present_dwm call");
		draw::present(swapchain);
		return fn_present_dwm(thisptr, swapchain, a3, a4, a5, a6, a7, a8, a9, a10);
	}

	uintptr_t* get_vtable()
	{
		uintptr_t* vtable = nullptr;

		do
		{
			//
			// 如果有不同特征，可以根据版本来判断
			// bool supported()；实现此函数
			//

			// 11  ->   57 48 83 EC 20 48 8D 05 ??  ??  ??  ?? 33 ED 48 8B D9 48 89 01
			uint8_t* address = utils::search("d2d1.dll", "48 8D 05 ??  ??  ??  ?? 33 ED 48 8D 71 08");
			if (!address)
			{
				break;
			}

			uint32_t offset = *reinterpret_cast<uint32_t*>(address + 0x3);
			vtable = reinterpret_cast<uintptr_t*>(address + offset + 0x7);
		} while (false);

		return vtable;
	}

	bool dwmhook()
	{
		bool ret = false;

		do
		{
			uintptr_t* vtable = get_vtable();
			if (!vtable)
			{
				break;
			}

			// minhook
			MH_STATUS status = MH_Initialize();
			if (status != MH_OK)
			{
				break;
			}

			status = MH_CreateHook(reinterpret_cast<void*>(vtable[6]), reinterpret_cast<void*>(&hk_present_dwm), reinterpret_cast<void**>(&fn_present_dwm));
			status = MH_CreateHook(reinterpret_cast<void*>(vtable[7]), reinterpret_cast<void*>(&hk_present_mpo), reinterpret_cast<void**>(&fn_present_mpo));

			logx("vtable = 0x%llx", vtable);
			logx("fn_present_dwm = 0x%llx", fn_present_dwm);
			logx("fn_present_mpo = 0x%llx", fn_present_mpo);
			logx("hk_present_dwm = 0x%llx", hk_present_dwm);
			logx("hk_present_mpo = 0x%llx", hk_present_mpo);

			status = MH_EnableHook(MH_ALL_HOOKS);
			if (status != MH_OK)
			{
				break;
			}

			ret = true;
		} while (false);

		return ret;
	}
}