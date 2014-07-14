#pragma once
#include "process.h"
#include <set>

namespace scheduler
{

class run_queue
{
private:
  static const priority_t NQUEUE = 100;
public:
  void insert(boost_slist& list);
  void remove(boost_slist& list);
  process* top();
private:
  run_list m_queue[NQUEUE];
  std::set<priority_t> m_nonempty;
};

}
