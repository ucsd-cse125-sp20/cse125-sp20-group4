#pragma once
#include <string>
#define START_STATE 0
#define READY_STATE 1
#define ROUND_STATE 2
#define END_STATE 3
class Phase
{
public:
    int dirty;
    int state; // 0 = start, 1 = ready, 2 = round, 3 = end
    int health;
    int count;
    int wave;
    // timer end??
    Phase();
	Phase(int st, int hp, int cnt, int wave);
    std::string serialize() const;
    void update(Phase&);
};