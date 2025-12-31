/**
 * @file Random.ixx
 * @brief Utility class for pseudo-random number generation.
 */
module;

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
     * auto& rng = helios::util::Random(12345);
     * float value = rng.randomFloat(0.0f, 1.0f);
     * ```
     */
    class Random {

    private:

        /**
         * @brief The Mersenne Twister pseudo-random number generator.
         */
        std::mt19937 gen_;


    public:

        /**
        * @brief Initializes the generator with a seed from `std::random_device`.
        */
        explicit Random(uint32_t seed) : gen_(seed) {};


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
            std::uniform_real_distribution<float> dis{a, b};
            return dis(gen_);
        }

    };


}