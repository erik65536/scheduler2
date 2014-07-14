#include "run_queue.h"
#include <iostream>

namespace scheduler
{

void run_queue::insert(boost_slist& list)
{
  for(process* proc : list)
  {
    priority_t priority = proc->priority();
    run_list& queue = m_queue[priority];
    if(queue.empty())
      m_nonempty.insert(priority);
    queue.push_back(proc);
  }
}

void run_queue::remove(boost_slist& list)
{
  for(process* proc : list)
  {
    priority_t priority = proc->priority();
    run_list& queue = m_queue[priority];
    queue.remove(proc);
    if(queue.empty())
      m_nonempty.erase(priority);
  }
}

process* run_queue::top()
{
  auto it = m_nonempty.rbegin();
  if(it == m_nonempty.rend())
    return nullptr;
  priority_t priority = *it;
  run_list& queue = m_queue[priority];
  process* proc = queue.pop_front();
  if(queue.empty())
    m_nonempty.erase(priority);
  return proc;
}

}
