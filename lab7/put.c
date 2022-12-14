pthread_mutex_t lock[NBUCKET];
static void put(int key, int value)
{
    int i = key % NBUCKET;

    pthread_mutex_lock(&lock[i]);
    // is the key already present?
    struct entry *e = 0;
    for (e = table[i]; e != 0; e = e->next)
    {
        if (e->key == key)
            break;
    }
    if (e)
    {
        // update the existing key.
        e->value = value;
    }
    else
    {
        // the new is new.
        insert(key, value, &table[i], table[i]);
    }
    pthread_mutex_unlock(&lock[i]);
}

static struct entry *
get(int key)
{
    int i = key % NBUCKET;

    pthread_mutex_lock(&lock[i]);
    struct entry *e = 0;
    for (e = table[i]; e != 0; e = e->next)
    {
        if (e->key == key)
            break;
    }
    pthread_mutex_unlock(&lock[i]);
    return e;
}