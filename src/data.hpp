#ifndef DATA_H_
#define DATA_H_

enum Mod { Fixed, Staccato };
enum Speed { Slow, Fast };

struct State {
  Mod mod = Fixed;
  Speed speed = Slow;
  bool enabled = false;

  unsigned int freq, ontime;
};

struct PWMView {
  unsigned int freq, duty;
};

struct StaccatoView {
  unsigned int skip, ontime;
};

#endif // DATA_H_
