/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#include "event_queue.h"

EventQueue::EventQueue() : head_(0), tail_(0), count_(0)
{
}

bool EventQueue::push(const SystemEvent& event)
{
    if (isFull()) {
        return false;
    }

    // The tail always points to the next free slot. Wrapping with modulo turns
    // the fixed array into a circular buffer.
    buffer_[tail_] = event;
    tail_ = (tail_ + 1u) % config::kEventQueueCapacity;
    count_++;
    return true;
}

bool EventQueue::pop(SystemEvent& eventOut)
{
    if (isEmpty()) {
        return false;
    }

    // The head always points to the oldest queued event, which preserves FIFO
    // ordering even after the indices wrap back to the start of the array.
    eventOut = buffer_[head_];
    head_ = (head_ + 1u) % config::kEventQueueCapacity;
    count_--;
    return true;
}

void EventQueue::clear()
{
    head_ = 0;
    tail_ = 0;
    count_ = 0;
}

bool EventQueue::isEmpty() const
{
    return count_ == 0;
}

bool EventQueue::isFull() const
{
    return count_ >= config::kEventQueueCapacity;
}

size_t EventQueue::size() const
{
    return count_;
}
