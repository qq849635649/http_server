#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "singleton.h"

class MConfig : public Singleton<MConfig>
{
    friend class Singleton<MConfig>;
public:
    MConfig();
};

#endif // __CONFIG_H__
