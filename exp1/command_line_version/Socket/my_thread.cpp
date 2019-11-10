//
// Created by samuel on 11/6/19.
//

#include "my_thread.h"

#include "utils.h"
#include "Request.h"

using namespace std;
void my_thread::create_thread(int new_socket, string ip_in){
    thread client(client_thread, new_socket, ip_in, this); // 创建一个新的线程
    client.detach(); // 分发
}

my_thread::my_thread() {
    this->finished = false;
}
