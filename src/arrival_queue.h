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
  static const size_t PROCESS_START = 1024;
public:
  arrival_queue(safe_file_handle&& file);
  bool empty() const;
  void get(time_run_t time,boost_slist& list);
private:
  bool m_empty;
  file_read<process_input,INPUT_BUFFER> m_file;
  sparse_vector<process> m_process;
};

}
