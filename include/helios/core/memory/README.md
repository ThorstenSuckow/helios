# helios::core::memory

Low-level memory utilities for type-erased ownership.

## Overview

This module provides foundational memory primitives used by the engine's resource management layer. The primary type is `ErasedUnique`, a lightweight ownership wrapper that enables heterogeneous collections without requiring a common base class.

## Key Classes

| Class | Purpose |
|-------|---------|
| `ErasedUnique` | Type-erased unique ownership wrapper (16 bytes) |

## ErasedUnique

`ErasedUnique` provides `unique_ptr`-like ownership semantics without requiring a virtual destructor or common base class. It stores a `void*` and a typed deleter function pointer:

```cpp
struct ErasedUnique {
    void* ptr = nullptr;
    void (*destroy)(void*) noexcept = nullptr;
};
```

### Construction

Constructed from any `std::unique_ptr<T>`. The deleter captures the concrete type at construction time:

```cpp
auto obj = std::make_unique<MyResource>(args...);
ErasedUnique erased{std::move(obj)};
// erased.ptr → raw pointer
// erased.destroy → calls delete static_cast<MyResource*>(ptr)
```

### Move-Only Semantics

`ErasedUnique` is move-only. Moving transfers ownership and nulls the source. Copy construction and assignment are implicitly deleted.

```cpp
ErasedUnique a{std::make_unique<Foo>()};
ErasedUnique b = std::move(a); // a is now empty
```

### Primary Consumer

`ErasedUnique` is used by `ResourceRegistry` to store Managers, CommandBuffers, and other heterogeneous resources in a single `std::vector<ErasedUnique>` without vtable overhead for destruction.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::core::memory
@brief Low-level memory utilities for type-erased ownership.
@details Provides ErasedUnique, a 16-byte type-erased unique ownership wrapper used by ResourceRegistry to store heterogeneous resource types without requiring a common base class or virtual destructor.
</p></details>

