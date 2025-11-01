module;

#include <cstdint>

export module helios.window.event.WindowEvent;

import helios.util.Guid;
import helios.event.Event;

export namespace helios::window::event {

    /**
     * @brief Abstract base class for Window related events.
     *
     * The WindowEvent extends the Event-base class by adding a unique
     * sourceGuid, so the source of the event can be identified.
     */
    class  WindowEvent : public helios::event::Event {

    protected:
        /**
         * @brief Constructs a new WindowEvent with the specific source Guid.
         *
         * @param sourceGuid The Guid of the Window that triggered the event.
         */
        explicit WindowEvent(helios::util::Guid sourceGuid);

        /**
         * @brief Constructs a new WindowEvent with the specific source Guid and
         * the specific tag.
         *
         * @param sourceGuid The Guid of the Window that triggered the event.
         * @param tag A `uint64_t` value identifying the specific type of event.
         */
        explicit WindowEvent(helios::util::Guid sourceGuid, uint64_t tag);

    public:

        /**
         * @brief The unique and immutable Guid of the source of this event, i.e.
         * the Window that trigegred the event.
         */
        const helios::util::Guid sourceGuid;

    };

}