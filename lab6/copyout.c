int copyout(pagetable_t pagetable, uint64 dstva, char *src, uint64 len)
{
    uint64 n, va0, pa0;
    pte_t *pte;

    while (len > 0)
    {
        va0 = PGROUNDDOWN(dstva);
        if (cow_alloc(pagetable, va0) != 0)
            return -1;
        pa0 = walkaddr(pagetable, va0);
        if (pa0 == 0)
            return -1;
        n = PGSIZE - (dstva - va0);
        if (n > len)
            n = len;
        pte = walk(pagetable, va0, 0);
        if (pte == 0)
            return -1;
        memmove((void *)(pa0 + (dstva - va0)), src, n);

        len -= n;
        src += n;
        dstva = va0 + PGSIZE;
    }
    return 0;
}