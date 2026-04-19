# helios::platform::lifecycle

Platform lifecycle orchestration for startup, warmup completion, and shutdown.

## Overview

This module contains lifecycle-oriented command and system APIs that were split
from broader platform runtime concerns. It centralizes session/platform state
transitions such as initialize, warmup-done, shutdown, and final session
destruction.

## Subdirectories

| Directory | Purpose |
|-----------|---------|
| `commands/` | Lifecycle command markers (`PlatformInitCommand`, `ShutdownCommand`) |
| `systems/` | Lifecycle systems coordinating init/warmup/shutdown flow |

## Notes

Per-frame event polling remains in `helios::platform::environment`.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::platform::lifecycle
@brief Platform lifecycle orchestration for startup and shutdown transitions.
</p></details>

