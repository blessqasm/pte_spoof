#include "./mapper.h"
#include "./utils.h"
#include "./kdmapper.h"

bool mapper::map(std::uint64_t imageBase, std::uint64_t imageSize) {
	log("[?] Kdmapper start execution\n");
	HANDLE iqvw64e_device_handle = intel_driver::Load();
	if (iqvw64e_device_handle == INVALID_HANDLE_VALUE) {
		return false;
	}
	constexpr const wchar_t driver_name[] = L"driver.sys";
	std::vector<std::uint8_t> raw_image{ 0 };
	
	if (!utils::ReadFileToMemory(std::wstring(driver_name), &raw_image)) {
		intel_driver::Unload(iqvw64e_device_handle);
		return false;
	}
	
	NTSTATUS exitCode = 0;
	if (!kdmapper::MapDriver(iqvw64e_device_handle, raw_image.data(), imageBase, imageSize, false, true, true, false, nullptr, &exitCode)) {
		log("[-] Failed to map : %s" , driver_name);
		intel_driver::Unload(iqvw64e_device_handle);
		return false;
	}

	intel_driver::Unload(iqvw64e_device_handle);
	log("[?] Kdmapepr end execution\n");
	return true;
}