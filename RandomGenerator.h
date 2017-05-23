#ifndef RandomGenerator_h
#define RandomGenerator_h

#include "InitialBoard.h"
#include <vector>
#include <ctime>
#include <stdlib.h>

using namespace std;
struct hexagon {
    int resources;
    //0:caffeine   size:4
    //1:lab        size:4
    //2:lecture    size:4
    //3:study      size:3
    //4:tutorial   size:3
    //5:netflix    size:1
    int dice_num;
    //one 2
    //one 12
    //two 3-6
    //two 8-11
    
};


class RandomGenerator: public InitialBoard{
    std::vector<struct hexagon> hexs;
    std::vector<int> pool_resource;//size = 19
    std::vector<int> pool_dice;//size = 18
    int seed0;
    int hex_size=19;
    
    // private helper
    void init(int seed);
    struct hexagon getHex(int i);
    int getResource(int i);
    int getDice(int i);
public:
    RandomGenerator(int seed=time(NULL));
    ~RandomGenerator() = default;
    vector<int> output_resource() override;
    vector<int> output_value() override;
    vector<string> output_cri() override;
    vector<string> output_goal() override;
    vector<int> output_player_resource(int) override;
    vector<int> output_player_goal(int) override;
    vector<int> output_player_cri(int) override;
    vector<int> output_player_critype(int) override;
    int output_curturn() override;
    int output_geese() override;
};


#endif /* RandomGenerator_h */

