#include "arrival_queue.h"

namespace scheduler
{

arrival_queue::arrival_queue(safe_file_handle&& file)
:m_file(std::move(file)),
m_process(PROCESS_START)
{}

bool arrival_queue::empty() const
{
  return m_empty;
}

void arrival_queue::get(time_run_t time,boost_slist& list)
{
  if(m_empty)
    return;
}

}
