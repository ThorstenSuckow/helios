# Issue Submission Guide

A well-written issue is the first step to improving **helios**. It helps us understand your needs, reproduce problems, and ultimately work more effectively. This guide outlines what we expect from different types of issues. Reports that do not meet these criteria may be closed if the necessary information is not provided upon request.

## General Criteria

- **Clear and Concise Title**: The title should briefly summarize the topic. Use the appropriate prefix (`fix:`, `feat:`, `refactor:`).
  - **Character Limit**: Issue titles with Conventional Commits prefixes must not exceed **72 characters** for readability and GitHub display compatibility.
  - **Recommended Length**: Aim for **≤50 characters** in the subject (same rule as commit messages) when possible; keep additional clarification for the body.
  - **Capitalization & Style**: The word after the prefix starts lowercase and uses **imperative mood** (e.g. `feat: add scissor rect support`, not `Adds` / `Added`).
  - **Breaking Changes Indicator**: Use `!` after the type (`feat!:` / `refactor!:`) if the change breaks public API or behavior.
  - **Examples of well-scoped titles (with char count)**:
    - `fix: window resize causes input events to map to incorrect coordinates` (71 chars) ✅
    - `feat: add viewport scissor rect support for UI rendering` (57 chars) ✅
    - `refactor: simplify Material initialization and memory management` (64 chars) ✅
  - **Anti-pattern (exceeds limit)**: `fix: prevent the race condition that occurs in scene graph updates when modifying transforms at runtime` (110 chars) ❌
- **One Issue per Topic**: Please open separate issues for each bug, feature, or refactoring proposal.

---

## Bug Reports

A bug report helps us find and fix problems. To ensure your report can be addressed efficiently, please make sure it includes the information requested by our "Bug Report" template.

### Key Information for Bugs

1. **Steps to Reproduce**: This is the most critical part. If we cannot reproduce the issue, we cannot fix it.
2. **Expected Behavior**: What should have happened?
3. **Environment**: Your OS, compiler, and the **helios** version.

#### Good Bug Report Example

> [!TIP]
> **✅ Well-Structured Bug Report (Accepted)**
> 
> **Title:** `fix: window resize causes input events to map to incorrect coordinates`
>
> **Describe the bug:**
> After resizing the game window, mouse input coordinates are no longer correctly mapped to the render viewport. Clicks appear offset by the delta amount.
>
> **Steps to Reproduce:**
> 1. Clean build: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release`
> 2. Run `simple_cube_rendering` example
> 3. Resize window by dragging edge to ~800x600 (from ~1280x720)
> 4. Move mouse to window center and click
> 5. Click registers in wrong screen location
>
> **Minimal Code Example:**
> ```cpp
> #include <helios/window/Window.ixx>
> #include <helios/input/InputHandler.ixx>
>
> auto window = helios::window::Window::create(...);
> window->resize(800, 600); // Trigger issue
> // Input now misaligned
> ```
>
> **Expected Behavior:**
> Mouse cursor should map 1:1 to the viewport; click at visual center should register at center.
>
> **Actual Behavior / Output:**
> Click registers ~100 pixels lower-right from visual position after resize.
>
> **Performance Impact:**
> No significant frame time impact; pure coordinate mapping issue.
>
> **Regression Check:**
> - [x] Worked on previous release (v0.1.0)
> - [ ] New in current main
> - [ ] Intermittent (flaky)
> - [ ] Unknown
>
> **Environment:**
> - OS: Windows 11 Build 23H2
> - GPU: NVIDIA GeForce RTX 3060 (Driver 551.23)
> - CPU: Intel i5-11400
> - RAM: 16GB
> - Compiler: MSVC 19.38
> - CMake: 3.29
> - Build Type: Release
> - helios Commit: abc123de
> - Custom CMake flags: none
>
> **Logs / Crash dumps:**
> ```
> [input] DEBUG: Viewport: 1280x720 -> 800x600
> [input] DEBUG: Mouse position: (400, 360) -> registered as (500, 460)
> ```
>
> **Workaround:**
> Restarting the application resets coordinate mapping.

#### Poor Bug Report Example

> [!WARNING]
> **❌ Insufficient Bug Report (Rejected)**
>
> **Title:** `fix: clicking doesn't work`
>
> **Description:** Mouse clicks are broken after I resize. Fix it.
>
> **Problems identified:**
> - No reproduction steps
> - No environment details
> - No expected vs. actual behavior
> - Vague title
> - No logs or traces
>
> **How to improve:** Follow the template above and provide clear, specific information.

