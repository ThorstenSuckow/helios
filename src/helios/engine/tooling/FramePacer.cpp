module;

#include <memory>
#include <thread>
#include <chrono>
#include <cassert>

module helios.engine.tooling.FramePacer;

import helios.util.time.Stopwatch;
import helios.engine.tooling.FrameStats;

namespace helios::engine::tooling {

    FramePacer::FramePacer(std::unique_ptr<helios::util::time::Stopwatch> stopwatch) :
        stopwatch_(std::move(stopwatch)) {
        assert(stopwatch_ && "FramePacer requires a valid Stopwatch (non-null)");
    }

    void FramePacer::setTargetFps(float fps) {
        targetFps_ = fps;
    }

    float FramePacer::getTargetFps() const noexcept {
        return targetFps_;
    }

    void FramePacer::beginFrame() {
        stopwatch_->start();
    }

    FrameStats FramePacer::sync() {
        float workTime = stopwatch_->elapsedSeconds();

        float waitTime = 0.0f;
        float totalTime = workTime;

        if (targetFps_ > 0.0f) {
            float targetTime = 1.0f / targetFps_;
            if (targetTime > workTime) {
                auto requestedWaitTime = targetTime - workTime;
                auto sleepDuration = std::chrono::duration<float>(requestedWaitTime);
                std::this_thread::sleep_for(sleepDuration);
                totalTime = stopwatch_->elapsedSeconds();
                waitTime = totalTime - workTime;
            }
        }

        return FrameStats{ totalTime, workTime, waitTime };
    }

} // namespace helios::engine

