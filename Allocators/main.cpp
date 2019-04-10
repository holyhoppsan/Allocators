#include <iostream>

#include "Allocator.h"
#include "DHAssert.h"
#include "LinearAllocator.h"
#include "PointerMath.h"

struct alignas(16) Foo {
  int i;
  float f;
  char c;
  char e;
};

struct testwith3 {
  char a;
  int b;
};

struct Empty {};

struct alignas(64) Empty64 {};

class Base {
  char a;

  virtual void v(){};
};

class Derrived : public Base {
  virtual void v() override{};
  virtual void w(){};
  virtual void x(){};
};

int main() {
  std::cout << "Allocator tutorial" << std::endl;
  std::cin.get();

  std::cout << "Alignment of \n"
            << "char: " << alignof(char) << std::endl
            << "Pointer: " << alignof(int*) << std::endl
            << "class Foo: " << alignof(Foo) << std::endl
            << "empty class: " << alignof(Empty) << std::endl
            << "alignas (64) Empty: " << alignof(Empty64) << std::endl
            << "sizeof size_t : " << sizeof(size_t) << std::endl
            << "align of testwith3: " << sizeof(testwith3) << std::endl
            << "Sizeof base: " << sizeof(Base) << std::endl
            << "Sizeof Derrived: " << sizeof(Derrived) << std::endl
            << "Align forward 0x5637 with 4 byte alignment "
            << PointerMath::alignForward((void*)0x5637, 4) << std::endl;

  void* Memory = malloc(1024);

  Allocators::LinearAllocator Allocator(1024, Memory);

  int* NewInt = Allocators::AllocateNew<int>(Allocator);

  Allocator.Clear();
  free(Memory);

  std::cin.get();

  return 0;
}