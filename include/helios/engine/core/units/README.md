# helios::engine::core::units

Canonical unit definitions for consistent spatial and temporal measurements.

This module provides standardized units and conversion helpers to keep spatial and temporal data consistent across all helios modules. Using consistent units prevents scaling errors and simplifies physics calculations.

## Key Constants

| Constant | Value | Purpose |
|----------|-------|---------|
| `HELIOS_SPATIAL_UNIT` | `Unit::Meter` | Base length unit (1.0f = 1 meter) |
| `HELIOS_TEMPORAL_UNIT` | `Unit::Seconds` | Base time unit (1.0f = 1 second) |
| `METERS` | `1.0f` | Identity ratio for meters |
| `CENTIMETERS` | `0.01f` | Conversion ratio (cm → m) |
| `SECONDS` | `1.0f` | Identity ratio for seconds |
| `MILLISECONDS` | `0.001f` | Conversion ratio (ms → s) |

## Conversion Functions

| Function | Purpose |
|----------|---------|
| `cmToM(float cm)` | Converts centimeters to meters |
| `mToCm(float m)` | Converts meters to centimeters |
| `msToS(float ms)` | Converts milliseconds to seconds |
| `sToMs(float s)` | Converts seconds to milliseconds |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::engine::core::units
@brief Canonical unit definitions for consistent measurements.
@details Provides standard units and conversion helpers for spatial and temporal data, ensuring consistency across all engine modules. The base length unit is meters (1.0f = 1m) and the base time unit is seconds.
</p></details>


