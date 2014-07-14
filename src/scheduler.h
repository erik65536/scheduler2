#pragma once
#include "arrival_queue.h"
#include "boost_queue.h"

namespace scheduler
{

class scheduler
{
private:
  static const size_t PROCESS_INIT_STORAGE = 1024;
public:
  scheduler(time_run_t quantum,safe_file_handle&& arrivals);
  void run();
private:
  sparse_vector<process> m_process;
  arrival_queue m_arrival;
  boost_queue m_boost;
  //run_queue m_run;
  //running m_running;
};

}
