#include "./global.h"
#include "./mapper.h"

DWORD dummy_code() {
	return 1337;
}

void dummy_stub() {

};

int main() {

	PVOID testVa = VirtualAlloc(nullptr, 128, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!testVa)
		return false;
	log("test va : 0x%p", testVa);
	std::uint64_t pDummy_code = reinterpret_cast<std::uint64_t>(&dummy_code);
	std::size_t size_dummy = std::uint64_t(dummy_stub) - std::uint64_t(dummy_code);
	std::memcpy(testVa, reinterpret_cast<void*>(pDummy_code), size_dummy);
	

	
	mapper* mapper_instance = mapper::get_instance();
	if (!mapper_instance->map(std::uint64_t(testVa), 64))
		return false;

	getchar();
	/*test pte spoof*/
	using tDummy_code = DWORD(_stdcall*)();
	tDummy_code local_dummy_code = reinterpret_cast<tDummy_code>(testVa);
	local_dummy_code();
	
	getchar();
	Sleep(5000);
	return true;
}