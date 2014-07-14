#include "scheduler.h"
#include <iostream>

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
    std::cout << "start" << std::endl;
    m_boost.get(time,list);
    std::cout << "boost.get" << std::endl;
    m_run.remove(list);
    std::cout << "run.remove" << std::endl;
    m_boost.boost(list);
    std::cout << "boost.boost" << std::endl;

    m_running.get(time,list);
    std::cout << "runnning.get" << std::endl;

    if(arrivals)
    {
      try
      {
        m_arrival.get(time,arrive);
        std::cout << "arrivals.get" << std::endl;
        list.merge(arrive);
        std::cout << "list.merge" << std::endl;
      }
      catch(std::system_error& e)
      {
        if(e.code() != std::errc::io_error)
          throw;
        arrivals = false;
      }
    }

    m_run.insert(list);
    std::cout << "run.insert" << std::endl;
    m_boost.insert(time,list);
    std::cout << "boost.insert" << std::endl;

    if(m_running.empty())
    {
      process* proc = m_run.top();
      std::cout << "run.top" << proc << std::endl;
      if(proc != nullptr)
      {
        m_boost.remove(proc);
        std::cout << "boost.remove" << std::endl;
        m_running.run(time,proc);
        std::cout << "running.run" << std::endl;
      }
    }

    ++time;
  }
}

}
