#ifndef Board_h
#define Board_h
#include <vector>
#include <string>
#include <memory>
#include "Database.h"
#include "Observer.h"
using namespace std;


class Board{
    vector<int> resource_seq;
    vector<int> value_seq;
    vector<string> criterion;
    vector<string> goal;
    int geese_pos;
    
    shared_ptr<WatanData> data;
    
    // private helpers:
    void update();  // update data from the database before print
    void print_horizontal_side(int &,int &) const;
    void print_vertical_side(int &,int&,bool)const;
    void print_slash(char,char,string,bool)const;
    void print_between(int total_space, int &value,bool flag) const;
    // overload for printing a string
    void print_between(int total_space, string res, bool flag) const;
public:
    Board(shared_ptr<WatanData>);
    ~Board();
    void printboard();
};

#endif /* Board_h */

