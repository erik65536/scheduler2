#include "process.h"
#include <fstream>

namespace scheduler
{

class output_event
{
  static const char* FILE_NAME;
public:
  output_event();
  ~output_event();
  void event(time_run_t start,time_run_t stop,pid_t pid,priority_t priority);
private:
  std::ofstream m_file;
};

}
