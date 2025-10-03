module;

#include <memory>

export module helios.event.Event;

import helios.util.Guid;

export namespace helios::event {

    /**
     * Base class for events in the helios framework.
     *
     * `Event` provides a common interface for all application related events.
     * Each event instance is uniquely identified by a `Guid` generated upon
     * construction. An additional tag can be provided to identify groups of events
     * for further categorization or filtering.
     *
     * Events should be treated as immutable after construction.
     *
     * @todo can we reset the value for Guids in between frames, if events
     * are guaranteed to be dispatched and processed in between frames?
     */
    class Event {
    private:
        /**
         * The guid uniquely identifying _this_ event.
         */
        const helios::util::Guid guid_;

        /**
         * An optional tag for this event. Defaults to 0.
         */
        const uint64_t tag_;

    public:
        virtual ~Event() =default;


        /**
         * Constructs a new event with a unique Guid and a default tag of 0.
         */
        Event();


        /**
         * Constructs a new event with a unique Guid and teh specified default tag.
         *
         * @param tag
         */
        explicit Event(const uint64_t tag);


        /**
         * Returns a const reference to this event's unique Guid.
         *
         * @return
         */
        [[nodiscard]] const helios::util::Guid& guid() const;


        /**
         * Returns the tag for this event.
         * Will be 0 if none was specified.
         *
         * @return
         */
        [[nodiscard]] uint64_t tag() const;


        /**
         * Compares two event instances based on their unique Guids.
         * @param e
         * @return
         */
        bool operator==(const Event& e) const;

    };

}