#include <cstdlib>
#include "subrequest.h"

SubRequest::SubRequest(const string& url, struct event_base* base, struct evdns_base* db)
{
    pool = new ConnectionPool(url, base);
	pool->setDNSBase(db);
}

SubRequest::~SubRequest()
{
	delete pool;
}

Connection * SubRequest::getConnection(void)
{
	Connection * conn = NULL;
	try
	{
		conn = pool->getConnection();
	}
	catch(logic_error & e)
	{
		//do nothing
	}
	return conn;
}
