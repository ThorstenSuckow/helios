# Traits

The **Traits** module provides compile-time detection of component lifecycle hooks using C++20 concepts. This enables zero-overhead conditional invocation of optional methods.

## Overview

Traits use concepts to detect if a type implements specific methods:

```cpp
template<typename T>
concept HasOnAcquire = requires(T t) {
    {t.onAcquire()} -> std::same_as<void>;
};
```

At compile time, the `ComponentReflector` uses these traits to generate appropriate function pointers, avoiding runtime checks.

## Available Traits

| Trait | Required Method | Return Type | Purpose |
|-------|-----------------|-------------|---------|
| `HasOnAcquire` | `onAcquire()` | `void` | Pool acquisition callback |
| `HasOnRelease` | `onRelease()` | `void` | Pool release callback |
| `HasOnRemove` | `onRemove()` | `bool` | Removal interception |
| `HasToogleable` | `enable()`, `disable()` | `void` | Component enable/disable |
| `HasClone` | `onClone(const T&)` | `void` | Post-copy initialization |
| `HasActivatable` | `onActivate()`, `onDeactivate()` | `void` | GameObject activation |

## Trait Definitions

### HasOnAcquire

Detects components that need initialization when acquired from a pool:

```cpp
template<typename T>
concept HasOnAcquire = requires(T t) {
    {t.onAcquire()} -> std::same_as<void>;
};
```

### HasOnRelease

Detects components that need cleanup when released to a pool:

```cpp
template<typename T>
concept HasOnRelease = requires(T t) {
    {t.onRelease()} -> std::same_as<void>;
};
```

### HasOnRemove

Detects components that can intercept removal:

```cpp
template<typename T>
concept HasOnRemove = requires(T t) {
    {t.onRemove()} -> std::convertible_to<bool>;
};
```

Returns `true` to allow removal, `false` to prevent it.

### HasToogleable

Detects components that can be enabled/disabled:

```cpp
template<typename T>
concept HasToogleable = requires(T t) {
    {t.disable()} -> std::same_as<void>;
    {t.enable()} -> std::same_as<void>;
};
```

> **Note:** Both methods must be present for the trait to be satisfied.

### HasClone

Detects components that need post-copy initialization:

```cpp
template<typename T>
concept HasClone = requires(T t, const T& source) {
    {t.onClone(source)} -> std::same_as<void>;
};
```

### HasActivatable

Detects components that respond to GameObject activation:

```cpp
template<typename T>
concept HasActivatable = requires(T t) {
    {t.onActivate()} -> std::same_as<void>;
    {t.onDeactivate()} -> std::same_as<void>;
};
```

## Usage in ComponentReflector

The `ComponentReflector` uses traits with `if constexpr` for zero-overhead dispatch:

```cpp
template<typename T>
static void registerType() {
    ComponentOps ops{
        .onAcquire = [](void* ptr) {
            if constexpr (traits::HasOnAcquire<T>) {
                static_cast<T*>(ptr)->onAcquire();
            }
        },
        
        .onRemove = [](void* ptr) -> bool {
            if constexpr (traits::HasOnRemove<T>) {
                return static_cast<T*>(ptr)->onRemove();
            }
            return true;
        },
        
        // ... other hooks
    };
}
```

## Implementing Traits in Components

Components opt-in to lifecycle hooks by implementing the required methods:

```cpp
class HealthComponent {
    float health_;
    float maxHealth_;

public:
    // Satisfies HasOnAcquire
    void onAcquire() noexcept {
        health_ = maxHealth_;
    }
    
    // Satisfies HasOnRelease
    void onRelease() noexcept {
        health_ = maxHealth_;
    }
};

// Compile-time verification
static_assert(traits::HasOnAcquire<HealthComponent>);
static_assert(traits::HasOnRelease<HealthComponent>);
```

## Performance

Traits provide **zero runtime overhead**:

1. **Compile-time detection** - No runtime type checks
2. **Dead code elimination** - Empty `if constexpr` branches are removed
3. **Inlining** - Lambda bodies are typically inlined

```cpp
// If T doesn't have onAcquire(), this compiles to nothing:
if constexpr (traits::HasOnAcquire<T>) {
    ptr->onAcquire();  // Only compiled if trait is satisfied
}
```

## See Also

- [Component Lifecycle](../component-lifecycle.md) - How hooks are used
- [ComponentOps](component-ops.md) - Function pointers generated from traits
- [Component Registry](../component-registry.md) - Registration using traits

