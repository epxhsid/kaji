#pragma once

namespace kaji {

struct TransportState {
    bool playing = false;
    bool recording = false;
    bool looping = false;
    bool metronome = false;

    double position_seconds = 0.0;
    double tempo_bpm = 120.0;

    void play() noexcept {
        playing = true;
    }

    void stop() noexcept {
        playing = false;
        position_seconds = 0.0;
    }

    void pause() noexcept {
        playing = false;
    }

    void rewind(double seconds = 0.0) noexcept {
        position_seconds = seconds < 0.0 ? 0.0 : seconds;
    }

    void toggle_recording() noexcept {
        recording = !recording;
    }

    void toggle_loop() noexcept {
        looping = !looping;
    }

    void toggle_metronome() noexcept {
        metronome = !metronome;
    }
};

}
