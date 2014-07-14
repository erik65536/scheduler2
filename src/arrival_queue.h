#pragma once
#include "file_read.h"
#include "process_input.h"
#include "process.h"
#include "sparse_vector.h"

namespace scheduler
{

class arrival_queue
{
private:
  static const size_t INPUT_BUFFER = 256;
public:
  arrival_queue(safe_file_handle&& file,sparse_vector<process>& procs);
  void get(time_run_t time,boost_slist& list);
private:
  file_read<process_input,INPUT_BUFFER> m_file;
  sparse_vector<process>& m_process;
};

}
