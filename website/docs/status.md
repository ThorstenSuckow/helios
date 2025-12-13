---
sidebar_position: 2
---

# Project Status: Alpha

> **Current Phase:** Alpha – The core architecture is under active development. APIs may change without deprecation warnings.

## What Alpha Means

During the alpha phase we focus on:
- Establishing fundamental subsystems (rendering, scene graph, input, math)
- Iterating quickly on API surface without stability guarantees
- Identifying performance bottlenecks early
- Building example coverage + test foundation

Not guaranteed yet:
- API backward compatibility
- Performance optimization
- Full platform parity
- Complete math feature set (quaternions, SIMD still missing)

## Stability Overview

| Module        | Phase  | Relative Stability | Notes |
|---------------|--------|--------------------|-------|
| Rendering     | Alpha  | Medium             | Camera system refactored, RenderPass evolving |
| Scene Graph   | Alpha  | Medium             | CameraSceneNode integration, transforms stable |
| Input         | Alpha  | Medium             | Gamepad supported; Keyboard minimal (ESC only); Mouse pending |
| Math          | Alpha  | Low                | Missing quaternions & SIMD acceleration |
| Windowing     | Alpha  | High               | GLFW integration stable |
| Logging       | Alpha  | High               | Scoped logger & ImGui sinks stable |
| ImGui         | Alpha  | Medium             | Backend & Widgets (Log, Camera, Gamepad) added |
| Utilities     | Alpha  | High               | FramePacer, Stopwatch, Units system added |

## Milestones & Progress

| Milestone | State | Due | Progress | Description |
|-----------|-------|-----|----------|-------------|
| milestone_1 | Closed | 2025-10-20 | 100% | Application layer: event system, input, low level API interfaces |
| milestone_2 | Closed | 2025-12-12 | 100% | Functional rendering engine; Camera system refactor; ImGui tooling |
| milestone_3 | In Progress | 2025-12-22 | 10% | Main physics & player controls; shooting mechanisms |
| milestone_5 | Planned | 2026-02-02 | 0% | Playable prototype; tuned mechanics, controls & graphics |
| milestone_6 | Planned | 2026-03-16 | 0% | Project presentation & ~20 page paper |

### Interpretation
- Progress column lists closed issues vs total (open + closed) at time of snapshot.
- Milestones beyond the current date are marked Planned; active milestone shows "In Progress".
- Closed milestone_1 forms the historical baseline for the Alpha phase.

### Current Focus (milestone_3)
Goals for the next phase:
1. **Physics Integration:** Basic collision detection and response.
2. **Player Controls:** Refine spaceship movement (momentum, rotation).
3. **Game Mechanics:** Shooting, health, and scoring systems.
4. **Math Extensions:** Quaternions for robust rotation handling.
5. **Asset Pipeline:** Basic model loading improvements.

### Upcoming Sequence
1. milestone_5: Integrate game mechanics into a playable prototype
2. milestone_6: Formal documentation & presentation (paper, website refinement)

## Beta Criteria (Exit Alpha)
- Public API stability policy documented
- Core modules (rendering, input, scene) have stable interfaces
- Math feature completeness (quaternions, transforms, basic collision primitives)
- Continuous integration green across platforms

## Repository Stats

| Metric | Value |
|--------|-------|
| **Stars** | 2 |
| **Forks** | 1 |
| **Open Issues** | 26 |
| **Last Commit** | 2025-12-10 |

## Detailed Change History

For a complete list of all changes, breaking changes, and migration guides, see:

👉 **[View Full CHANGELOG](/docs/changelog)**

The changelog is automatically synchronized from the repository root and follows the [Keep a Changelog](https://keepachangelog.com/) format.

## Guidance for Users

If you depend on helios during alpha:
- Pin to a specific commit hash in your build scripts
- Expect breaking changes (see [CHANGELOG](/docs/changelog) for migration guides)
- Open issues for unclear APIs or missing extension points

## Contributing in Alpha

- Prefer small, well-scoped PRs
- Mark breaking proposals with `refactor!:` or `feat!:` in commits
- Add CHANGELOG entries under `[Unreleased]`
- Include minimal tests even for experimental code

## Feedback & Discussion

- Use GitHub Discussions for architectural questions
- Label issues with `feedback` for API clarity concerns
- Performance notes welcome (profiling traces, flamegraphs)

## Next Steps

- Introduce experimental stability annotations in docs (EXPERIMENTAL badge)
- Add benchmark suite under `benchmarks/`
- Expand math module (quaternions first)

---

_Last updated: 2025-12-13 • Source phase: alpha • Track changes in [CHANGELOG](/docs/changelog)_
