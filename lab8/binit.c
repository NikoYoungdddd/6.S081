void binit(void)
{
    struct buf *b;

    initlock(&bcache.biglock, "bcache_biglock");
    for (int i = 0; i < NBUCKET; i++)
        initlock(&bcache.lock[i], "bcache");

    // Create linked list of buffers
    // bcache.head.prev = &bcache.head;
    // bcache.head.next = &bcache.head;
    for (int i = 0; i < NBUCKET; i++)
    {
        bcache.head[i].next = &bcache.head[i];
        bcache.head[i].prev = &bcache.head[i];
    }
    for (b = bcache.buf; b < bcache.buf + NBUF; b++)
    {
        b->next = bcache.head[0].next;
        b->prev = &bcache.head[0];
        initsleeplock(&b->lock, "buffer");
        bcache.head[0].next->prev = b;
        bcache.head[0].next = b;
    }
}