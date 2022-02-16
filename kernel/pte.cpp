#include "./pte.h"


PT_ENTRY_64* pte::getPte(
	runtime::uint64_t VirtualAddress) {
	ADDRESS_TRANSLATION_HELPER helper;
	UINT32 level;
	PT_ENTRY_64* finalEntry;
	PML4E_64* pml4;
	PML4E_64* pml4e;
	PDPTE_64* pdpt;
	PDPTE_64* pdpte;
	PDE_64* pd;
	PDE_64* pde;
	PTE_64* pt;
	PTE_64* pte;

	helper.AsUInt64 = (UINT64)VirtualAddress;

	PHYSICAL_ADDRESS    addr;

	addr.QuadPart = cr3.AddressOfPageDirectory << PAGE_SHIFT;

	pml4 = (PML4E_64*)MmGetVirtualForPhysical(addr);

	pml4e = &pml4[helper.AsIndex.Pml4];

	if (pml4e->Present == FALSE) {
		finalEntry = (PT_ENTRY_64*)pml4e;
		goto Exit;
	}

	addr.QuadPart = pml4e->PageFrameNumber << PAGE_SHIFT;

	pdpt = (PDPTE_64*)MmGetVirtualForPhysical(addr);

	pdpte = &pdpt[helper.AsIndex.Pdpt];

	if ((pdpte->Present == FALSE) || (pdpte->LargePage != FALSE)) {
		finalEntry = (PT_ENTRY_64*)pdpte;
		goto Exit;
	}

	addr.QuadPart = pdpte->PageFrameNumber << PAGE_SHIFT;

	pd = (PDE_64*)MmGetVirtualForPhysical(addr);

	pde = &pd[helper.AsIndex.Pd];

	if ((pde->Present == FALSE) || (pde->LargePage != FALSE)) {
		finalEntry = (PT_ENTRY_64*)pde;
		goto Exit;
	}

	addr.QuadPart = pde->PageFrameNumber << PAGE_SHIFT;

	pt = (PTE_64*)MmGetVirtualForPhysical(addr);

	pte = &pt[helper.AsIndex.Pt];

	finalEntry = (PT_ENTRY_64*)pte;
	return  (PT_ENTRY_64*)pte;

Exit:
	return finalEntry;
}
bool pte::spoof(runtime::uint64_t startVa, runtime::size_t sizeVa) {
	
	KAPC_STATE apcState{ 0 };
	KeStackAttachProcess(objVa, &apcState);

	cr3.Flags = __readcr3();
	
	if (sizeVa <= 0x1000) {
		PTE_64* pte_64 = reinterpret_cast<PTE_64*>(getPte(startVa));
		if(pte_64)
			if (pte_64->Present) {
				log("pte_64 : 0x%p", pte_64);
				pte_64->ExecuteDisable = false;
			}
	}
	else {

	}
	KeUnstackDetachProcess(&apcState);
	return true;
}