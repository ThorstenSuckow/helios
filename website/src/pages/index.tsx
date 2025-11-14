import React, {ReactNode} from 'react';
import clsx from 'clsx';
import Link from '@docusaurus/Link';
import useDocusaurusContext from '@docusaurus/useDocusaurusContext';
import Layout from '@theme/Layout';
import HomepageFeatures from '@site/src/components/HomepageFeatures';
import Heading from '@theme/Heading';
import ParticleBackground from '@site/src/components/ParticleBackground';
import QuickInstall from '@site/src/components/QuickInstall';

import styles from './index.module.css';

function HomepageHeader() {
  const {siteConfig} = useDocusaurusContext();
  return (
    <header className={clsx('hero hero--primary', styles.heroBanner)} style={{position: 'relative', overflow: 'hidden'}}>
      <ParticleBackground />
      <div className="container" style={{display: 'flex', alignItems: 'center', justifyContent: 'center', padding: '4rem 2rem', gap: '3rem', position: 'relative', zIndex: 1}}>
        <div style={{textAlign: 'right'}}>
          <Heading as="h1" className="hero__title">
            {siteConfig.title}
            <Link to="/docs/status" className={styles.alphaBadge} title="Project status: alpha - details" aria-label="Alpha status details">alpha</Link>
          </Heading>
          <p className="hero__subtitle">{siteConfig.tagline}</p>
          <div className={styles.buttons} style={{justifyContent: 'flex-end'}}>
            <Link
              className="button button--secondary button--lg"
              to="/docs/intro">
              Get Started →
            </Link>
            <Link
              className="button button--outline button--lg"
              to="/docs/examples"
              style={{marginLeft: '1rem'}}>
              View Examples
            </Link>
          </div>
        </div>
        <div style={{flexShrink: 0}}>
          <img
            src="/helios/img/logo.png"
            alt="helios logo"
            style={{height: '200px', width: 'auto', display: 'block'}}
          />
        </div>
      </div>
    </header>
  );
}

export default function Home(): ReactNode {
  const {siteConfig} = useDocusaurusContext();
  return (
    <Layout
      title={`${siteConfig.title} - C++23 Game Framework`}
      description="A C++23 game framework built from first principles. Modern module architecture, flexible rendering pipeline, and cross-platform support.">
      <HomepageHeader />
      <main>
        <QuickInstall />
        <HomepageFeatures />
      </main>
    </Layout>
  );
}
