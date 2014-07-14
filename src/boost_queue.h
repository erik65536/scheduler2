#pragma once
#include "process.h"

namespace scheduler
{

class boost_queue
{
private:
  static const time_run_t BOOST_TIME = 100;
  static const time_run_t MAX_BOOST = 49;
  static const time_run_t BOOST_NONE = ~static_cast<time_run_t>(0);
  static const priority_t BOOST_TABLE[49];
public:
  void get(time_run_t time,boost_slist& list);
  void insert(time_run_t time,boost_slist& list);
  void remove(process* proc);
  void boost(boost_slist& list);
private:
  boost_list m_queue;
};

}
