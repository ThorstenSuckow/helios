# Website Documentation Sync

This directory contains the helios project website built with Docusaurus.

## Documentation Synchronization

The website automatically synchronizes documentation from the repository root and `docs/` folder to avoid duplication and ensure consistency.

### Auto-Synced Files

The following files are **automatically copied** from the repository during the build process:

| Source (Repository) | Destination (Website)                 | Purpose |
|---------------------|---------------------------------------|---------|
| `CHANGELOG.md` | `docs/changelog.md`                   | Project changelog |
| `docs/styleguide.md` | `docs/contributing/styleguide.md`     | C++ coding standards |
| `docs/doxygen-style.md` | `docs/contributing/doxygen-style.md`  | Documentation style guide |
| `docs/CONTRIBUTING.md` | `docs/contributing/commits.md`        | Commit message conventions |
| `docs/CHANGELOG_GUIDE.md` | `docs/contributing/changelog-guide.md` | Changelog maintenance guide |
| `docs/core-concepts/conventions.md` | `docs/core-concepts/conventions.md`   | Technical conventions (LHS, matrices, units) |
| `docs/core-concepts/scene-graph.md` | `docs/core-concepts/scene-graph.md` | Scene graph architecture |
| `docs/core-concepts/component-system.md` | `docs/core-concepts/component-system.md` | Composition-based entity design |
| `docs/core-concepts/gameloop-architecture.md` | `docs/core-concepts/gameloop-architecture.md` | Game loop phases and passes |
| `docs/core-concepts/command-system.md` | `docs/core-concepts/command-system.md` | Command pattern, dispatchers, managers |
| `docs/core-concepts/resource-registry.md` | `docs/core-concepts/resource-registry.md` | Type-indexed resource registry |
| `docs/core-concepts/event-system.md` | `docs/core-concepts/event-system.md` | Phase/pass event propagation |
| `docs/core-concepts/spawn-system.md` | `docs/core-concepts/spawn-system.md` | Spawn scheduling and profiles |
| `docs/core-concepts/object-pooling.md` | `docs/core-concepts/object-pooling.md` | Entity pooling system |
| `docs/core-concepts/gameobject-builder.md` | `docs/core-concepts/gameobject-builder.md` | Fluent builder pattern |
| `docs/core-concepts/text-rendering.md` | `docs/core-concepts/text-rendering.md` | FreeType-based text rendering |
| `docs/core-concepts/sparse-set.md` | `docs/core-concepts/sparse-set.md` | O(1) component storage |
| `docs/core-concepts/component-registry.md` | `docs/core-concepts/component-registry.md` | Modular component registration system |
| `docs/core-concepts/component-lifecycle.md` | `docs/core-concepts/component-lifecycle.md` | Component lifecycle hooks |
| `docs/core-concepts/state-management.md` | `docs/core-concepts/state-management.md` | Game and match state management |
| `docs/core-concepts/ecs/README.md` | `docs/core-concepts/ecs/overview.md` | ECS architecture overview |
| `docs/core-concepts/ecs/component-structure.md` | `docs/core-concepts/ecs/component-structure.md` | Component Copy/Move requirements |
| `docs/core-concepts/ecs/gameobject.md` | `docs/core-concepts/ecs/gameobject.md` | High-level entity wrapper |
| `docs/core-concepts/ecs/entity-manager.md` | `docs/core-concepts/ecs/entity-manager.md` | Entity and component management |
| `docs/core-concepts/ecs/entity-registry.md` | `docs/core-concepts/ecs/entity-registry.md` | Entity lifecycle and versioning |
| `docs/core-concepts/ecs/entity-handle.md` | `docs/core-concepts/ecs/entity-handle.md` | Versioned entity reference |
| `docs/core-concepts/ecs/view.md` | `docs/core-concepts/ecs/view.md` | Component-based entity queries |
| `docs/core-concepts/ecs/system.md` | `docs/core-concepts/ecs/system.md` | Game logic processor base class |
| `docs/core-concepts/ecs/traits.md` | `docs/core-concepts/ecs/traits.md` | Compile-time lifecycle detection |
| `docs/core-concepts/ecs/component-ops.md` | `docs/core-concepts/ecs/component-ops.md` | Lifecycle function pointers |
| `examples/README.md` | `docs/examples/overview.md` | Examples overview |
| `examples/v0.0.1-alpha/simple_cube_rendering/README.md` | `docs/examples/simple-cube.md` | Simple cube rendering tutorial |
| `examples/v0.0.1-alpha/game_controller_input/README.md` | `docs/examples/gamepad-input.md` | Gamepad input tutorial |
| `examples/v0.0.1-alpha/spaceship_control/README.md` | `docs/examples/spaceship-control.md` | Spaceship control example |
| `examples/v0.0.1-alpha/spaceship_shooting/README.md` | `docs/examples/spaceship-shooting.md` | Twin-stick shooting example |
| `examples/v0.0.1-alpha/enemy_spawn/README.md` | `docs/examples/enemy-spawn.md` | Enemy spawning example |
| `examples/v0.0.1-alpha/collision_detection/README.md` | `docs/examples/collision-detection.md` | Collision detection example |
| `examples/v0.0.1-alpha/render_text_demo/README.md` | `docs/examples/render-text-demo.md` | Text rendering demo |
| `examples/v0.0.1-alpha/scoring_demo/README.md` | `docs/examples/scoring-demo.md` | Scoring system demo |
| `examples/v0.0.1-alpha/runtime_test/README.md` | `docs/examples/runtime-test.md` | Runtime stress test |

