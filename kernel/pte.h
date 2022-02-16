#ifndef _PTE_H
#define _PTE_H

#include "./global.h"
#include "./ia32.h"

class pte {
private:
	CR3 cr3;
	PEPROCESS objVa;
private:
	PT_ENTRY_64* getPte(runtime::uint64_t vaForPte);
public:
	static pte* get_instance() {
		pte  pte_instance;
		return &pte_instance;
	}
	void object(PEPROCESS _objVa) {
		objVa = _objVa;
	}
	bool spoof(runtime::uint64_t startVa , runtime::size_t sizeVa);
};

#endif
