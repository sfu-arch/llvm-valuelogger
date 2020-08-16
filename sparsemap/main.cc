#include "Address.hh"
#include "SparseMemory.hh"
#include <llvm/ADT/BitVector.h>
#include <vector>

typedef enum
{
   FLT,
   DBL,
   BOL,
   CHR,
   HLF,
   WHL,
   LNG,
   PTR,
   INVALID
} vty;

typedef union {
   void *ptr;
   uint8_t i8;
   uint16_t i16;
   uint32_t i32;
   float f32;
   uint8_t m_bytes[sizeof(float)];
} value_store;

struct entry
{
   uint8_t m_bytes[4096];
   bitset<4096> flag;
   entry()
   {
      memset(m_bytes, 0, 4096);
      flag.reset();
   };

   void printbytes()
   {
      for (size_t i = 0; i < 4096; i++)
      {
         //    printf("x%.2x \n", m_bytes[i]);
      }

      cout << " Flags \n ";
      cout << flag.to_string();
   };
};
int main(int argc, char const *argv[])
{

   SparseMemory<entry> S2(5);
   entry *a_entry = new entry();
   Address addr(0x5010);
   physical_address_t offset;
   offset = addr.getOffset();
   value_store v;
   v.f32 = 5.24;
   //   printf("%lld offset ", offset);
   a_entry->m_bytes[offset] = v.m_bytes[0];
   a_entry->m_bytes[offset + 1] = v.m_bytes[1];
   a_entry->m_bytes[offset + 2] = v.m_bytes[2];
   a_entry->m_bytes[offset + 3] = v.m_bytes[3];
   a_entry->flag.set(offset, offset + 3);
   addr.makeLineAddress();
   S2.add(addr, a_entry);
   a_entry = S2.lookup(addr);
   if (a_entry)
      a_entry->printbytes();
   return 0;
}
