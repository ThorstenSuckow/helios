# helios::rendering::shader::commands

Shader command payload types.

This namespace contains command objects used by command buffers or systems to
request shader compilation work.

| Command | Description |
|---------|-------------|
| `ShaderCompileCommand<THandle>` | Compile a single shader handle |
| `ShaderBatchCompileCommand<THandle>` | Compile multiple shader handles in one request |

---
<details>
<summary>Doxygen</summary><p>
@namespace helios::rendering::shader::commands
@brief Shader command payload types.
@details Defines command objects for single and batched shader compilation workflows.
</p></details>

