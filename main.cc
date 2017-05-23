#include "Watan.h"
#include <vector>
#include <string.h>
#include <iostream>
#include <sstream>
using namespace std;

bool check_int(string);

int convert_int(string);

vector<bool> extra_feature_parsing();

int main(int argc, const char * argv[]) {
    string cur_command="seed";
    int seed_num = 0;
    string file_name="";
    for (int i=1; i<argc; ++i) {
        if(i%2==1){
            if(!strcmp(argv[i], "-seed")){
                if(!check_int(argv[i+1])){
                    cout<<"Invalid command line arguments."<<endl;
                    exit(1);
                }
                if(i==1){
                    seed_num=convert_int(argv[i+1]);
                }
            }
            else if(!strcmp(argv[i], "-load")){
                if(cur_command=="seed"){
                    cur_command="load";
                    file_name=argv[i+1];
                }
            }
            else if(!strcmp(argv[i], "-board")){
                if(cur_command=="seed"){
                    cur_command="board";
                    file_name=argv[i+1];
                }
            }
            else{
                cout<<"Invalid command line arguments."<<endl;
                exit(1);
            }
        }
    }
    string command;
    Watan w(argc!=1?cur_command:"",file_name,seed_num,extra_feature_parsing());
    try{
        w.run();
    }
    catch(const char *msg){
        while(1){
            cout<<"> ";
            if(!(cin>>command)) exit(1);
            if(command=="yes"){
                Watan w_another("",file_name,seed_num,extra_feature_parsing());
                w_another.run();
            }
            else if(command=="no")
                return 0;
        }
    }
}


vector<bool> extra_feature_parsing(){
    vector<bool> extra_feature{false,false};
    cout<<"Do you want to enable trading with bank? (y/n)"<<endl;
    string command;
    while(1){
        cout<<"> ";
        if(!(cin>>command)) exit(1);
        if(command=="y"){
            extra_feature[0]=true;
            break;
        }
        else if(command=="n"){
            extra_feature[0]=false;
            break;
        }
        else
            cout<<"Invalid command."<<endl;
    }
    cout<<"Do you want to enable AI feature? (y/n)"<<endl;
    while(1){
        cout<<"> ";
        if(!(cin>>command)) exit(1);
        if(command=="y"){
            extra_feature[1]=true;
            break;
        }
        else if(command=="n"){
            extra_feature[1]=false;
            break;
        }
        else
            cout<<"Invalid command."<<endl;
    }
    return extra_feature;
}


bool check_int(string s){
    for(int i=0;i<s.length();++i){
        if(s[i]<48||s[i]>57)
            return false;
    }
    return true;
}

int convert_int(string s){
    stringstream ss(s);
    int temp;
    ss>>temp;
    return temp;
}

