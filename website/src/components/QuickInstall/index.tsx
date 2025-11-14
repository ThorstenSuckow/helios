import React from 'react';
import styles from './styles.module.css';

export default function QuickInstall(): JSX.Element {
  const copyToClipboard = () => {
    const code = `git clone https://github.com/thorstensuckow/helios.git
cd helios
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cd build
ctest -C Release --output-on-failure`;
    navigator.clipboard.writeText(code);
  };

  return (
    <section className={styles.quickInstall}>
      <div className="container">
        <div className={styles.content}>
          <div className={styles.codeWrapper}>
            <button
              className={styles.copyButton}
              onClick={copyToClipboard}
              title="Copy to clipboard"
            >
              Copy
            </button>
            <div className={styles.codeBlock}>
              <pre>
                <code>
                  <span className={styles.comment}># Clone the repository</span>
                  {'\n'}git clone https://github.com/thorstensuckow/helios.git
                  {'\n'}cd helios
                  {'\n\n'}
                  <span className={styles.comment}># Configure with CMake</span>
                  {'\n'}cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
                  {'\n\n'}
                  <span className={styles.comment}># Build</span>
                  {'\n'}cmake --build build --config Release
                  {'\n\n'}
                  <span className={styles.comment}># Run tests</span>
                  {'\n'}cd build
                  {'\n'}ctest -C Release --output-on-failure
                </code>
              </pre>
            </div>
          </div>
        </div>
      </div>
    </section>
  );
}
