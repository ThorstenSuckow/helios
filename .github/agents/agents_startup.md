# agents_startup.md

## Purpose
This file describes the agent's recommended startup behaviour when the repository is opened, cloned, or when an automated agent (IDE assistant / CI helper) is first invoked. The goal is to perform safe, fast, and reproducible repository checks without performing disruptive actions or leaking secrets.

## When to run
- On repository open (IDE), after clone, or in CI job initialization.
- Agent should run the minimal sequence below automatically; any heavy or networked step must be gated by configuration or explicit user consent.

## High-level rules
- Prefer read-only and fast checks on startup. Avoid long builds or heavy tests unless explicitly requested.
- Do not publish files, logs, or environment variables containing secrets.
- Respect repository-local config flags (e.g., `agents_disabled`, `agents_allow_network`) if present.

## Startup sequence (recommended)
1. Basic repo sanity (read-only)
   - Verify required top-level files exist (CMakeLists.txt, include/, src/, tests/).
   - Report uncommitted changes if present (do not modify them).
2. Environment capability probe (read-only)
   - Detect available toolchain (CMake, compiler, Python, clang-format).
   - If a required tool is missing, report recommended install steps and stop.
3. Dependency cache check (safe)
   - If `external/` contains pre-fetched dependencies (glfw/glad), prefer using them.
   - If no cache and network access is disallowed, log that FetchContent will fail without network.
   - Do not automatically fetch remote content unless allowed by config.
4. CMake configure (safe, short)
   - Run a configure-only step (no full build) to validate generator and module settings.
   - Abort if configure fails and report errors.
5. Quick static checks (fast, read-only)
   - Run format checks (`clang-format --dry-run` or equivalent), static analyzers that are fast and local.
   - Run unit-test discovery (do not run heavy/interactive tests).
6. Optional short build / smoke test (guarded)
   - Only run if agent is configured to allow builds on startup and within resource/time limits. Prefer building a single tiny example or a "smoke" target.
7. Report results
   - Write a short machine-readable startup report: `agents_startup_report.json` with status, timestamp, errors, and required next actions.
   - Summarize the human-readable result in the IDE/console.

## Safety / limits
- Default: no network access. If network allowed, only for fetching pinned dependencies and only after user consent.
- Time limit: any auto-start operation should enforce a short timeout (e.g., 60–300 seconds depending on action).
- CPU/RAM: avoid running heavy builds by default in IDE startup.

## Configuration (suggested keys)
- `agents_disabled` (bool) — completely disable agent startup.
- `agents_allow_network` (bool) — allow FetchContent/network operations.
- `agents_quick_build` (enum: none|smoke|full) — default `none`.
- `agents_timeout_seconds` (int) — per-step timeout.
- `agents_report_path` (string) — path for the startup report (default: `.agents/startup_report.json`).

## Example config (JSON)
```
{
  "agents_disabled": false,
  "agents_allow_network": false,
  "agents_quick_build": "smoke",
  "agents_timeout_seconds": 120,
  "agents_report_path": ".agents/startup_report.json"
}
```

## Minimal Windows cmd.exe startup commands (examples)
- Configure only (safe):
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```
- Configure + quick build of a smoke target (only if allowed):
```
cmake --build build --config Debug --target helios -- /m:1
```
- Run fast tests only (if configured to run tests on startup):
```
cd build
ctest -C Debug -R quick -I 1,20 --output-on-failure
```
(assumes there are tests tagged/named `quick`; adjust regex accordingly)

## Reporting
- Always write a small JSON report with at least:
  - status: ok|warning|error
  - actions: list of checks performed
  - errors: if any
  - timestamp
- Keep logs local (e.g., `.agents/startup.log`) and avoid printing secrets.

## CI usage
- The same sequence can be used in CI job setup but with:
  - `agents_allow_network: true` to allow FetchContent, or use a pre-populated cache.
  - longer timeouts and explicit artifact upload steps for logs/reports.

## Why this approach
- Fast, safe feedback on repository health without surprising the user.
- Keeps network and heavy operations explicit and auditable.
- Produces machine-readable reports for automation and troubleshooting.

## Suggested placement and filename
- `agents_startup.md` at repository root, or `docs/agents_startup.md`.
- Keep short and link from `agents.md` to it.

## Optional: an `agents_startup.yaml` (or `.json`) with runtime flags for the agent — this lets maintainers tune automatic behaviour per CI vs developer environment.

## Rationale / integration notes for helios
- `helios` uses FetchContent for glfw/glad: automatic network fetch on agent startup may be surprising — default to disallow network and recommend a manual configure step.
- Because the project uses C++ modules and `.ixx`, configure-only step is valuable to validate module scanning without building everything.
- Running a small smoke target (an example binary in `examples/`) is helpful if the agent is explicitly allowed to build.

---


