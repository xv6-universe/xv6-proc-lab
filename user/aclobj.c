#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

unsigned short lfsr = 0x4434u;
unsigned bit;

unsigned rand() {
  bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
  return lfsr =  (lfsr >> 1) | (bit << 15);
}

void print(const char *s) {
  write(1, s, strlen(s));
}

void fail() {
  print("test failed!\n");
  exit(0);
}

// read / write 1 is disable, 0 is enable
unsigned long generate_acl_key(int idx, int read, int write, int oldkey) {
  unsigned long key = oldkey;
  key = key | ((read ? 0UL : 1UL) << (idx * 2));
  key = key | ((write ? 0UL : 1UL) << (idx * 2 + 1));
  return key;

}

int main(void) {
  int i = 0;
  unsigned long obj_value = readobject(0);
  if(obj_value != 0) fail();
  for(i = 1; i < 32; ++i){
    int ret = writeobject(i, 0x6f6f);
    if(ret != 0) fail();
  }
  int T = 100;
  while(T--){
    for(i = 1; i < 32; ++i){
      int ret = writeacl(0);
      if(ret != 0) fail();
      ret = writeobject(i, 0x6f6f);
      if(ret != 0) fail();
    }
    int idx = rand() % 32;
    int read = rand() % 2;
    int write = rand() % 2;
    unsigned long key = generate_acl_key(idx, read, write, 0);
    int ret = writeacl(key);
    if(ret != 0) fail();
    obj_value = readacl();
    if(obj_value != key) fail();

    if(write && read){
      ret = writeobject(idx, 0x1234);
      if(ret != 0) fail();
      obj_value = readobject(idx);
      if(obj_value != 0x1234) fail();
    } else if(!write && !read) {
      ret = writeobject(idx, 0x1234);
      // if(ret == 0) fail(); You should implement your own error message
      obj_value = readobject(idx);
      if(obj_value > 0) fail();
    } else if(write && !read) {
      ret = writeobject(idx, 0x1234);
      if(ret != 0) fail();
      obj_value = readobject(idx);
      if(obj_value > 0) fail();
      int ret = writeacl(0);
      if(ret != 0) fail();
      obj_value = readobject(idx);
      if(obj_value != 0x1234) fail();
    } else if(!write && read) {
      ret = writeobject(idx, 0x1234);
      // if(ret == 0) fail(); Check your own error here
      obj_value = readobject(idx);
      if(obj_value != 0x6f6f) fail();
      ret = writeacl(0);
      if(ret != 0) fail();
      ret = writeobject(idx, 0x1234);
      if(ret != 0) fail();
      obj_value = readobject(idx);
      if(obj_value != 0x1234) fail();
    }
  }

  print("pretest accepted!\n");
  exit(0);
}
