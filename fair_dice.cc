#include "fair_dice.h"
#include <string>
#include <cstdlib>
#include <ctime>

int FairDice::roll(){
    srand(time(NULL));
    int a=(rand() % 6)+1;
    int b=(rand() % 6)+1;
    return a+b;
}

