# helios::ui::widgets::commands

UI action commands.

This namespace contains commands for triggering UI actions.

## Commands

| Command | Description |
|---------|-------------|
| `UiActionCommand` | Encapsulates a UI action request with source entity and action ID |



## UiActionCommand

A world command created when a UI element is activated (e.g., button pressed, menu item selected). Carries the source entity and an `ActionId` that identifies the action to execute.

Commands are routed through the `CommandHandlerRegistry` to the registered `UiActionCommandManager`.

## Usage

```cpp
// Issue a UI action command when menu item is activated
updateContext.queueCommand<EngineCommandBuffer, UiActionCommand>(
    menuItementity.handle(),
    ActionId::StartGame
);
```

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::ui::widgets::commands
@brief UI action commands.
@details Contains commands for triggering UI actions that route them to registered handlers.
</p></details>

