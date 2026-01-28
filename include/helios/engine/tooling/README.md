# helios::engine::tooling

Utilities for runtime diagnostics and developer tooling such as FPS and frame timing metrics.

## Key Classes

| Class | Purpose |
|-------|---------|
| `FramePacer` | Controls frame rate through timing and pacing (vsync-independent) |
| `FpsMetrics` | Aggregates frame timing data over a rolling window for analysis |
| `FrameStats` | POD struct containing per-frame timing (work, wait, total) |

## Usage

```cpp
auto stopwatch = std::make_unique<helios::util::time::Stopwatch>();
helios::engine::tooling::FramePacer pacer(std::move(stopwatch));
pacer.setTargetFps(60.0f);

helios::engine::tooling::FpsMetrics metrics;
metrics.setHistorySize(120);

while (running) {
    pacer.beginFrame();
    // ... game logic and rendering ...
    auto stats = pacer.sync();
    metrics.addFrame(stats);
    
    float fps = metrics.getFps();
    float avgFrameTime = metrics.getFrameTimeMs();
}
```

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::tooling
@brief Tooling utilities for diagnostics, metrics and developer overlays.
@details This namespace contains helper classes for runtime diagnostics (e.g. FPS meters, frame time graphs) intended for debug builds and developer tools.
</p></details>


