#ifndef fair_dice_h
#define fair_dice_h

#include "Dice.h"

class FairDice: public Dice{
public:
    ~FairDice()=default;
    int roll() override;
};


#endif /* fair_dice_h */

