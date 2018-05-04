#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <sys/types.h>
#include <stddef.h>
#include <stdint.h>
#include <list.h>
#include "slabs.h"

//哈希桶

// 节点
struct HashNode
{
    uint64_t value;
    size_t len;
    uint8_t * key;
    uint64_t hash;
    struct hlist_node hnode;
};

// 哈希桶定义
struct HashTable
{
    int32_t nrows;
    uint32_t size;
    struct hlist_head * buckets;
};

// 计算hash
uint64_t CalcHashKey(const char * data, size_t len);

// 创建哈希桶
HashTable* HashTableNew(int32_t rows);

// 往哈希桶插入数据
HashNode* HashInsert(HashTable * table, uint64_t hash, uint8_t * key, size_t len, uint64_t value);

// 从哈希桶中删除数据
void HashDelete(HashTable * table, uint64_t hash, uint8_t * key, size_t len);

// 查找数据
HashNode* HashFind(HashTable * table, uint64_t hash, uint8_t * key, size_t len);

#endif // HASHTABLE_H
