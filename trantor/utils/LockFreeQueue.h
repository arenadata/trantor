#include <trantor/utils/NonCopyable.h>
#include <atomic>
#include <memory>
#include <type_traits>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace trantor
{
// Thread safe queue implementation
template <typename T>
class ThreadSafeQueue : public NonCopyable
{
  public:
    ThreadSafeQueue() = default;

    ~ThreadSafeQueue() = default;

    void enqueue(T&& input)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(std::forward<T>(input));
    }

    void enqueue(const T& input)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(input);
    }

    bool dequeue(T& output)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty())
        {
            return false;
        }

        output = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }

    size_t size() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

  private:
    mutable std::mutex mutex_;
    std::queue<T> queue_;
};

template <typename T>
using MpscQueue = ThreadSafeQueue<T>;

}  // namespace trantor
