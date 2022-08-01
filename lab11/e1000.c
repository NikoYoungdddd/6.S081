int e1000_transmit(struct mbuf *m)
{
    //
    // Your code here.
    //
    // the mbuf contains an ethernet frame; program it into
    // the TX descriptor ring so that the e1000 sends it. Stash
    // a pointer so that it can be freed after sending.
    //
    acquire(&e1000_lock);
    uint index = regs[E1000_TDT];
    if ((tx_ring[index].status & E1000_TXD_STAT_DD) == 0)
    {
        release(&e1000_lock);
        return -1;
    }
    if (tx_mbufs[index])
        mbuffree(tx_mbufs[index]);
    tx_mbufs[index] = m;
    tx_ring[index].length = m->len;
    tx_ring[index].addr = (uint64)m->head;
    tx_ring[index].cmd = E1000_TXD_CMD_RS | E1000_TXD_CMD_EOP;
    regs[E1000_TDT] = (index + 1) % TX_RING_SIZE;
    release(&e1000_lock);
    return 0;
}

int e1000_transmit(struct mbuf *m)
{
    //
    // Your code here.
    //
    // the mbuf contains an ethernet frame; program it into
    // the TX descriptor ring so that the e1000 sends it. Stash
    // a pointer so that it can be freed after sending.
    //
    acquire(&e1000_lock);
    uint index = regs[E1000_TDT];
    if ((tx_ring[index].status & E1000_TXD_STAT_DD) == 0)
    {
        release(&e1000_lock);
        return -1;
    }
    if (tx_mbufs[index])
        mbuffree(tx_mbufs[index]);
    tx_mbufs[index] = m;
    tx_ring[index].length = m->len;
    tx_ring[index].addr = (uint64)m->head;
    tx_ring[index].cmd = E1000_TXD_CMD_RS | E1000_TXD_CMD_EOP;
    regs[E1000_TDT] = (index + 1) % TX_RING_SIZE;
    release(&e1000_lock);
    return 0;
}