---

## Feature Requests

Feature requests are how new ideas come to life. To help us understand your proposal, please provide the information requested in our "Feature Request" template.

### Key Information for Features

1. **Problem Description**: What problem does your feature solve?
2. **Proposed Solution**: How do you envision the feature working?
3. **Alternatives**: Have you considered other solutions?

#### Good Feature Request Example

> [!TIP]
> **✅ Well-Structured Feature Request (Accepted)**
>
> **Title:** `feat: add viewport scissor rect support to reduce overdraw in UI rendering`
>
> **Problem / Motivation:**
> Currently, rendering UI elements requires manual clipping in fragment shaders. This causes unnecessary fragment shader invocations for off-screen pixels, increasing GPU waste.
>
> **Proposed Solution (High-Level):**
> Expose scissor test configuration in the Renderer API to clip fragments at the GPU level.
>
> Example usage: `renderer.setScissor(x, y, width, height);`
>
> **Performance Considerations:**
> Scissor test is a fixed-function operation; expect 5–15% GPU time savings in UI-heavy scenes (measured on mobile GPUs; desktop impact typically 2–5%).
>
> **Optional API Sketch:**
> ```cpp
> class Renderer {
>   void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
>   void disableScissor();
> };
> ```
>
> **Affected Components:**
> - [x] rendering
> - [ ] scene
> - [ ] input
> - [ ] math
> - [ ] window
> - [x] util
> - [ ] ext.opengl
> - [ ] ext.glfw
> - [ ] build / cmake
> - [ ] docs
> - [ ] examples
>
> **Alternatives Considered:**
> - Custom clipping shaders – rejected (duplicates GPU work, harder to maintain)
> - Render-to-texture for each UI element – rejected (memory overhead)
>
> **Risks / Potential Downsides:**
> Requires careful management of scissor state; interaction with viewport transformations needs testing on all platforms.
>
> **Additional Context:**
> OpenGL spec: GL_SCISSOR_TEST; used in most game engines for UI rendering optimization.

#### Poor Feature Request Example

> [!WARNING]
> **❌ Insufficient Feature Request (Rejected)**
>
> **Title:** `feat: make rendering faster`
>
> **Description:** Please add GPU optimizations. I want better performance.
>
> **Problems identified:**
> - No specific problem stated
> - No measurable goal
> - Vague scope
> - No technical direction
> - Missing alternatives or trade-offs
>
> **How to improve:** Clearly describe what problem you're solving and why it matters. Include performance metrics and specific use cases.

---

## Refactoring Proposals

Proposing a refactoring helps us improve the quality of our codebase. Use the "Refactoring Proposal" template to provide the necessary context.

### Key Information for Refactorings

1. **Area of the Codebase**: Which part of the code is affected?
2. **Motivation**: Why is this refactoring necessary (e.g., performance, readability)?
3. **Proposed Changes**: A summary of the planned changes.

#### Good Refactoring Proposal Example

> [!TIP]
> **✅ Well-Structured Refactoring Proposal (Accepted)**
>
> **Title:** `refactor: consolidate event dispatch in EventBus to reduce allocation overhead`
>
> **Area:** `event/EventBus`
>
> **Motivation:**
> Per-frame event dispatch allocates a temporary `std::vector` for listeners (~50–80 allocations per frame). Profiler shows 2–3% CPU time spent in allocation/deallocation. Consolidating to a pre-allocated pool should reduce this.
>
> **Current Issues:**
> - Temporary `std::vector` created per event type per frame
> - No listener reuse between frames
> - Multiple heap fragmentations
>
> **Proposed Changes:**
> Replace dynamic listener lookup with a pre-reserved ring buffer. Collapse listener iteration into a cached bitfield to reduce branch mispredictions.
>
> **Performance Impact:**
> Expected reduction of allocations from ~70 to <2 per frame; estimated 1.5–2.5ms improvement on mid-tier hardware.
>
> **Risk Level:**
> medium (internal API only; public event subscription interface unchanged)
>
> **Test Impact:**
> Update existing EventBus tests; add benchmark for dispatch latency.
>
> **Migration Plan:**
> No public API changes. Existing event listeners continue to work without modification.

