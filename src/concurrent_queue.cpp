#include "concurrent_queue.hpp"

#include "pico/mutex.h"

#include <queue>

template <class T>
concurrent_queue<T>::concurrent_queue() { mutex_init(&mtx_); }

template <class T>
T &concurrent_queue<T>::front()
{
  mutex_enter_blocking(&mtx_);
  auto &f = q_.front();
  mutex_exit(&mtx_);
  return f;
}

template <class T>
const T &concurrent_queue<T>::front() const
{
  mutex_enter_blocking(&mtx_);
  const auto &f = q_.front();
  mutex_exit(&mtx_);
  return f;
}

template <class T>
T &concurrent_queue<T>::back()
{
  mutex_enter_blocking(&mtx_);
  auto &f = q_.back();
  mutex_exit(&mtx_);
  return f;
}

template <class T>
const T &concurrent_queue<T>::back() const
{
  mutex_enter_blocking(&mtx_);
  const auto &f = q_.back();
  mutex_exit(&mtx_);
  return f;
}

template <class T>
bool concurrent_queue<T>::empty() const
{
  mutex_enter_blocking(&mtx_);
  const auto b = q_.empty();
  mutex_exit(&mtx_);
  return b;
}

template <class T>
void concurrent_queue<T>::push(T &&e)
{
  mutex_enter_blocking(&mtx_);
  q_.push(std::forward(e));
  mutex_exit(&mtx_);
}

template <class T>
void concurrent_queue<T>::pop()
{
  mutex_enter_blocking(&mtx_);
  q_.pop();
  mutex_exit(&mtx_);
}

template <class T>
std::size_t concurrent_queue<T>::size() const
{
  mutex_enter_blocking(&mtx_);
  const auto n = q_.size();
  mutex_exit(&mtx_);
  return n;
}
