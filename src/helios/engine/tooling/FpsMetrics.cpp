module;

#include <deque>
#include <numeric>

module helios.engine.tooling.FpsMetrics;

import helios.engine.tooling.FrameStats;

namespace helios::engine::tooling {

    void FpsMetrics::update() noexcept {
        if (!needsUpdate_ || history_.empty()) {
            return;
        }

        const auto& stats = history_.back();

        lastWorkTime_ = stats.workTime * 1000.0f;
        lastWaitTime_ = stats.waitTime * 1000.0f;

        float sumMs = 0.0f;
        for (const auto& s : history_) {
            sumMs += s.totalFrameTime * 1000.0f;
        }

        avgFrameTime_ = sumMs / static_cast<float>(history_.size());
        avgFps_ = (avgFrameTime_ > 0.0001f) ? (1000.0f / avgFrameTime_) : 0.0f;

        needsUpdate_ = false;
    }

    void FpsMetrics::addFrame(const helios::engine::tooling::FrameStats& stats) {
        needsUpdate_ = true;

        history_.push_back(stats);
        if (history_.size() > historySize_) {
            history_.pop_front();
        }

        frameCount_++;
    }

    void FpsMetrics::setHistorySize(size_t size) {
        historySize_ = size;
        while (history_.size() > historySize_) {
            history_.pop_front();
        }
        needsUpdate_ = true;
    }

    size_t FpsMetrics::getHistorySize() const noexcept {
        return historySize_;
    }

    float FpsMetrics::getFps() noexcept {
        update();
        return avgFps_;
    }

    float FpsMetrics::getFrameTimeMs() noexcept {
        update();
        return avgFrameTime_;
    }

    float FpsMetrics::getWorkTimeMs() noexcept {
        update();
        return lastWorkTime_;
    }

    float FpsMetrics::getIdleTimeMs() noexcept {
        update();
        return lastWaitTime_;
    }

    unsigned long long FpsMetrics::getFrameCount() const noexcept {
        return frameCount_;
    }

    float FpsMetrics::getFrameTimeSeconds() noexcept {
        update();
        return avgFrameTime_ * 0.001f;
    }

    float FpsMetrics::getWorkTimeSeconds() noexcept {
        update();
        return lastWorkTime_ * 0.001f;
    }

    float FpsMetrics::getIdleTimeSeconds() noexcept {
        update();
        return lastWaitTime_ * 0.001f;
    }

    const std::deque<helios::engine::tooling::FrameStats>& FpsMetrics::getHistory() const noexcept {
        return history_;
    }

    void FpsMetrics::reset() {
        history_.clear();
        avgFps_ = 0.0f;
        avgFrameTime_ = 0.0f;
        lastWorkTime_ = 0.0f;
        lastWaitTime_ = 0.0f;
        frameCount_ = 0;
        needsUpdate_ = false;
    }

} // namespace helios::tooling

