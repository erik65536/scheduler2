#include "arrival_queue.h"

namespace scheduler
{

arrival_queue::arrival_queue(safe_file_handle&& file,sparse_vector<process>& procs)
:m_file(std::move(file)),
m_process(procs)
{}

void arrival_queue::get(time_run_t time,boost_slist& list)
{
  while(true)
  {
    const process_input& in = m_file.get();
    if(in.arrival() != time)
      return;
    m_file.next();
    list.push_back(m_process.alloc(in.arrival(),in.burst(),in.pid(),in.priority()));
  }
}

}
