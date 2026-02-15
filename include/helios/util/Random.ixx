/**
 * @file Random.ixx
 * @brief Utility class for pseudo-random number generation.
 */
module;

#include <cstdint>
#include <random>

export module helios.util.Random;


export namespace helios::util {

    /**
     * @brief Utility class for generating pseudo-random numbers.
     *
     * @details This class provides a random number generator based on the
     * Mersenne Twister algorithm (`std::mt19937`).
     *
     * Example usage:
     * ```cpp
     * auto rng = helios::util::Random(12345);
     * float value = rng.randomFloat(0.0f, 1.0f);
     * ```
     */
    class Random {

    private:

        /**
         * @brief The Mersenne Twister pseudo-random number generator.
         */
        std::mt19937 gen_;

        /**
         * @brief Uniform real distribution for generating floating-point numbers.
         *
         * @see https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
         */
        std::uniform_real_distribution<float> uniformDist_;

        /**
         * @brief The initial seed used to initialize the generator.
         *
         * @details Stored for use by `reset()` to restore the generator to its
         * initial state.
         */
        uint32_t initialSeed_;

    public:

        /**
         * @brief Initializes the generator with the provided seed value.
         *
         * @param seed The seed value for the random number generator.
         */
        explicit Random(const uint32_t seed) : gen_(seed), initialSeed_(seed) {};

        /**
         * @brief Resets the generator to its initial state.
         *
         * @details Re-seeds the generator with the original seed value and
         * resets the distribution state. Useful for reproducible sequences.
         */
        void reset() {
            gen_.seed(initialSeed_);

            uniformDist_.reset();
        }

        /**
         * @brief Generates a pseudo-random float in the range [a, b).
         *
         * @details Uses `std::uniform_real_distribution` to generate a uniformly
         * distributed floating-point number.
         *
         * @param a The lower bound of the range (inclusive).
         * @param b The upper bound of the range (exclusive).
         *
         * @return A random float value in the range [a, b).
         *
         * @see https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
         */
        [[nodiscard]] float randomFloat(float a, float b) noexcept {

            if (b < a) {
                auto tmp = a; a = b; b = tmp;
            }

            if (a == b) {
                return a;
            }

            return uniformDist_(
                gen_,
                std::uniform_real_distribution<float>::param_type{a, b}
            );
        }

    };


}