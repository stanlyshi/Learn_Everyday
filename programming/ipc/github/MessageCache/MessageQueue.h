/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MessageQueue.h
 * Author: U0151316
 *
 * Created on May 15, 2020, 9:32 AM
 */

#ifndef MESSAGING_MESSAGEQUEUE_H
#define MESSAGING_MESSAGEQUEUE_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace Messaging {

    template <typename MSGTYPE>
    class MessageQueue {
    public:

    public:

        MessageQueue() {
        }

        void close() {
            m_open = false;
            m_cv.notify_all();
        }

        bool canBeClosed() {
            return !m_open && m_queue.empty();
        }

        void push(MSGTYPE&& msg) {
            std::lock_guard<std::mutex> lk(m_mutex);
            m_queue.push(std::forward<MSGTYPE>(msg));
            m_cv.notify_all();
        }

        bool waitAndPop(MSGTYPE& res) {
            std::unique_lock<std::mutex> lk(m_mutex);
            m_cv.wait(lk, [&] {
                return !m_queue.empty() || !m_open;
            });
            if (m_queue.empty()) {
                return false;
            }
            res = std::move(m_queue.front());
            m_queue.pop();
            return true;
        }
    private:
        std::mutex m_mutex;
        std::condition_variable m_cv;
        std::queue<MSGTYPE> m_queue;
        bool m_open = true;
    };
}

#endif /* MESSAGING_MESSAGEQUEUE_H */

