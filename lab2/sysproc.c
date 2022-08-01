uint64
sys_trace(void)
{
    int mask;
    // 取 a0 寄存器中的值返回给 mask
    if (argint(0, &mask) < 0)
        return -1;

    // 把 mask 传给现有进程的 mask
    myproc()->mask = mask;
    return 0;
}

static char *syscall_names[] = {
    "",
    "fork",
    "exit",
    "wait",
    "pipe",
    "read",
    "kill",
    "exec",
    "fstat",
    "chdir",
    "dup",
    "getpid",
    "sbrk",
    "sleep",
    "uptime",
    "open",
    "write",
    "mknod",
    "unlink",
    "link",
    "mkdir",
    "close",
    "trace",
    "sysinfo",
};

void syscall(void)
{
    int num;
    struct proc *p = myproc();

    num = p->trapframe->a7;
    if (num > 0 && num < NELEM(syscalls) && syscalls[num])
    {
        p->trapframe->a0 = syscalls[num]();

        // Start  从a7读取系统调用的编号，将1<<num与进程的mask比较，相等则打印
        if ((1 << num) & p->mask)
        {
            printf("%d: syscall %s -> %d\n", p->pid, syscall_names[num], p->trapframe->a0);
        }
        // End
    }
    else
    {
        printf("%d %s: unknown sys call %d\n",
               p->pid, p->name, num);
        p->trapframe->a0 = -1;
    }
}
