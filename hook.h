#pragma once
#include <cstdint>
namespace Hook
{
	void VMT(void* addr, void* pDes, int index, void** ret)
	{
		auto vtable = *(uintptr_t**)addr;
		int methods = 0;
		do {
			++methods;
		} while (*(uintptr_t*)((uintptr_t)vtable + (methods * 0x8)));
		auto vtable_buf = new uint64_t[methods * 0x8];
		for (auto count = 0; count < methods; ++count) {
			vtable_buf[count] = *(uintptr_t*)((uintptr_t)vtable + (count * 0x8));

			*ret = (void*)vtable[index];

			vtable_buf[index] = (uintptr_t)(pDes);
			*(uint64_t**)addr = vtable_buf;
		}
	}

	void vhook(void* addr, void* pDes, int index, void** ret) {
		auto vtable = Memory::ReadStub<uintptr_t*>(std::uintptr_t(addr));
		int methods = 0;

		for (int i = 0; Memory::ReadStub<uintptr_t>((uintptr_t)vtable + (i * 0x8)); i++) methods++;

		auto vtable_buf = new uint64_t[methods * 0x8];
		for (int count = 0; count < methods; ++count) {
			vtable_buf[count] = Memory::ReadStub<uintptr_t>((uintptr_t)vtable + (count * 0x8));

			*ret = (void*)vtable[index];

			vtable_buf[index] = (uintptr_t)(pDes);
			*(uint64_t**)(std::uintptr_t(addr)) = vtable_buf;
			//memory::write<uint64_t*>(std::uintptr_t(addr), vtable_buf);
		}
	}
}