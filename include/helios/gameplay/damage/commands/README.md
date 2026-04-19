# helios::gameplay::damage::commands

Damage-related commands for the command buffer.

This namespace contains command classes used to request damage application through the engine's command system.

## Commands

| Command | Description |
|---------|-------------|
| `ApplyDamageCommand` | Carries a `DamageContext` to request damage application to a target entity |

## Usage

Commands are typically issued by `DamageOnCollisionSystem` when a collision between damageable entities is detected:

```cpp
auto ctx = DamageContext{
    .interactionContext = {
        .target = targetHandle,
        .instigator = instigatorHandle,
        .causer = causerHandle,
        .contact = contactPoint
    },
    .damage = 10.0f
};

updateContext.queueCommand<EngineCommandBuffer, ApplyDamageCommand>(ctx);
```

The command is routed to `HealthManager` via the registered command handler `submit` function and processed during the manager flush phase.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::gameplay::damage::commands
@brief Damage-related commands for the command buffer.
@details Contains command classes for requesting damage application through the engine's command system.
</p></details>
