#include "_6221.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <chrono>

EventBased::EventBased() {}

void EventBased::event(std::function<void()> coro) {
    std::string name = typeid(coro).name();
    if (name.substr(0, 3) != "on_") {
        throw InvalidEvent("'" + name + "' isn't a correct event naming.");
    }

    if (!coro) {
        std::string message = "Couldn't register a non-coroutine function for the event " + name + ".";
        throw InvalidEvent(message);
    }

    events[name] = coro;
}

std::future<void> EventBased::waitFor(const std::string& event, std::function<bool()> condition, int timeout, bool stopPropagation) {
    std::promise<void> promise;
    auto future = promise.get_future();

    std::string eventLower = event;
    std::transform(eventLower.begin(), eventLower.end(), eventLower.begin(), ::tolower);

    if (!condition) {
        condition = []() { return true; };
    }

    waiters[eventLower].emplace_back(condition, std::move(promise), stopPropagation);

    if (timeout > 0) {
        std::thread([timeout, future = std::move(future)]() mutable {
            std::this_thread::sleep_for(std::chrono::seconds(timeout));
            if (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
                future.set_value();
            }
            }).detach();
    }

    return future;
}

void EventBased::dispatch(const std::string& event, std::vector<std::any> args) {
    std::string method = "on_" + event;

    auto it = waiters.find(method);
    if (it != waiters.end()) {
        std::vector<int> toRemove;
        auto& waitersList = it->second;

        for (int i = 0; i < waitersList.size(); ++i) {
            auto& [cond, promise, stop] = waitersList[i];
            if (promise.get_future().wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
                toRemove.push_back(i);
                continue;
            }

            try {
                bool result = cond();
                if (result) {
                    promise.set_value();
                    if (stop) {
                        waitersList.erase(waitersList.begin() + i);
                        return;
                    }
                    toRemove.push_back(i);
                }
            }
            catch (const std::exception& e) {
                promise.set_exception(std::make_exception_ptr(e));
            }
        }

        for (auto i = toRemove.rbegin(); i != toRemove.rend(); ++i) {
            waitersList.erase(waitersList.begin() + *i);
        }

        if (waitersList.empty()) {
            waiters.erase(it);
        }
    }

    auto coroIt = events.find(method);
    if (coroIt != events.end()) {
        try {
            coroIt->second();
        }
        catch (const std::exception& e) {
            onError(method, e);
        }
    }
}

void EventBased::onError(const std::string& event, const std::exception& e) {
    std::cerr << "! \"" << event << "\": " << e.what() << std::endl;
}
