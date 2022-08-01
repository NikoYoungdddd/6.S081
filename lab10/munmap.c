uint64
sys_munmap(void)
{
    uint64 addr;
    int length;
    struct proc *p = myproc();
    struct vma *vma = 0;
    if (argaddr(0, &addr) || argint(1, &length))
        return -1;
    addr = PGROUNDDOWN(addr);
    length = PGROUNDUP(length);
    for (int i = 0; i < VMASIZE; i++)
    {
        if (addr >= p->vma[i].addr || addr < p->vma[i].addr + p->vma[i].length)
        {
            vma = &p->vma[i];
            break;
        }
    }
    if (vma == 0)
        return 0;
    if (vma->addr == addr)
    {
        vma->addr += length;
        vma->length -= length;
        if (vma->flags & MAP_SHARED)
            filewrite(vma->file, addr, length);
        uvmunmap(p->pagetable, addr, length / PGSIZE, 1);
        if (vma->length == 0)
        {
            fileclose(vma->file);
            vma->used = 0;
        }
    }
    return 0;
}