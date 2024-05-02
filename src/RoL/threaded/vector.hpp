#pragma once

#include "pico/mutex.h"

#include <vector>

/**
 * \class concurrent_vector
 *
 * \brief Wraps around std::vector and as name implies, allows concurrent access
 * accross execution flows (cores/threads, etc..). Relays all operations to the
 * underlying vector, concurrently.
 */
template <typename T>
class concurrent_vector
{
  /**
   * \var v_
   * \brief Underlying `std::vector`, protected by a mutex, `mtx_`.
   */
  std::vector<T> v_;
  mutable mutex mtx_ = {};

public:
  /**
   * \fn concurrent_vector()
   *
   * Default constructor.
   * \remark Copy construction is not made available to avoid weird
   * locking mechanisms.
   */
  concurrent_vector();

  concurrent_vector(const concurrent_vector &) = delete;
  concurrent_vector<T> &operator=(const concurrent_vector<T> &) = delete;
  concurrent_vector<T> &operator=(concurrent_vector<T> &&) = delete;

  /**
   * \fn front()
   * \returns `&` to first item.
   *
   * \brief Reference to first item in the vector.
   */
  T &front();

  /**
   * \fn back()
   * \returns `&` to last item.
   *
   * \brief Reference to last item in the vector.
   */
  T &back();

  /**
   * \fn empty()
   * \returns `true` if vector empty, `false` otherwise.
   */
  bool empty();

  /**
   * \fn size()
   * \returns number of elements in the vector.
   */
  std::size_t size();

  /**
   * \fn push(T &&)
   * \brief Push an item onto the stack, doing an `emplace_back()` in the background.
   */
  void emplace_back(T &&);

  /** \fn begin()|end() Helpers for iterations */
  auto begin();
  auto end();

  /** Use this when actively using container, e.g. find() */
  void lock() { mutex_enter_blocking(&mtx_); }
  void unlock() { mutex_exit(&mtx_); }

  T &operator[](std::size_t n) { return v_[n]; }
  void erase(std::vector<T>::iterator it) { v_.erase(it); }
};

template <typename T>
concurrent_vector<T>::concurrent_vector() { mutex_init(&mtx_); }

template <typename T>
T &concurrent_vector<T>::front()
{
  mutex_enter_blocking(&mtx_);
  auto &f = v_.front();
  mutex_exit(&mtx_);
  return f;
}

template <typename T>
T &concurrent_vector<T>::back()
{
  mutex_enter_blocking(&mtx_);
  auto &f = v_.back();
  mutex_exit(&mtx_);
  return f;
}

template <typename T>
bool concurrent_vector<T>::empty()
{
  mutex_enter_blocking(&mtx_);
  const auto b = v_.empty();
  mutex_exit(&mtx_);
  return b;
}

template <typename T>
void concurrent_vector<T>::emplace_back(T &&e)
{
  mutex_enter_blocking(&mtx_);
  v_.emplace_back(std::forward<T>(e));
  mutex_exit(&mtx_);
}

template <typename T>
std::size_t concurrent_vector<T>::size()
{
  mutex_enter_blocking(&mtx_);
  const auto n = v_.size();
  mutex_exit(&mtx_);
  return n;
}

template <typename T>
auto concurrent_vector<T>::begin()
{
  return v_.begin();
}

template <typename T>
auto concurrent_vector<T>::end()
{
  return v_.end();
}
