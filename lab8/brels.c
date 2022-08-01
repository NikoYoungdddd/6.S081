int hash(int blockno)
{
    return blockno % NBUCKET;
}
void brelse(struct buf *b)
{
    if (!holdingsleep(&b->lock))
        panic("brelse");

    releasesleep(&b->lock);

    int i = hash(b->blockno);

    acquire(&bcache.lock[i]);
    b->refcnt--;
    if (b->refcnt == 0)
    {
        // no one is waiting for it.
        // b->next->prev = b->prev;
        // b->prev->next = b->next;
        // b->next = bcache.head[i].next;
        // b->prev = &bcache.head[i];
        // bcache.head[i].next->prev = b;
        // bcache.head[i].next = b;
        b->lastuse = ticks;
    }

    release(&bcache.lock[i]);
}

void bpin(struct buf *b)
{
    int i = hash(b->blockno);
    acquire(&bcache.lock[i]);
    b->refcnt++;
    release(&bcache.lock[i]);
}

void bunpin(struct buf *b)
{
    int i = hash(b->blockno);
    acquire(&bcache.lock[i]);
    b->refcnt--;
    release(&bcache.lock[i]);
}