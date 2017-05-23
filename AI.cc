#include "AI.h"
#include <iostream>
using namespace std;


struct transinfo {
    int name;
    string resource_name;
};

// Constants:
const int CAFF=0;
const int LAB=1;
const int LEC=2;
const int STU=3;
const int TUT=4;

const int ASSI=1;
const int MT=2;
const int EXAM=3;

const int best_up=27;
const int best_low=15;

const int worst_up=30;
const int worst_low=12;

AI::AI(Player &p):Player(p) {
    value=value_sum();
    search_goal();
}

AI::~AI() {}

void AI::AIrun(){
    updateBest();
    int best=best_cri.pos;
    unsigned long length=best_cri.goal_required.size();
    if(length==1){
        try{
            while(1){
                complete(best);
                updateBest();
            }
        }
        catch(char const *msg){}
    }
    else{
        try{
            while(1){
                achieve(best_cri.goal_required[length-1]);
                updateBest();
            }
        }
        catch(char const *msg){}
    }
    try{
        while(1){
            improve(max_cri_loc());
            updateBest();
        }
    }
    catch(char const *msg){}
}


void AI::achieve(int pos){
    if(resource[STU]<1||resource[TUT]<1)
        throw "AI does not have enough resources.";
    data->being_notified(Type::Goal,pos,*this);
    goal_loc.emplace_back(pos);
    resource[STU]--; resource[TUT]--;
}


void AI::complete(int pos){
    if(resource[CAFF]<1||resource[LAB]<1||resource[LEC]<1||resource[TUT]<1)
        throw "AI does not have enough resources.";
    criterion_loc.emplace_back(pos);
    criterion_type.emplace_back(ASSI);
    notify_observers(Type::Cri,pos);
    vp++;
    resource[CAFF]--;
    resource[LAB]--;
    resource[LEC]--;
    resource[TUT]--;
}

void AI::improve(int pos){
    for (int i=0; i<criterion_loc.size(); ++i) {
        if(criterion_loc[i]==pos){
            if(criterion_type[i]==EXAM)
                throw "Already an exam.";
            else if(criterion_type[i]==ASSI){
                if(resource[LEC]<2||resource[STU]<3)
                    throw "You do not have enough resources.";
                else{
                    resource[LEC]-=2;
                    resource[STU]-=3;
                }
            }
            else{
                if(resource[CAFF]<3||resource[LAB]<2||resource[LEC]<2||
                   resource[TUT]<1||resource[STU]<2)
                    throw "You do not have enough resources.";
                else{
                    resource[CAFF]-=3;
                    resource[LAB]-=2;
                    resource[LEC]-=2;
                    resource[TUT]--;
                    resource[STU]-=2;
                }
            }
            cri_type=++criterion_type[i];
            notify_observers(Type::Improve,pos);
            vp++;
            return;
        }
    }
}



