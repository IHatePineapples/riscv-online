#pragma once

#include "pico/mutex.h"

#include <queue>
#include <string>

/**
 * \class concurrent_queue
 *
 * \brief Wraps around std::queue and as name implies, allows concurrent access
 * accross execution flows (cores/threads, etc..). Relays all operations to the
 * underlying queue, concurrently.
 */
template <typename T>
class concurrent_queue
{
  /**
   * \var q_
   * \brief Underlying `std::queue`, protected by a mutex, `mtx_`.
  */
  std::queue<T> q_;
  mutex mtx_ = {};

public:

  /**
   * \fn concurrent_queue()
   * 
   * Default constructor.
   * \remark Copy construction is not made available to avoid weird
   * locking mechanisms.
  */
  concurrent_queue();

  concurrent_queue<T> &operator=(const concurrent_queue<T> &) = delete;
  concurrent_queue<T> &operator=(concurrent_queue<T> &&) = delete;

  /**
   * \fn front()
   * \returns `&` to first item.
   * 
   * \brief Reference to first item in the queue.
  */
  T &front();
  const T &front() const;

  /**
   * \fn back()
   * \returns `&` to last item.
   * 
   * \brief Reference to last item in the queue.
  */
  T &back();
  const T &back() const;

  /**
   * \fn empty()
   * \returns `true` if queue empty, `false` otherwise.
  */
  bool empty() const;

  /**
   * \fn size()
   * \returns number of elements in the queue.
  */
  std::size_t size() const;

  /**
   * \fn push(T &&)
   * \brief Push an item onto the stack, doing an `emplace_back()` in the background.
  */
  void push(T &&);
  /**
   * \fn pop()
   * \brief Pops the first element of the queue, pointed to by `front()`.
  */
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
