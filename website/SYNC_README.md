# Website Documentation Sync

This directory contains the helios project website built with Docusaurus.

## Documentation Synchronization

The website automatically synchronizes documentation from the repository root and `docs/` folder to avoid duplication and ensure consistency.

### Auto-Synced Files

The following files are **automatically copied** from the repository during the build process:

| Source (Repository) | Destination (Website) | Purpose |
|---------------------|----------------------|---------|
| `CHANGELOG.md` | `docs/changelog.md` | Project changelog |
| `docs/styleguide.md` | `docs/contributing/styleguide.md` | C++ coding standards |
| `docs/doxygen-style.md` | `docs/contributing/doxygen-style.md` | Documentation style guide |
| `docs/CONTRIBUTING.md` | `docs/contributing/commits.md` | Commit message conventions |
| `docs/CHANGELOG_GUIDE.md` | `docs/contributing/changelog-guide.md` | Changelog maintenance guide |

### How It Works

1. **Build Time**: When you run `npm run build` or `npm start`, the `scripts/sync-changelog.mjs` script runs first
2. **Sync Process**: The script copies markdown files from the repository root to the website's `docs/` folder
3. **MDX Compatibility**: The script automatically:
   - Adds Docusaurus frontmatter (title) if missing
   - Escapes MDX-problematic characters (`<`, `>`) outside code blocks
   - Adds a sync banner comment at the top
4. **Build**: Docusaurus processes the synced files along with other documentation

### Important Notes

⚠️ **Do NOT edit the synced files directly in `website/docs/`** – they will be overwritten on next build!

✅ **Always edit the source files** in the repository root or `docs/` folder.

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

- `npm start` – Start development server with hot reload
- `npm run build` – Build static site for production
- `npm run serve` – Serve the production build locally
- `npm run clear` – Clear Docusaurus cache
- `node ./scripts/sync-changelog.mjs` – Manually sync documentation

## File Structure

```
website/
├── docs/                      # Docusaurus documentation pages
│   ├── changelog.md          # ⚠️ Auto-synced (do not edit)
│   ├── contributing/
│   │   ├── styleguide.md     # ⚠️ Auto-synced (do not edit)
│   │   ├── doxygen-style.md  # ⚠️ Auto-synced (do not edit)
│   │   ├── commits.md        # ⚠️ Auto-synced (do not edit)
│   │   ├── changelog-guide.md # ⚠️ Auto-synced (do not edit)
│   │   └── guide.md          # ✅ Manually maintained
│   └── ...
├── scripts/
│   └── sync-changelog.mjs    # Auto-sync script
├── src/                       # React components, CSS
├── static/                    # Static assets (images, logo, etc.)
├── docusaurus.config.ts       # Docusaurus configuration
├── sidebars.ts                # Sidebar navigation structure
└── package.json               # Dependencies & scripts
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

