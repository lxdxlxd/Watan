#include "Player.h"
#include "AI.h"
#include <iostream>
#include <fstream>
#include "string"
#include "Info.h"
using namespace std;


// Constants:
const int CAFF=0;
const int LAB=1;
const int LEC=2;
const int STU=3;
const int TUT=4;

const int ASSI=1;
const int MT=2;
const int EXAM=3;

struct transinfo {
    int name;
    string resource_name;
};

Player::Player(int name,vector<int> r,vector<int> cl,vector<int> ct,
               vector<int> gl, shared_ptr<WatanData> d):
dicetype(make_shared<FairDice>()),vp(0),name(name),data(d),
resource(r),criterion_loc(cl),criterion_type(ct),goal_loc(gl){
    for(int i=0;i<criterion_type.size();++i){
        vp+=criterion_type[i];
    }
}

Player::Player(Player &other):name(other.name),resource(other.resource),
criterion_loc(other.criterion_loc),criterion_type(other.criterion_type),
goal_loc(other.goal_loc),cri_type(other.cri_type),vp(other.vp),data(other.data),
dicetype(other.dicetype),givepos(other.givepos),takepos(other.takepos){}


Player::~Player() {}


void Player::print_status()const {
    string str;
    switch (name) {
        case 0:
            str="BLUE";break;
        case 1:
            str="RED"; break;
        case 2:
            str="ORANGE"; break;
        case 3:
            str="YELLOW"; break;
    }
    cout << str << " has " << vp << " victory points, ";
    cout << resource[0] << " cafferines, " ;
    cout << resource[1] << " labs, " ;
    cout << resource[2] << " lectures, " ;
    cout << resource[3] << " studies, " ;
    cout << resource[4] << " tutorials." ;
    cout<<endl;
}

void Player::print_criteria() const{
    string str;
    switch (name) {
        case 0:
            str="BLUE";break;
        case 1:
            str="RED"; break;
        case 2:
            str="ORANGE"; break;
        case 3:
            str="YELLOW"; break;
    }
    cout<<str<<" has completed:"<<endl;
    for (int i=0;i<criterion_loc.size();++i){
        cout<<criterion_loc[i]<<" "<<criterion_type[i]<<endl;
    }
}

void Player::print_help()const{
    cout << "Valid commands:" << endl;
    cout << "board" << endl;
    cout << "status" << endl;
    cout << "criteria" << endl;
    cout << "achieve <goal>" << endl;
    cout << "complete <criterion>" << endl;
    cout << "improve <criterion>" << endl;
    cout << "trade <colour> <give> <take>" << endl;
    cout << "next" << endl;
    cout << "save <file>" << endl;
    cout << "help" << endl;
}


void Player::achieve(int pos) {
    //determine does player have enough resources
    if(resource[STU]<1||resource[TUT]<1)
        throw "You do not have enough resources.";
    //determine does chosen pos is available;
    if(data->get_goal()[pos]!="") throw "You cannot build here.";
    if(search(data->get_g_c()[pos][0],criterion_loc)||
       search(data->get_g_c()[pos][1], criterion_loc)){
        //notify database
        data->being_notified(Type::Goal,pos,*this);
        goal_loc.emplace_back(pos);
        resource[STU]--; resource[TUT]--;
        return;
    }

    else{
        for(int j=0;j<2;++j){
            int temp=data->get_g_c()[pos][j];
            if(data->get_cri()[temp]==""){
                for (int i=0; i<72; ++i) {
                    if(i!=pos&&search(temp,data->get_g_c()[i])){
                        if(search(i, goal_loc)){
                            //notify database
                            data->being_notified(Type::Goal,pos,*this);
                            goal_loc.emplace_back(pos);
                            resource[STU]--; resource[TUT]--;
                            return;
                        }
                    }
                }
            }
        }
    }
    throw "You cannot build here.";
}


void Player::complete(int pos,bool start) {
    if (data->get_cri()[pos]!=""){
        throw "You cannot build here.";
    }
    // determined if the pos ouccupied
    for (int j=0;j<data->get_c_c()[pos].size();++j){
        int temp=data->get_c_c()[pos][j];
        if (data->get_cri()[temp]!="")
            throw "You cannot build here.";
    }
    // complete 2 criterions at start phase
    if(start){
        criterion_loc.emplace_back(pos);
        criterion_type.emplace_back(ASSI);
        notify_observers(Type::Cri,pos);
        vp++;
        return;
    }
    else{
        //determine if there is enough resource to build
        if(resource[CAFF]<1||resource[LAB]<1||resource[LEC]<1||resource[TUT]<1)
            throw "You do not have enough resources.";
        if (!is_goal_nearby(pos))
            throw "You cannot build here.";
        criterion_loc.emplace_back(pos);
        criterion_type.emplace_back(ASSI);
        notify_observers(Type::Cri,pos);
        vp++;
        resource[CAFF]--;
        resource[LAB]--;
        resource[LEC]--;
        resource[TUT]--;
        // game won
        if(vp>=10) throw "Victory!";
    }
}


