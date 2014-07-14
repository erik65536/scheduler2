#include "output_event.h"
#include <system_error>

namespace scheduler
{

const char* output_event::FILE_NAME = "process.js";

output_event::output_event()
{
  m_file.open(FILE_NAME,std::ios_base::trunc);
  if(!m_file)
    throw std::system_error(std::make_error_code(std::errc::permission_denied),FILE_NAME);
  m_file << "var g_event=[";
}

output_event::~output_event()
{
  m_file << "];";
}

void output_event::event(time_run_t start,time_run_t stop,pid_t pid,priority_t priority)
{
  m_file << '['
    << start << ','
    << stop << ','
    << pid << ','
    << static_cast<uint32_t>(priority) << "]," << std::endl;
}

}
