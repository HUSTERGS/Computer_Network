//
// Created by samuel on 11/6/19.
//

#ifndef SOCKET_HTTP_SERVER_H
#define SOCKET_HTTP_SERVER_H

#include <string>
#include <QThread>
#include <QTextBrowser>
using namespace std;
class http_server : public QThread{
    Q_OBJECT
public:
//    http_server();
//    http_server(QObject * parent = nullptr);
//    http_server(string ip, int port, int max_thread, string path);
    http_server(QObject * parent = 0);
//    ~http_server();
    void set_port(int port);
    void set_max_thread_num(int num);
    void set_ip_to_listen(string ip);
    void set_base_path(string path);
    void set_all(string, string, string);
    void print(string s);
    int get_port();
    int get_max_thread_num();
    string get_ip_to_listen();
    string get_base_path();

    QTextBrowser * output;

    void run();
    void stop();
    void restart();
signals:
    void print_message(QString);

private:
    bool stoped;
    int port;
    int max_thread_num;
    string ip_to_listen;
    string base_path;
    const string not_find = "/home/samuel/exp1/404.html";
    const string bad_request = "/home/samuel/exp1/400.html";
    const string index = "/home/samuel/exp1/index.html";
    int server_fd;
};


#endif //SOCKET_HTTP_SERVER_H
