#pragma once
#include <stdint.h>
#include <windows.h>

#define FILE_MAP_NAME "file-map-name-msgs"


//
// write only msg
//

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
			handle = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, FILE_MAP_NAME);
			if (!handle)
			{
				break;
			}

			buffer = MapViewOfFile(handle, FILE_MAP_ALL_ACCESS, 0, 0, 0);
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

	auto copy(void* src,uint32_t size) -> bool
	{
		if (!mapping)
		{
			return false;
		}

		memcpy(mapping, src, size);	
		return true;
	}

	template <typename type>
	auto set(uint32_t offset, type value) -> bool
	{
		if (!mapping)
		{
			return false;
		}

		auto address = reinterpret_cast<char*>(mapping) + offset;
		write<type>(reinterpret_cast<uintptr_t>(address), value);
		return true;
	}

	auto connet() -> bool
	{
		return true;
	}

protected:
	explicit msgs() {}

	template <typename type>
	auto read(uintptr_t address) -> type
	{
		return *reinterpret_cast<type*>(address);
	}

	template <typename type>
	auto write(uintptr_t address, type value) -> void
	{
		*reinterpret_cast<type*>(address) = value;
		return;
	}

private:
	void* mapping = nullptr;
};

msgs* msg = msgs::instance();

