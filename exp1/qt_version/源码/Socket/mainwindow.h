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
