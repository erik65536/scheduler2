#pragma once
#include <new>
#include <type_traits>
#include <system_error>

namespace scheduler
{

template<typename T> class sparse_vector
{
private:
  static const size_t NARRAY_MAX = 32;
public:
  sparse_vector(size_t capacity)
  :m_free{nullptr},
  m_size{0},
  m_capacity{capacity},
  m_narray{0},
  m_array{}
  {
    //not supported by GCC
    //static_assert(std::is_trivially_copyable<T>::value,"Type must be trivially copyable.");
    static_assert(std::is_trivially_destructible<T>::value,"Type must be trivially destructable.");

    if(capacity == 0)
      throw std::system_error(std::make_error_code(std::errc::invalid_argument),"Zero capacity sparse_vector not supported.");

    m_free = static_cast<T*>(std::realloc(nullptr,sizeof(T*)*capacity));
    if(m_free == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));

    m_array[m_narray] = static_cast<T*>(std::malloc(sizeof(T)*capacity));
    if(m_array[m_narray] == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    ++m_narray;

    for(size_t i=0; i<capacity; ++i)
      m_free[i] = m_array[m_narray-1]+i;
  }
  ~sparse_vector()
  {
    free(m_free);
    for(size_t i=0; i<m_narray; ++i)
      free(m_array[i]);
  }
  bool empty() const
  {
    return m_size == 0;
  }
  size_t size() const
  {
    return m_size;
  }
  template<class...Args> T* alloc(Args&&...args)
  {
    if(m_size == m_capacity)
      grow();
    T* t = m_free[m_size++];
    new(t)T(args...);
    return t;
  }
  void free(T* t)
  {
    m_free[--m_size] = t;
  }
private:
  void grow()
  {
    if(m_narray == NARRAY_MAX)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));

    m_array[m_narray] = static_cast<T*>(std::malloc(sizeof(T)*m_capacity));
    if(m_array[m_narray] == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    ++m_narray;

    size_t* free = static_cast<size_t*>(std::realloc(m_free,sizeof(size_t)*(m_capacity+m_capacity)));
    if(free == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    m_free = free;

    for(size_t i=0; i<m_capacity; ++i)
      m_free[m_capacity+i] = m_array[m_narray-1]+i;

    m_capacity += m_capacity;
  }
private:
  T* m_free;
  size_t m_size;
  size_t m_capacity;
  size_t m_narray;
  T* m_array[NARRAY_MAX];
};

}
