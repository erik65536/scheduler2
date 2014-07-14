#pragma once
#include "process.h"
#include "sparse_vector.h"
#include "output_event.h"

namespace scheduler
{

class running
{
public:
  running(time_run_t quantum,sparse_vector<process>& process);
  bool empty() const;
  void get(time_run_t time,boost_slist& list);
  void run(time_run_t time,process* proc);
private:
  output_event m_out_event;
  sparse_vector<process>& m_process;
  time_run_t m_quantum;
  process* m_running;
  time_run_t m_stop;
};

}
