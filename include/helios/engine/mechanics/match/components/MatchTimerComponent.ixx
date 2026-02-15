/**
 * @file MatchTimerComponent.ixx
 * @brief Component for tracking match elapsed time.
 */
module;


export module helios.engine.mechanics.match.components.MatchTimerComponent;



export namespace helios::engine::mechanics::match::components {

    /**
     * @brief Tracks elapsed time during a match.
     *
     * Used for timing match phases, countdowns, or total match duration.
     */
    struct MatchTimerComponent  {


        float elapsedSeconds_ = 0.0f;


    public:

        /**
         * @brief Adds time to the elapsed counter.
         *
         * @param seconds Seconds to add.
         */
        void addElapsedSeconds(const float seconds) noexcept {
            elapsedSeconds_ += seconds;
        }

        /**
         * @brief Sets the elapsed time.
         *
         * @param seconds The new elapsed time in seconds.
         */
        void setElapsedSeconds(const float seconds) noexcept {
            elapsedSeconds_ = seconds;
        }




    };


}