#ifndef QUEUE_H
#define QUEUE_H

#include <sxc/message.hpp>

#include <mutex>
#include <deque>
#include <condition_variable>

/**
 * This class handles message passing between the user interface (main) thread 
 * and the networking thread
 *
 * @ref https://stackoverflow.com/questions/12805041/c-equivalent-to-javas-blockingqueue
 * @ref https://gist.github.com/thelinked/6997598
 */
class Queue
{
private:
  std::mutex m_mutex;
  std::deque<Message> m_queue;
  std::condition_variable m_signal;

public:
  void Push(const Message & message);
  Message Pop();
};

#endif
