/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: U0151316
 *
 * Created on May 12, 2020, 10:34 AM
 */

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <unistd.h>

#include "MessageCache.h"
#include "MessageQueue.h"

using namespace std;
using namespace std::chrono;
using namespace Messaging;

#define LOOP 2000000
#define BUFF 200000

/*
 * 
 */
int main(int argc, char** argv) {
    system_clock::duration d1 = system_clock::now().time_since_epoch();

    MessageQueue<Entity<>> queue;
    uint8_t buf[4096]{1};
    std::thread t1 = std::thread([&]() {
        int i = 1;
        for (; i < LOOP; i++) {
            Entity<> msg;
            memcpy(buf, &i, sizeof (int));
            msg.assign(i, buf, 1024);
            queue.push(std::move(msg));
        }
        queue.close();
        std::cout << "close " << i << std::endl;
    });

    MessageCache<> cache{BUFF};
    std::thread t2 = std::thread([&]() {
        std::cout << "start " << std::endl;
        uint32_t seq;
        while (1) {
            Entity<> msg{};
            bool res = queue.waitAndPop(msg);
            if (!res) {
                break;
            }
            seq = msg.MessageSeq;
            cache.addMessage(std::move(msg));
        }
        std::cout << "done " << seq << std::endl;
    });

    t2.join();
    t1.join();

    system_clock::duration d2 = system_clock::now().time_since_epoch();
    system_clock::duration d = d2 - d1;
    milliseconds mil = duration_cast<milliseconds>(d);
    cout << mil.count() << "ms" << endl;
    sleep(1);
    return 0;
}