uint64
sys_sbrk(void)
{
    int addr;
    int n;

    if (argint(0, &n) < 0)
        return -1;
    struct proc *p = myproc();
    addr = p->sz;
    // if (addr + n < 0) return -1;
    if (addr + n >= MAXVA || addr + n <= 0)
        return addr;
    p->sz = addr + n;
    // if(growproc(n) < 0)
    //   return -1;
    if (n < 0)
        uvmdealloc(p->pagetable, addr, p->sz);
    return addr;
}