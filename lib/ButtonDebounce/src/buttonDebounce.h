/*
 * Author: Michael Garcia, M&E Design
 * License: MIT
 * Date: 2026-04-17
 * Contact: michael@mandedesign.studio
 * Website: https://mandedesign.studio
 */

#pragma once

#include "ButtonDebounceVersion.h"

#include <stdbool.h>
#include <stdint.h>

class ButtonDebounce {
public:
    struct Config {
        // These defaults are tuned for a small polling interval such as 5 ms.
        Config()
            : integ_max(6),
              integ_on(4),
              integ_off(2),
              consec_n(3),
              edge_threshold(4),
              unstable_timeout(16),
              bounce_confirm(1)
        {
        }

        uint8_t integ_max;
        uint8_t integ_on;
        uint8_t integ_off;
        uint8_t consec_n;
        uint8_t edge_threshold;
        uint8_t unstable_timeout;
        uint8_t bounce_confirm;
    };

    explicit ButtonDebounce(const Config& cfg = Config{});

    /**
     * Advances the debounce engine by one sample.
     * `raw_down` means "the button is electrically down right now".
     */
    void update(bool raw_down);

    void updateActiveLow(bool pin_level_high)
    {
        update(!pin_level_high);
    }

    void updateActiveHigh(bool pin_level_high)
    {
        update(pin_level_high);
    }

    bool pressed() const
    {
        return pressed_;
    }

    bool released() const
    {
        return released_;
    }

    bool down() const
    {
        return state_;
    }

    bool up() const
    {
        return !state_;
    }

    uint8_t history() const;
    void reset(bool start_down = false);

private:
    Config cfg_;
    bool state_ = false;
    bool pressed_ = false;
    bool released_ = false;

    struct IntegratorState {
        uint8_t acc = 0;
    };

    struct HistoryState {
        uint8_t hist = 0;
        uint8_t unstable = 0;
        uint8_t bounce_k = 0;
    };

    union EngineState {
        IntegratorState integrator;
        HistoryState history;

        EngineState() : integrator{} {}
    } eng_;

protected:
    // Small bitwise helper used by the optional history-based engines.
    static inline uint8_t popcount8(uint8_t x)
    {
        x = (uint8_t)((x & 0x55u) + ((x >> 1) & 0x55u));
        x = (uint8_t)((x & 0x33u) + ((x >> 2) & 0x33u));
        return (uint8_t)((x + (x >> 4)) & 0x0Fu);
    }

    static inline uint8_t edgeCount8(uint8_t hist)
    {
        const uint8_t transitions = (uint8_t)(hist ^ (hist >> 1));
        return popcount8(transitions);
    }
};
