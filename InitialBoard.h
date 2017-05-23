#ifndef INITIALBOARD_H
#define INITIALBOARD_H
#include <vector>
#include <string>

using namespace std;

class InitialBoard{
public:
    virtual ~InitialBoard()=default;
    virtual vector<int> output_resource()=0;
    virtual vector<int> output_value()=0;
    virtual vector<string> output_cri()=0;
    virtual vector<string> output_goal()=0;
    virtual vector<int> output_player_resource(int)=0;
    virtual vector<int> output_player_goal(int)=0;
    virtual vector<int> output_player_cri(int)=0;
    virtual vector<int> output_player_critype(int)=0;
    virtual int output_curturn()=0;
    virtual int output_geese()=0;
};


#endif //INITIALBOARD_H


