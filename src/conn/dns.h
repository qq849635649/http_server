#ifndef __M_DNS_H__
#define __M_DNS_H__

#include <event2/dns.h>
#include "singleton.h"
class DNSClient : public Singleton<DNSClient>
{
private:
    struct evdns_base * dns_base_;
protected:
    friend class Singleton<DNSClient>;
    DNSClient() : dns_base_(NULL) {}
    ~DNSClient()
    {
        if(dns_base_)
            evdns_base_free(dns_base_, 0);
    }
public:
    int Init(struct event_base * base)
    {
        dns_base_ = evdns_base_new(base, 1);
        if(!dns_base_)
            return -1;
        return evdns_base_set_option(dns_base_, "randomize-case:", "0");
    }

    struct evdns_base * GetBase()
    {
        return dns_base_;
    }
};


#endif
