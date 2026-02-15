# helios::engine::mechanics::gamestate::dispatcher

Dispatchers for routing game state commands.

## Dispatchers

| Dispatcher | Description |
|------------|-------------|
| `GameStateCommandDispatcher` | Routes `GameStateCommand` to the registered handler |

## GameStateCommandDispatcher

A typed world command dispatcher that forwards `GameStateCommand` instances to the `GameStateCommandHandler` registered with the `GameWorld`.

---

<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::mechanics::gamestate::dispatcher
@brief Dispatchers for routing game state commands.
@details Contains dispatchers that route state commands to their handlers.
</p></details>

