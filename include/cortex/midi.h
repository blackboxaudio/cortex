#pragma once

#include <bbx_ffi.h>
#include <cstdint>

namespace cortex {

inline BbxMidiStatus getMidiStatus(uint8_t statusByte) noexcept
{
    switch (statusByte & 0xF0) {
    case 0x80:
        return BBX_MIDI_STATUS_NOTE_OFF;
    case 0x90:
        return BBX_MIDI_STATUS_NOTE_ON;
    case 0xA0:
        return BBX_MIDI_STATUS_POLYPHONIC_AFTERTOUCH;
    case 0xB0:
        return BBX_MIDI_STATUS_CONTROL_CHANGE;
    case 0xC0:
        return BBX_MIDI_STATUS_PROGRAM_CHANGE;
    case 0xD0:
        return BBX_MIDI_STATUS_CHANNEL_AFTERTOUCH;
    case 0xE0:
        return BBX_MIDI_STATUS_PITCH_WHEEL;
    default:
        return BBX_MIDI_STATUS_UNKNOWN;
    }
}

} // namespace cortex
