//
// Created by samuel on 11/6/19.
//

#include "http_server.h"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <mutex>

#include "Request.h"
#include "utils.h"
#include "Fs.h"
#include "my_thread.h"

http_server::http_server() {
    this->port = 8080;
    this->ip_to_listen = "0.0.0.0";
    this->max_thread_num = 30;
    this->base_path = "/home/samuel/exp1";
    this->stoped = true;
}

http_server::http_server(string ip, int port, int max_thread, string path) {
    this->ip_to_listen = ip;
    this->port = port;
    this->max_thread_num = max_thread;
    this->base_path = path;
    this->stoped = true;
}

string http_server::get_base_path() {
    return this->base_path;
}

string http_server::get_ip_to_listen() {
    return this->ip_to_listen;
}

int http_server::get_max_thread_num() {
    return this->max_thread_num;
}

int http_server::get_port() {
    return this->port;
}

void http_server::set_base_path(string path) {
    if (this->base_path != path) {
        this->base_path = path;
        restart();
    }
}

void http_server::set_ip_to_listen(string ip) {
    if (this->ip_to_listen != ip) {
        this->ip_to_listen = ip;
        restart();
    }
}

void http_server::set_max_thread_num(int num) {
    if (this->max_thread_num != num) {
        this->max_thread_num = num;
        restart();
    }
}

void http_server::set_port(int port) {
    if (port != this->port) {
        this->port = port;
        restart();
    }
}

void http_server::run() {
    this->stoped = false;
    cout << "Server Started" << endl;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    // 创建新的socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    // 配置socket参数
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  & opt, sizeof(opt))) {
        perror("setsocketopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(this->ip_to_listen.c_str());
    address.sin_port = htons(this->port);
    // 绑定端口等
    if (bind(server_fd, (struct sockaddr * ) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // 开始监听
    if (listen(server_fd, this->max_thread_num) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "* Running on http://" << this->ip_to_listen << ":" <<  this->port << "/ (Press CTRL+C to quit)" << endl;
    vector<my_thread *> threads_pool;
    vector<bool> threads_status;
    while(true) {
        // 阻塞监听
        if (this->stoped) {
            cout << "* Server Stoped" << endl;
            return;
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        string ip_in = string(inet_ntoa(address.sin_addr));

        for (int i = 0; i < threads_pool.size(); i++) {
            if (threads_pool.at(i)->finished) {
                threads_pool.erase(threads_pool.begin() + i);
                i--;
            }
        }
//        cout << "目前有" << threads_pool.size() << "个连接" << endl;
        if (threads_pool.size() < this->max_thread_num) {
            // 如果还可以加入
            my_thread client;
            client.create_thread(new_socket, ip_in);
            threads_pool.push_back(&client);
            client.added = true;
        } else {
            // 如果已经满了，持续等待直到有空闲
            while (1) {
                if (this->stoped) {
                    return;
                }
                bool flag = false;
                for (int i = 0; i < threads_pool.size(); i++) {
                    if (threads_pool.at(i)->finished) {
                        threads_pool.erase(threads_pool.begin() + i);
                        i--;
                        flag = true;
                    }
                }
                if (flag) {
                    break;
                }
            }
        }
    }
}


void http_server::stop() {
    this->stoped = true;
}

void http_server::restart() {
    cout << "* Server Restarting" << endl;
    this->stoped = true;
    sleep(1);
    this->run();
}