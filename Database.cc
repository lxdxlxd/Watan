#include "Database.h"
#include "Subject.h"
#include "Info.h"


WatanData::WatanData(vector<int>rs,vector<int>vs,
                     vector<string>cri,vector<string>goal,int geese):
resource_seq(rs),value_seq(vs),criterion(cri),goal(goal),geese_pos(geese){
    construct_cri();
    construct_goal();
    construct_cri_goal();
    construct_tile_goal();
}

WatanData::~WatanData() {}


void WatanData::being_notified(Type t, int pos, Subject& who_noti){
    string temp;
    if(who_noti.getInfo().name==0) temp="B";
    else if(who_noti.getInfo().name==1) temp="R";
    else if(who_noti.getInfo().name==2) temp="O";
    else if(who_noti.getInfo().name==3) temp="Y";
    if(t==Type::Goal)
        goal[pos]=temp+"A";
    else if(t==Type::Cri)
        criterion[pos]=temp+"A";
    else if(t==Type::Improve){
        if(who_noti.getInfo().cri_type==2)
            criterion[pos]=temp+"M";
        else if(who_noti.getInfo().cri_type==3)
            criterion[pos]=temp+"E";
    }
        
}




void WatanData::construct_goal(){
    cri_cri.resize(54);
    int flag=1; // left: +1, right: -1
    // cri_cri initialize
    for(int i=0;i<54;++i){
        if(i==0||i==1){
            cri_cri[i].emplace_back(i+3);
            cri_cri[i].emplace_back(i+flag);
            toggle(flag);
        }
        else if(i==52||i==53){
            cri_cri[i].emplace_back(i-3);
            cri_cri[i].emplace_back(i+flag);
            toggle(flag);
        }
        else if(i==2||i==5){
            cri_cri[i].emplace_back(i+5);
            cri_cri[i].emplace_back(i+flag);
            toggle(flag);
        }
        else if(i==48||i==51){
            cri_cri[i].emplace_back(i-5);
            cri_cri[i].emplace_back(i+flag);
            toggle(flag);
        }
        else if(i==3||i==4){
            cri_cri[i].emplace_back(i-3);
            cri_cri[i].emplace_back(i+5);
            cri_cri[i].emplace_back(i+flag);
            toggle(flag);
        }
        else if(i==49||i==50){
            cri_cri[i].emplace_back(i-5);
            cri_cri[i].emplace_back(i+3);
            cri_cri[i].emplace_back(i+flag);
            toggle(flag);
        }
        else if(i>=7&&i<=10){
            cri_cri[i].emplace_back(i-5);
            cri_cri[i].emplace_back(i+6);
            cri_cri[i].emplace_back(i+flag);
            toggle(flag);
        }
        else if(i>=43&&i<=46){
            cri_cri[i].emplace_back(i-6);
            cri_cri[i].emplace_back(i+5);
            cri_cri[i].emplace_back(i+flag);
            toggle(flag);
        }
        else{
            if(i!=6&&i!=11){
                cri_cri[i].emplace_back(i-6);
            }
            if(i!=42&&i!=47){
                cri_cri[i].emplace_back(i+6);
            }
            if(i%12!=0&&i%12!=5){
                cri_cri[i].emplace_back(i+flag);
                toggle(flag);
            }
        }
    }
}


