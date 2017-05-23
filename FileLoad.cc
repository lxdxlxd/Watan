#include "FileLoad.h"
#include <fstream>
#include <sstream>
#include <iostream>

FileLoad::FileLoad(string fn, string type):filename(fn),Loadtype(type),
geese(-1),cur_turn(0),criterion(54,""),goal(72,""),resource(19),value(19),
player_resource(4,vector<int>(5,0)){
    parse();
}


void FileLoad::parse(){
    ifstream file;
    file.open(filename);
    string s;
    if(Loadtype=="board"){
        getline(file,s);
        stringstream ss(s);
        parse_board(ss);
    }
    else if(Loadtype=="load"){
        for (int i=0; i<7; ++i) {
            getline(file,s);
            if(i==6) {geese=string_int(s);break;}
            if(i==0) cur_turn=string_int(s);
            else if(i!=5){
                stringstream ss(s);
                parse_resource(ss,i-1);
                parse_goal(ss, i-1);
                parse_cri(ss, i-1);
            }
            else{
                stringstream ss(s);
                parse_board(ss);
                if(file.peek()==EOF) {geese=-1;break;}
            }
        }
    }
    file.close();
}


int FileLoad::string_int(string s){
    stringstream ss(s);
    int n;
    ss>>n;
    return n;
}


void FileLoad::parse_board(stringstream &ss){
    string s;
    for(int i=0;i<19;++i){
        ss>>s;
        resource[i]=string_int(s);
        ss>>s;
        value[i]=string_int(s);
    }
}

void FileLoad::parse_resource(stringstream &ss, int player){
    string s;
    for(int i=0;i<5;++i){
        ss>>s;
        player_resource[player][i]=string_int(s);
    }
}

void FileLoad::parse_goal(stringstream &ss,int player){
    string s;
    while(ss>>s){
        if(s=="c") return;
        int temp=string_int(s);
        player_goal[player].emplace_back(temp);
        switch (player) {
            case 0: goal[temp]="BA"; break;
            case 1: goal[temp]="RA"; break;
            case 2: goal[temp]="OA"; break;
            case 3: goal[temp]="YA"; break;
        }
    }
}

void FileLoad::parse_cri(stringstream &ss,int player){
    string s;
    while (ss>>s) {
        int temp1=string_int(s);
        ss>>s;
        int temp2=string_int(s);
        player_cri[player].emplace_back(temp1);
        player_type[player].emplace_back(temp2);
        string temp;
        switch (temp2) {
            case 1: temp="A"; break;
            case 2: temp="M"; break;
            case 3: temp="E"; break;
        }
        switch (player) {
            case 0: criterion[temp1]="B"+temp; break;
            case 1: criterion[temp1]="R"+temp; break;
            case 2: criterion[temp1]="O"+temp; break;
            case 3: criterion[temp1]="Y"+temp; break;
        }
    }
}


vector<int> FileLoad::output_resource(){
    return resource;
}

vector<int> FileLoad::output_value(){
    return value;
}

vector<string> FileLoad::output_cri(){
    return criterion;
}

vector<string> FileLoad::output_goal(){
    return goal;
}

vector<int> FileLoad::output_player_resource(int i){
    return player_resource[i];
}

vector<int> FileLoad::output_player_goal(int i){
    return player_goal[i];
}

vector<int> FileLoad::output_player_cri(int i){
    return player_cri[i];
}

vector<int> FileLoad::output_player_critype(int i){
    return player_type[i];
}

int  FileLoad::output_curturn(){
    return cur_turn;
}


int FileLoad::output_geese(){
    return geese;
}


