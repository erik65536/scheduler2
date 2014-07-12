#pragma once
#include <system_error>
#include <type_traits>
#include <new>
#include <algorithm>
#include <unordered_set>

namespace scheduler
{

template<typename T> class trivial_vector
{
private:
  //static const size_t PARALLEL_SORT_MIN = 1024;
public:
  trivial_vector(size_t capacity)
  :m_array{nullptr},
  m_size{0},
  m_capacity{capacity}
  {
    //not supported by GCC
    //static_assert(std::is_trivially_copyable<T>::value,"Type must be trivially copyable.");
    static_assert(std::is_trivially_destructible<T>::value,"Type must be trivially destructable.");
    if(capacity == 0)
      throw std::system_error(std::make_error_code(std::errc::invalid_argument),"Zero capacity trivial_vector not supported.");
    m_array = static_cast<T*>(std::realloc(nullptr,sizeof(T)*capacity));
    if(m_array == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
  }
  ~trivial_vector()
  {
    free(m_array);
  }
  bool empty() const
  {
    return m_size == 0;
  }
  size_t size() const
  {
    return m_size;
  }
  size_t capacity() const
  {
    return m_capacity;
  }
  void clear()
  {
    m_size = 0;
  }
  const T& operator[](size_t i) const
  {
    return m_array[i];
  }
  T& operator[](size_t i)
  {
    return m_array[i];
  }
  const T* data() const
  {
    return m_array;
  }
  T* data()
  {
    return m_array;
  }
  const T* begin() const
  {
    return m_array;
  }
  T* begin()
  {
    return m_array;
  }
  const T* end() const
  {
    return m_array+m_size;
  }
  T* end()
  {
    return m_array+m_size;
  }
  void push_back(const T& t)
  {
    if(m_size == m_capacity)
      grow();
    new(m_array+(m_size++))T(t);
  }
  template<class...Args> void emplace_back(Args&&...args)
  {
    if(m_size == m_capacity)
      grow();
    new(m_array+(m_size++))T(args...);
  }
  void unique()
  {
    if(m_size < 2)
      return;
    if(unique_is_sorted())
      unique_sorted();
    else
      unique_unsorted();
  }
  void sort()
  {
    std::sort(begin(),end());
    /*
    size_t nthread = std::thread::hardware_concurrency();
 
    if(m_size < PARALLEL_SORT_MIN || m_size < (16 * nthread))
    {
      std::sort(begin(),end());
      return;
    }

    //ceil
    size_t size = m_size/nthread + ((m_size % nthread) != 0);

    std::unique_ptr<std::thread[]> thread(new std::thread[nthread]);
    for(size_t i=0; i<nthread; ++i)
      thread[i] = std::move(std::thread(&trivial_vector::thread_sort,this,size,i));
    for(size_t i=0; i<nthread; ++i)
    {
      if(thread[i].joinable())
        thread[i].join();
    }
    thread.release();

    size_t start;
    while(size < m_size)
    {
      start = 0;
      while(start+size < m_size)
      {
        std::inplace_merge(begin()+start,begin()+start+size,begin()+std::min(start+size+size,m_size));
        start += size+size;
      }
      size += size;
    }
    */
  }
private:
  //void thread_sort(size_t size,size_t i)
  //{
  //  std::sort(begin()+size*i,begin()+std::min(size*(i+1),m_size));
  //}
  bool unique_is_sorted()
  {
    for(size_t i=0; i<m_size-1; ++i)
    {
      if(m_array[i+1].unique() < m_array[i].unique())
        return false;
    }
    return true;
  }
  void unique_sorted()
  {
    size_t j=0;
    for(size_t i=1; i<m_size; ++i)
    {
      if(m_array[j].unique() == m_array[i].unique())
        continue;
      ++j;
      if(i != j)
        m_array[j] = m_array[i];
    }
    m_size = j+1;
  }
  void unique_unsorted()
  {
    std::unordered_set<decltype(std::declval<T>().unique())> set(m_size);
    size_t j=0;
    for(size_t i=0; i<m_size; ++i)
    {
      if(set.find(m_array[i].unique()) != set.end())
        continue;
      set.insert(m_array[i].unique());
      if(i != j)
        m_array[j] = m_array[i];
      ++j;
    }
    m_size = j;
  }
  void grow()
  {
    size_t capacity = m_capacity+m_capacity;
    T* array = static_cast<T*>(std::realloc(m_array,sizeof(T)*capacity));
    if(array == nullptr)
      throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
    m_array = array;
    m_capacity = capacity;
  }
private:
  T* m_array;
  size_t m_size;
  size_t m_capacity;
};

}
