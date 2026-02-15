# helios::engine::mechanics::match::dispatcher

Dispatchers for routing match state commands.

## Dispatchers

| Dispatcher | Description |
|------------|-------------|
| `MatchStateCommandDispatcher` | Routes `MatchStateCommand` to the registered handler |

## MatchStateCommandDispatcher

A typed world command dispatcher that forwards `MatchStateCommand` instances to the `MatchStateCommandHandler` registered with the `GameWorld`.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::match::dispatcher
@brief Dispatchers for routing match state commands.
@details Contains dispatchers that route state commands to their handlers.
</p></details>

