#include "hashtable.h"

uint64_t CalcHashKey(const char * data, size_t len)
{
    uint64_t value = 0;
    for(size_t i = 0; i < len; i++)
        value = value * 31 + data[i];
    return value;
}

HashTable* HashTableNew(int32_t rows)
{
    HashTable * table = Hund::Slabs::I().Calloc<HashTable>();
    table->nrows = rows;
    table->size = 0;
    table->buckets = Hund::Slabs::I().Calloc<hlist_head>(rows);

    for (int32_t i = 0; i < rows; i++)
        INIT_HLIST_HEAD(&table->buckets[i]);
    return table;
}

HashNode* HashInsert(HashTable *table, uint64_t hash, uint8_t *key, size_t len, uint64_t value)
{
    uint64_t index = hash % table->nrows;
    HashNode * node = NULL;
    struct hlist_head * head = &table->buckets[index];
    struct hlist_node * iter;

    hlist_for_each_entry(node, iter, head, hnode)
    {
        if(hash == node->hash && len == node->len)
        {
            if(memcmp(key, node->key, len) == 0)
            {
                node->value = value;
                return node;
            }
        }
    }
    node = Hund::Slabs::I().Calloc<HashNode>();
    node->key = Hund::Slabs::I().Malloc<uint8_t>(len);
    node->hash = hash;
    node->len = len;
    node->value = value;
    memcpy(node->key, key, len);
    INIT_HLIST_NODE(&node->hnode);
    hlist_add_head(&node->hnode, &table->buckets[index]);
    return node;
}

void HashDelete(HashTable *table, uint64_t hash, uint8_t *key, size_t len)
{
    HashNode * node = HashFind(table, hash, key, len);
    if(!node)
        return;
    hlist_del_init(&node->hnode);
    Hund::Slabs::I().Free(node->key);
    Hund::Slabs::I().Free(node);
}

HashNode* HashFind(HashTable *table, uint64_t hash, uint8_t *key, size_t len)
{
    uint64_t index = hash % table->nrows;
    HashNode * node = NULL;
    struct hlist_head * head = &table->buckets[index];
    struct hlist_node * iter;

    hlist_for_each_entry(node, iter, head, hnode)
    {
        if(hash == node->hash && len == node->len)
        {
            if(memcmp(key, node->key, len) == 0)
            {
                return node;
            }
        }
    }
    return NULL;
}
