uint64
sys_symlink(void)
{
    char path[MAXPATH], target[MAXPATH];
    struct inode *ip;
    // 读取参数
    if (argstr(0, target, MAXPATH) < 0)
        return -1;
    if (argstr(1, path, MAXPATH) < 0)
        return -1;
    // 开启事务
    begin_op();
    // 为这个符号链接新建一个 inode
    if ((ip = create(path, T_SYMLINK, 0, 0)) == 0)
    {
        end_op();
        return -1;
    }
    // 在符号链接的 data 中写入被链接的文件
    if (writei(ip, 0, (uint64)target, 0, MAXPATH) < MAXPATH)
    {
        iunlockput(ip);
        end_op();
        return -1;
    }
    // 提交事务
    iunlockput(ip);
    end_op();
    return 0;
}