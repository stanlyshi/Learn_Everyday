/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MessagePool.h
 * Author: U0151316
 *
 * Created on May 12, 2020, 3:34 PM
 */

#ifndef MESSAGING_MESSAGECACHE_H
#define MESSAGING_MESSAGECACHE_H

#include <stdlib.h>
#include <cstring>
#include <stdint.h>
#include <vector>
#include <mutex>
#include <gperftools/tcmalloc.h>

namespace Messaging {

    template<typename SEQTYPE = uint32_t>
    struct Entity {
        uint8_t* MessageValue = 0;
        SEQTYPE MessageSeq = 0;
        size_t MessageSize = 0;

        Entity() {
        }

        Entity(SEQTYPE seq) {
            MessageSeq = seq;
        }

        Entity(Entity<SEQTYPE>&& entity) {
            MessageSeq = entity.MessageSeq;
            MessageSize = entity.MessageSize;
            MessageValue = entity.MessageValue;
            entity.MessageSeq = 0;
            entity.MessageSize = 0;
            entity.MessageValue = 0;
        }

        Entity& operator=(Entity<SEQTYPE>&& entity) {
            MessageSeq = entity.MessageSeq;
            MessageSize = entity.MessageSize;
            MessageValue = entity.MessageValue;
            entity.MessageSeq = 0;
            entity.MessageSize = 0;
            entity.MessageValue = 0;
        }

        Entity(SEQTYPE seq, uint8_t* msg, size_t size) {
            assign(seq, msg, size);
        }

        void assign(SEQTYPE seq, uint8_t* msg, size_t size) {
            if (msg != 0 && size != 0) {
                MessageValue = (uint8_t*) tc_malloc(size);
                memcpy(MessageValue, msg, size);
                MessageSize = size;
                MessageSeq = seq;
            } else {
                MessageValue = msg;
                MessageSize = size;
                MessageSeq = seq;
            }
        }

        void move(Entity<SEQTYPE>&& entity) {
            release();
            MessageSeq = entity.MessageSeq;
            MessageSize = entity.MessageSize;
            MessageValue = entity.MessageValue;
            entity.MessageSeq = 0;
            entity.MessageSize = 0;
            entity.MessageValue = 0;
        }

        void release() {
            if (MessageValue != 0) {
                tc_free(MessageValue);
                MessageValue = 0;
                MessageSeq = 0;
                MessageSize = 0;
            }
        }

        ~Entity() {
            release();
        }
    };

    template <typename SEQTYPE = uint32_t>
    class MessageCache {
    public:
        typedef Entity<SEQTYPE> MessageEntity;
    private:
        MessageCache(MessageCache<SEQTYPE> const&) = delete;
        MessageCache<SEQTYPE>& operator=(MessageCache<SEQTYPE> const&) = delete;

        struct Cursor {
            size_t Index;

            Cursor(std::vector<MessageEntity>& cache, size_t maxMessageNumber) :
            m_cache(cache), m_maxMessageNumber(maxMessageNumber) {
                Index = 0;
            }

            uint32_t seq() const {
                return m_cache[Index].MessageSeq;
            }

            void moveToNext() {
                if (Index + 1 < m_maxMessageNumber) {
                    Index++;
                } else {
                    Index = 0;
                }
            }

            MessageEntity& getForward(SEQTYPE seq) const {
                size_t delta = seq - this->seq();
                return Index + delta < m_maxMessageNumber ? m_cache[Index + delta] : m_cache[Index + delta - m_maxMessageNumber];
            }

            void updateForward(MessageEntity& entity) const {
                MessageEntity& current = getForward(entity.MessageSeq);
                current.move(std::move(entity));
            }

            void updateAndMoveToNext(MessageEntity& entity) {
                moveToNext();
                MessageEntity& next = m_cache[Index];
                next.move(std::move(entity));
            }

        private:
            std::vector<MessageEntity>& m_cache;
            size_t m_maxMessageNumber;
        };

    public:

        MessageCache(size_t maxMessageNumber) :
        m_cache(),
        m_start(m_cache, maxMessageNumber),
        m_end(m_cache, maxMessageNumber) {
            m_maxMessageNumber = maxMessageNumber;
            reset();
        }

        void addMessage(SEQTYPE seq, uint8_t* msg, size_t size) {
            std::lock_guard<std::mutex> mtx(m_mutex);
            innerAddMessage(MessageEntity(seq, msg, size));
        }

        void addMessage(MessageEntity&& entity) {
            std::lock_guard<std::mutex> mtx(m_mutex);
            innerAddMessage(std::move(entity));
        }

        bool getMessage(SEQTYPE seq, MessageEntity& res) {
            std::lock_guard<std::mutex> mtx(m_mutex);
            if (seq < m_start.seq() || m_end.seq() < seq) {
                return false;
            }
            auto msg = m_start.getForward(seq);
            res.assign(msg.MessageSeq, msg.MessageValue, msg.MessageSize);
            return true;
        }

        void reset() {
            m_cache.clear();
            m_cache.resize(m_maxMessageNumber);
            m_end.Index = m_maxMessageNumber - 1;
            m_start.Index = -1;
        }
    private:

        void innerAddMessage(MessageEntity&& entity) {
            uint32_t endSeq = m_end.seq();
            if (endSeq + 1 == entity.MessageSeq || endSeq == 0) {
                // Normal, No gap
                m_end.updateAndMoveToNext(entity);
                if (m_end.Index == m_start.Index || m_start.Index == -1) {
                    m_start.moveToNext();
                }
            } else if (endSeq + 1 < entity.MessageSeq) {
                // Gap
                // Clear all missed message
                for (SEQTYPE i = endSeq + 1; i < entity.MessageSeq; i++) {
                    innerAddMessage(MessageEntity{i});
                }
                innerAddMessage(std::move(entity));
            } else if (endSeq > entity.MessageSeq) {
                // Recovered message
                if (m_start.seq() <= entity.MessageSeq) {
                    m_start.updateForward(entity);
                }
                // Do nothing
            } else {
                // Error
            }
        }

    private:
        std::vector<MessageEntity> m_cache;
        size_t m_maxMessageNumber;
        Cursor m_start;
        Cursor m_end;
        std::mutex m_mutex;
    };
}

#endif /* MESSAGING_MESSAGECACHE_H */

