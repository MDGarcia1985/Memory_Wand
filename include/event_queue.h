/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "btn_event.h"
#include "config.h"

/**
 * Top-level event families routed through the central queue.
 * Additional event sources can extend this enum without changing queue logic.
 */
enum class SystemEventType : uint8_t {
    Button = 0,
};

/**
 * FIFO payload used throughout the firmware event pipeline.
 * Right now the queue carries only button events, but it is shaped as a system
 * event so the architecture can grow to include timers, audio requests, or
 * communication events later.
 */
struct SystemEvent {
    SystemEventType type;
    ButtonEvent button;

    static SystemEvent fromButtonEvent(const ButtonEvent& buttonEvent)
    {
        return {SystemEventType::Button, buttonEvent};
    }
};

/**
 * Fixed-capacity ring buffer for deterministic embedded event processing.
 *
 * A ring buffer is used here instead of a dynamic container so the queue has a
 * known maximum memory cost and predictable push/pop behavior on every loop.
 */
class EventQueue {
public:
    EventQueue();

    /**
     * Appends one event to the tail of the queue.
     * Returns false if the queue is already full.
     */
    bool push(const SystemEvent& event);

    /**
     * Removes the oldest queued event and writes it into `eventOut`.
     * Returns false when no event is available.
     */
    bool pop(SystemEvent& eventOut);

    void clear();
    bool isEmpty() const;
    bool isFull() const;
    size_t size() const;

private:
    SystemEvent buffer_[config::kEventQueueCapacity];
    size_t head_;
    size_t tail_;
    size_t count_;
};
