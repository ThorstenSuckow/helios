# Agent Policy for helios

## Short description
This document describes the rules for an automated coding agent (e.g., Copilot-like assistant / CI agent) that makes changes in the `helios` repository or automates development steps. Goal: provide fast, safe and high-quality assistance for implementation, refactoring and testing of the C++-based, modular engine.

## Scope
- Code in `include/helios/*`, `src/helios/*`, `include/ext/*`, `src/ext/*`
- The CMake build system, tests, examples, benchmarks
- Documentation under `docs/` and `README.md`

## Primary goals of the agent
- Prefer small, self-contained changes (one topic / PR).
- Preserve buildability: every commit must be buildable locally.
- Follow quality gates: Build → Lint/Format → Unit tests.
- Do not introduce unexpected breaks to module interfaces (.ixx / public headers).
- Do not exfiltrate or commit private/semi-private secrets.

## Success criteria (for each change)
- Project builds without new errors (locally, Debug/Release).
- Relevant unit tests pass (or a justified test extension is provided).
- Formatting follows project conventions (use existing `.clang-format` if present).
- Modifications to public APIs are noted in `CHANGELOG.md` and `docs/`.

## Forbidden actions
- Do not commit credentials/secrets (tokens, private keys) into the repo.
- Do not perform untested, large-scale architectural rewrites without coordination (public PR + description required).
- Do not perform aggressive reformatting across many files in one commit (unless explicitly requested).

## Quality gates / workflow
1. Local build: configure and build (see build commands below).
2. Formatting: apply / check `clang-format`.
3. Unit tests: run `ctest`, show output on failures.
4. Minimal smoke test (e.g., run a small example binary if available).
5. Commit + PR: small, descriptive commit messages, PR including tests + CI info.

## Branching and PR guidelines
- Branch names: `feature/<short-description>`, `fix/<ticket>`, `chore/<description>`
- PR description: problem, solution, files changed, test instructions, risk assessment
- At least 1 reviewer (core maintainer) before merge
- If a change breaks an API, mark the PR as "major" and add a CHANGELOG entry

## Security rules
- Never write system or environment variables containing secret values into PRs or logs.
- Do not automatically publish artifacts without review.
- Use of external FetchContent is allowed, but if a third-party pin is removed/updated, document the change and request review.

## Concrete technical notes (repo-specific)
- C++ Standard: the project uses C++23 modules. Pay attention to `.ixx` files in `include/` and the module scan in CMake.
- Build system: CMake v4.0+; external libraries (glfw, glad) are fetched via FetchContent. When building in isolated environments (CI), ensure network access or provide an `external/` cache.
- Changes to module interfaces (.ixx): avoid breaking changes. If necessary, provide backward-compatibility wrappers or open a major PR.
- Formatting: the project contains a `.clang-format` → apply it before committing.
- Tests: `tests/` contains CMake/ctest-configured tests; include new tests for bug fixes/features.

## Windows build and test commands (examples for developer environment `cmd.exe`)
- First-time build (generic):
    - cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
    - cmake --build build --config Debug
- Using Visual Studio toolchain (if needed):
    - cmake -S . -B build -G "Visual Studio 17 2022" -A x64
    - cmake --build build --config Release
- Run tests:
    - cd build
    - ctest -C Debug --output-on-failure
- Quick format / lint check (if clang-format is installed):
    - clang-format -i <changed_files>

## Checklist for each agent change (short)
- [ ] Compiles locally (Debug/Release)?
- [ ] Relevant tests pass?
- [ ] Formatting OK?
- [ ] No secrets committed?
- [ ] PR with description + tests created?

## Rollback / emergency
- If a commit breaks the build: create an immediate revert PR and notify maintainers.
- For larger risky changes: keep them in a feature branch and follow a strict review process.

## Other notes
- The agent may automatically update helper files (README, docs) but content changes (design decisions) must be reviewed by a maintainer.
- Every automated change must have a clear commit message and link to the related issue/PR.

## Versioning the agent policy
- This file (`helios.agent.md`) is located in `.github/agents/`; update the policy version at the top when changes are made.
