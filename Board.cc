#include "Board.h"
#include <iostream>
#include <iomanip>
using namespace std;


Board::Board(shared_ptr<WatanData>d): data(d){}

Board::~Board() {}


void Board::update(){
    resource_seq=data->get_resource();
    value_seq=data->get_value();
    criterion=data->get_cri();
    goal=data->get_goal();
    geese_pos=data->get_geese();
}



// global constants representing spaces before a line begins
const int front_space1=35;
const int front_space2=20;
const int front_space3=5;
const int inside_space=16;
const vector<string> table={"CAFFEINE","LAB","LECTURE","STUDY","TUTORIAL",
    "NETFLIX"};

void Board::printboard(){
    update();
    int row=0;
    int cur_vertice=0;
    int cur_side=0;
    int tile_num=0;
    int geese_cur=0;
    int resource_pos=0; string res="";
    int value_pos=0;  int value=0;
    while(row<41){
        // print horizontal sides
        if(row==0||row==40){
            cout<<string(front_space1,' ');
            print_horizontal_side(cur_vertice, cur_side);
        }
        else if(row==4||row==36){
            cout<<string(front_space2,' ');
            print_horizontal_side(cur_vertice, cur_side);
            // avoid passing a ref of a vector element
            value=value_seq[value_pos++];
            print_between(inside_space, value, value!=7);
            print_horizontal_side(cur_vertice, cur_side);
        }
        else if(row%8==0){
            cout<<string(front_space3,' ');
            print_horizontal_side(cur_vertice, cur_side);
            // avoid passing a ref of a vector element
            value=value_seq[value_pos++];
            print_between(inside_space, value, value!=7);
            print_horizontal_side(cur_vertice, cur_side);
            value=value_seq[value_pos++];
            print_between(inside_space, value, value!=7);
            print_horizontal_side(cur_vertice, cur_side);
        }
        else if(row%4==0){
            cout<<"|";
            cout<<setfill(' ')<<setw(2);
            if(criterion[cur_vertice]=="")
                cout<<cur_vertice++;
            else
                cout<<criterion[cur_vertice++];
            cout<<"|";
            // avoid passing a ref of a vector element
            value=value_seq[value_pos++];
            print_between(inside_space, value, value!=7);
            print_horizontal_side(cur_vertice, cur_side);
            value=value_seq[value_pos++];
            print_between(inside_space, value, value!=7);
            print_horizontal_side(cur_vertice, cur_side);
            value=value_seq[value_pos++];
            print_between(inside_space, value, value!=7);
            cout<<"|";
            cout<<setfill(' ')<<setw(2);
            if(criterion[cur_vertice]=="")
                cout<<cur_vertice++;
            else
                cout<<criterion[cur_vertice++];
            cout<<"|";
        }
        // print vertical sides (only numbers)
        else if(row==2||row==38){
            cout<<string(front_space1-2,' ');
            print_vertical_side(cur_side,tile_num,row==2);
        }
        else if(row==6||row==34){
            cout<<string(front_space2-2,' ');
            print_vertical_side(cur_side,tile_num,row==6);
            print_between(inside_space-3, tile_num, row==34);
            print_vertical_side(cur_side,tile_num,row==6);
        }
        else if(row%2==0){
            cout<<string(front_space3-2,' ');
            print_vertical_side(cur_side,tile_num,row%8==2);
            print_between(inside_space-3, tile_num, row%8==6);
            print_vertical_side(cur_side,tile_num,row%8==2);
            print_between(inside_space-3, tile_num, row%8==6);
            print_vertical_side(cur_side,tile_num,row%8==2);
        }
        // print slashes ('/' and '\')
        else if(row==1||row==3){
            cout<<string(row%4==1?front_space1:front_space1-2,' ');
            res=table[resource_seq[resource_pos]];
            if(row==3) resource_pos++;
            print_slash('/','\\', res,row%4==1);
        }
        else if(row==37||row==39){
            cout<<string(row%4==3?front_space1:front_space1-2,' ');
            if(row==37){
                cout<<"\\";
                cout<<string(5,' ');
                cout<<(geese_pos==geese_cur++?"GEESE":string(5,' '));
                cout<<string(inside_space-10,' ');
                cout<<"/";
            }
            else
                print_slash('\\','/', "",row%4-2==1);
        }
        else if(row==5||row==7){
            cout<<string(row%4==1?front_space2:front_space2-2,' ');
            res=table[resource_seq[resource_pos]];
            if(row==7) resource_pos++;
            print_slash('/','\\', res,row%4==1);
            if(row==5){
                cout<<string(5,' ');
                cout<<(geese_pos==geese_cur++?"GEESE":string(5,' '));
                cout<<string(inside_space-10,' ');
            }
            else
                cout<<string(row%4==1?inside_space:inside_space-4,' ');
            res=table[resource_seq[resource_pos]];
            if(row==7) resource_pos++;
            print_slash('/','\\', res,row%4==1);
        }
        else if(row==33||row==35){
            cout<<string(row%4==3?front_space2:front_space2-2,' ');
            if(row==33){
                cout<<"\\";
                cout<<string(5,' ');
                cout<<(geese_pos==geese_cur++?"GEESE":string(5,' '));
                cout<<string(inside_space-10,' ');
                cout<<"/";
            }
            else
                print_slash('\\','/', "",row%4-2==1);
            res=table[resource_seq[resource_pos]];
            if(row==35) resource_pos++;
            print_between(row%4==3?inside_space:inside_space-4,res,row%4==3);
            if(row==33){
                cout<<"\\";
                cout<<string(5,' ');
                cout<<(geese_pos==geese_cur++?"GEESE":string(5,' '));
                cout<<string(inside_space-10,' ');
                cout<<"/";
            }
            else
                print_slash('\\','/', "",row%4-2==1);
        }
        else if(row==9||row==11||row==17||row==19||row==25||row==27){
            cout<<string(row%4==1?front_space3:front_space3-2,' ');
            res=table[resource_seq[resource_pos]];
            if(row%4==3) resource_pos++;
            print_slash('/','\\', res,row%4==1);
            if(row%4==1){
                cout<<string(5,' ');
                cout<<(geese_pos==geese_cur++?"GEESE":string(5,' '));
                cout<<string(inside_space-10,' ');
            }
            else
                cout<<string(row%4==1?inside_space:inside_space-4,' ');
            res=table[resource_seq[resource_pos]];
            if(row%4==3) resource_pos++;
            print_slash('/','\\', res,row%4==1);
            if(row%4==1){
                cout<<string(5,' ');
                cout<<(geese_pos==geese_cur++?"GEESE":string(5,' '));
                cout<<string(inside_space-10,' ');
            }
            else
                cout<<string(row%4==1?inside_space:inside_space-4,' ');
            res=table[resource_seq[resource_pos]];
            if(row%4==3) resource_pos++;
            print_slash('/','\\', res,row%4==1);
        }
        else{
            cout<<string(row%4==3?front_space3:front_space3-2,' ');
            if(row%4==1){
                cout<<"\\";
                cout<<string(5,' ');
                cout<<(geese_pos==geese_cur++?"GEESE":string(5,' '));
                cout<<string(inside_space-10,' ');
                cout<<"/";
            }
            else
                print_slash('\\','/', "",row%4-2==1);
            res=table[resource_seq[resource_pos]];
            if(row%4==3) resource_pos++;
            print_between(row%4==3?inside_space:inside_space-4,res,row%4==3);
            if(row%4==1){
                cout<<"\\";
                cout<<string(5,' ');
                cout<<(geese_pos==geese_cur++?"GEESE":string(5,' '));
                cout<<string(inside_space-10,' ');
                cout<<"/";
            }
            else
                print_slash('\\','/', "",row%4-2==1);
            res=table[resource_seq[resource_pos]];
            if(row%4==3) resource_pos++;
            print_between(row%4==3?inside_space:inside_space-4,res,row%4==3);
            if(row%4==1){
                cout<<"\\";
                cout<<string(5,' ');
                cout<<(geese_pos==geese_cur++?"GEESE":string(5,' '));
                cout<<string(inside_space-10,' ');
                cout<<"/";
            }
            else
                print_slash('\\','/', "",row%4-2==1);
        }
        ++row;
        cout<<endl;
    }
}



