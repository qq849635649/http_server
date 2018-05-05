#include "connections.h"

Connection::Connection(ConnectionPool * p):pool(p), req(NULL)
{
    struct evhttp_uri * uri = p->getURI();
    int port = evhttp_uri_get_port(uri);

    path = string(evhttp_uri_get_path(uri));
    cn = evhttp_connection_base_new(p->getBase(), p->getDNSBase(), evhttp_uri_get_host(uri), (port == -1 ? 80 : port));
    if(!cn)
        throw logic_error("evhttp_connection_base_new error");

    evhttp_connection_set_timeout(cn, 1);
}

Connection::~Connection()
{
    evhttp_connection_free(cn);
}

void Connection::release(void)
{
    pool->release(this);
}

bool Connection::start(void (*cb)(struct evhttp_request *, void *), void * args,
                       const char * data, size_t len, const char* pid)
{
    struct evhttp_uri * uri = pool->getURI();
    const char* path = evhttp_uri_get_path(uri);
    req = evhttp_request_new(cb, args);

    if(req == NULL)
        return false;

    if(path == NULL)
        path = "/";

    evbuffer_add(req->output_buffer, data, len);
    evhttp_add_header(req->output_headers, "Connection", "keep-alive");
    evhttp_add_header(req->output_headers, "Content-Type", "application/x-protobuf");
    evhttp_add_header(req->output_headers, "Host", evhttp_uri_get_host(uri));
    if(NULL != pid)
    {
        evhttp_add_header(req->output_headers, "Pid", pid);
    }
    if(evhttp_make_request(cn, req, EVHTTP_REQ_POST, path) != 0)
        return false;
    return true;
}

ConnectionPool::ConnectionPool(const string & d_url, struct event_base * b)
    : count(0),
      base(b),
      dnsBase(NULL)
{
    url = d_url;
    uri = evhttp_uri_parse(d_url.c_str());
    if(!uri)
        throw logic_error("evhttp_uri_parse error");
}

ConnectionPool::~ConnectionPool()
{
    evhttp_uri_free(uri);
    while(!freeList.empty())
    {
        Connection * c = freeList.front();
        freeList.pop();
        delete c;
        count--;
    }
}

void ConnectionPool::release(Connection * c)
{
    freeList.push(c);
}

Connection * ConnectionPool::getConnection(void)
{
    while(!freeList.empty())
    {
        Connection * c = freeList.front();
        freeList.pop();
        if(c->isConnected())
        {
            return c;
        }
        else
        {
            delete c;
            count--;
        }
    }
    count++;
    return new Connection(this);
}

struct evhttp_uri * ConnectionPool::getURI(void)
{
    return uri;
}

struct event_base * ConnectionPool::getBase(void)
{
    return base;
}

struct evdns_base * ConnectionPool::getDNSBase(void)
{
    return dnsBase;
}

void ConnectionPool::setDNSBase(struct evdns_base * dns)
{
    dnsBase = dns;
}