void WatanData::construct_cri(){
    // goal_goal initialize
    vector<int> store;
    for(int i=0;i<72;++i){
        store.emplace_back(i);
    }
    goal_goal.resize(72);
    for(int i=0;i<72;++i){
        goal_goal[i].resize(4);
        if(i==0){
            goal_goal[i][0]=goal_goal[i][1]=-1;
            goal_goal[i][2]=1;
            goal_goal[i][3]=2;
        }
        else if(i==71){
            goal_goal[i][0]=69;
            goal_goal[i][1]=70;
            goal_goal[i][2]=goal_goal[i][3]=-1;
        }
        else if(i==3){
            goal_goal[i][0]=-1;
            goal_goal[i][1]=1;
            goal_goal[i][2]=5;
            goal_goal[i][3]=6;
        }
        else if(i==67){
            goal_goal[i][0]=63;
            goal_goal[i][1]=64;
            goal_goal[i][2]=-1;
            goal_goal[i][3]=69;
        }
        else if(i==4){
            goal_goal[i][0]=2;
            goal_goal[i][1]=-1;
            goal_goal[i][2]=7;
            goal_goal[i][3]=8;
        }
        else if(i==68){
            goal_goal[i][0]=65;
            goal_goal[i][1]=66;
            goal_goal[i][2]=70;
            goal_goal[i][3]=-1;
        }
        else if(i==10){
            goal_goal[i][0]=6;
            goal_goal[i][1]=7;
            goal_goal[i][2]=14;
            goal_goal[i][3]=15;
        }
        else if(i==61){
            goal_goal[i][0]=56;
            goal_goal[i][1]=57;
            goal_goal[i][2]=64;
            goal_goal[i][3]=65;
        }
        else if(i%17==9){
            if(i!=9){
                goal_goal[i][0]=i-6;
                goal_goal[i][1]=i-5;
            }
            else{
                goal_goal[i][0]=-1;
                goal_goal[i][1]=5;
            }
            if(i!=60){
                goal_goal[i][2]=i+3;
                goal_goal[i][3]=i+4;
            }
            else{
                goal_goal[i][2]=-1;
                goal_goal[i][3]=63;
            }
        }
        else if(i%17==11){
            if(i!=11){
                goal_goal[i][0]=i-4;
                goal_goal[i][1]=i-3;
            }
            else{
                goal_goal[i][0]=8;
                goal_goal[i][1]=-1;
            }
            if(i!=62){
                goal_goal[i][2]=i+5;
                goal_goal[i][3]=i+6;
            }
            else{
                goal_goal[i][2]=66;
                goal_goal[i][3]=-1;
            }
        }
        else if(i%17==1&&i!=1&&i!=69){
            goal_goal[i][0]=i-5;
            goal_goal[i][1]=i-4;
            goal_goal[i][2]=i+3;
            goal_goal[i][3]=i+4;
        }
        else if(i%17==2&&i!=2&&i!=70){
            goal_goal[i][0]=i-4;
            goal_goal[i][1]=i-3;
            goal_goal[i][2]=i+4;
            goal_goal[i][3]=i+5;
        }
        else if(i%17==10){
            goal_goal[i][0]=i-5;
            goal_goal[i][1]=i-4;
            goal_goal[i][2]=i+4;
            goal_goal[i][3]=i+5;
        }
        else{
            goal_goal[i].clear();
            store[i]=-1;
        }
    }
    for(int i=0;i<72;++i){
        if(goal_goal[i].size()!=0) continue;
        for(int j=0;j<72;++j){
            if(store[j]==-1) continue;
            int temp=search(i, goal_goal[j]);
            if(temp!=-2){
                goal_goal[i].emplace_back(j);
                goal_goal[i].emplace_back(temp);
            }
        }
        if(i%17==12&&i!=63)
            goal_goal[i].emplace_back(i+8);
        if(i%17==3&&i!=3&&i!=71)
            goal_goal[i].emplace_back(i-8);
        if(i%17==0&&i!=0&&i!=68)
            goal_goal[i].emplace_back(i+8);
        if(i%17==8&&i!=8&&i!=71)
            goal_goal[i].emplace_back(i-8);
    }
}


