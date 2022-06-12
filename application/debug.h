#pragma once

#include <iostream>
using namespace std;

#define FUNCTION_ERROR_CONTROL(__FUNCTION_CALL__, ERROR_RETURN_VAL) \
  {                                                                 \
    if (__FUNCTION_CALL__ == -1) {                                  \
      cout << "Error in " << #__FUNCTION_CALL__ << endl;            \
      return ERROR_RETURN_VAL;                                      \
    }                                                               \
  }
