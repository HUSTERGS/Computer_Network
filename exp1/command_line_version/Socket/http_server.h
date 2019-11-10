//
// Created by samuel on 11/6/19.
//

#ifndef SOCKET_HTTP_SERVER_H
#define SOCKET_HTTP_SERVER_H

#include <string>
using namespace std;
class http_server {
public:
    http_server();
    http_server(string ip, int port, int max_thread, string path);
    void set_port(int port);
    void set_max_thread_num(int num);
    void set_ip_to_listen(string ip);
    void set_base_path(string path);

    int get_port();
    int get_max_thread_num();
    string get_ip_to_listen();
    string get_base_path();

    void run();
    void stop();
    void restart();
private:
    bool stoped;
    int port;
    int max_thread_num;
    string ip_to_listen;
    string base_path;
    const string not_find = "/home/samuel/exp1/404.html";
    const string bad_request = "/home/samuel/exp1/400.html";
    const string index = "/home/samuel/exp1/index.html";
};


#endif //SOCKET_HTTP_SERVER_H
