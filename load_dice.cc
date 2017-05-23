#include "load_dice.h"
#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
using namespace std;

int LoadDice::roll(){
    string str;
    while(1){
        cout<<"Input a roll between 2 and 12:"<<endl;
        cout<<"> ";
        if(!(cin>>str)) throw "eof";
        if(!check_int(str)) {cout<<"Invalid input."<<endl; continue;}
        int temp=convert_int(str);
        if(temp>12||temp<2) {cout<<"Invalid input."<<endl; continue;}
        return temp;
    }
}


bool LoadDice::check_int(string s){
    for(int i=0;i<s.length();++i){
        if(s[i]<48||s[i]>57)
            return false;
    }
    return true;
}

int LoadDice::convert_int(string s){
    stringstream ss(s);
    int temp;
    ss>>temp;
    return temp;
}


