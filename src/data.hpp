#ifndef DATA_H_
#define DATA_H_

struct State {
  unsigned int interval, length, ac;
  unsigned long now, lastEdge;
  bool fixed;
  bool signal;
};

#endif // DATA_H_
