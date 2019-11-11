#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <mutex>
#include <string>

#include "Request.h"
#include "utils.h"
#include "Fs.h"
#include "my_thread.h"
#include "http_server.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    void set_port(int port);
//    void set_max_thread_num(int num);
//    void set_ip_to_listen(string ip);
//    void set_base_path(string path);

//    int get_port();
//    int get_max_thread_num();
//    string get_ip_to_listen();
//    string get_base_path();

//    void run_server();
//    void stop_server();
//    void restart_server();
    http_server * server = nullptr;
    QThread workerThread;
    void start_wraper();
    void stop_wrapper();
    void restart_wrapper();
    void update_text();
private:
    Ui::MainWindow *ui;

//    bool stoped;
//    int port;
//    int max_thread_num;
//    string ip_to_listen;
//    string base_path;
//    const string not_find = "/home/samuel/exp1/404.html";
//    const string bad_request = "/home/samuel/exp1/400.html";
//    const string index = "/home/samuel/exp1/index.html";
};
#endif // MAINWINDOW_H
