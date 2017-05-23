#ifndef Watan_h
#define Watan_h
#include <vector>
#include <string>
#include <utility>
#include "Board.h"
#include "Player.h"
#include "InitialBoard.h"
#include "RandomGenerator.h"
#include "FileLoad.h"
#include "Dice.h"
using namespace std;


class Watan {
    bool init_type=false; // load: true non-load: false
    unique_ptr<InitialBoard> init0; // for initializing;
    shared_ptr<WatanData> data;  // database for Watan
    
    int cur_turn;
    Board b;
    vector<shared_ptr<Player>> player_set{4};
    
    vector<bool> extra_feature;
    
    // private helpers:
    
    bool check_int(string);
    int convert_int(string);
    
    void begin_game();
    void begin_turn(int);
    void end_turn(int);
    
    void save(string)const;
    
public:
    Watan(string, string,int=0,vector<bool> ={false,1} );
    unique_ptr<InitialBoard> creat(string, string, int);
    void run();
    
    // Test
};

#endif /* Watan_h */

