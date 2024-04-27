#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print(const char *s) {
  write(1, s, strlen(s));
}

void fail(){
  print("test failed!\n");
  exit(0);
}

int main(void) {
  unsigned long obj_value = readobject(0);
  printf("first write test\n");
  if(obj_value != 0) {
    printf("obj_value: %lu\n", obj_value);
    fail();
  } else {
    printf("passed!\n");
  }
  int ret = writeobject(0, 0x6f6f);
  printf("first write test\n");
  if(ret != 0) {
    printf("ret: %d\n", ret);
    fail();
  } else {
    printf("passed!\n");
  }
  obj_value = readobject(0);
  printf("read object 0\n");
  if(obj_value != 0x6f6f) {
    fail();
  } else {
    printf("passed!\n");
  }
  int i = 0;
  for(i = 1; i < 32; ++i){
    obj_value = readobject(i);
    if(obj_value != 0) fail();
    int ret = writeobject(i, 0x6f6f);
    if(ret != 0) fail();
    obj_value = readobject(i);
    if(obj_value != 0x6f6f) fail();
    ret = writeobject(i, 0);
    if(ret != 0) fail();
  }
  print("pretest accepted!\n");
  exit(0);
}
