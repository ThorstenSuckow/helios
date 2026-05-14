# helios::rendering::common::commands

Command payloads shared by render submission and execution paths.

## Overview

This module contains lightweight command objects used to transfer extracted
render work between runtime systems and rendering managers/backends.

## Commands

| Command | Description |
|---------|-------------|
| `RenderCommand<THandle>` | Wraps one `SceneMemberRenderContext<THandle>` for queued render submission |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::common::commands
@brief Command payloads shared by render submission and execution paths.
</p></details>

