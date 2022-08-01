static struct buf *
bget(uint dev, uint blockno)
{
    struct buf *b, *b2 = 0;

    int i = hash(blockno), min_ticks = 0;
    acquire(&bcache.lock[i]);

    // 1. Is the block already cached?
    for (b = bcache.head[i].next; b != &bcache.head[i]; b = b->next)
    {
        if (b->dev == dev && b->blockno == blockno)
        {
            b->refcnt++;
            release(&bcache.lock[i]);
            acquiresleep(&b->lock);
            return b;
        }
    }
    release(&bcache.lock[i]);

    // 2. Not cached.
    acquire(&bcache.biglock);
    acquire(&bcache.lock[i]);
    // 2.1 find from current bucket.
    for (b = bcache.head[i].next; b != &bcache.head[i]; b = b->next)
    {
        if (b->dev == dev && b->blockno == blockno)
        {
            b->refcnt++;
            release(&bcache.lock[i]);
            release(&bcache.biglock);
            acquiresleep(&b->lock);
            return b;
        }
    }
    // 2.2 find a LRU block from current bucket.
    for (b = bcache.head[i].next; b != &bcache.head[i]; b = b->next)
    {
        if (b->refcnt == 0 && (b2 == 0 || b->lastuse < min_ticks))
        {
            min_ticks = b->lastuse;
            b2 = b;
        }
    }
    if (b2)
    {
        b2->dev = dev;
        b2->blockno = blockno;
        b2->refcnt++;
        b2->valid = 0;
        // acquiresleep(&b2->lock);
        release(&bcache.lock[i]);
        release(&bcache.biglock);
        acquiresleep(&b2->lock);
        return b2;
    }
    // 2.3 find block from the other buckets.
    for (int j = hash(i + 1); j != i; j = hash(j + 1))
    {
        acquire(&bcache.lock[j]);
        for (b = bcache.head[j].next; b != &bcache.head[j]; b = b->next)
        {
            if (b->refcnt == 0 && (b2 == 0 || b->lastuse < min_ticks))
            {
                min_ticks = b->lastuse;
                b2 = b;
            }
        }
        if (b2)
        {
            b2->dev = dev;
            b2->refcnt++;
            b2->valid = 0;
            b2->blockno = blockno;
            // remove block from its original bucket.
            b2->next->prev = b2->prev;
            b2->prev->next = b2->next;
            release(&bcache.lock[j]);
            // add block
            b2->next = bcache.head[i].next;
            b2->prev = &bcache.head[i];
            bcache.head[i].next->prev = b2;
            bcache.head[i].next = b2;
            release(&bcache.lock[i]);
            release(&bcache.biglock);
            acquiresleep(&b2->lock);
            return b2;
        }
        release(&bcache.lock[j]);
    }
    release(&bcache.lock[i]);
    release(&bcache.biglock);
    panic("bget: no buffers");
}