void AI::search_goal(){
    for(int i=0;i<72;++i){
        if(data->get_goal()[i]!="") continue;
        if(search(data->get_g_c()[i][0],criterion_loc)||
           search(data->get_g_c()[i][1], criterion_loc)){
            goal_available.emplace_back(i);
        }
        else{
            bool flag_found=false;
            for(int j=0;j<2;++j){
                if(flag_found) break;
                int temp=data->get_g_c()[i][j];
                if(data->get_cri()[temp]==""){
                    for (int n=0; n<72; ++n) {
                        if(n!=i&&search(temp,data->get_g_c()[n])){
                            if(search(n, goal_loc)){
                                goal_available.emplace_back(i);
                                flag_found=true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}


int AI::max_cri_loc(){
    int max_pos=0;
    int cur=0;
    for(int i=0;i<criterion_loc.size();++i){
        if(criterion_type[i]==3) continue;
        if(value[criterion_loc[i]]>cur){
            cur=value[criterion_loc[i]];
            max_pos=criterion_loc[i];
        }
    }
    return max_pos;
}



// return all adjacent values of cri.
vector<int> AI::search_value(int cri){
    vector<int> store_num;
    for (int i=0; i<19; ++i) {
        if(search(cri,data->get_t_c()[i])){
            int temp=data->get_value()[i];
            store_num.emplace_back(temp);
        }
    }
    return store_num;
}


// return the value_sum of all criteria.
vector<int> AI::value_sum(){
    int temp_sum=0;
    vector<int> temp;
    for(int i=0;i<54;++i){
        for(int j=0;j<search_value(i).size();++j){
            temp_sum+=search_value(i)[j];
        }
        temp.emplace_back(temp_sum);
    }
    return temp;
}



//new
vector<int> AI::search_cri_available() {
    vector<int >cri_nearby;
    vector<int> cri_available;
    for (int i = 0; i < criterion_loc.size(); ++i) {
        
        for (int j = 0; j < data->get_c_c()[criterion_loc[i]].size(); ++j) {
            int temp = data->get_c_c()[criterion_loc[i]][j];
            if (data->get_cri()[temp] == "") {
                cri_nearby.emplace_back(data->get_c_c()[criterion_loc[0]][j]);
            }
        }
    }
    
    for (int k = 0; k < cri_nearby.size(); ++k) {
        for (int i = 0; i < data->get_c_c()[cri_nearby[k]].size(); ++i) {
            int temp = data->get_c_c()[cri_nearby[k]][i];
            if (data->get_cri()[temp]=="") {
                cri_available.emplace_back(temp);
            }
            
        }
    }
    return cri_available;
}

bool AI::is_cri_available(int i) {
    bool flag = false;
    vector<int> cri_nearby=data->get_c_c()[i];
    vector<int> goal_nearby;
    for (int k = 0; k < data->get_g_c().size(); ++k) {
        if (search(i,data->get_g_c()[k])&&data->get_goal()[k]=="") {
            goal_nearby.emplace_back(k);
        }
    }
    vector<int> goal_nearby_nearby;
    for (int l = 0; l < goal_nearby.size(); ++l) {
        for (int j = 0; j < data->get_g_g()[goal_nearby[l]].size(); ++j) {
            int cur_goal=data->get_g_g()[goal_nearby[l]][j];
            if ((!search(i,data->get_g_c()[cur_goal])&&(data->get_goal()[cur_goal]==""))) {
                goal_nearby_nearby.emplace_back(cur_goal);
            }
        }
    }
    
    for (int m = 0; m < goal_nearby_nearby.size(); ++m) {
        for (int j = 0; j < criterion_loc.size(); ++j) {
            if (search(criterion_loc[j],data->get_g_c()[goal_nearby_nearby[m]])) {
                flag=true;
                break;
            }
        }
    }
    
    
    
    for (int j = 0; j < cri_nearby.size(); ++j) {
        if (data->get_cri()[cri_nearby[j]]!="") {
            return false;
        }
    }
    
    return flag;
}

void AI::updateBest() {
    vector<int> cri_available;
    cri_available=search_cri_available();
    vector<int> cri_value=value_sum();
    int temppos=0;
    int temp=0;
    int best_value_for_now=0;
    for (int i = 0; i < cri_available.size(); ++i) {
        if (is_cri_available(cri_available[i])) {
            temp = cri_value[cri_available[i]];
            if (temp <=24&&temp>=18) {
                
                temppos=cri_available[i];
                break;
            }
            int abs_diff=(temp-21>0)? temp-21:21-temp;
            if (abs_diff<best_value_for_now) {
                best_value_for_now=abs_diff;
                temppos=i;
            }
        }
        
    }
    vector<int> goal_required;
    vector<int> goal_needed_1=search_goal_near_cri(temppos);
    for (int j = 0; j < criterion_loc.size(); ++j) {
        vector<int> goal_needed_2=search_goal_near_cri(criterion_loc[j]);
        for (int i = 0; i < goal_needed_1.size(); ++i) {
            if (search(goal_needed_1[i],goal_needed_2)) {
                if (!search(goal_needed_1[i],goal_loc)) {
                goal_required.emplace_back(goal_needed_1[i]);
                break;
                }
            }
        }
        for (int k = 0; k < goal_needed_2.size(); ++k) {
            if (search(goal_needed_2[k],goal_needed_1)) {
                if (!search(goal_needed_2[k],goal_loc)) {
                goal_required.emplace_back(goal_needed_2[k]);
                break;
                }
            }
            
        }
    }
    best_cri.goal_required=goal_required;
    best_cri.pos=temppos;
}

vector<int> AI::search_goal_near_cri(int) {
    vector<int> goal_nearby;
    for (int i = 0; i < data->get_goal().size(); ++i) {
        for (int j = 0; j < data->get_g_c()[i].size(); ++j) {
            if (search(i,data->get_g_c()[i])) {
                goal_nearby.emplace_back(i);
            }
        }
    }
    return goal_nearby;
}

void AI::geese() {
    vector<int> tile_not_affected;
    for (int i = 0; i < data->get_t_c().size(); ++i) {
        for (int j = 0; j < criterion_loc.size(); ++j) {
            if (!search(criterion_loc[j],data->get_t_c()[i])) {
                tile_not_affected.emplace_back(i);
            }
        }
    }
    srand(time(NULL));
    int random_num=(rand()%tile_not_affected.size());
    int tile_num=tile_not_affected[random_num];
    data->set_geese(tile_num);
    vector<int> cri_geeseable=data->get_t_c()[tile_num];
    vector<string> s_stealable;
    vector<string> criterion=data->get_cri();
    for (int i = 0; i < cri_geeseable.size(); ++i) {
        string str=criterion[cri_geeseable[i]];
        if(str[0]=='B' &&name!=0) {
            if (is_enough_resource(0)) {
                if (s_stealable.size() == 0) {
                    s_stealable.emplace_back("BLUE");
                } else {
                    for (int j = 0; j < s_stealable.size(); ++j) {
                        if (s_stealable[j] == "BLUE") {
                            break;
                        } else if (j == s_stealable.size() - 1) {
                            s_stealable.emplace_back("BLUE");
                        }
                    }
                }
            }
            
        }
        if(str[0]=='R' &&name!=1) {
            if (is_enough_resource(1)) {
                if (s_stealable.size() == 0) {
                    s_stealable.emplace_back("RED");
                } else {
                    for (int j = 0; j < s_stealable.size(); ++j) {
                        
                        if (s_stealable[j] == "RED") {
                            break;
                        } else if (j == s_stealable.size() - 1) {
                            s_stealable.emplace_back("RED");
                        }
                    }
                }
            }
        }
        if(str[0]=='O' &&name!=2) {
            if (is_enough_resource(2)) {
                if (s_stealable.size() == 0) {
                    s_stealable.emplace_back("ORANGE");
                } else {
                    for (int j = 0; j < s_stealable.size(); ++j) {
                        if (s_stealable[j] == "ORANGE") {
                            break;
                        } else if (j == s_stealable.size() - 1) {
                            s_stealable.emplace_back("ORANGE");
                        }
                    }
                }
            }
        }
        if(str[0]=='Y' &&name!=3) {
            if (is_enough_resource(3)) {
                if (s_stealable.size() == 0) {
                    s_stealable.emplace_back("YELLOW");
                } else {
                    for (int j = 0; j < s_stealable.size(); ++j) {
                        if (s_stealable[j] == "YELLOW") {
                            break;
                        } else if (j == s_stealable.size() - 1) {
                            s_stealable.emplace_back("YELLOW");
                        }
                    }
                }
            }
        }
    }
    
    if (s_stealable.size()==0) {
        cout<<"Student "<< int_to_str(name)<<" has no students to steal from."<<endl;
        return;
    } else {
        cout<<"Student "<< int_to_str(name)<<" can choose to steal from ";
        for (int i = 0; i < s_stealable.size(); ++i) {
            cout<<s_stealable[i]<<" ";
        }
        cout<<endl;
    }
    //use geese to steal a resource from someone
    cout<<"Choose a student to steal from."<<endl;
    random_num=(rand()%s_stealable.size());
    string be_stealed = s_stealable[random_num];
    int stealtarget=str_to_int(be_stealed);
    if (name < stealtarget) {
        stealtarget--;
    }
    
    try {
        notify_observers(Type::Steal,stealtarget);
    } catch(transinfo info) {
        
        int stealed=str_to_int(info.resource_name);
        resource[stealed]++;
        string s_name=int_to_str(name);
        string bs_name=int_to_str(info.name);
        cout << "Student "<<s_name<<" steals "<< info.resource_name<<" from Student "<< be_stealed<<endl;
    }
}