#### Poor Refactoring Proposal Example

> [!WARNING]
> **❌ Insufficient Refactoring Proposal (Rejected)**
>
> **Title:** `refactor: improve EventBus code`
>
> **Description:** The EventBus is too complicated. I will rewrite it to be cleaner.
>
> **Problems identified:**
> - No measurable goal
> - No scope definition
> - No identified pain points
> - No risk assessment
> - Missing performance or maintainability metrics
>
> **How to improve:** Identify specific code quality issues and propose concrete solutions with measurable benefits. Include before/after code examples and risk analysis.

---

## Best Practices

### For Bug Reports
- ✅ Provide a **minimal reproducible example**
- ✅ Include **exact error messages** and stack traces
- ✅ Specify your **environment** (OS, compiler, version)
- ✅ Search existing issues first to avoid duplicates
- ✅ Test on a **clean build** to rule out local issues
- ❌ Don't report multiple unrelated bugs in one issue
- ❌ Don't use vague titles like "crashes" or "broken"

### For Feature Requests
- ✅ Explain the **problem** you're trying to solve
- ✅ Provide **use cases** and benefits
- ✅ Consider **backward compatibility** and breaking changes
- ✅ Include **API design examples** where applicable
- ✅ Estimate **performance implications**
- ❌ Don't request features that violate helios design principles
- ❌ Don't propose features without understanding the problem

### For Refactoring Requests
- ✅ Clearly identify the **technical debt** or code smell
- ✅ Propose **concrete improvements** with measurable benefits
- ✅ Assess **impact and risks** (breaking changes, files affected)
- ✅ Include **before/after code examples**
- ✅ Prioritize **performance, readability, and maintainability**
- ❌ Don't propose refactoring without clear benefits
- ❌ Don't refactor just for style preferences

---

## Labels Reference

When creating or updating issues, use relevant labels to improve discoverability and organization.

### Type Labels
- `bug` – Something isn't working as expected
- `enhancement` – New feature or improvement request
- `refactoring` – Code improvement without functional changes
- `tech-debt` – Technical debt that should be addressed
- `documentation` – Documentation improvements

### Component Labels
- `rendering` – Rendering system (`helios.rendering`)
- `scene` – Scene management (`helios.scene`)
- `input` – Input system (`helios.input`)
- `math` – Math library (`helios.math`)
- `window` – Window management (`helios.window`)
- `opengl` – OpenGL backend (`helios.ext.opengl`)
- `glfw` – GLFW integration (`helios.ext.glfw`)
- `utilities` – Utility modules (`helios.util`)
- `build` – Build system (CMake)
- `examples` – Example projects

### Priority Labels
- `critical` – Crashes, security issues, data loss
- `high` – Major functionality broken, blocking workflows
- `medium` – Important but not blocking
- `low` – Nice to have, enhancement

### Status Labels
- `needs-triage` – Needs initial review by maintainers
- `needs-reproduction` – Cannot reproduce, needs more info
- `confirmed` – Bug confirmed or feature approved
- `in-progress` – Work in progress on this issue
- `blocked` – Blocked by other issues

---

## Conventional Commits

When creating pull requests related to issues, use conventional commit prefixes in your commit messages and PR titles:

### Commit Types
- `fix:` – Bug fixes
- `feat:` – New features
- `refactor:` – Code refactoring without functional changes
- `docs:` – Documentation changes
- `test:` – Test additions or fixes
- `chore:` – Build/tooling changes
- `perf:` – Performance improvements

### Breaking Changes

Add `!` after the type for breaking changes:

- `feat!:` – New feature with breaking changes
- `refactor!:` – Refactoring with breaking changes
- `fix!:` – Bug fix with breaking changes

### Examples

```
fix: prevent null-pointer crash in RenderQueue::add()
feat: add particle system support to rendering module
refactor!: redesign Material API for better extensibility
docs: add tutorial for scene graph usage
perf: optimize matrix multiplication with SIMD
test: add unit tests for EventBus listener removal
```

---

Thank you for helping us improve **helios**!
