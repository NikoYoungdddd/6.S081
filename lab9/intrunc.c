void itrunc(struct inode *ip)
{
    ...

        if (ip->addrs[NDIRECT + 1])
    {
        bp = bread(ip->dev, ip->addrs[NDIRECT + 1]);
        a = (uint *)bp->data;
        for (j = 0; j < NINDIRECT; j++)
        {
            if (a[j])
            {
                bp2 = bread(ip->dev, a[j]);
                a2 = (uint *)bp2->data;
                for (i = 0; i < NINDIRECT; i++)
                {
                    if (a2[i])
                        bfree(ip->dev, a2[i]);
                }
                brelse(bp2);
                bfree(ip->dev, a[j]);
                a[j] = 0;
            }
        }
        brelse(bp);
        bfree(ip->dev, ip->addrs[NDIRECT + 1]);
        ip->addrs[NDIRECT] = 0;
    }

    ip->size = 0;
    iupdate(ip);
}