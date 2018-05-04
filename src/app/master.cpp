#include "master.h"

#include <event2/buffer.h>
#include <event2/http.h>
#include <event2/http_struct.h>

MasterApp::MasterApp()
{}

MasterApp::~MasterApp()
{}

void MasterApp::Init()
{}

// 函数功能: 健康检查窗口
void MasterApp::HealthCheckCallback(struct evhttp_request * req, void * arg)
{
    pid_t pid =  getpid();
    cout << "worker pid = " << pid << endl;
    evbuffer_add(req->output_buffer, "alive", 5);
    evhttp_send_reply(req, 200, "OK", NULL);
}
