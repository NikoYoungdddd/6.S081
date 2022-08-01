uint64
sys_sigalarm(void)
{
    int ticks = 0;
    uint64 alarmhandler = 0;

    if (argint(0, &ticks) < 0 || argaddr(1, &alarmhandler) < 0)
        return -1;

    struct proc *p = myproc();
    p->alarmticks = ticks;
    p->alarmhandler = alarmhandler;
    p->tickspass = 0;
    p->accessable = 1;

    return 0;
}

uint64
sys_sigreturn(void)
{
    struct proc *p = myproc();
    // 恢复系统调用时用户的栈帧
    if (0 == p->accessable)
    {
        memmove(p->trapframe, p->alarmtrapframe, sizeof(struct trapframe));
        p->accessable = 1;
    }

    return 0;
}