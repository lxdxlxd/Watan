#ifndef load_dice_h
#define load_dice_h

#include "Dice.h"

class LoadDice: public Dice {
    // Private helpers:
    bool check_int(string);
    int convert_int(string);
public:
    ~LoadDice()=default;
    int roll() override;
};


#endif /* load_dice_h */

