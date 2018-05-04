#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "singleton.h"
#include "shm/hashtable.h"

class DataManager : public Singleton<DataManager>
{
    friend class Singleton<DataManager>;
public:
    DataManager();
    ~DataManager();
};

#endif // DATAMANAGER_H
