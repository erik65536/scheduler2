#include "input_parser.h"
#include "scheduler.h"
#include <iostream>

void print_help();
bool parse_arguments(int narg,const char** arg,uint64_t& quantum,const char*& input_file,bool& output);
bool iequals(const char* s1,const char* s2);
bool string_to_uint64(const char* string,uint64_t& value);
namespace scheduler
{
  bool schedule(uint64_t quantum,const char* input_file,bool output);
  void get_input(const char* input_file,safe_file_handle& out);
}

int main(int argc,const char** argv)
{
  uint64_t quantum;
  const char* input_file;
  bool output;

  if(!parse_arguments(argc,argv,quantum,input_file,output))
  {
    print_help();
    return -1;
  }
  return scheduler::schedule(quantum,input_file,output) ? 0 : -1;
}

namespace scheduler
{

bool schedule(uint64_t quantum,const char* input_file,bool output)
{
  safe_file_handle arrivals;
  try
  {
    get_input(input_file,arrivals);
    std::cout << "\r                                                                                    \r" << std::flush;
    scheduler s(quantum,std::move(arrivals));
    s.run();
  }
  catch(std::system_error& e)
  {
    std::cerr << std::endl << e.what() << std::endl;
    return false;
  }
  return true;
}

void get_input(const char* input_file,safe_file_handle& out)
{
  trivial_vector<process_input> processes(256);
  std::cout << "\rParsing processes from " << input_file << "...                                        \r" << std::flush;
  {
    input_parser input(input_file);
    input.parse(processes);
  }
  std::cout << "\rRemoving displicate PIDs from " << processes.size() << " processes...                 \r" << std::flush;
  processes.unique();
  std::cout << "\rSorting " << processes.size() << " processes...                                       \r" << std::flush;
  processes.sort();
  std::cout << "\rWriting " << processes.size() << " processes to a temporary file...                   \r" << std::flush;
  out = std::tmpfile();
  if(!out)
    throw std::system_error(std::make_error_code(std::errc::permission_denied),"Unable to create temporaty file");
  out.no_buffer();
  if(std::fwrite(processes.data(),sizeof(process_input),processes.size(),out) != processes.size())
    throw std::system_error(std::make_error_code(std::errc::no_space_on_device));
}

}

bool string_to_uint64(const char* string,uint64_t& value)
{
  const uint8_t* str = reinterpret_cast<const uint8_t*>(string);
  uint64_t ch = *(str++);
  if(ch == 0)
    return false;
  value = ch-'0';
  if(value > 9)
    return false;
  while((ch = *(str++)) != 0)
  {
    ch -= '0';
    if(ch > 9)
      return false;
    value = 10*value + ch;
  }
  return true;
}

bool iequals(const char* s1,const char* s2)
{
  char c1 = *(s1++);
  char c2 = *(s2++);
  while(c1 != 0 && c2 != 0)
  {
    if(std::tolower(c1) != std::tolower(c2))
      return false;
    c1 = *(s1++);
    c2 = *(s2++);
  }
  return c1 == c2;
}

void print_help()
{
  std::cout << "scheduler -q [quantum] -i [process file] -n|-o" << std::endl
    << "  --help     Print help." << std::endl
    << "   -h" << std::endl
    << "  --quantum  Time quantum." << std::endl
    << "   -q" << std::endl
    << "  --input    Input file." << std::endl
    << "   -i" << std::endl
    << "  --out      Output to files. (default)" << std::endl
    << "   -o" << std::endl
    << "  --noout    Do not output to files." << std::endl
    << "   -n" << std::endl;
}

bool parse_arguments(int narg,const char** arg,uint64_t& quantum,const char*& input_file,bool& output)
{
  if(narg == 1)
    return false;

  bool is_quantum = false;
  bool is_input = false;
  bool is_noout = false;
  bool is_out = false;

  output = true;
  for(int i=1; i<narg; ++i)
  {
    if(iequals("-h",arg[i]) || iequals("--h",arg[i]) ||
       iequals("-help",arg[i]) || iequals("--help",arg[i]) ||
       iequals("help",arg[i]))
    {
      return false;
    }
    else if(iequals("-q",arg[i]) || iequals("--quantum",arg[i]))
    {
      if(is_quantum)
      {
        std::cerr << "Error: Quantum specified multiple times." << std::endl;
        return false;
      }
      if(++i >= narg)
      {
        std::cerr << "Error: No quantum specified." << std::endl;
        return false;
      }
      if(!string_to_uint64(arg[i],quantum))
      {
        std::cerr << "Error: Quantum is not a positive integer." << std::endl;
        return false;
      }
      if(quantum == 0)
      {
        std::cerr << "Error: Quantum is zero." << std::endl;
        return false;
      }
      is_quantum = true;
    }
    else if(iequals("-i",arg[i]) || iequals("--input",arg[i]))
    {
      if(is_input)
      {
        std::cerr << "Error: Input file specified multiple times." << std::endl;
        return false;
      }
      if(++i >= narg)
      {
        std::cerr << "Error: Input file not specified." << std::endl;
        return false;
      }
      input_file = arg[i];
      is_input = true;
    }
    else if(iequals("-o",arg[i]) || iequals("--out",arg[i]))
    {
      if(is_out)
      {
        std::cerr << "Error: Output to file specified multiple times." << std::endl;
        return false;
      }
      if(is_noout)
      {
        std::cerr << "Error: Output to file conflicts with no output to file." << std::endl;
        return false;
      }
      is_out = true;
    }
    else if(iequals("-n",arg[i]) || iequals("--noout",arg[i]))
    {
      if(is_noout)
      {
        std::cerr << "Error: No output to file specified multiple times." << std::endl;
        return false;
      }
      if(is_out)
      {
        std::cerr << "Error: No output to file conflicts with output to file." << std::endl;
        return false;
      }
      output = false;
      is_noout = true;
    }
    else
    {
      std::cerr << "Error: Unrecognized Option: " << arg[i] << std::endl;
      return false;
    }
  }
  if(!is_quantum)
  {
    std::cerr << "Error: No quantum specified." << std::endl;
    return false;
  }
  if(!is_input)
  {
    std::cerr << "Error: No input file specified." << std::endl;
    return false;
  }
  return true;
}
