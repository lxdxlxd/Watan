#ifndef FileLoad_h
#define FileLoad_h
#include "InitialBoard.h"
using namespace std;

class FileLoad: public InitialBoard{
    string filename;
    string Loadtype;
    
    // For Board
    vector<int> resource;
    vector<int> value;
    vector<string> criterion{54};
    vector<string> goal{72};
    
    // For Player
    int cur_turn;
    int geese;
    vector<vector<int>> player_resource{4};
    vector<vector<int>> player_goal{4};
    vector<vector<int>> player_cri{4};
    vector<vector<int>> player_type{4};
    
    // Private helpers:
    void parse();
    void parse_board(stringstream&);
    void parse_goal(stringstream&,int);
    void parse_cri(stringstream&,int);
    void parse_resource(stringstream&,int);
    int string_int(string s);
public:
    FileLoad(string,string);
    ~FileLoad()=default;
    // For Board
    vector<int> output_resource() override;
    vector<int> output_value() override;
    vector<string> output_cri() override;
    vector<string> output_goal() override;
    // For Player
    vector<int> output_player_resource(int) override;
    vector<int> output_player_goal(int) override;
    vector<int> output_player_cri(int) override;
    vector<int> output_player_critype(int) override;
    int output_curturn() override;
    int output_geese() override;
};

#endif /* FileLoad_h */

