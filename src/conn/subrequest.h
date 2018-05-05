#ifndef __M_SUBREQUEST_H__
#define __M_SUBREQUEST_H__
#include <stdexcept>
#include "connections.h"
#include "config.h"
#include "dns.h"
using namespace std;

class SubRequest
{
public:
    SubRequest(const string& url, struct event_base * base, struct evdns_base * db = NULL);
	~SubRequest();

	Connection * getConnection(void);
private:
	ConnectionPool * pool;

	SubRequest(const SubRequest & c);
	SubRequest & operator=(const SubRequest & c);
};
#endif
