#pragma once

#include "pico/mutex.h"
#include <queue>
#include <string>

/***
 * \class concurrent_queue
 *
 * \brief Wraps around std::queue and as name implies, allows concurrent access
 * accross execution flows (cores/threads, etc..). Relays all operations to the
 * underlying queue, concurrently.
 */
template <class T>
class concurrent_queue
{
  std::queue<T> q_;
  mutex mtx_ = {};

public:
  concurrent_queue();
  ~concurrent_queue() = default;

  concurrent_queue<T> &operator=(const concurrent_queue<T> &) = delete;
  concurrent_queue<T> &operator=(concurrent_queue<T> &&) = delete;

  T &front();
  const T &front() const;

  T &back();
  const T &back() const;

  bool empty() const;
  std::size_t size() const;

  void push(T &&);
  void pop();
};

using job_t = std::pair<std::string, std::string>;
using job_queue = concurrent_queue<job_t>;
