uint64
walkaddr(pagetable_t pagetable, uint64 va)
{
    pte_t *pte;
    uint64 pa;

    if (va >= MAXVA)
        return 0;

    pte = walk(pagetable, va, 0);
    // if(pte == 0)
    //   return 0;
    // if((*pte & PTE_V) == 0)
    //   return 0;
    if (pte == 0 || (*pte & PTE_V) == 0)
    {
        // pa = lazyalloc(va);
        struct proc *p = myproc();
        if (va >= p->sz || va < PGROUNDUP(p->trapframe->sp))
            return 0;
        pa = (uint64)kalloc();
        if (pa == 0)
            return 0;
        if (mappages(p->pagetable, va, PGSIZE, pa, PTE_W | PTE_R | PTE_U | PTE_X) != 0)
        {
            kfree((void *)pa);
            return 0;
        }
        return pa;
    }
    if ((*pte & PTE_U) == 0)
        return 0;
    pa = PTE2PA(*pte);
    return pa;
}
