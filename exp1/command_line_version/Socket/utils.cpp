//
// Created by samuel on 11/6/19.
//
#include <iostream>
#include <unistd.h>

#include "utils.h"
#include "Request.h"
#include "Response.h"
#include "Fs.h"
#include "my_thread.h"

using namespace std;
void print_log(Response response, Request request, string ip) {
    auto current = chrono::system_clock::now();
    auto current_time = chrono::system_clock::to_time_t(current);
    auto out_put_time = string(ctime(&current_time));
    out_put_time = out_put_time.substr(0, out_put_time.size() - 1); // 去掉换行符
    cout << ip << " - - "
         << " [" << out_put_time << "] \" "
         << request.type << " "
         << request.url << " "
         << request.http_version << "\""
         << response.status << " - " << endl;
}

/*
 * 接收客户端的数据
 * */
void client_thread(int new_socket, string ip_in, my_thread * t) {
    char buffer[1024] = {0};
    int valread = read(new_socket, buffer, 1024);

    Request request = Request(string(buffer));
    Response response = Response(request);
    string path = PATH + request.url;
    Fs fs;
    if (request.url == "/") {
        path = INDEX;
    }
    char file_buffer[255];
    if (!Fs::check_path(path)) {
        // 如果检测不通过，访问了上级目录
        response.set_status(400);
        fs = Fs(BAD_REQUEST);
    } else {
        fs = Fs(path);
        if (fs.ok) {
            response.set_status(200);
        } else {
            fs = Fs(NOT_FIND);
            response.set_status(404);
        }
    }
//    m.lock();
    print_log(response, request, ip_in);
//    m.unlock();
    response.add_header("Content-Type", fs.file_type);
    write(new_socket, response.to_string().c_str(), response.to_string().size()); // 写入返回头
    // 写入返回主体
    while(fs.file.readsome(file_buffer, sizeof(file_buffer))) {
        write(new_socket, file_buffer, fs.file.gcount());
    }
    close(new_socket);
     t->finished = true;
}
