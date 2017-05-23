#ifndef Database_h
#define Database_h
#include <vector>
#include <string>
#include "Observer.h"
using namespace std;

class WatanData:public Observer {
    // neighbour data:
    vector<vector<int>> cri_cri;
    vector<vector<int>> goal_goal;
    vector<vector<int>> goal_cri;
    vector<vector<int>> tile_cri;
    
    // criterion and goal data:
    vector<int> resource_seq;//related to tile num
    vector<int> value_seq;//related to tile num
    vector<string> criterion;
    vector<string> goal;


    int geese_pos;//-1 if geese is not placed else it respond to the tile num geese at
    
    // private helper functions
    void construct_cri();   // called in ctor for constructing cri
    void construct_goal();  // called in ctor for constructing goal
    void construct_cri_goal(); // called in ctor for constructing cri_goal
    void construct_tile_goal(); // called in ctor for constructing tile_goal
    int search(int,const vector<int>&) const; //usefule search helper
    void toggle(int &);
public:
    WatanData(vector<int>,vector<int>,vector<string>,vector<string>,int);
    ~WatanData();
    void being_notified(Type,int,Subject&) override;
    
    // for accessing data from outside class
    const vector<vector<int>>& get_c_c()const;
    const vector<vector<int>>& get_g_g()const;
    const vector<vector<int>>& get_g_c()const;
    const vector<vector<int>>& get_t_c()const;
    const vector<int>& get_resource()const;
    const vector<int>& get_value()const;
    const vector<string>& get_cri()const;
    const vector<string>& get_goal()const;
    int get_geese()const;
    void set_geese(int i);
};

#endif /* Database_h */


