# helios::core::buffer

Double-buffering infrastructure for thread-safe message passing.

This module provides read and write buffers for implementing double-buffered message systems. Double-buffering enables safe producer-consumer patterns across frame boundaries, where messages pushed in one frame become available for reading in the next.

**Components:**

- **Buffer** - Abstract base class for type-erased buffer implementations.
- **DoubleBuffer** - Abstract base class for double-buffered containers with swap semantics.
- **ReadBuffer** - Read-only buffer for consuming messages (consumer-side of double-buffering).
- **WriteBuffer** - Write-only buffer for producing messages (producer-side of double-buffering).
- **ReadWriteBuffer** - Combined single-buffer with immediate visibility for same-phase access.
- **ReadWriteDoubleBuffer** - Type-safe double-buffered container pairing ReadBuffer and WriteBuffer.
- **TypeIndexedDoubleBuffer** - Type-indexed container managing multiple ReadWriteDoubleBuffers by message type.
- **TypeIndexedReadWriteBuffer** - Type-indexed container for immediate-access single-buffered patterns.

**Usage patterns:**

- **Cross-phase communication:** Use `TypeIndexedDoubleBuffer` with `swapBuffers()` at frame boundaries.
- **Same-phase communication:** Use `TypeIndexedReadWriteBuffer` for immediate message visibility.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::core::buffer
@brief Double-buffering infrastructure for thread-safe message passing.
@details This module provides read and write buffers for implementing double-buffered message systems, enabling safe producer-consumer patterns across frame boundaries. Includes both double-buffered (ReadBuffer, WriteBuffer, ReadWriteDoubleBuffer) and single-buffered (ReadWriteBuffer) implementations, with type-indexed variants for heterogeneous message handling.
</p></details>

