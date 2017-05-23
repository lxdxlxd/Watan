#ifndef Dice_h
#define Dice_h
#include <string>

using namespace std;

class Dice {
public:
    virtual ~Dice()=default;
    virtual int roll()=0;
};



#endif /* Dice_h */

