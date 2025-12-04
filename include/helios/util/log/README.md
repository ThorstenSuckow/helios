## helios::util::log

Flexible logging system with configurable output destinations. Sinks register themselves with unique identifiers, allowing dynamic enable/disable at runtime without compile-time knowledge of all sink types.

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::util::log
@brief Logging system with self-registering output sinks.
@details Sinks define their own type identifiers and register with the LogManager. Enable/disable is done by string identifier, allowing extensibility without modifying core logging code.
</p></details>
