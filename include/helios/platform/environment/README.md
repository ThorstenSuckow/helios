# helios::platform::environment

Runtime platform orchestration layer.

## Overview

This module defines the runtime domain for platform initialization and frame
service commands (init, poll, shutdown), plus supporting components, systems,
entity manager aliases, and handle types.

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `commands/` | Runtime command types (`PlatformInitCommand`, `PollEventsCommand`, `ShutdownCommand`) |
| `components/` | Runtime state markers (initialized/context-ready/current-context) |
| `concepts/` | Runtime-specific handle constraints |
| `systems/` | Runtime execution systems for command handling |
| `types/` | Strong ids and handles for the runtime domain |

## Key Types

| Type | Purpose |
|------|---------|
| `PlatformEntityManager` | ECS manager alias for runtime platform entities |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::platform::environment
@brief Runtime platform orchestration layer.
</p></details>

