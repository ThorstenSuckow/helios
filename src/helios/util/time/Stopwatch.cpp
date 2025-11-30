module;

#include <chrono>

module helios.util.time.Stopwatch;


namespace helios::util::time {

    void Stopwatch::start() noexcept {
        start_ = std::chrono::steady_clock::now();
    }

    float Stopwatch::elapsedMs() const noexcept {
        auto end = std::chrono::steady_clock::now();
        // Wichtig: Expliziter Cast zu float Millisekunden
        std::chrono::duration<float, std::milli> duration = end - start_;
        return duration.count();
    }

    float Stopwatch::elapsedSeconds() const noexcept {
        auto end = std::chrono::steady_clock::now();
        std::chrono::duration<float> duration = end - start_;
        return duration.count();
    }


}