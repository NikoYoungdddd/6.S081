void u2kvmcopy(pagetable_t upagetable, pagetable_t kpagetable, uint64 oldsz, uint64 newsz)
{
    oldsz = PGROUNDUP(oldsz);
    for (uint64 i = oldsz; i < newsz; i += PGSIZE)
    {
        pte_t *pte_from = walk(upagetable, i, 0);
        pte_t *pte_to = walk(kpagetable, i, 1);
        if (pte_from == 0)
            panic("u2kvmcopy: src pte do not exist");
        if (pte_to == 0)
            panic("u2kvmcopy: dest pte walk fail");
        uint64 pa = PTE2PA(*pte_from);
        uint flag = (PTE_FLAGS(*pte_from)) & (~PTE_U);
        *pte_to = PA2PTE(pa) | flag;
    }
}