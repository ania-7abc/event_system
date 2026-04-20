// event_bus.cpp

#include <event_system/event_bus.hpp>
#include <utility>

namespace event_system
{

EventBus::EventBus() : work_guard_(boost::asio::make_work_guard(io_)), thread_([this]() { io_.run(); })
{
}

EventBus::~EventBus()
{
    work_guard_.reset();
    io_.stop();
    if (thread_.joinable())
        thread_.join();
}

void EventBus::subscribe_impl(const std::type_index &event_type, std::function<void(EventPtr)> handler,
                              std::type_index sender_filter, std::weak_ptr<void> weak_subscriber)
{
    std::lock_guard<std::mutex> lock(mutex_);
    subscriptions_[event_type].push_back({std::move(handler), sender_filter, std::move(weak_subscriber)});
}

void EventBus::publish(const EventPtr &event)
{
    const Event *raw = event.get();
    if (raw == nullptr)
    {
        return;
    }
    auto event_type = std::type_index(typeid(*raw));
    std::list<Subscription> subs_copy;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto subs = subscriptions_.find(event_type);
        if (subs != subscriptions_.end())
        {
            subs_copy = subs->second;
        }
    }

    for (const auto &sub : subs_copy)
    {
        if (sub.sender_filter != typeid(void) && sub.sender_filter != event->sender())
        {
            continue;
        }
        if (sub.weak_subscriber.expired())
        {
            continue;
        }
        boost::asio::post(io_, [handler = sub.handler, event]() { handler(event); });
    }

    auto any_type = std::type_index(typeid(AnyEvent));
    std::list<Subscription> any_subs;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        auto subs = subscriptions_.find(any_type);
        if (subs != subscriptions_.end())
        {
            any_subs = subs->second;
        }
    }
    if (!any_subs.empty())
    {
        auto any_event = std::make_shared<AnyEvent>(event);
        any_event->set_sender(event->sender());
        for (const auto &sub : any_subs)
        {
            if (sub.sender_filter != typeid(void) && sub.sender_filter != event->sender())
            {
                continue;
            }
            if (sub.weak_subscriber.expired())
            {
                continue;
            }
            boost::asio::post(io_, [handler = sub.handler, any_event]() { handler(any_event); });
        }
    }
}

void EventBus::cleanup()
{
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto &[type, subs] : subscriptions_)
    {
        subs.remove_if([](const Subscription &subscribtion) { return subscribtion.weak_subscriber.expired(); });
    }
}

} // namespace event_system
