#include <sxc/queue.hpp>

/**
 * Push a new message into the deque
 * @param message Message object to push into the inbound deque
 */
void Queue::Push(const std::string & message)
{
  {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_queue.push_back(message);
  }

  m_signal.notify_one();
}

/**
 * Pop a new message from the deque if not empty
 */
std::string Queue::Pop()
{
  std::unique_lock<std::mutex> lock(m_mutex);

  // Using a lock in combination with a conditional function prevents accidental
  // trigger of pops
  m_signal.wait(lock, [&]()
    {
      return !m_queue.empty();
    }
  );

  std::string message(std::move(m_queue.front()));
  m_queue.pop_front();
  return message;
}
