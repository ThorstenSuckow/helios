# helios Documentation Website

This directory contains the Docusaurus-based documentation website for the helios framework.

## Quick Start

### Development Server

Start the development server to preview the website locally:

```bash
cd website
npm start
```

The website will be available at `http://localhost:3000`.

### Build

Build the website for production:

```bash
npm run build
```

The static files will be generated in the `build/` directory.

### Serve

Serve the production build locally:

```bash
npm run serve
```

## Project Structure

```
website/
├── docs/                    # Documentation markdown files
│   ├── intro.md            # Getting started page
│   ├── getting-started.md  # Installation guide
│   ├── api/                # API reference
│   ├── examples/           # Example tutorials
│   ├── core-concepts/      # Architecture explanations
│   └── contributing/       # Contribution guide
├── src/
│   ├── components/         # React components
│   ├── css/                # Custom styles
│   └── pages/              # Custom pages (landing page)
├── static/                 # Static assets (images, etc.)
├── docusaurus.config.ts    # Docusaurus configuration
└── sidebars.ts             # Sidebar structure
```

## Adding Documentation

### Create a New Page

1. Create a markdown file in the appropriate `docs/` subdirectory
2. Add frontmatter:

```markdown
---
sidebar_position: 1
---

# Your Page Title

Content here...
```

3. The page will automatically appear in the sidebar

### Link Between Pages

Use relative links:

```markdown
See the [API Reference](./api/overview.md) for details.
```

## Deployment

### GitHub Pages

To deploy to GitHub Pages:

```bash
npm run deploy
```

This will build the website and push it to the `gh-pages` branch.

### Manual Deployment

1. Build the website: `npm run build`
2. Copy the `build/` directory to your web server

## Configuration

Edit `docusaurus.config.ts` to customize:

- Site title and tagline
- GitHub repository links
- Footer content
- Theme colors
- Navbar items

## Syncing with Main Documentation

The website **automatically syncs** documentation from the main repository during build.

**What is synced:**

| Source | Destination | Description |
|--------|-------------|-------------|
| `CHANGELOG.md` | `docs/changelog.md` | Project changelog |
| `docs/styleguide.md` | `docs/contributing/styleguide.md` | Code style guide |
| `docs/doxygen-style.md` | `docs/contributing/doxygen-style.md` | Documentation style |
| `docs/CONTRIBUTING.md` | `docs/contributing/guide.md` | Contributing guide |
| `docs/CHANGELOG_GUIDE.md` | `docs/contributing/changelog-guide.md` | Changelog maintenance |
| `examples/README.md` | `docs/examples/overview.md` | Examples overview |
| `examples/*/README.md` | `docs/examples/*.md` | Individual example tutorials |

**How it works:**

The sync script (`scripts/sync-changelog.mjs`) runs automatically during:
- `npm run build` (local builds)
- GitHub Actions deployment workflow

**Editing synced documents:**

⚠️ **Do not edit synced files directly** in `website/docs/`! 

Instead:
1. Edit the source file in the repository root
2. Run `npm run build` or manually sync: `node scripts/sync-changelog.mjs`
3. Synced files have a banner: `<!-- Auto-synced from repository ... -->`

**Adding new synced documents:**

See [`scripts/README.md`](./scripts/README.md) for instructions.

## Customization

### Theme

Edit `src/css/custom.css` to customize colors and styles:

```css
:root {
  --ifm-color-primary: #ff3d00;
  --ifm-color-primary-dark: #ff3d00;
  /* ... more colors ... */
}
```

### Components

Custom React components are in `src/components/`. The main feature highlights are in `HomepageFeatures/index.tsx`.

## Troubleshooting

### Port Already in Use

If port 3000 is in use, specify a different port:

```bash
npm start -- --port 3001
```

### Build Errors

Clear the cache and rebuild:

```bash
npm run clear
npm run build
```

## Learn More

- [Docusaurus Documentation](https://docusaurus.io/)
- [Markdown Features](https://docusaurus.io/docs/markdown-features)
- [Styling and Layout](https://docusaurus.io/docs/styling-layout)

