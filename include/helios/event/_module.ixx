module;

/**
 * @brief Event system module for event-driven architecture.
 *
 * @details This module provides a comprehensive event management system including
 * event managers, queues, dispatchers, and base event types. Supports
 * deque-based event queue implementations for flexible event processing.
 */
export module helios.event;

export import helios.event.BasicEventManager;
export import helios.event.EventManager;
export import helios.event.EventQueue;
export import helios.event.DequeEventQueue;
export import helios.event.Dispatcher;
export import helios.event.Event;