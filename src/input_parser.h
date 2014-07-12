#pragma once
#include "process_input.h"
#include "trivial_vector.h"
#include "file_read.h"
#include <string>

namespace scheduler
{

class input_parser
{
private:
  static const size_t BUFFER_SIZE = 1024*1024;
  static const uint32_t COL_CHAR_MAX = 12;
  static const priority_t MAX_PRIORITY = 99;
public:
  input_parser(const char* file_name);
  void parse(trivial_vector<process_input>& procs);
private:
  //bool string_to_uint32(const uint8_t* str,uint32_t len,uint32_t& value);
  uint8_t* string_to_uint32(uint8_t* str,uint32_t& value);
private:
  file_read<uint8_t,BUFFER_SIZE> m_file;
};

}
