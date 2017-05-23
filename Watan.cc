#include <string.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Watan.h"
#include "AI.h"
#include "Info.h"
using namespace std;




Watan::Watan(string choice,string filename,int n,vector<bool> flag):
init0(creat(choice, filename, n)),extra_feature(flag),
cur_turn(init0->output_curturn()),
data(make_shared<WatanData>(init0->output_resource(),init0->output_value(),
                            init0->output_cri(),init0->output_goal(),
                            init0->output_geese())),b(data){
    for(int i=0;i<4;++i){
        player_set[i]=make_shared<Player>(i,
                                          init0->output_player_resource(i),
                                          init0->output_player_cri(i),
                                          init0->output_player_critype(i),
                                          init0->output_player_goal(i),data);
    }
    // make Players observer to each other
    for(int i=0;i<4;++i){
        for(int j=0;j<4;j++){
            if(j!=i)
                player_set[i]->attach(player_set[j]);
        }
        player_set[i]->attach(data);
    }
}


void Watan::run (){
    try{
        while(1){
            if(!init_type){
                begin_game();
                init_type=true;
            }
            if(cur_turn%2==0)
                begin_turn(cur_turn/2);
            end_turn(cur_turn/2);
        }
    }
    catch(char const *msg){
        if(!strcmp(msg, "Victory!"))
            throw "Victory!";
    }
}


void Watan::save(string name)const{
    ofstream file;
    file.open(name);
    file<<cur_turn<<endl;
    for(int i=0;i<4; ++i) {
        player_set[i]->player_save(file);
    }
    for(int i=0;i<19;++i){
        file<<data->get_resource()[i]<<" ";
        file<<data->get_value()[i]<<" ";
    }
    file<<endl;
    file<<data->get_geese();
}


void Watan::begin_game(){
    string loc;
    int name=0;
    string p;
    int flag=1;
    b.printboard();
    while (1) {
        switch (name) {
            case 0: p="BLUE"; break;
            case 1: p="RED"; break;
            case 2: p="ORANGE"; break;
            case 3: p="YELLOW"; break;
        }
        cout<<"Student "<<p;
        cout<<", where do you want to complete an Assignment?"<<endl;
        cout<<"> ";
        if(!(cin>>loc)) {
            //save("backup.sv");
            cout<<endl;
            throw "eof";
        }
        if(!check_int(loc)) {cout<<"Invalid input."<<endl; continue;}
        int temp=convert_int(loc);
        if(temp>53) {cout<<"Invalid input."<<endl; continue;}
        try {
            player_set[name]->complete(temp, true);
            name+=flag;
            if(name==4) {flag=-1;name=3;}
            b.printboard();
        }
        catch (char const *msg) {
            cout<<msg<<endl;
        }
        if(name==-1) break;
    }
}


void Watan::begin_turn(int t){
    string command;
    int name=player_set[t]->getInfo().name;
    string p;
    switch (name) {
        case 0: p="BLUE"; break;
        case 1: p="RED"; break;
        case 2: p="ORANGE"; break;
        case 3: p="YELLOW"; break;
    }
    cout << "Student "<<p<< "'s turn." << endl;
    player_set[t]->print_status();
    while (1){
        cout<<"> ";
        if(!(cin>>command)) {
            cout<<endl;
            save("backup.sv");
            throw "eof";
        }
        if (command=="board")
            b.printboard();
        else if (command=="status"){
            for(int i=0;i<4;++i){
                player_set[i]->print_status();
            }
        }
        else if (command=="criteria")
            player_set[t]->print_criteria();
        else if (command=="load")
            player_set[t]->set_dice_type("load");
        else if (command=="fair")
            player_set[t]->set_dice_type("fair");
        else if (command=="roll"){
            int dicenum;
            try{
                dicenum=player_set[t]->get_roll();
                cout<<"you roll "<<dicenum<<endl;
                
            }
            catch(char const *msg){
                save("backup.sv");
                cout<<endl;
                throw "eof";
            }
            if (dicenum==7) {
                for (int i=0; i<4; ++i) {
                    player_set[i]->lose_to_geese();
                }
                cout<<"Choose where to place the GEESE"<<endl;
                try{
                    player_set[t]->geese();
                }
                catch(char const *msg){
                    save("backup.sv");
                    cout<<endl;
                    throw "eof";
                }
            }
            else{
                for (int i=0; i<4; ++i) {
                    player_set[i]->acquire_resource(dicenum);
                }
            }
            cur_turn++;
            break;
        }
        else if(extra_feature[1]==true&&command=="exit"){
            for(int i=0;i<4;++i){
                if(i!=t){
                    player_set[i]->swap(i<t?t-1:t,player_set[t]->create());
                }
            }
        }
        else
            cout<<"Invalid command."<<endl;
    }
}


