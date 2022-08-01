void backtrace(void)
{
    // 获取栈帧首地址
    uint64 fpaddr = r_fp();
    // 获取当前进程在kernel中stack的最大地址
    uint64 max = PGROUNDUP(fpaddr);
    // 因为栈是从高地址至低地址增长的，所以这里用小于号判断
    while (fpaddr < max)
    {
        printf("%p\n", *((uint64 *)(fpaddr - 8)));
        fpaddr = *((uint64 *)(fpaddr - 16));
    }
}