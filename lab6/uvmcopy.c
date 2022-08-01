int uvmcopy(pagetable_t old, pagetable_t new, uint64 sz)
{
    pte_t *pte;
    uint64 pa, i;
    uint flags;
    // char *mem;

    for (i = 0; i < sz; i += PGSIZE)
    {
        if ((pte = walk(old, i, 0)) == 0)
            panic("uvmcopy: pte should exist");
        if ((*pte & PTE_V) == 0)
            panic("uvmcopy: page not present");
        pa = PTE2PA(*pte);
        *pte = (*pte & ~PTE_W) | PTE_COW;
        flags = PTE_FLAGS(*pte);
        // if((mem = kalloc()) == 0)
        //   goto err;
        // memmove(mem, (char*)pa, PGSIZE);
        if (mappages(new, i, PGSIZE, pa, flags) != 0)
        {
            // kfree(mem);
            goto err;
        }
        // acquire(&ref_lock);
        page_ref[COW_INDEX(pa)]++;
        // release(&ref_lock);
    }
    return 0;

err:
    uvmunmap(new, 0, i / PGSIZE, 1);
    return -1;
}