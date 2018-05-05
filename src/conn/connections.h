#ifndef __M_CONN_POOL_H__
#define __M_CONN_POOL_H__
#include <queue>
#include <stdexcept>
#include <string>
#include <map>
#include <cstddef>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/listener.h>
#include <event2/buffer.h>
#include "dns.h"
extern "C" {
#include <http-internal.h>
}
using namespace std;

class Connection;

class ConnectionPool
{
public:
    ConnectionPool(const string &d_url, struct event_base * b);
    ~ConnectionPool();

    void release(Connection * c);
    Connection * getConnection(void);
    struct evhttp_uri * getURI(void);
    struct event_base * getBase(void);
    struct evdns_base * getDNSBase(void);
    void setDNSBase(struct evdns_base * dns);

private:
    string url;
    unsigned int count;
    struct evhttp_uri * uri;
    struct event_base * base;
    struct evdns_base * dnsBase;
    queue<Connection *> freeList;

    ConnectionPool(const ConnectionPool & c);
    ConnectionPool & operator=(const ConnectionPool & c);
};

class Connection
{
public:
    Connection(ConnectionPool * p);
    ~Connection();

    void release(void);
    /**
     * @brief get get方式发出请求
     * @param args 回调函数时携带的参数
     * @param params get参数，直接以字符串形式追加到uri中
     * @param headers http头信息，map<string, string>类型
     * @return 成功返回true，失败返回false
     */
    bool get(void (*cb)(struct evhttp_request *, void *),
             void *args, const char* params, const map<string, string> &headers);

    /**
     * @brief post post方式发出请求
     * @param args 回调函数时携带的参数
     * @param data 请求时携带的post数据
     * @param len 数据的长度
     * @param headers http头信息，map<string, string>类型
     * @return 成功返回true，失败返回false
     */
    bool post(void (*cb)(struct evhttp_request *, void *),
              void* args, const char *data, size_t len, const map<string, string> &headers);

    // 连接是否正常
    bool isConnected(void)
    {
        switch (cn->state)
        {
        case EVCON_DISCONNECTED:
        case EVCON_CONNECTING:
            return false;
        case EVCON_IDLE:
        case EVCON_READING_FIRSTLINE:
        case EVCON_READING_HEADERS:
        case EVCON_READING_BODY:
        case EVCON_READING_TRAILER:
        case EVCON_WRITING:
        default:
            return true;
        }
    }
private:
    ConnectionPool * pool;
    struct evhttp_request * req;
    struct evhttp_connection * cn;
    string path;
};
#endif
