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
int main() {
    int server_fd, new_socket, valread;
    mutex m; // 输出日志锁
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
    address.sin_addr.s_addr = inet_addr(IP_TO_LISTEN);
    address.sin_port = htons(PORT);
    // 绑定端口等
    if (bind(server_fd, (struct sockaddr * ) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // 开始监听
    if (listen(server_fd, MAX_THREAD_NUM) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "* Running on http://" << IP_TO_LISTEN << ":" <<  PORT << "/ (Press CTRL+C to quit)" << endl;
    vector<my_thread *> threads_pool;
    vector<bool> threads_status;
    while(true) {
        // 阻塞监听
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
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
        cout << "目前有" << threads_pool.size() << "个连接" << endl;
        if (threads_pool.size() < MAX_THREAD_NUM) {
            // 如果还可以加入
            my_thread client;
            client.create_thread(new_socket, ip_in);
            threads_pool.push_back(&client);
            client.added = true;
        } else {
            // 如果已经满了，持续等待直到有空闲
            while(1) {
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

//        string ip_in = string(inet_ntoa(address.sin_addr));
////
//        valread = read(new_socket, buffer, 1024);
////        printf("%s\n", buffer);
//
//        Request request = Request(string(buffer));
//
//        Response response = Response(request);
//        string path = PATH + request.url;
//        Fs fs;
//        if (request.url == "/") {
//            path = INDEX;
//        }
//        char file_buffer[255];
//        if (!Fs::check_path(path)) {
//            // 如果检测不通过，访问了上级目录
//            response.set_status(400);
//            fs = Fs(BAD_REQUEST);
//        } else {
//            fs = Fs(path);
//            if (fs.ok) {
//                response.set_status(200);
//            } else {
//                fs = Fs(NOT_FIND);
//                response.set_status(404);
//            }
//        }
//
//
//        /*
//         * 输出日志
//         * */
//        print_log(response, request, ip_in);
//
//        response.add_header("Content-Type", fs.file_type);
//        write(new_socket, response.to_string().c_str(), response.to_string().size()); // 写入返回头
//        // 写入返回主体
//        while(fs.file.readsome(file_buffer, sizeof(file_buffer))) {
//            write(new_socket, file_buffer, fs.file.gcount());
//        }
//        close(new_socket);
    }
}