### How It Works

1. **Build Time**: When you run `npm run build` or `npm start`, the `scripts/sync-changelog.mjs` script runs first
2. **Sync Process**: The script copies markdown files from the repository root to the website's `docs/` folder
3. **MDX Compatibility**: The script automatically:
   - Adds Docusaurus frontmatter (title) if missing
   - Escapes MDX-problematic characters (`<`, `>`) outside code blocks
   - Adds a sync banner comment at the top
4. **Build**: Docusaurus processes the synced files along with other documentation

### Important Notes

âš ï¸ **Do NOT edit the synced files directly in `website/docs/`** â€“ they will be overwritten on next build!

âœ… **Always edit the source files** in the repository root or `docs/` folder.

### Linking Between Documents

Since all synced documents become regular Docusaurus pages, you can use **relative Docusaurus paths**:

```markdown
<!-- Good: Docusaurus internal link -->
[See Changelog](/docs/changelog)
[Contributing Guide](/docs/contributing/guide)

<!-- Bad: Absolute paths will break -->
[Changelog](../../CHANGELOG.md)
```

### Adding New Synced Documents

To sync additional documentation files:

1. Edit `website/scripts/sync-changelog.mjs`
2. Add a new entry to the `syncMappings` array:
   ```javascript
   {
     src: path.join(repoRoot, 'docs', 'your-file.md'),
     dest: path.join(websiteRoot, 'docs', 'target-folder', 'your-file.md'),
     title: 'Your Document Title'
   }
   ```
3. Add the file to `website/.gitignore` so it's not committed
4. Update `website/sidebars.ts` to include it in navigation

### Development Workflow

```powershell
# Start development server (auto-syncs first)
npm start

# Build for production (auto-syncs first)
npm run build

# Manually trigger sync (if needed)
node ./scripts/sync-changelog.mjs

# Clear cache if you encounter stale content
npm run clear
```

### CI/CD Integration

The sync script runs automatically during CI builds, ensuring the deployed website always reflects the latest documentation from the repository.

## Scripts

- `npm start` â€“ Start development server with hot reload
- `npm run build` â€“ Build static site for production
- `npm run serve` â€“ Serve the production build locally
- `npm run clear` â€“ Clear Docusaurus cache
- `node ./scripts/sync-changelog.mjs` â€“ Manually sync documentation

## File Structure

```
website/
â”œâ”€â”€ docs/                      # Docusaurus documentation pages
â”‚   â”œâ”€â”€ changelog.md          # âš ï¸ Auto-synced (do not edit)
â”‚   â”œâ”€â”€ contributing/
â”‚   â”‚   â”œâ”€â”€ styleguide.md     # âš ï¸ Auto-synced (do not edit)
â”‚   â”‚   â”œâ”€â”€ doxygen-style.md  # âš ï¸ Auto-synced (do not edit)
â”‚   â”‚   â”œâ”€â”€ commits.md        # âš ï¸ Auto-synced (do not edit)
â”‚   â”‚   â”œâ”€â”€ changelog-guide.md # âš ï¸ Auto-synced (do not edit)
â”‚   â”‚   â””â”€â”€ guide.md          # âœ… Manually maintained
â”‚   â””â”€â”€ ...
â”œâ”€â”€ scripts/
â”‚   â””â”€â”€ sync-changelog.mjs    # Auto-sync script
â”œâ”€â”€ src/                       # React components, CSS
â”œâ”€â”€ static/                    # Static assets (images, logo, etc.)
â”œâ”€â”€ docusaurus.config.ts       # Docusaurus configuration
â”œâ”€â”€ sidebars.ts                # Sidebar navigation structure
â””â”€â”€ package.json               # Dependencies & scripts
```

## Troubleshooting

### Build fails with "Can't resolve ..."
Run `npm run clear` to clear the cache, then rebuild.

### Links to synced docs are broken
Use Docusaurus internal paths (`/docs/...`) instead of relative file paths.

### MDX compilation errors
Check if the source markdown contains unescaped `<` or `>` outside code blocks. The sync script should handle this automatically, but complex cases may need manual adjustment.

### Changes not appearing
1. Ensure you edited the **source file** (in repository root/docs), not the synced copy
2. Re-run `npm start` or `node ./scripts/sync-changelog.mjs`
3. Clear cache with `npm run clear` if needed

## Further Reading

- [Docusaurus Documentation](https://docusaurus.io/)
- [MDX Syntax](https://mdxjs.com/)
- [helios Contributing Guide](../docs/CONTRIBUTING.md)

