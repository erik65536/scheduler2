#include "scheduler.h"

namespace scheduler
{

scheduler::scheduler(time_run_t quantum,safe_file_handle&& arrivals)
:m_process(PROCESS_INIT_STORAGE),
m_arrival(std::move(arrivals),m_process),
m_running(quantum,m_process)
{}

void scheduler::run()
{
  time_run_t time = 0;
  boost_slist list;
  boost_slist arrive;
  bool arrivals = true;
  while(!m_process.empty() || arrivals)
  {
    m_boost.get(time,list);
    m_run.remove(list);
    m_boost.boost(list);

    m_running.get(time,list);

    if(arrivals)
    {
      try
      {
        m_arrival.get(time,arrive);
      }
      catch(std::system_error& e)
      {
        if(e.code() != std::errc::io_error)
          throw;
        arrivals = false;
      }
      list.merge(arrive);
    }

    m_run.insert(list);
    m_boost.insert(time,list);

    if(m_running.empty())
    {
      process* proc = m_run.top();
      if(proc != nullptr)
      {
        m_boost.remove(proc);
        m_running.run(time,proc);
      }
    }

    ++time;
  }
}

}