void Player::improve(int pos){
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
            if(vp>=10) throw "Victory!";
            return;
        }
    }
    throw "You cannot improve it";
}


void Player::trade(std::string playername, std::string give, string take,bool extrafeature) {
    
    if (extrafeature) {
        //trade with bank of ratio 4:1;
        if (playername=="BANK") {
            if (resource[str_to_int(give)]>=4) {
                resource[str_to_int(give)]-=4;
                resource[str_to_int(take)]++;
            } else {
                cout<<"not enough resource to trade"<<endl;
            }
            return;
        }
        
        
    }
    string str_name;
    
    if (name == 0) { str_name = "BLUE"; }
    if (name == 1) { str_name = "RED"; }
    if (name == 2) { str_name = "ORANGE"; }
    if (name == 3) { str_name = "YELLOW"; }
    
    
    givepos = str_to_int(give);
    if (resource[givepos] < 1) {
        cout << "not enough resource to give" << endl;
        return;
    }
    
    
    takepos = str_to_int(take);
    
    int tradetarget = str_to_int(playername);
    
    
    cout << str_name << " offers " << playername << " one " << give << " for one " << take << endl;
    cout << "Does " << playername << " accept this offer?" << endl;
    string resp;
    cin >> resp;
    if (resp == "yes") {
        
        if (name < tradetarget) {
            tradetarget--;
        }
        try {
            notify_observers(Type::Trade, tradetarget);
            resource[givepos]--;
            resource[takepos]++;
            cout<<"Students " <<str_name<<" gained one " << take <<" lose one "<<give<<endl;
            cout<<"Students " <<playername<<" gained one " << give <<" lose one " << take <<endl;
        } catch (char const *MEG) {
            cout << MEG << endl;
        }
        
        
    } else {
        cout << "Trade denied " << endl;
    }
    
}



Info Player::getInfo(){
    return {name,cri_type,takepos,givepos};
}



bool Player::is_goal_nearby(int i) {
    for (int j=0;j<goal_loc.size();++j) {
        int cur_goal = goal_loc[j];
        if (search(i,data->get_g_c()[cur_goal]))
            return true;
    }
    return false;
}




void Player::being_notified(Type t, int pos, Subject& who_noti){
    if (t==Type::Trade) {
        Info who_notify = who_noti.getInfo();
        takepos=who_notify.takepos;
        givepos=who_notify.givepos;
        if (resource[takepos]<1) {
            throw "not enought resource";
        }
        resource[takepos]--;
        resource[givepos]++;
    } else if (t==Type::Steal) {
        srand(time(NULL));
        int sum_of_resource=0;
        for (int i = 0; i < resource.size(); ++i) {
            sum_of_resource+=resource[i];
        }
        
        int random_num = (rand() % sum_of_resource);
        string stealed_resource;
        for (int j = 0; j < resource.size(); ++j) {
            random_num=random_num-resource[j];
            if (random_num<1&&resource[j]!=0) {
                switch (j) {
                    case 0: stealed_resource="cafferines";break;
                    case 1: stealed_resource="labs";break;
                    case 2: stealed_resource="lectures";break;
                    case 3: stealed_resource="studies";break;
                    case 4: stealed_resource="tutorials";break;
                }
                break;
            }
        }
        int steal_num = str_to_int(stealed_resource);
        resource[steal_num]--;
        transinfo i{name,stealed_resource};
        throw i;
        
        
    } else if (t==Type::Check) {
        int sum_of_resource=0;
        for (int i = 0; i < resource.size(); ++i) {
            sum_of_resource+=resource[i];
        }
        if (sum_of_resource==0) {
            transinfo i{-1,""};
            throw i;
        } else {
            transinfo i{0,""};
            throw i;
        }
    }
}


void Player::player_save(ofstream &file){
    for (int i=0; i<resource.size(); ++i) {
        file<<resource[i]<<" ";
    }
    file<<"g ";
    for (int i=0; i<goal_loc.size(); ++i) {
        file<<goal_loc[i]<<" ";
    }
    file<<"c ";
    for (int i=0; i<criterion_loc.size(); ++i) {
        file<<criterion_loc[i]<<" ";
        file<<criterion_type[i]<<" ";
    }
    file<<endl;
}


void Player::set_dice_type(string str){
    if(str=="fair")
        dicetype=make_shared<FairDice>();
    else if(str=="load")
        dicetype=make_shared<LoadDice>();
}


int Player::get_roll(){
    return dicetype->roll();
}



bool Player::search(int n, const vector<int> &v){
    for(int i=0;i<v.size();++i){
        if(n==v[i])
            return true;
    }
    return false;
}



