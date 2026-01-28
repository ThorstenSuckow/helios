import type {SidebarsConfig} from '@docusaurus/plugin-content-docs';
import doxygenSidebarItems from './sidebar-category-doxygen-helios.json';

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
    {
      type: 'category',
      label: 'Installation',
      link: {
        type: 'doc',
        id: 'getting-started'
      },
      items: [
          'prerequisites'
      ]
    },
    {
      type: 'category',
      label: 'Core Concepts',
      items: [
        'core-concepts/conventions',
        'core-concepts/scene-graph',
        'core-concepts/component-system',
        'core-concepts/gameloop-architecture',
        'core-concepts/gameobject-builder',
        'core-concepts/command-system',
        'core-concepts/event-system',
        'core-concepts/spawn-system'
      ],
    },
    {
      type: 'category',
      label: 'Examples',
      link: {
        type: 'doc',
        id: 'examples/overview'
      },
      items: [
        'examples/simple-cube',
        'examples/gamepad-input',
        'examples/spaceship-control',
        'examples/spaceship-shooting',
      ],
    },
    'testing',
    {
      type: 'category',
      label: 'Contributing',
      link: {
        type: 'doc',
        id: 'contributing/overview'
      },
      items: [
        'contributing/guide',
        'contributing/issue-submission',
        'contributing/changelog-guide',
        'contributing/styleguide',
        'contributing/doxygen-style',
      ],
    },
  ],
  doxygenSidebar: [
    doxygenSidebarItems,
  ]
};

export default sidebars;
