//
// Created by samuel on 11/6/19.
//

#ifndef SOCKET_MY_THREAD_H
#define SOCKET_MY_THREAD_H
#include <thread>
#include <mutex>
using namespace std;
class my_thread {
public:
    bool finished;
    bool added;
    int flag;
    my_thread();
    void create_thread(int , string);
};


#endif //SOCKET_MY_THREAD_H
