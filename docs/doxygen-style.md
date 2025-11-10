# helios Doxygen Styleguide

## Purpose
This document summarizes the minimal rules we use for Doxygen comments in the Helios project. Goal: consistent, readable API documentation that Doxygen produces reliably.

## General Rules
- Language: English for this translation (code comments in the repo are German by default). Technical terms may remain in English.
- Every public/header file should have a short file header: a 1–2 line @brief description (e.g. `@brief Matrix utilities`).
- Use these Doxygen tags as needed: `@brief, @param, @return, @tparam, @see, @note, @todo`.
- No empty`@return` or `@param` tags. Omit tags if there is nothing to say.
- Write complete sentences, simple phrasing, correct capitalization and punctuation.

## Format & Examples

### File header (at the top of each header file):

```cpp
/**
* @brief Short file description.
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
- [ ] No obvious spelling or grammar mistakes.
- [ ] @return matches the actual return value.
- [ ] Template parameters have `@tparam` descriptions.

## Further notes
- For larger documentation changes (restructuring API docs, adding new sections), add a dedicated Markdown file under `docs/`.
- This project uses Doxygen with the existing toolchain; run a local Doxygen pass (see [README.md](./README.md)) to verify generated HTML output.

## Contact
If you are unsure, ask in the repo chat or open a PR with `type: docs` and request reviewers.

