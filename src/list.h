#pragma once
#include "slist.h"
#include <iostream>

namespace scheduler
{

template<typename T,typename N> class list
{
public:
  class iterator
  {
  public:
    iterator()
    :m_t{nullptr}
    {}
    iterator(T* t)
    :m_t{t}
    {}
    const T* operator->() const
    {
      return m_t;
    }
    T* operator->()
    {
      return m_t;
    }
    const T* operator*() const
    {
      return m_t;
    }
    T* operator*()
    {
      return m_t;
    }
    iterator& operator++()
    {
      m_t = static_cast<T*>(m_t->N::prev);
      return *this;
    }
    iterator operator++(int)
    {
      iterator it(m_t);
      m_t = static_cast<T*>(m_t->N::prev);
      return it;
    }
    bool operator==(const iterator& it) const
    {
      return m_t == it.m_t;
    }
    bool operator!=(const iterator& it) const
    {
      return m_t != it.m_t;
    }
  private:
    T* m_t;
  };
public:
  list(const list& list) = delete;
  list& operator=(const list& list) = delete;
  list()
  :m_front{nullptr},
  m_back{nullptr}
  {}
  bool empty() const
  {
    return m_front == nullptr;
  }
  void push_front(T* t)
  {
    t->N::prev = m_front;
    t->N::next = nullptr;
    if(m_front != nullptr)
      m_front->N::next = t;
    else
      m_back = t;
    m_front = t;
  }
  void push_back(T* t)
  {
    t->N::prev = nullptr;
    t->N::next = m_back;
    if(m_back != nullptr)
      m_back->N::prev = t;
    else
      m_front = t;
    m_back = t;
  }
  T* pop_front()
  {
    if(m_front == nullptr)
      return nullptr;
    T* t = m_front;
    m_front = static_cast<T*>(m_front->N::prev);
    if(m_front != nullptr)
      m_front->N::next = nullptr;
    else
      m_back = nullptr;
    return t;
  }
  T* pop_back()
  {
    if(m_back == nullptr)
      return nullptr;
    T* t = m_back;
    m_back = m_back->N::next;
    if(m_back != nullptr)
      m_back->N::prev = nullptr;
    else
      m_front = nullptr;
    return t;
  }
  void remove(T* t)
  {
    if(m_front != t)
      static_cast<T*>(t->N::next)->N::prev = t->N::prev;
    else
      m_front = static_cast<T*>(t->N::prev);
    if(m_back != t)
      static_cast<T*>(t->N::prev)->N::next = t->N::next;
    else
      m_back = static_cast<T*>(t->N::next);
  }
  const T* front() const
  {
    return m_front;
  }
  T* front()
  {
    return m_front;
  }
  const T* back() const
  {
    return m_back;
  }
  T* back()
  {
    return m_back;
  }
  iterator begin()
  {
    return iterator(m_front);
  }
  iterator end()
  {
    return iterator();
  }
  void split_to_slist(iterator it,slist<T,N>& list)
  {
    if(it == begin())
    {
      list.m_front = nullptr;
      list.m_back = nullptr;
      return;
    }
    if(it == end())
    {
      list.m_front = m_front;
      list.m_back = m_back;
      m_front = nullptr;
      m_back = nullptr;
      return;
    }
    list.m_front = m_front;
    list.m_back = static_cast<T*>(it->N::next);
    static_cast<T*>(it->N::next)->N::prev = nullptr;
    it->N::next = nullptr;
    m_front = *it;
  }
  template<class FuncIf,class FuncThen,class FuncElse> void move_if_then_else(slist<T,N>& list,FuncIf fun_if,FuncThen fun_then,FuncElse fun_else)
  {
    for(T* t=list.m_front; t!=nullptr;)
    {
      T* prev = static_cast<T*>(t->N::prev);
      if(fun_if(t))
      {
        fun_then(t);
        push_back(t);
      }
      else
        fun_else(t);
      t = prev;
    }
    list.m_front = nullptr;
    list.m_back = nullptr;
  }
private:
  T* m_front;
  T* m_back;
};

}
