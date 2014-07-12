#pragma once
#include <cstdio>

namespace scheduler
{

class safe_file_handle
{
public:
  safe_file_handle();
  safe_file_handle(safe_file_handle&& file);
  safe_file_handle(std::FILE* file);
  ~safe_file_handle();
  safe_file_handle& operator=(std::FILE* file);
  operator std::FILE*();
  operator bool() const;
  void close();
  void no_buffer();
  void rewind();
private:
  std::FILE* m_file;
};

}
