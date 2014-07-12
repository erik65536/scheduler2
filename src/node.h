#pragma once

namespace scheduler
{

template<typename TAG> class node
{
  template<typename A,typename B> friend class list;
  template<typename A,typename B> friend class slist;
private:
  void* prev;
  void* next;
};

}
