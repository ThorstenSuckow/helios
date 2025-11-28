module;

#include <chrono>

export module helios.util.time.Stopwatch;


export namespace helios::util::time {

    class Stopwatch {

    private:
        std::chrono::time_point<std::chrono::steady_clock> start_;

        Stopwatch() = default;

    public:


        static Stopwatch& getInstance() noexcept;

        void start() noexcept;

        void stop(double& deltaTime) const noexcept;


    };


}
