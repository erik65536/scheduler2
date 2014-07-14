#include "running.h"

namespace scheduler
{

running::running(time_run_t quantum,sparse_vector<process>& process)
:m_process(process),
m_quantum{quantum},
m_running{nullptr}
{}

bool running::empty() const
{
  return m_running == nullptr;
}

void running::get(time_run_t time,boost_slist& list)
{
  if(m_running == nullptr)
    return;
  if(time != m_stop)
    return;
  if(m_running->remaining() != 0)
    list.insert_sorted(m_running);
  else
    m_process.free(m_running);
  m_running = nullptr;
}

void running::run(time_run_t time,process* proc)
{
  m_running = proc;
  time_run_t remaining = proc->remaining();
  time_run_t run = std::min(m_quantum,remaining);
  remaining -= run;
  m_stop = time + run;
  proc->remaining(remaining);

  m_out_event.event(time,m_stop,proc->pid(),proc->priority());

  time_run_t priority = proc->priority();
  priority_t pri_diff = priority - proc->base_priority();
  run = std::min(run,static_cast<time_run_t>(255));
  priority -= std::min(pri_diff,static_cast<priority_t>(run));
  proc->priority(priority);
}

}
