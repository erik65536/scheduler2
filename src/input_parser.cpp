#include "input_parser.h"
#include <stdexcept>

namespace scheduler
{

input_parser::input_parser(const char* file_name)
:m_file(file_name)
{}

/*
bool input_parser::string_to_uint32(const uint8_t* str,uint32_t len,uint32_t& value)
{
  if(len == 0)
    return false;
  value = str[0]-'0';
  if(value > 9)
    return false;
  uint32_t ch;
  for(size_t i=1; i<len; ++i)
  {
    ch = str[i]-'0';
    if(ch > 9)
      return false;
    value = 10*value + ch;
  }
  return true;
}
*/

uint8_t* input_parser::string_to_uint32(uint8_t* str,uint32_t& value)
{
  value = *str++;
  if(value == 0)
    return nullptr;
  value = value-'0';
  if(value > 9)
    return nullptr;
  uint32_t ch;
  while((ch = *str++) != 0)
  {
    ch = ch-'0';
    if(ch > 9)
      return nullptr;
    value = 10*value+ch;
  }
  return str;
  /*
  uint32_t ch = *str++;
  if(ch == 0)
    return nullptr;
  value = ch-'0';
  if(value > 9)
    return nullptr;
  while((ch = *str++) != 0)
  {
    ch = ch-'0';
    if(ch > 9)
      return nullptr;
    value = 10*value+ch;
  }
  return str;
  */
}

void input_parser::parse(trivial_vector<process_input>& procs)
{
  uint32_t value[4];
  uint32_t col;

  while(true)
  {
    col = 0;
    value[0] = 0;
    value[1] = 0;
    value[2] = 0;
    value[3] = 0;
    while(true)
    {
      uint32_t ch = m_file.get();
      m_file.next();
      if(ch-'0' <= 9)
        value[col] = value[col]*10+(ch-'0');
      else if(ch == '\t')
      {
        ++col;
      }
      else if(ch == '\r' || ch == '\n')
        break;
      }
    }
  }

  bool eof = false;
  uint8_t str[128];
  uint32_t cur;
  while(!eof)
  {
    try
    {
      cur = 0;
      while(true)
      {
        str[cur++] = m_file.get();
        m_file.next();
        switch(str[cur-1])
        {
        case '\n':
        case '\r':
          break;
        default:
          continue;
        }
        break;
      }
    }
    catch(std::system_error& e)
    {
      if(e.code() != std::errc::io_error)
        throw;
      eof = true;
    }
    str[cur] = 0;
  }

  /*
  uint8_t str[4][COL_CHAR_MAX];
  //uint8_t str[4*COL_CHAR_MAX+4];
  uint32_t nstr[4];
  uint32_t col;
  uint8_t ch;
  bool eof = false;
  uint8_t* cur_col;
  uint8_t* end_col;
  while(!eof)
  {
    try
    {
      col = 0;
      cur_col = str[col];
      end_col = cur_col+COL_CHAR_MAX;
      while(true)
      {
        ch = m_file.get();
        m_file.next();
        switch(ch)
        {
        case '\n':
        case '\r':
          break;
        case '\t':
          nstr[col] = cur_col-str[col];
          ++col;
          if(col < 4)
          {
            cur_col = str[col];
            end_col = cur_col+COL_CHAR_MAX;
          }
          else
          {
            do
            {
              ch = m_file.get();
              m_file.next();
            }
            while(ch != '\r' && ch != '\n');
            break;
          }
          continue;
        default:
          if(cur_col < end_col)
            *cur_col++ = ch;
          continue;
        }
        break;
      }
    }
    catch(std::system_error& e)
    {
      if(e.code() != std::errc::io_error)
        throw;
      eof = true;
    };
    //if(col == 3)
    //  nstr[col] = cur_col-str[col];
    //else if(col < 3)
    //  continue;
    cur_col = str;
    cur_col = string_to_uint32(cur_col,pid);
    if(cur_col == nullptr)
      continue;
    cur_col = string_to_uint32(cur_col,burst);
    if(cur_col == nullptr)
      continue;
    cur_col = string_to_uint32(cur_col,arrival);
    if(cur_col == nullptr)
      continue;
    cur_col = string_to_uint32(cur_col,priority);
    if(cur_col == nullptr)
      continue;
    //if(!string_to_uint32(str[0],nstr[0],pid))
    //  continue;
    //if(!string_to_uint32(str[1],nstr[1],burst))
    //  continue;
    //if(!string_to_uint32(str[2],nstr[2],arrival))
    //  continue;
    //if(!string_to_uint32(str[3],nstr[3],priority))
    //  continue;
    if(burst == 0)
      continue;
    if(priority > MAX_PRIORITY)
      continue;
    procs.emplace_back(static_cast<time_input_t>(arrival),
      static_cast<time_input_t>(burst),
      static_cast<pid_t>(pid),
      static_cast<priority_t>(priority));
  }
  */
}

}
