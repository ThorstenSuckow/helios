# helios Doxygen Styleguide

## Purpose
This document summarizes the minimal rules we use for Doxygen comments in the Helios project. Goal: consistent, readable API documentation that Doxygen produces reliably.

## General Rules
- Language: English for this translation (code comments in the repo are German by default). Technical terms may remain in English.
- Every public/header file should have a short file header: a 1–2 line @brief description (e.g. `@brief Matrix utilities`).
- Use these Doxygen tags as needed: `@brief, @param, @return, @tparam, @see, @note, @todo`.
- No empty `@return` or `@param` tags. Omit tags if there is nothing to say.
- Write complete sentences, simple phrasing, correct capitalization and punctuation.
- **No inline comments:** Do not place comments on the same line as code. Always place comments on a separate line above the code they describe.
- **Always use multi-line comment blocks:** Do not use single-line `/** @brief ... */` comments. Always use the multi-line format with `@brief` on its own line.
- **Always document return types:** Every method with a return value must have an `@return` tag describing what is returned.
- **Blank line between @param and @return:** Always insert a blank line between the last `@param` tag and the `@return` tag.

## Idiom & Brevity

**Prefer brevity over verbosity.** Documentation should add value, not restate what is obvious from the code.

### Guidelines

- **Getters/Setters:** For simple accessors, a minimal `@brief` is sufficient. Do not over-explain.
  - Do: `@brief Returns the entity ID.`
  - Avoid: `@brief Returns the unique identifier that was assigned to this entity when it was created.`

- **Self-documenting names:** If the method name is self-explanatory, keep the `@brief` short.
  - Do: `@brief Enables this component.`
  - Avoid: `@brief Sets the internal enabled flag to true, which causes the component to be active.`

- **Avoid restating types:** Do not repeat type information that is already in the signature.
  - Do: `@return The current position.`
  - Avoid: `@return A vec3f representing the current position in 3D space.`

- **Focus on "what" and "why", not "how":** Describe purpose and side effects, not implementation details.

- **Omit documentation for trivial code:** Default constructors, destructors, or simple one-liners may not need documentation if their purpose is obvious.

### Examples

**Too verbose:**
```cpp
/**
 * @brief Sets the selected state of this UI element to the specified value.
 *
 * This method updates the internal isSelected_ member variable to reflect
 * whether the UI element is currently selected or not.
 *
 * @param selected A boolean value indicating whether the element should be selected.
 */
void setSelected(bool selected) noexcept;
```

**Idiomatic:**
```cpp
/**
 * @brief Sets the selected state.
 *
 * @param selected True if selected.
 */
void setSelected(bool selected) noexcept;
```

**Getter with return documentation:**
```cpp
/**
 * @brief Returns the selected state.
 *
 * @return True if selected.
 */
[[nodiscard]] bool isSelected() const noexcept;

## Format & Examples

### File header (at the top of each header file):

```cpp
/**
 * @file MyClass.ixx
 * @brief Short file description.
 */
```

### Module file header (for `_module.ixx` aggregator files):

Module aggregator files only need a minimal header with `@file` and `@brief`:

```cpp
/**
 * @file _module.ixx
 * @brief Short module description.
 */
```

### Function / method documentation:

```cpp
/**
 * @brief Short description of the function.
 *
 * Optional detailed description (one to three sentences).
 *
 * @param foo Description of parameter `foo`.
 * @param bar Description of parameter `bar`.
 *
 * @return Description of the return value (mention the type if not obvious).
 */
```

### Templates / generics:

```cpp
/**
* @brief Description.
* @tparam T Description of the type parameter.
  */
```

### Member variables:

Document class member variables with a `@brief` comment. Always use multi-line format:

```cpp
class MyClass {
    /**
     * @brief Current position in world space.
     */
    vec3f position_;

    /**
     * @brief True if the entity is currently active.
     */
    bool isActive_ = false;
};
```


## Tips for Doxygen quality
- A short @brief is enough for file comments (do not add long documentation in every file).
- Use @see to reference symbols or related classes/methods.
- Avoid unexplained abbreviations (e.g. `rgt` -> `right`).
- Keep comments in sync with implementation (e.g. make sure @return matches actual return type).
- Do not repeat code in the documentation - explain the intention and side effects.

## Conventions for smart pointers & ownership
- For return values: indicate whether a pointer/reference may be null and who owns the object.
    - Example: `@return A shared_ptr to the Mesh used by this Renderable (may be nullptr).`
- If a method accepts a unique_ptr, document that ownership is transferred.
    - Example: `@param renderQueue Ownership of the RenderQueue is transferred to the caller.`

### Examples

Before:
```cpp
/** @brief Returns pointer to the mesh. */
std::shared_ptr<Mesh> mesh() const;
```


After:

```cpp
/**
 * @brief Returns a shared_ptr to the associated Mesh.
 *
 * @return A shared_ptr to the Mesh. May be nullptr if no mesh is assigned.
 */
std::shared_ptr<Mesh> mesh() const;
```

## Review checklist for PRs
- [ ] Each modified header file has a file `@brief`.
- [ ] Public methods have `@brief` plus appropriate `@param` / `@return`; no empty tags.
- [ ] All methods with return values have `@return` documentation.
- [ ] All comments use multi-line format (no single-line `/** ... */`).
- [ ] Class member variables have `@brief` descriptions.
- [ ] No obvious spelling or grammar mistakes.
- [ ] @return matches the actual return value.
- [ ] Template parameters have `@tparam` descriptions.

## Further notes
- For larger documentation changes (restructuring API docs, adding new sections), add a dedicated Markdown file under `docs/`.
- This project uses Doxygen with the existing toolchain; run a local Doxygen pass (see [README.md](./README.md)) to verify generated HTML output.

## Contact
If you are unsure, ask in the repo chat or open a PR with `type: docs` and request reviewers.

