# Runtime Stress Test

Interactive performance test for measuring the framerate impact of increasing
numbers of active `GameObject` instances.

## Purpose

Use the ImGui slider widget to dynamically adjust the number of active pooled
objects on the playing field. The built-in FPS widget shows real-time frame
timings so you can identify the point at which performance begins to degrade.

## Controls

| Key   | Action                      |
|-------|-----------------------------|
| `~`   | Toggle ImGui overlay        |
| `ESC` | Exit                        |

## ImGui Widgets

- **Runtime Stress Test** — Slider controlling the target object count
  (0 ... 10 000). Objects are acquired from / released back to a pool.
- **FPS Metrics** — Current FPS, frame time, and history graph.
- **Camera Control** — Adjust the camera to zoom in/out.
- **Log Console** — Engine log output.

## How It Works

1. A prefab (spinning purple rectangle, this time in pink!) is registered with a `GameObjectPoolManager` pool of 10 000 pre-allocated clones.
2. Every frame, the `RuntimeTestWidget::sync()` method acquires or releases
   objects to match the slider value.
3. Acquired objects become active, are randomly placed inside the arena, and move with random direction. Released objects are deactivated and returned to the pool.
4. The game loop processes transforms, collision, and rendering for all active objects, letting you observe the FPS impact.

