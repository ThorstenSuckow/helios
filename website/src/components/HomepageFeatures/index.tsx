import type {ReactNode} from 'react';
import clsx from 'clsx';
import Heading from '@theme/Heading';
import styles from './styles.module.css';

type FeatureItem = {
  title: string;
  description: ReactNode;
};

const FeatureList: FeatureItem[] = [
  {
    title: '▸ Modern C++23',
    description: (
      <>
        Built with C++23 modules for clean dependency management and fast compile times.
        Leverages the latest language features for type-safe, performant code.
      </>
    ),
  },
  {
    title: '▸ Flexible Rendering',
    description: (
      <>
        Modular rendering pipeline with support for multiple backends. Ships with OpenGL 4.5+,
        extensible to Vulkan, DirectX, or custom renderers.
      </>
    ),
  },
  {
    title: '▸ Scene Graph',
    description: (
      <>
        Hierarchical scene management with automatic transform propagation.
        Supports frustum culling and efficient batch rendering.
      </>
    ),
  },
  {
    title: '▸ Unified Input',
    description: (
      <>
        Cross-platform input handling for keyboard, mouse, and gamepad.
        Consistent API across Windows, Linux, and macOS.
      </>
    ),
  },
  {
    title: '▸ Math Library',
    description: (
      <>
        Complete 3D math library with vectors, matrices, and transforms.
        Foundation for all geometric operations in the engine.
      </>
    ),
  },
  {
    title: '▸ Educational',
    description: (
      <>
        Built from first principles as an educational project. Every component is
        documented with clear explanations of design decisions.
      </>
    ),
  },
];

function Feature({title, description}: FeatureItem) {
  return (
    <div className={clsx('col col--4')}>
      <div className="text--center padding-horiz--md">
        <Heading as="h3">{title}</Heading>
        <p>{description}</p>
      </div>
    </div>
  );
}

export default function HomepageFeatures(): ReactNode {
  return (
    <section className={styles.features}>
      <div className="container">
        <div className="row">
          {FeatureList.map((props, idx) => (
            <Feature key={idx} {...props} />
          ))}
        </div>
      </div>
    </section>
  );
}
