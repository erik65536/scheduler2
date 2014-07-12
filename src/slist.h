#pragma once

namespace scheduler
{

template<typename T,typename N> class slist
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
      m_t = static_cast<T*>(m_t->N::next);
      return *this;
    }
    iterator operator++(int)
    {
      iterator it(m_t);
      m_t = static_cast<T*>(m_t->N::next);
      return it;
    }
    bool operator==(const iterator& it) const
    {
      return m_t == m_t;
    }
    bool operator!=(const iterator& it) const
    {
      return m_t != m_t;
    }
  private:
    T* m_t;
  };
public:
  slist(const slist& slist) = delete;
  slist& operator=(const slist& slite) = delete;
  slist()
  :m_front{nullptr},
  m_back{nullptr}
  {}
  bool empty() const
  {
    return m_front == nullptr;
  }
  void push_front(T* t)
  {
    t->N::next = m_front;
    m_front = t;
    if(m_back == nullptr)
      m_back = t;
  }
  void push_back(T* t)
  {
    t->N::next = nullptr;
    if(m_back != nullptr)
      m_back->N::next = t;
    if(m_front == nullptr)
      m_front = t;
  }
  T* pop_front()
  {
    T* t = m_front;
    if(m_front != nullptr)
    {
      m_front = static_cast<T*>(m_front->N::next);
      if(m_front == nullptr)
        m_back = nullptr;
    }
    return t;
  }
  T* front()
  {
    return m_front;
  }
  iterator begin()
  {
    return iterator(m_front);
  }
  iterator end()
  {
    return iterator();
  }
  void merge(slist& list)
  {
    if(list.empty())
      return;

    if(empty())
    {
      m_front = list.m_front;
      m_back = list.m_back;
      list.m_front = nullptr;
      list.m_back = nullptr;
      return;
    }

    T* el1 = m_back;
    T* el2 = list.m_back;
    T* l1 = m_front;
    T* l2 = list.m_front;

    if(*l2 < *l1)
    {
      T* back = l2;
      l2 = static_cast<T*>(l2->N::next);
      m_front = back;
      m_back = back;
      if(l2 == nullptr)
      {
        m_back->N::next = l1;
        m_back = el1;
        return;
      }
    }
    else
    {
      T* back = l1;
      l1 = static_cast<T*>(l1->N::next);
      m_front = back;
      m_back = back;
      if(l1 == nullptr)
      {
        m_back->N::next = l2;
        m_back = el2;
        return;
      }
    }

    while(true)
    {
      if(*l2 < *l1)
      {
        T* back = l2;
        l2 = static_cast<T*>(l2->N::next);
        m_back->N::next = back;
        m_back = back;
        if(l2 == nullptr)
        {
          m_back->N::next = l1;
          m_back = el1;
          return;
        }
      }
      else
      {
        T* back = l1;
        l1 = static_cast<T*>(l1->N::next);
        m_back->N::next = back;
        m_back = back;
        if(l1 == nullptr)
        {
          m_back->N::next = l2;
          m_back = el2;
          return;
        }
      }
    }
  }
private:
  T* m_front;
  T* m_back;
};

}