void WatanData::construct_cri_goal() {
    goal_cri.resize(72);
    int row = 0;
    //add critarion to goal horizontally (special cases)
    goal_cri[0].emplace_back(0);
    goal_cri[0].emplace_back(1);
    goal_cri[3].emplace_back(2);
    goal_cri[3].emplace_back(3);
    goal_cri[4].emplace_back(4);
    goal_cri[4].emplace_back(5);
    // add critarion to goal vertically (special cases)
    goal_cri[1].emplace_back(0);
    goal_cri[1].emplace_back(3);
    goal_cri[2].emplace_back(1);
    goal_cri[2].emplace_back(4);
    for (int j = 5; j < 9; ++j) {
        goal_cri[j].emplace_back(j-3);
        goal_cri[j].emplace_back(j+2);
        
    }
    int critarion = 5;
    int upperdiff=4;
    int lowerdiff=-2;
    
    for (int i = 9; i <= 62; i++) {
        
        //set horizontal
        if ((i-9)%17<3||(i-9)%17==9||(i-9)%17==10) {
            critarion++;
            goal_cri[i].emplace_back(critarion);
            critarion++;
            goal_cri[i].emplace_back(critarion);
            
            if ((i-9)%17==2||(i-9)%17==10) {
                critarion++;
            }
            //set vertical
        } else {
            goal_cri[i].emplace_back(i-upperdiff);
            goal_cri[i].emplace_back(i-lowerdiff);
        }
        //increment row num
        if ((i-9)%17==2||(i-9)%17==8||(i-9)%17==10||(i-9)%17==16) {
            row++;
            if (row % 4 ==3||row==1) {
                upperdiff+=2;
                lowerdiff+=2;
            } else if (row%4==1) {
                upperdiff+=3;
                lowerdiff+=3;
            }
        }
    }
    //special cases set vertical
    for (int k = 63; k <= 66; ++k) {
        goal_cri[k].emplace_back(k-20);
        goal_cri[k].emplace_back(k-15);
    }
    for (int l = 69; l <= 70; ++l) {
        goal_cri[l].emplace_back(l-20);
        goal_cri[l].emplace_back(l-17);
        
    }
    //special cases set horizontal
    goal_cri[67].emplace_back(48);
    goal_cri[67].emplace_back(49);
    goal_cri[68].emplace_back(50);
    goal_cri[68].emplace_back(51);
    goal_cri[71].emplace_back(52);
    goal_cri[71].emplace_back(53);
}


void WatanData::construct_tile_goal(){
    tile_cri.resize(19);
    for(int i=0;i<19;++i){
        int first=0;
        int temp=0;
        if(i<=5) first=2*i;
        else if(i<=7) first=2*i+1;
        else if(i<=10) first=2*i+2;
        else if(i<=12) first=2*i+3;
        else if(i<=15) first=2*i+4;
        else if(i<=17) first=2*i+5;
        else first=2*i+8;
        tile_cri[i].emplace_back(first);
        for(int j=0;j<cri_cri[first].size();++j){
            if(cri_cri[first][j]>first){
                tile_cri[i].emplace_back(cri_cri[first][j]);
            }
        }
        temp=tile_cri[i][1];
        for(int j=0;j<cri_cri[temp].size();++j){
            if(cri_cri[temp][j]>temp)
                tile_cri[i].emplace_back(cri_cri[temp][j]);
        }
        tile_cri[i].emplace_back(tile_cri[i][3]+1);
        temp=tile_cri[i][2];
        for(int j=0;j<cri_cri[temp].size();++j){
            if(cri_cri[temp][j]>temp)
                tile_cri[i].emplace_back(cri_cri[temp][j]);
        }
    }
}

const vector<vector<int>>& WatanData::get_c_c()const{
    return cri_cri;
}


const vector<vector<int>>& WatanData::get_g_g()const{
    return goal_goal;
}


const vector<vector<int>>& WatanData::get_g_c()const{
    return goal_cri;
}


const vector<vector<int>>& WatanData::get_t_c()const{
    return tile_cri;
}

const vector<int>& WatanData::get_resource()const{
    return resource_seq;
}


const vector<int>& WatanData:: get_value()const{
    return value_seq;
}


const vector<string>& WatanData::get_cri()const{
    return criterion;
}


const vector<string>& WatanData::get_goal()const{
    return goal;
}

int WatanData::get_geese()const{
    return  geese_pos;
}



// switch between 1 and -1
void WatanData::toggle(int &flag){
    flag=(flag==1?-1:1);
}


// a search algorithm for constructing the neighbour of a goal
int WatanData::search(int n, const vector<int> &v)const{
    for(int i=0;i<v.size();++i){
        if(n==v[i])
            return v[i>1?i-2:i+2];
    }
    return -2;
}

void WatanData::set_geese(int i) {
    geese_pos=i;
}



