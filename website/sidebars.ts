import type {SidebarsConfig} from '@docusaurus/plugin-content-docs';

// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

/**
 * Creating a sidebar enables you to:
 - create an ordered group of docs
 - render a sidebar for each doc of that group
 - provide next/previous navigation

 The sidebars can be generated from the filesystem, or explicitly defined here.

 Create as many sidebars as you want.
 */
const sidebars: SidebarsConfig = {
  // Main documentation sidebar
  tutorialSidebar: [
    'intro',
    'getting-started',
    {
      type: 'category',
      label: 'Core Concepts',
      items: [
        'core-concepts/scene-graph',
      ],
    },
    {
      type: 'category',
      label: 'Examples',
      items: [
        'examples/overview',
        'examples/simple-cube',
        'examples/gamepad-input',
      ],
    },
    'testing',
    {
      type: 'category',
      label: 'Contributing',
      items: [
        'contributing/overview',
        'contributing/guide',
        'contributing/changelog-guide',
        'contributing/styleguide',
        'contributing/doxygen-style',
      ],
    },
  ],

  // API Reference sidebar
  apiSidebar: [
    'api/overview',
  ],
};

export default sidebars;
