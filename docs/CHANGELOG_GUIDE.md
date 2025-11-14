# CHANGELOG Maintenance Guide

## Philosophy

The CHANGELOG is **manually curated** to provide high-quality, user-focused release notes. While we use conventional commits and GitHub issues for tracking, the CHANGELOG focuses on **what matters to users**, not individual commits.

## Workflow

### During Development

When closing an issue or merging a PR:

1. Add an entry under `[Unreleased]` in `CHANGELOG.md`
2. Use the appropriate category: `Added`, `Changed`, `Fixed`, `Breaking Changes`
3. Reference the issue number: `(#XX)`
4. Add context and migration guides for breaking changes

Example:
```markdown
## [Unreleased]

### Added
- New feature description (#42)
  - Additional context if needed
  - Usage example if helpful
```

### At Release Time

1. **Run the helper script** (optional):
   ```powershell
   .github/scripts/changelog-helper.ps1 -milestone 2
   ```
   This lists all closed issues to verify nothing was missed.

2. **Rename `[Unreleased]`** to version and date:
   ```markdown
   ## [0.2.0] - 2025-11-17
   ```

3. **Create a new empty `[Unreleased]` section**:
   ```markdown
   ## [Unreleased]
   
   _No unreleased changes yet._
   
   ## [0.2.0] - 2025-11-17
   ...
   ```

4. **Tag the release**:
   ```bash
   git tag -a v0.2.0 -m "Release version 0.2.0"
   git push origin v0.2.0
   ```

## Categories

### Added
New features, APIs, or capabilities.

### Changed
Modifications to existing functionality (non-breaking).

### Fixed
Bug fixes.

### Breaking Changes
Use `**BREAKING**:` prefix and provide:
- What changed
- Why it changed
- Migration guide with before/after examples

Example:
```markdown
- **BREAKING**: Enum counter entries renamed to `size_` (#34)
  - Consistent naming across all enums
  - Improved code generation and Doxygen compatibility
  
  **Migration:** Search and replace `COUNT` → `size_` in enum usage.
  
  ```cpp
  // Before
  for (int i = 0; i < Key::COUNT; i++) { ... }
  
  // After  
  for (int i = 0; i < static_cast<int>(Key::size_); i++) { ... }
  ```
```

## Grouping Rules

- **Combine related commits** into a single entry
- **Focus on user impact**, not implementation details
- **Order by importance**: Breaking changes first, then features, then fixes
- **Be specific** but concise

### Bad Example ❌
```markdown
- Updated Material.ixx
- Fixed typo in comment
- Refactored internal function
```

### Good Example ✅
```markdown
- Material ownership structure refactored (#13)
  - Material now owns both Shader and MaterialData
  - MaterialData is optional for simple materials
  - Reduces coupling between rendering components
```

## Commit Message Correlation

We use **Conventional Commits** for commit messages:
- `feat:` → `Added` in CHANGELOG
- `fix:` → `Fixed` in CHANGELOG
- `refactor:` → `Changed` in CHANGELOG
- `feat!:` or `refactor!:` → `Breaking Changes` in CHANGELOG

The CHANGELOG entry should be more detailed than the commit message.

## Version Numbering

We follow **Semantic Versioning** (semver):

- **MAJOR** (X.0.0): Breaking changes
- **MINOR** (0.X.0): New features (backward compatible)
- **PATCH** (0.0.X): Bug fixes

Examples:
- `0.1.0` → `0.2.0`: Added scene graph (new feature)
- `0.2.0` → `1.0.0`: Renamed all public APIs (breaking)
- `0.2.0` → `0.2.1`: Fixed crash in renderer (bugfix)

## Tools

### Helper Script
`.github/scripts/changelog-helper.ps1` generates a draft from closed issues.

Usage:
```powershell
# For Milestone 2
.github/scripts/changelog-helper.ps1 -milestone 2

# Output is printed to console, copy and refine manually
```

### Automation (Future)
Consider `github-changelog-generator` for auto-drafts:
```bash
npm install -g github-changelog-generator
github-changelog-generator --user thorstensuckow --project helios
```

But **always review and enhance** the generated output.

## Examples from helios

### Good Entries

✅ **Feature with context:**
```markdown
- Scoped logger implementation (#8)
  - Default loggers for specific scopes (e.g., `helios.rendering.shader`)
  - LogManager for centralized logger access
```

✅ **Breaking change with migration:**
```markdown
- **BREAKING**: `MeshData` merged with `Mesh` (#22)
  - Simplified architecture by removing redundant abstraction
  
  **Migration:** Use `Mesh` directly instead of wrapping `MeshData`.
```

✅ **Bug fix with impact:**
```markdown
- Potential nullptr dereference in `MaterialData` (#16)
  - Added proper null checks
  - Prevents crashes when material has no custom data
```

### Bad Entries

❌ **Too vague:**
```markdown
- Updated rendering (#22)
```

❌ **Too technical:**
```markdown
- Refactored Material.ixx to use shared_ptr instead of unique_ptr in line 42
```

❌ **Missing context:**
```markdown
- Fixed bug (#16)
```

## Checklist

Before releasing, verify:

- [ ] All closed issues/PRs are in CHANGELOG
- [ ] Breaking changes have migration guides
- [ ] Version number follows semver
- [ ] Links work (issues, docs)
- [ ] Grammar and spelling checked
- [ ] No duplicate entries
- [ ] Release date is correct

## Questions?

See:
- [Keep a Changelog](https://keepachangelog.com/)
- [Semantic Versioning](https://semver.org/)
- [Conventional Commits](https://www.conventionalcommits.org/)

