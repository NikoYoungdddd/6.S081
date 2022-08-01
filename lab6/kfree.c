void kfree(void *pa)
{
    struct run *r;

    if (((uint64)pa % PGSIZE) != 0 || (char *)pa < end || (uint64)pa >= PHYSTOP)
        panic("kfree");

    // acquire(&ref_lock);
    if (page_ref[COW_INDEX(pa)] > 1)
    {
        page_ref[COW_INDEX(pa)]--;
        // release(&ref_lock);
        return;
    }
    page_ref[COW_INDEX(pa)] = 0;
    // release(&ref_lock);

    // Fill with junk to catch dangling refs.
    memset(pa, 1, PGSIZE);

    r = (struct run *)pa;

    acquire(&kmem.lock);
    r->next = kmem.freelist;
    kmem.freelist = r;
    release(&kmem.lock);
}