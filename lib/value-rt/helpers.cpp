#include <inttypes.h>
#include <map>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iterator>

using namespace std;
#include "SparseMemory.hh"
#include "helpers.h"
#include <map>

std::ofstream instruction_stream("instruction_stream.inst");

value_store getValue(void *ptr, vty typ)
{
  value_store v;
  switch (typ)
  {
  case FLT:
    v.f32 = *((float *)ptr);
    break;
  case DBL:
    v.d64 = *((double *)ptr);
    break;
  case BOL:
    v.i8 = *((uint8_t *)ptr);
    break;
  case CHR:
    v.i8 = *((uint8_t *)ptr);
    break;
  case HLF:
    v.i16 = *((uint16_t *)ptr);
    break;
  case WHL:
    v.i32 = *((uint32_t *)ptr);
    break;
  case LNG:
    v.i64 = *((uint64_t *)ptr);
  case PTR:
    if (sizeof(void *) == 8)
      v.ptr = *((uint64_t *)ptr);
    else
      v.ptr = *((uint32_t *)ptr);
    break;
  default:
    v.i32 = 0;
    std::cout << "unkown";
    break;
  }
  return v;
}

static std::map<char *, address *> addrMap;
SparseMemory<entry> InitMap(5);
SparseMemory<entry> FinalMap(5);
std::string ArgRet;

FILE *fp_in = 0;
FILE *fp_out = 0;
FILE *fp_succ = 0;
FILE *fp_mlog = 0;

extern "C" void __attribute__((__noinline__))
__InstruMem_load(void *ptr, uint64_t size, vty typ, uint64_t id)
{
  asm("");
  uint32_t addr_val = reinterpret_cast<intptr_t>(ptr);
  Address addr(addr_val);
  physical_address_t offset;
  offset = addr.getOffset();
  addr.makeLineAddress();

  entry *directoryentry = InitMap.lookup(addr);

  ///* Check if memory already initialized */
  if (!directoryentry)
  {
    entry *new_entry = new entry();
    InitMap.add(addr, new_entry);
    directoryentry = new_entry;
  }

  /* Overwrite address */
  directoryentry->address = addr.getLineAddress();
  /* Write the bytes of the memory if not already initialized */
  for (size_t i = 0; i < size; i++)
  {
    /* If already logged. don't log again */
    if (!directoryentry->flag[offset + i])
    {
      value_store val(ptr, typ);
      directoryentry->flag.set(offset + i, 1);
      directoryentry->m_bytes[offset + i] = val.m_bytes[i];
    }
  }

  //Print to instruction stream
  // std::cout << value << "\n";
  value_store val(ptr, typ);
  std::stringstream addr_stream;
  addr_stream << std::hex << addr_val;
  std::string addr_str(addr_stream.str());

  switch (typ)
  {
  case WHL:
    instruction_stream << "[ID: " << id << "]"
                       << " [LOAD] "
                       << "[Address: 0X" << addr_str << "] [Value: " << val.i32 << "]\n";
    break;
  case LNG:
    instruction_stream << "[ID: " << id << "]"
                       << " [LOAD] "
                       << "[Address: 0X" << addr_str << "] [Value: " << val.i64 << "]\n";
    break;
  case FLT:
    instruction_stream << "[ID: " << id << "]"
                       << " [LOAD] "
                       << "[Address: 0X" << addr_str << "] [Value: " << val.f32 << "]\n";
    break;
  case DBL:
    instruction_stream << "[ID: " << id << "]"
                       << " [LOAD] "
                       << "[Address: 0X" << addr_str << "] [Value: " << val.d64 << "]\n";
    break;
  default:
    instruction_stream << "[ID: " << id << "]"
                       << " [LOAD] "
                       << "[Address: 0X" << addr_str << "] [Value: UKNOWN FORMAT]\n";    break;
  }
}

extern "C" void __attribute__((__noinline__))
__InstruMem_store(char *ptr, uint64_t size, vty typ, uint64_t id)
{
  asm("");
  uint32_t addr_val = reinterpret_cast<intptr_t>(ptr);
  Address addr(addr_val);
  physical_address_t offset;
  offset = addr.getOffset();
  addr.makeLineAddress();

  /*  */

  entry *directoryentry = FinalMap.lookup(addr);

  ///* Check if memory already initialized */
  if (!directoryentry)
  {
    entry *new_entry = new entry();
    FinalMap.add(addr, new_entry);
    directoryentry = new_entry;
  }

  // std::function<void(entry &, std::ostream &)> callback = &entry::dump;
  // callback(*directoryentry, std::cout);
  /* Overwrite address */
  directoryentry->address = addr.getLineAddress();
  /* Write the bytes of the memory if not already initialized */
  for (size_t i = 0; i < size; i++)
  {
    // Always update   if (!directoryentry->flag[offset + i])
    {
      value_store val(ptr, typ);
      directoryentry->flag.set(offset + i, 1);
      directoryentry->m_bytes[offset + i] = val.m_bytes[i];
    }
  }

  //Print to instruction stream
  std::stringstream addr_stream;
  addr_stream << std::hex << addr_val;
  std::string addr_str(addr_stream.str());
  instruction_stream << "[ID: " << id << "]"
                     << " [STORE] "
                     << "[Address: 0X" << addr_str << "]\n";
}
void PrintVal(uint64_t *val, vty t)
{
  void *valptr = (void *)val;

  if (t == FLT)
    fprintf(stderr, "%f", *((float *)valptr));
  else if (t == DBL)
    fprintf(stderr, "%lf", *((double *)valptr));
  else if (t == BOL)
    fprintf(stderr, "0x%x", *((int *)valptr));
  else if (t == CHR)
    fprintf(stderr, "%c", *((char *)valptr));
  else if (t == HLF)
    return;
  else if (t == WHL)
    fprintf(stderr, "0x%x", *((int *)valptr));
  else if (t == LNG)
    fprintf(stderr, "0x%lx", *((long *)valptr));
  else if (t == PTR)
    fprintf(stderr, "0x%x", *((unsigned *)valptr));
}

extern "C" void __attribute__((__noinline__))
__PrintArg(char *ptr, uint64_t no, vty typ)
{
  asm("");
  value_store val((uint64_t *)ptr, typ);
  ArgRet += "Arg" + to_string(no) + ":";
  stringstream s;
  val.dump(s, typ);
  ArgRet += s.str();
  ArgRet += "\n\n";
  //  fprintf(stderr, "\targ%ld\n", no);

} //__PrintArg

extern "C" void __attribute__((__noinline__)) __PrintRet(char *ptr, vty typ)
{
  asm("");
  value_store val((void *)ptr, typ);
  ArgRet += "Ret:";
  stringstream s;
  val.dump(s, typ);
  ArgRet += s.str();
  ArgRet += "\n";
} //__PrintRet

extern "C" void __attribute__((__noinline__)) __Fini()
{
  asm("");
  ofstream Argsfile;
  Argsfile.open("Args.mem");
  Argsfile << std::hex << ArgRet;
  Argsfile.close();

  ofstream Initfile;
  Initfile.open("Init.mem");
  InitMap.tear(&entry::dump, Initfile);
  Initfile.close();

  ofstream Finalfile;
  Finalfile.open("Final.mem");
  FinalMap.tear(&entry::dumpwrites, Finalfile);
  Finalfile.close();
}
