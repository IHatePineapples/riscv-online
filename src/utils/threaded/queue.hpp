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
template <typename T>
class concurrent_queue
{
  std::queue<T> q_;
  mutex mtx_ = {};

public:
  concurrent_queue();

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

template <typename T>
concurrent_queue<T>::concurrent_queue() { mutex_init(&mtx_); }

template <typename T>
T &concurrent_queue<T>::front()
{
  mutex_enter_blocking(&mtx_);
  auto &f = q_.front();
  mutex_exit(&mtx_);
  return f;
}

template <typename T>
const T &concurrent_queue<T>::front() const
{
  mutex_enter_blocking(&mtx_);
  const auto &f = q_.front();
  mutex_exit(&mtx_);
  return f;
}

template <typename T>
T &concurrent_queue<T>::back()
{
  mutex_enter_blocking(&mtx_);
  auto &f = q_.back();
  mutex_exit(&mtx_);
  return f;
}

template <typename T>
const T &concurrent_queue<T>::back() const
{
  mutex_enter_blocking(&mtx_);
  const auto &f = q_.back();
  mutex_exit(&mtx_);
  return f;
}

template <typename T>
bool concurrent_queue<T>::empty() const
{
  mutex_enter_blocking(&mtx_);
  const auto b = q_.empty();
  mutex_exit(&mtx_);
  return b;
}

template <typename T>
void concurrent_queue<T>::push(T &&e)
{
  mutex_enter_blocking(&mtx_);
  q_.push(std::forward(e));
  mutex_exit(&mtx_);
}

template <typename T>
void concurrent_queue<T>::pop()
{
  mutex_enter_blocking(&mtx_);
  q_.pop();
  mutex_exit(&mtx_);
}

template <typename T>
std::size_t concurrent_queue<T>::size() const
{
  mutex_enter_blocking(&mtx_);
  const auto n = q_.size();
  mutex_exit(&mtx_);
  return n;
}

using job_t = std::pair<std::string, std::string>;
using job_queue = concurrent_queue<job_t>;
