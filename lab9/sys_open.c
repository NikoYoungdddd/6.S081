int depth = 0;
...
    // 不断判断该 inode 是否为符号链接
    while (ip->type == T_SYMLINK && !(omode & O_NOFOLLOW))
{
    // 如果访问深度过大，则退出
    if (depth++ >= 20)
    {
        iunlockput(ip);
        end_op();
        return -1;
    }
    // 读取对应的 inode
    if (readi(ip, 0, (uint64)path, 0, MAXPATH) < MAXPATH)
    {
        iunlockput(ip);
        end_op();
        return -1;
    }
    iunlockput(ip);
    // 根据文件名称找到对应的 inode
    if ((ip = namei(path)) == 0)
    {
        end_op();
        return -1;
    }
    ilock(ip);
}
...