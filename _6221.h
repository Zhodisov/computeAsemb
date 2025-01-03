#include <any>
#include <vector>
#include <functional>
#include <unordered_map>
#include <future>
#include <stdexcept>
#include <any>

class EventBased {
public:
    EventBased();
    virtual ~EventBased() = default;

    void event(std::function<void()> coro);
    std::future<void> waitFor(const std::string& event, std::function<bool()> condition = nullptr, int timeout = 0, bool stopPropagation = false);
    void dispatch(const std::string& event, std::vector<std::any> args);

protected:
    virtual void onError(const std::string& event, const std::exception& e);

private:
    std::unordered_map<std::string, std::vector<std::tuple<std::function<bool()>, std::promise<void>, bool>>> waiters;
    std::unordered_map<std::string, std::function<void()>> events;
};
