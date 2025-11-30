/**
 * @file FramePacer.ixx
 * @brief Module defining the FramePacer class for frame rate control and timing synchronization.
 */
module;

#include <memory>
#include <thread>
#include <chrono>
#include <cassert>

export module helios.engine.FramePacer;

import helios.util.time.Stopwatch;
import helios.engine.FrameStats;

export namespace helios::engine {

    /**
     * @class FramePacer
     * @brief Controls and maintains a target frame rate through precise timing and pacing.
     *
     * The FramePacer class utilizes a `Stopwatch` to measure frame execution time and
     * introduces necessary sleep delays to maintain a consistent target frame rate.
     * It returns detailed timing statistics via the `FrameStats` structure upon synchronization.
     *
     * @details The pacing mechanism helps in achieving smoother frame delivery by minimizing
     * jitter, although strict adherence depends on the OS scheduler's precision.
     *
     * @note If `targetFps` is set to 0.0f, no frame pacing is performed, and frames
     * run as fast as the hardware and OS allow (unlocked framerate).
     *
     * @par Usage Example:
     * ```cpp
     * auto stopwatch = std::make_unique<helios::util::time::Stopwatch>();
     * FramePacer pacer(std::move(stopwatch));
     * pacer.setTargetFps(60.0f);
     *
     * while (running) {
     * pacer.beginFrame();
     * // ... game logic and rendering ...
     * FrameStats stats = pacer.sync();
     * }
     * ```
     */
    class FramePacer {
        /**
         * @brief The stopwatch used for high-resolution time measurement.
         */
        std::unique_ptr<helios::util::time::Stopwatch> stopwatch_;

        /**
         * @brief The target frame rate in Frames Per Second (FPS).
         */
        float targetFps_ = 0.0f;

    public:
        /**
         * @brief Constructs a FramePacer with the given stopwatch.
         *
         * Initializes a new FramePacer instance in unlimited FPS mode (targetFps = 0.0f).
         *
         * @param stopwatch Unique pointer to a valid `Stopwatch` instance. Ownership
         * is transferred to the FramePacer.
         */
        explicit FramePacer(std::unique_ptr<helios::util::time::Stopwatch> stopwatch) :
        stopwatch_(std::move(stopwatch)) {
            // Enforce the precondition: a valid stopwatch must be provided.
            assert(stopwatch_ && "FramePacer requires a valid Stopwatch (non-null)");
        }

        /**
         * @brief Sets the desired target frame rate.
         *
         * @param fps The target frame rate in Frames Per Second (FPS).
         * Set to 0.0f to disable pacing (unlocked framerate).
         */
        void setTargetFps(float fps) {
            targetFps_ = fps;
        }

        /**
         * @brief Retrieves the current target frame rate.
         *
         * Returns the frame rate target that has been set for pacing, expressed in
         * Frames Per Second (FPS). If the target frame rate is set to 0.0f, frame pacing
         * is disabled, and the frame rate is unlocked.
         *
         * @return The target frame rate in FPS.
         *
         * @note A return value of 0.0f indicates that the frame pacing mechanism is not active.
         */
        [[nodiscard]] float getTargetFps() const noexcept{
            return targetFps_;
        }

        /**
         * @brief Marks the beginning of a new frame.
         *
         * Starts the internal stopwatch to begin measuring the frame's work time.
         * This method must be called at the very beginning of each frame cycle,
         * before any game logic, physics, or rendering operations.
         */
        void beginFrame() {
            stopwatch_->start();
        }

        /**
         * @brief Synchronizes frame timing and returns frame statistics.
         *
         * Measures the elapsed work time since `beginFrame()`. If a target FPS is set
         * and the work time is less than the target frame duration, this method
         * sleeps the current thread to meet the target timing.
         *
         * @return A `FrameStats` structure containing the total frame time (including wait),
         * the actual work time (CPU processing), and the wait time (idle).
         *
         * @note If `targetFps` is 0.0f or the frame took longer than the target duration,
         * no sleeping occurs, and `waitTime` in the returned stats will be 0.0f.
         *
         * @todo Implement hybrid spinning for the last millisecond of the wait time
         * to improve timing precision and mitigate OS scheduler wake-up latency.
         */
        [[nodiscard]] FrameStats sync() {
            float workTime = stopwatch_->elapsedSeconds();

            float waitTime = 0.0f;
            float totalTime = workTime;

            if (targetFps_ > 0.0f) {
                float targetTime = 1.0f / targetFps_;
                if (targetTime > workTime) {
                    auto requestedWaitTime = targetTime - workTime;
                    auto sleepDuration = std::chrono::duration<float>(requestedWaitTime);
                    std::this_thread::sleep_for(sleepDuration);
                    // In a simple sleep model, we assume we slept for approximately the needed time.
                    // Real-world drift correction would re-measure the stopwatch here.
                    totalTime = stopwatch_->elapsedSeconds();
                    // This accounts for the overhead introduced by OS thread scheduling.
                    waitTime = totalTime-workTime;
                }
            }

            return FrameStats{ totalTime, workTime, waitTime };
        }
    };
}