// print each horizontal side (ex: | 0|-- 0--| 1| )
// use references of int to keep the number increasing.
void Board::print_horizontal_side(int &cur_vertice,int &cur_side)const{
    cout<<"|";
    cout<<setfill(' ')<<setw(2);
    if(criterion[cur_vertice]=="")
        cout<<cur_vertice++;
    else
        cout<<criterion[cur_vertice++];
    cout<<"|";
    cout<<"--";
    cout<<setfill(' ')<<setw(2);
    if(goal[cur_side]=="")
        cout<<cur_side++;
    else
        cout<<goal[cur_side++];
    cout<<"--";
    cout<<"|";
    cout<<setfill(' ')<<setw(2);
    if(criterion[cur_vertice]=="")
        cout<<cur_vertice++;
    else
        cout<<criterion[cur_vertice++];
    cout<<"|";
}


// print each vertical side (ex: 1              2)
// use references of int to keep the number increasing.
void Board::print_vertical_side(int &cur_side,int &tile_num,bool flag)const{
    cout<<setfill(' ')<<setw(2);
    if(goal[cur_side]=="")
        cout<<cur_side++;
    else
        cout<<goal[cur_side++];
    print_between(inside_space-3, tile_num,flag);
    cout<<setfill(' ')<<setw(2);
    if(goal[cur_side]=="")
        cout<<cur_side++;
    else
        cout<<goal[cur_side++];
}


// print slashes ('/' and '\')
void Board::print_slash(char first,char second,string res,bool flag)const{
    cout<<first;
    if(flag==true)
        print_between(inside_space-4, res, false);
    else
        print_between(inside_space, res, res==""?false:true);
    cout<<second;
}


// print the value and spaces between two numbers
void Board::print_between(int total_space, int &value, bool flag)const{
    cout<<string(total_space-(total_space-2)/2-2,' ');
    if(flag==true)
        cout<<setfill(' ')<<setw(2)<<value++;
    else
        cout<<"  ";
    cout<<string((total_space-2)/2,' ');
}


// print the string and spaces between two numbers
void Board::print_between(int total_space, string res, bool flag)const{
    if(flag==true){
        cout<<string(5,' ');
        cout<<res;
        cout<<string(total_space-res.size()-5,' ');
    }
    else
        cout<<string(total_space,' ');
}


