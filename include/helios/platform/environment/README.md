# helios::platform::environment

Platform environment module for per-frame platform services.

## Overview

This module contains environment-facing APIs used during normal frame updates,
including event polling and environment state components. Lifecycle transitions
(init, warmup done, shutdown/session destroy) are provided by
`helios::platform::lifecycle`.

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `commands/` | Environment service commands (`PollEventsCommand`) |
| `components/` | Environment state markers (initialized/context-ready/current-context) |
| `concepts/` | Environment-specific handle constraints |
| `systems/` | Per-frame environment systems (`PollEventsSystem`) |
| `types/` | Strong ids and handles for the environment domain |

## Key Types

| Type | Purpose |
|------|---------|
| `PlatformEntityManager` | ECS manager alias for platform environment entities |

## See Also

- [Lifecycle](../lifecycle/README.md)

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::platform::environment
@brief Platform environment APIs for per-frame services.
</p></details>
