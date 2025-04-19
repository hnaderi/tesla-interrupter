#ifndef INPUTS_H_
#define INPUTS_H_

#include "data.hpp"

class InputReader {
public:
  InputReader();
  void read(State &state);
};

#endif // INPUTS_H_