int Player::str_to_int(string str) {
    if (str=="BLUE") {return 0;}
    if (str=="RED") {return 1;}
    if (str=="ORANGE") {return 2;}
    if (str=="YELLOW") {return 3;}
    if (str=="cafferines") {return 0;}
    if (str=="labs") {return 1;}
    if (str=="lectures") {return 2;}
    if (str=="studies") {return 3;}
    if (str=="tutorials") {return 4;}
    else return -1;
}


void Player::acquire_resource(int dice_num) {
    int sum_resource=0;
    vector<vector<int >> tile_cri=data->get_t_c();
    vector<int> board_resource=data->get_resource();
    vector<int> board_value=data->get_value();
    vector<int> original_resource=resource;
    for (int i = 0; i < criterion_loc.size(); ++i) {
        int cur_cri=criterion_loc[i];
        for (int j = 0; j < tile_cri.size(); ++j) {
            if (j==data->get_geese()) {
                continue;
            }
            if (search(cur_cri,tile_cri[j])&&dice_num==board_value[j]) {
                for (int k = 0; k < criterion_type[i]; ++k) {
                    sum_resource++;
                    resource[board_resource[j]]++;
                }
            }
        }
    }
    string str_name=int_to_str(name);
    
    if (sum_resource!=0) {
        cout << "Student " << str_name << " gained:" << endl;
        for (int l = 0; l < resource.size(); ++l) {
            if (resource[l] - original_resource[l] != 0) {
                string resource_name;
                switch (l) {
                    case 0:
                        resource_name = "cafferines";
                        break;
                    case 1:
                        resource_name = "labs";
                        break;
                    case 2:
                        resource_name = "lectures";
                        break;
                    case 3:
                        resource_name = "studies";
                        break;
                    case 4:
                        resource_name = "tutorials";
                        break;
                }
                cout << resource_name << "  " << resource[l] - original_resource[l] << endl;
            }
        }
    }
}

void Player::geese() {
    int tile_num;
    // check invalid input
    while(1){
        bool flag=false;
        cout<<"> ";
        if(!(cin>>tile_num)) throw "eof";
        if (tile_num < 0 || tile_num > 18 || tile_num == data->get_geese()) {
            cout << "Invalid input"<<endl;
        } else {
            flag=true;
            break;
        }
        if(flag) {break;}
        cout<<"Invalid command."<<endl;
    }
    data->set_geese(tile_num);
    vector<int> cri_geeseable=data->get_t_c()[tile_num];
    vector<string> s_stealable;
    vector<string> criterion=data->get_cri();
    //search player who can be stealed
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

    // start steal
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
    string be_stealed;
    while(1){
        bool flag=false;
        cout<<"> ";
        if(!(cin>>be_stealed)) throw "eof";
        for(int i=0;i<s_stealable.size();++i){
            if(be_stealed==s_stealable[i]){
                flag=true;
                break;
            }
        }
        if(flag) break;
        cout<<"Invalid command."<<endl;
    }
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


// everybody lose !!! viva la geese
void Player::lose_to_geese() {
    vector<int> lose_resource;
    int lose_sum=0;
    for (int i = 0; i < resource.size(); ++i) {
        if (resource[i]>=10) {
            int lose_num=resource[i]-resource[i]/2;
            lose_resource.emplace_back(lose_num);
            
            resource[i]=resource[i]/2;
            lose_sum+=lose_num;
        } else {
            lose_resource.emplace_back(0);
        }
    }
    if (lose_sum!=0) {
        cout << "Student " << int_to_str(name) << " loses " << lose_sum << " resources to the geese. They lose:"
        << endl;
        for (int j = 0; j < lose_resource.size(); ++j) {
            if (lose_resource[j] != 0) {
                string resource_name;
                switch (j) {
                    case 0:
                        resource_name = "cafferines";
                        break;
                    case 1:
                        resource_name = "labs";
                        break;
                    case 2:
                        resource_name = "lectures";
                        break;
                    case 3:
                        resource_name = "studies";
                        break;
                    case 4:
                        resource_name = "tutorials";
                        break;
                }
                cout << lose_resource[j] << "  " << resource_name << endl;
            }
        }
    }
}

string Player::int_to_str(int i) {
    string s_name;
    if (name==0) {s_name="BLUE";}
    if (name==1) {s_name="RED";}
    if (name==2) {s_name="ORANGE";}
    if (name==3) {s_name="YELLOW";}
    return s_name;
}

bool Player::is_enough_resource(int stealtarget) {
    int enough_resource;
    if (name < stealtarget) {
        stealtarget--;
    }
    try {
        notify_observers(Type::Check,stealtarget);
    } catch (transinfo I) {
        enough_resource=I.name;
    }
    if (enough_resource==-1) {
        return false;
    }
    return true;
}

shared_ptr<AI> Player::create(){
    return make_shared<AI>(*this);
}




