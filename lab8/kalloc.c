void *
kalloc(void)
{
    struct run *r;

    push_off();
    int id = cpuid();
    pop_off();

    acquire(&kmem[id].lock);
    r = kmem[id].freelist;
    if (r)
        kmem[id].freelist = r->next;
    else
    {
        for (int i = 0; i < NCPU; i++)
        {
            if (i == id)
                continue;
            acquire(&kmem[i].lock);
            r = kmem[i].freelist;
            if (r)
                kmem[i].freelist = r->next;
            release(&kmem[i].lock);
            if (r)
                break;
        }
    }
    release(&kmem[id].lock);

    if (r)
        memset((char *)r, 5, PGSIZE); // fill with junk
    return (void *)r;
}