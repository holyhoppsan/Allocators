#pragma once

#include <iostream>

#ifndef _DEBUG
#define DH_ASSERT(expression) ((void)0)
#else
#define DH_ASSERT(expression)                                              \
  do {                                                                     \
    if (!(expression)) {                                                     \
      std::cerr << "Assert:" << #expression << ", " << __FUNCTION__ << "," \
                << __LINE__ << "," << __FILE__ << std::endl;               \
      __debugbreak();                                                      \
    }                                                                      \
  } while (0)
#endif

#ifndef _DEBUG
#define DH_ASSERT_MESSAGE(expression, message) ((void)0)
#else
#define DH_ASSERT_MESSAGE(expression, message)                         \
  do {                                                                 \
    if (!(expression)) {                                                 \
      std::cerr << "Assert:" << message << ", " << __FUNCTION__ << "," \
                << __LINE__ << "," << __FILE__ << std::endl;           \
      __debugbreak();                                                  \
    }                                                                  \
  } while (0)
#endif