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
| Rendering     | Alpha  | Low                | RenderPass & material pipeline evolving |
| Scene Graph   | Alpha  | Medium             | Transforms & hierarchy stable, culling pending |
| Input         | Alpha  | Medium             | Keyboard/mouse OK, gamepad refinements pending |
| Math          | Alpha  | Low                | Missing quaternions & SIMD acceleration |
| Windowing     | Alpha  | Medium             | GLFW integration functional |
| Logging       | Alpha  | Medium             | Scoped logger integrated |
| Utilities     | Alpha  | Medium             | GUID + file I/O stable |

## Milestones & Progress

| Milestone | State | Due | Progress | Description |
|-----------|-------|-----|----------|-------------|
| milestone_1 | Closed | 2025-10-20 | 3 / 3 (100%) | Application layer: event system, input, low level API interfaces |
| milestone_2 | In Progress | 2025-11-17 | 3 / 6 (50%) | Functional rendering engine; draft main screen (player view) |
| milestone_3 | Planned | 2025-12-22 | 0 / 1 (0%) | Main physics & player controls; shooting mechanisms |
| milestone_5 | Planned | 2026-02-02 | 0 / 1 (0%) | Playable prototype; tuned mechanics, controls & graphics |
| milestone_6 | Planned | 2026-03-16 | 0 / 5 (0%) | Project presentation & ~20 page paper |

### Interpretation
- Progress column lists closed issues vs total (open + closed) at time of snapshot.
- Milestones beyond the current date are marked Planned; active milestone shows "In Progress".
- Closed milestone_1 forms the historical baseline for the Alpha phase.

### Current Focus (milestone_2)
Remaining goals before completion:
1. Rendering pipeline consolidation
2. Material / shader lifecycle cleanup
3. Initial performance profiling
4. (Issues still open) Refactor Renderable to use RenderPrototype
5. AABB support for Mesh / SceneNode
6. Shader-side matrix computation (model/view/world)

### Upcoming Sequence
1. milestone_3: Math extensions (quaternions, SIMD), camera polish & frustum culling, basic asset pipeline
2. milestone_5: Integrate game mechanics into a playable prototype
3. milestone_6: Formal documentation & presentation (paper, website refinement)

## Beta Criteria (Exit Alpha)
- Public API stability policy documented
- Core modules (rendering, input, scene) have stable interfaces
- Math feature completeness (quaternions, transforms, basic collision primitives)
- Continuous integration green across platforms

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

_Last updated: 2025-11-14 • Source phase: alpha • Track changes in [CHANGELOG](/docs/changelog)_
