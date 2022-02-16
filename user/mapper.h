#ifndef _MAPPER_H
#define _MAPPER_H

#include "./global.h"

class mapper {
public:
	static mapper* get_instance() {
		mapper mapper_instance;
		return &mapper_instance;
	}
	bool map(std::uint64_t imageBase , std::uint64_t imageSize);
};


#endif
