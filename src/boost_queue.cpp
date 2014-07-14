#include "boost_queue.h"

namespace scheduler
{

const priority_t boost_queue::BOOST_TABLE[49] = {10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,49,49,49,49,49,49,49,49,49};

void boost_queue::get(time_run_t time,boost_slist& list)
{
  auto it = m_queue.begin();
  while(it != m_queue.end())
  {
    if(it->boost() != time)
      break;
  }
  m_queue.split_to_slist(it,list);
}

void boost_queue::insert(time_run_t time,boost_slist& list)
{
  time_run_t boost = time + BOOST_TIME;
  m_queue.move_if_and(list,
    [](process* proc){return proc->priority() < MAX_BOOST;},
    [boost](process* proc){proc->boost(boost);});
}

void boost_queue::boost(boost_slist& list)
{
  for(process* p : list)
    p->priority(BOOST_TABLE[p->priority()]);
  //list.sort();
}

}
