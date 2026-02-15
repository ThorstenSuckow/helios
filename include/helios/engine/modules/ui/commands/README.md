# helios::engine::modules::ui::commands

UI action commands and dispatchers.

This namespace contains commands for triggering UI actions and the dispatchers that route them to handlers.

## Commands

| Command | Description |
|---------|-------------|
| `UiActionCommand` | Encapsulates a UI action request with source entity and action ID |

## Dispatchers

| Dispatcher | Description |
|------------|-------------|
| `UiActionCommandDispatcher` | Routes UI action commands to the registered handler |

## UiActionCommand

A world command created when a UI element is activated (e.g., button pressed, menu item selected). Carries the source entity and an `ActionId` that identifies the action to execute.

Direct execution via `execute()` is a no-op; commands must be dispatched.

## Usage

```cpp
// Issue a UI action command when menu item is activated
updateContext.commandBuffer().add<UiActionCommand>(
    menuItemEntity.entityHandle(),
    ActionId::StartGame
);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::modules::ui::commands
@brief UI action commands and dispatchers.
@details Contains commands for triggering UI actions and dispatchers that route them to registered handlers.
</p></details>

