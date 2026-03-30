# Event System

[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Boost.Asio](https://img.shields.io/badge/Boost.Asio-required-green.svg)](https://www.boost.org/doc/libs/release/doc/html/boost_asio.html)

A lightweight, asynchronous event bus for C++17 built on top of Boost.Asio.

## Description

This library provides a simple event bus that allows decoupled communication between components via type‑safe events. Events are published asynchronously on a dedicated thread managed by Boost.Asio, and subscribers can register handlers that are invoked when matching events are published. Subscribers can optionally filter events by the sender.

Key features:
- Type‑safe events derived from a common base class.
- Asynchronous event dispatching via Boost.Asio.
- Sender filtering: subscribers can ignore events originating from a specific sender.
- Automatic cleanup of expired subscribers.
- Thread‑safe for both subscription and publishing.

## Requirements

- C++17 compiler
- [Boost.Asio](https://www.boost.org/doc/libs/release/doc/html/boost_asio.html) (header‑only part of Boost)

## Installation

### Using CMake FetchContent

Add the following to your `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
    event_system
    GIT_REPOSITORY https://github.com/ania-7abc/event_system.git
    GIT_TAG v1.0.1
)

FetchContent_MakeAvailable(event_system)

# Link against the library
target_link_libraries(your_target PRIVATE event_system)
```

The library exports the target `event_system`. Boost.Asio is a header‑only library, so no additional linking is required beyond ensuring the Boost include directories are available.

## Usage

### Define an Event

Events must derive from `event_system::Event`. They can carry any data needed.

```cpp
#include <event_system/event.hpp>

struct MyEvent : public event_system::Event {
    int value;
    explicit MyEvent(int v) : value(v) {}
};
```

### Create an Event Bus and a Subscriber

The `EventBus` manages event dispatching. `Subscriber` is a helper class that simplifies subscription and publishing.

```cpp
#include <event_system/event_bus.hpp>
#include <event_system/subscriber.hpp>

int main() {
    event_system::EventBus bus;

    // Create a subscriber that receives all MyEvent events.
    auto sub = bus.make_subscriber<MyEvent>(
        [](const MyEvent& e) {
            std::cout << "Received MyEvent with value: " << e.value << std::endl;
        }
    );

    // Publish an event from the subscriber (or from any other source)
    sub->publish<MyEvent>(42);

    // Wait a moment for the asynchronous dispatch to complete.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
}
```

### Subscribing with Sender Filtering

You can filter events so that only those from a different sender are received.

```cpp
auto sub1 = bus.make_subscriber<MyEvent>(handler1);      // receives all
auto sub2 = bus.make_subscriber<MyEvent>(handler2, sub1.get()); // ignores events sent by sub1
```

### Manual Subscription

Instead of `make_subscriber`, you can manage a `Subscriber` object manually and use its `subscribe` method.

```cpp
class Example : public event_system::Subscriber
{
  public:
    explicit Example(event_system::EventBus &bus)
        : event_system::Subscriber(bus)
    {
        // Don't use subscribe and publish in constructor!
    }

    void start()
    {
        subscribe<MyEvent>([](const MyEvent &e) {
            std::cout << "Received MyEvent with value: " << e.value << std::endl;
        },
        nullptr); // From all senders
        publish<MyEvent>(10 /* v */);
    }
};
```

### Custom Subscriber Lifetime

`Subscriber` inherits from `std::enable_shared_from_this`, so it is typically managed by a `shared_ptr`. The event bus holds a `weak_ptr` to the subscriber and automatically skips calls when the subscriber is destroyed.

### Cleanup

Call `bus.cleanup()` periodically to remove expired subscribers from the internal subscription lists. This is optional but recommended if many subscribers are created and destroyed.

## License

This project is licensed under the MIT License – see the [LICENSE](LICENSE) file for details.
