#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "singleton.h"

class DataManager : public Singleton<DataManager>
{
    friend class Singleton<DataManager>;
public:
    DataManager();
};

#endif // DATAMANAGER_H
