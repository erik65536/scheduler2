#pragma once

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
    const T& operator*() const
    {
      return *m_t;
    }
    T& operator*()
    {
      return *m_t;
    }
    iterator& operator++()
    {
      m_t = m_t->N::next;
      return *this;
    }
    iterator operator++(int)
    {
      iterator it(m_t);
      m_t = m_t->N::next;
      return it;
    }
    bool operator==(const iterator& it) const
    {
      return m_t == it.m_t;
    }
    bool operator!=(const iterator& it) const
    {
      return m_t == it.m_t;
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
    t->N::prev = nullptr;
    t->N::next = m_front;
    if(m_front != nullptr)
      m_front->N::prev = t;
    m_front = t;
    if(m_back == nullptr)
      m_back = t;
  }
  void push_back(T* t)
  {
    t->N::prev = m_back;
    t->N::next = nullptr;
    if(m_back != nullptr)
      m_back->N::next = t;
    m_back = t;
    if(m_front == nullptr)
      m_front = t;
  }
  T* pop_front()
  {
    T* t = m_front;
    if(t == nullptr)
      return t;
    m_front = m_front->N::next;
    if(m_front != nullptr)
      m_front->N::prev = nullptr;
    else
      m_back = nullptr;
    return t;
  }
  T* pop_back()
  {
    T* t = m_back;
    if(t == nullptr)
      return t;
    m_back = m_back->N::prev;
    if(m_back != nullptr)
      m_back->N::next = nullptr;
    else
      m_front = nullptr;
    return t;
  }
  void remove(T* t)
  {
    if(m_front == t)
      m_front = t->N::next;
    if(m_back == t)
      m_back = t->N::prev;
    if(t->N::next != nullptr)
      t->N::next->N::prev = t->N::prev;
    if(t->N::prev != nullptr)
      t->N::prev->N::next = t->N::next;
  }
  T* front()
  {
    return m_front;
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
private:
  T* m_front;
  T* m_back;
};

}
