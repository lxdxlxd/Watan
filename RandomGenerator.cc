#include "RandomGenerator.h"

void RandomGenerator::init(int seed) {
    for (int i = 0; i < hex_size; ++i) {
        srand(seed);
        int random_num_r = (rand() % pool_resource.size());
        int pool_r = pool_resource[random_num_r];
        if (pool_r != 5) {
            int random_num_d = (rand() % pool_dice.size());
            int pool_d = pool_dice[random_num_d];
            struct hexagon H = {pool_r,pool_d};
            hexs.push_back(H);
            pool_dice.erase(pool_dice.begin()+random_num_d);
        } else {
            struct hexagon H = {pool_r,7};// netflix with dice_num 0
            hexs.push_back(H);
        }
        pool_resource.erase(pool_resource.begin()+random_num_r);
    }
}

RandomGenerator::RandomGenerator(int seed):seed0(seed) {
    pool_resource={0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,4,4,4,5};
    pool_dice={2,3,3,4,4,5,5,6,6,8,8,9,9,10,10,11,11,12};
    init(seed0);
}

struct hexagon RandomGenerator::getHex(int i) {
    return hexs[i];
}

int RandomGenerator::getResource(int i) {
    return hexs[i].resources;
}

int RandomGenerator::getDice(int i) {
    return hexs[i].dice_num;
}

vector<int> RandomGenerator::output_resource() {
    vector<int> Resourse;
    for (int i = 0; i < 19; ++i) {
        Resourse.push_back(getResource(i));
    }
    return Resourse;
}

vector<int> RandomGenerator::output_value() {
    vector<int> Dice;
    for (int i = 0; i < 19; ++i) {
        Dice.push_back(getDice(i));
    }
    return Dice;
}


vector<int> RandomGenerator::output_player_resource(int i){
    return vector<int>(5,0);
}

vector<int> RandomGenerator::output_player_goal(int i){
    return vector<int>{};
}

vector<string> RandomGenerator::output_cri(){
    
    return vector<string>(54,"");
}

vector<string> RandomGenerator::output_goal(){
    return vector<string>(72,"");
}

vector<int> RandomGenerator::output_player_cri(int i){
    return vector<int>{};
}

vector<int> RandomGenerator::output_player_critype(int i){
    return vector<int>{};
}
int RandomGenerator::output_curturn(){
    return 0;
}

int RandomGenerator::output_geese(){
    return -1;
}

