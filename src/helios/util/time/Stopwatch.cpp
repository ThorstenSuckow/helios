module;

#include <chrono>

module helios.util.time.Stopwatch;


namespace helios::util::time {


    Stopwatch& Stopwatch::getInstance() noexcept {
        static auto stopwatch = Stopwatch();

        return stopwatch;
    }

    void Stopwatch::start() noexcept {
        start_ = std::chrono::steady_clock::now();
    }

    void Stopwatch::stop(double& deltaTime) const noexcept {
        std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
        const std::chrono::duration<double> delta = end - start_;

        deltaTime = delta.count();
    }



}