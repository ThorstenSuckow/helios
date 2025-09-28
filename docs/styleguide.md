## helios Styleguide

## `core`-Folders
`core` folders contain the foundational functionality of their owning top-level module. Items within core are exported individually.
The core folder itself **MUST NOT** be exported as a whole.

## module facades
Module facades are prefixed with an underscore `_`, followed by the name of the exported module.
Modules **MUST NOT** export other low-level modules, only their own partitions.
