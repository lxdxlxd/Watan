#ifndef AI_h
#define AI_h

#include <vector>
#include "Player.h"
using namespace std;


struct BestCri {
    int pos;
    vector<int> goal_required;
};


class AI: public Player {
    vector<int> available_cri_opt_value;
    vector<int> goal_available;
    vector<int> value;
    void search_goal();
    vector<int> search_value(int);
    vector<int> value_sum();
    int max_cri_loc();
    
    vector<int> search_cri_available();
    bool is_cri_available(int);
    void updateBest();
    vector<int> search_goal_near_cri(int);
    void geese();
    BestCri best_cri;
public:
    AI(Player &);
    ~AI();
    //void updateBest();
    void AIrun();
    void achieve(int);
    void complete(int);
    void improve(int);
};

#endif /* AI_h */

