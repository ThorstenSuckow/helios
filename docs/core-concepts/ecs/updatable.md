# Updatable (Removed)

The `Updatable` interface has been removed. Systems are now plain classes that provide
`update(UpdateContext&)` and are wrapped by the type-erased `System` class internally.

## Migration

| Before | After |
|--------|-------|
| `class MySystem : public System` | `class MySystem` (plain class) |
| `void update(UpdateContext&) noexcept override` | `void update(UpdateContext&) noexcept` |
| `void init(GameWorld&) noexcept override` | `void init(GameWorld&)` (optional, detected via concept) |
| `gameWorld_->view<...>()` | `ctx.view<...>()` |

## See Also

- [System](system.md) — Current system architecture
- [Game Loop Architecture](../gameloop-architecture.md) — How systems execute
