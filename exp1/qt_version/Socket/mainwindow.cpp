#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include "http_server.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    this->port = 8080;
//        this->ip_to_listen = "0.0.0.0";
//        this->max_thread_num = 30;
//        this->base_path = "/home/samuel/exp1";
//        this->stoped = true;
    this->server = new http_server();

    update_text();

    // 输出
    QTextBrowser * output = this->findChild<QTextBrowser *>(QString("output"));
//    output->setText("噜啦啦");
    server->output = output;
    QPushButton * run_button = this->findChild<QPushButton *>(QString("run"));
    QPushButton * stop_button = this->findChild<QPushButton *>(QString("stop"));
    QPushButton * restart_button = this->findChild<QPushButton *>(QString("reset"));
    connect(run_button, &QPushButton::clicked, this, &MainWindow::start_wraper);
    connect(stop_button, &QPushButton::clicked, this, &MainWindow::stop_wrapper);
    connect(restart_button, &QPushButton::clicked, this, &MainWindow::restart_wrapper);
    stop_button->setEnabled(false);
    restart_button->setEnabled(false);
}

void MainWindow::start_wraper() {
//    this->findChild<QTextBrowser *>(QString("output"))->setText("刚刚点击");
    QPushButton * run_button = this->findChild<QPushButton *>(QString("run"));
    QPushButton * stop_button = this->findChild<QPushButton *>(QString("stop"));
    QPushButton * restart_button = this->findChild<QPushButton *>(QString("reset"));
    restart_button->setEnabled(true);
    run_button->setEnabled(false);
    stop_button->setEnabled(true);


    QTextBrowser * output = this->findChild<QTextBrowser *>(QString("output"));
    server->moveToThread(&workerThread);
    server->start();
//    workerThread.start();
    connect(server, SIGNAL(print_message(QString)), output, SLOT(setText(QString)));
}

void MainWindow::stop_wrapper() {
//    server->stop();
//    server->quit();
//    server = new http_server;
//    server->quit();
//    workerThread.quit();
    QPushButton * run_button = this->findChild<QPushButton *>(QString("run"));
    QPushButton * stop_button = this->findChild<QPushButton *>(QString("stop"));
    run_button->setEnabled(true);
    stop_button->setEnabled(false);
    server->stop();
    server->quit();
//    workerThread.quit();
}

void MainWindow::restart_wrapper() {
    string ip = string(this->findChild<QTextEdit *>(QString("ip"))->toPlainText().toUtf8().constData());
    string port = string(this->findChild<QTextEdit *>(QString("port"))->toPlainText().toUtf8().constData());
    string path = string(this->findChild<QTextEdit *>(QString("path"))->toPlainText().toUtf8().constData());
    if (!check_ip(ip)) {
        this->server->print("New Ip is illigal\n");
        return;
    }
    if (!check_path(path)) {
        this->server->print("New base path is illigal\n");
        return;
    }
    if (!check_port(port)) {
        this->server->print("New port is illigal\n");
        return;
    }
    server->set_all(ip, path, port);
    stop_wrapper();
//    start_wraper();

    update_text();
    start_wraper();
}
MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::update_text() {
    QTextEdit * ip = this->findChild<QTextEdit *>(QString("ip"));
    QTextEdit * port = this->findChild<QTextEdit *>(QString("port"));
    QTextEdit * path = this->findChild<QTextEdit *>(QString("path"));

    ip->setText(QString::fromStdString(server->get_ip_to_listen()));
    port->setText(QString::fromStdString(to_string(server->get_port())));
    path->setText(QString::fromStdString(server->get_base_path()));
}

//string MainWindow::get_base_path() {
//    return this->base_path;
//}

//string MainWindow::get_ip_to_listen() {
//    return this->ip_to_listen;
//}

//int MainWindow::get_max_thread_num() {
//    return this->max_thread_num;
//}

//int MainWindow::get_port() {
//    return this->port;
//}

//void MainWindow::set_base_path(string path) {
//    if (this->base_path != path) {
//        this->base_path = path;
//        restart_server();
//    }
//}

//void MainWindow::set_ip_to_listen(string ip) {
//    if (this->ip_to_listen != ip) {
//        this->ip_to_listen = ip;
//        restart_server();
//    }
//}

//void MainWindow::set_max_thread_num(int num) {
//    if (this->max_thread_num != num) {
//        this->max_thread_num = num;
//        restart_server();
//    }
//}

//void MainWindow::set_port(int port) {
//    if (port != this->port) {
//        this->port = port;
//        restart_server();
//    }
//}

//void MainWindow::run_server() {
//    this->stoped = false;
//        cout << "Server Started" << endl;
//        int server_fd, new_socket;
//        struct sockaddr_in address;
//        int opt = 1;
//        int addrlen = sizeof(address);
//        char buffer[1024] = {0};
//        // 创建新的socket
//        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//            perror("socket failed");
//            exit(EXIT_FAILURE);
//        }
//        // 配置socket参数
//        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,  & opt, sizeof(opt))) {
//            perror("setsocketopt");
//            exit(EXIT_FAILURE);
//        }

//        address.sin_family = AF_INET;
//        address.sin_addr.s_addr = inet_addr(this->ip_to_listen.c_str());
//        address.sin_port = htons(this->port);
//        // 绑定端口等
//        if (bind(server_fd, (struct sockaddr * ) &address, sizeof(address)) < 0) {
//            perror("bind failed");
//            exit(EXIT_FAILURE);
//        }
//        // 开始监听
//        if (listen(server_fd, this->max_thread_num) < 0) {
//            perror("listen");
//            exit(EXIT_FAILURE);
//        }

//        cout << "* Running on http://" << this->ip_to_listen << ":" <<  this->port << "/ (Press CTRL+C to quit)" << endl;
//        vector<my_thread *> threads_pool;
//        vector<bool> threads_status;
//        while(true) {
//            // 阻塞监听
//            if (this->stoped) {
//                cout << "* Server Stoped" << endl;
//                return;
//            }
//            if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t * ) & addrlen)) < 0) {
//                perror("accept");
//                exit(EXIT_FAILURE);
//            }

//            string ip_in = string(inet_ntoa(address.sin_addr));

//            for (int i = 0; i < threads_pool.size(); i++) {
//                if (threads_pool.at(i)->finished) {
//                    threads_pool.erase(threads_pool.begin() + i);
//                    i--;
//                }
//            }
//    //        cout << "目前有" << threads_pool.size() << "个连接" << endl;
//            if (threads_pool.size() < this->max_thread_num) {
//                // 如果还可以加入
//                my_thread client;
//                client.create_thread(new_socket, ip_in);
//                threads_pool.push_back(&client);
//                client.added = true;
//            } else {
//                // 如果已经满了，持续等待直到有空闲
//                while (1) {
//                    if (this->stoped) {
//                        return;
//                    }
//                    bool flag = false;
//                    for (int i = 0; i < threads_pool.size(); i++) {
//                        if (threads_pool.at(i)->finished) {
//                            threads_pool.erase(threads_pool.begin() + i);
//                            i--;
//                            flag = true;
//                        }
//                    }
//                    if (flag) {
//                        break;
//                    }
//                }
//            }
//        }
//}

//void MainWindow::stop_server() {
//    this->stoped = true;
//}

//void MainWindow::restart_server() {
//    cout << "* Server Restarting" << endl;
//    this->stoped = true;
//    sleep(1);
//    this->run_server();
//}
