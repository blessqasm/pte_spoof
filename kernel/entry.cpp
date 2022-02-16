#include "./global.h"
#include "./pte.h"

void real_ep(runtime::uint64_t imageBase, runtime::uint64_t imageSize) {

	log("imageBase 0x%p", imageBase);
	log("imageSize : 0x%x", imageSize);
	PEPROCESS user_object = other::getObjectProcess(L"user.exe");
	if (!user_object)
		return;
	log("user object : 0x%p", user_object);
	pte* pte_instance = pte::get_instance();
	pte_instance->object(user_object);

	if (!pte_instance->spoof(imageBase, imageSize))
		return;
	return;
}