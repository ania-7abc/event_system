// event.hpp
#pragma once
#include <memory>
#include <typeindex>

namespace event_system
{

class Subscriber;

class Event
{
  public:
    virtual ~Event() = default;
    Event() = default;
    Event(const Event &) = default;
    auto operator=(const Event &) -> Event & = default;
    Event(Event &&) = default;
    auto operator=(Event &&) -> Event & = default;

    [[nodiscard]] auto sender() const -> std::type_index
    {
        return sender_;
    }
    void set_sender(std::type_index type)
    {
        sender_ = type;
    }

  private:
    std::type_index sender_ = typeid(void);
};

struct AnyEvent : Event
{
    // NOLINTNEXTLINE(clang-diagnostic-potentially-evaluated-expression)
    explicit AnyEvent(const std::shared_ptr<const Event> &event) : original_type(typeid(*event)), original(event)
    {
    }
    std::type_index original_type;
    std::shared_ptr<const Event> original;
};

} // namespace event_system
