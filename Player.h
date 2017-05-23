#ifndef Player_h
#define Player_h
#include <vector>
#include <string>
#include <utility>
#include <memory>
#include "Database.h"
#include "Observer.h"
#include "Subject.h"
#include "Dice.h"
#include "fair_dice.h"
#include "load_dice.h"
#include "Subscription.h"
using namespace std;

class AI;

class Player:public Subject,public Observer{
protected:
    int name; // 0: BLUE  1: RED  2: ORANGE  3: YELLOW
    vector<int> resource;  // resources a player owns
    //case 0: "cafferines";
    //case 1: "labs";
    //case 2: "lectures";
    //case 3: "studies";
    //case 4: "tutorials";
    vector<int> criterion_loc;   // all locations of cri a player owns
    vector<int> criterion_type; // type of cri a player owns(matched)
    vector<int> goal_loc; // all locations of goal a player owns
    int cri_type;  //currently working cri_type, used for improve
    int vp;  // victory points
    
    shared_ptr<WatanData> data; // access to database
    shared_ptr<Dice> dicetype;  // the Dice a player owns
    
    int givepos;
    int takepos;

    
    // private helper:
    bool search(int , const vector<int>&);
    bool is_goal_nearby(int);
    int str_to_int(string);
    string int_to_str(int);
    bool is_enough_resource(int stealtarget);
public:
    Player(int,vector<int>,vector<int>,vector<int>,vector<int>,
           shared_ptr<WatanData>);
    Player(Player &);
    ~Player();
    void print_status() const;
    //print all resource all players have
    void print_criteria() const;
    //print criterion the certain player owned
    void achieve(int);
    //achieve a goal if the goal is not achieved and near player's own goal or own criterion
    //need 1 study and 1 tutorial to achieve one goal
    //the goal achieve must be neighbour of criterion player owned or goal player owned
    //will using observer pattern notify database

    void complete(int,bool);//complete a criterion if it is not near another criterion and have a owned goal adjacent
    // need 1 caffeine 1 lab 1 lecture 1 tutorial to complete

    void improve(int);//improve a criterion if it is a assignment or a midterm
    //improve to midterm need 2 lectures 3 studies
    //improve to exam need 3 caffeiene 2 lab 2 lecture 1 tutorial 2 study

    void trade(string,string,string, bool);
    //trade with chosen target one given reosurce with one taken resource
    // bool enable trade with BANK

    void print_help()const;
    // print command tha a player can input
    void acquire_resource(int);
    //acquire resource with dicenum int compare it with tile's value and resource it stands
    //for
    void being_notified(Type,int,Subject&) override;
    //observer pattern used when one player want to trade with another player or steal resource from other player
    void lose_to_geese();
    // will lose half of the certain resource to geese if it is bigger and equal than 10
    
    
    void geese();
    //move geese to a chosen tile and steal one resource from one player who have critarions near the tile
    void player_save(ofstream&);
    //save file
    void set_dice_type(string);
    //set dice type to fair or load (load for testing)
    int get_roll();

    Info getInfo() override;
    //get Info needed as helper

    shared_ptr<AI> create();
};

#endif /* Player_h */