void Watan::end_turn(int t){
    string command;
    string p;
    switch (t/2) {
        case 0: p="BLUE"; break;
        case 1: p="RED"; break;
        case 2: p="ORANGE"; break;
        case 3: p="YELLOW"; break;
    }
    while (1){
        cout<<"> ";
        if(!(cin>>command)) {
            cout<<endl;
            save("backup.sv");
            throw "eof";
        }
        if (command=="board")
            b.printboard();
        else if (command=="status"){
            for(int i=0;i<4;++i){
                player_set[i]->print_status();
            }
        }
        else if (command=="criteria")
            player_set[t]->print_criteria();
        else if (command=="achieve"){
            string loc;
            cin>>loc;
            if(!check_int(loc)) {cout<<"Invalid input."<<endl; continue;}
            int temp=convert_int(loc);
            if(temp>72) {cout<<"Invalid input."<<endl; continue;}
            try {
                player_set[t]->achieve(temp);
            }
            catch (char const *msg) {
                cout<<msg<<endl;
            }
            b.printboard();
        }
        else if (command=="complete"){
            string loc;
            cin>>loc;
            if(!check_int(loc)) {cout<<"Invalid input."<<endl; continue;}
            int temp=convert_int(loc);
            if(temp>53) {cout<<"Invalid input."<<endl; continue;}
            try {
                player_set[t]->complete(temp, false);
            }
            catch (char const *msg) {
                if(!strcmp(msg,"Victory!")){
                    cout<<"Student "<<p<<" won!"<<endl;
                    cout<<"Would you like to play again? (yes/no)"<<endl;
                    throw "Victory!";
                }
                cout<<msg<<endl;
            }
            b.printboard();
        }
        else if (command=="improve"){
            string loc;
            cin>>loc;
            if(!check_int(loc)) {cout<<"Invalid input."<<endl; continue;}
            int temp=convert_int(loc);
            if(temp>53) {cout<<"Invalid input."<<endl; continue;}
            try {
                player_set[t]->improve(temp);
            }
            catch (char const *msg) {
                if(!strcmp(msg,"Victory!")){
                    cout<<"Student "<<p<<" won!"<<endl;
                    cout<<"Would you like to play again? (yes/no)"<<endl;
                    throw "Victory!";
                }
                cout<<msg<<endl;
            }
            b.printboard();
        }
        else if (command=="trade"){
            string name,give,take;
            while(1){
                cout<<"> ";
                if(!(cin>>name>>give>>take)) {cout<<endl; throw "eof";}
                if((name!="BLUE"&&name!="RED"&&name!="YELLOW"&&name!="ORANGE")||
                   (give!="cafferines"&&give!="labs"&&give!="lectures"&&
                    give!="studies"&&give!="tutorials")||
                   (give!="cafferines"&&give!="labs"&&give!="lectures"&&
                    give!="studies"&&give!="tutorials"))
                    cout<<"Invalid input."<<endl;
                else break;
            }
            player_set[t]->trade(name,give,take,extra_feature[0]);
        }
        else if (command=="next"){
            cur_turn=(cur_turn==7?0:cur_turn+1);
            break;
        }
        else if (command=="save"){
            string name;
            cin>>name;
            save(name);
        }
        else if(command=="help")
            player_set[t]->print_help();
        else if(extra_feature[1]==true&&command=="exit"){
            for(int i=0;i<4;++i){
                if(i!=t){
                    player_set[i]->swap(i<t?t-1:t,player_set[t]->create());
                }
            }
        }
        else
            cout<<"Invalid command."<<endl;
    }
}


unique_ptr<InitialBoard> Watan::creat(string choice,string filename,int n){
    if(choice=="seed")
        return make_unique<RandomGenerator>(n);
    else if(choice=="load"){
        init_type=true;
        return make_unique<FileLoad>(filename,"load");
    }
    else if(choice=="board")
        return make_unique<FileLoad>(filename,"board");
    else
        return make_unique<RandomGenerator>();
}


bool Watan::check_int(string s){
    for(int i=0;i<s.length();++i){
        if(s[i]<48||s[i]>57)
            return false;
    }
    return true;
}

int Watan::convert_int(string s){
    stringstream ss(s);
    int temp;
    ss>>temp;
    return temp;
}


