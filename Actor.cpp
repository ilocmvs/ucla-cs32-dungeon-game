//
//  Actor.cpp
//  CS32PJ3
//
//  Created by Zhe Wang on 5/15/20.
//  Copyright © 2020 Zhe Wang. All rights reserved.
//

#include "Actor.h"
#include "Dungeon.h"
#include "utilities.h"
#include <iostream>
#include "Item.h"
#include <cmath>
#include <queue>
using namespace std;

Actor::Actor(char symbol, string name, int row, int col, int maxHP,
    int ArP, int SP, int DP)
    : m_symbol(symbol), m_name(name), m_row(row), m_col(col), m_maxHP(maxHP),                //Initializes all of the Actor's member variables
m_ArP(ArP), m_SP(SP), m_DP(DP), m_sleepTime(0), m_weapon(nullptr), currentGridState(0)
{
    m_HP = m_maxHP;
}

Actor::~Actor()
{
    delete m_weapon;
}

string Actor::attack(Actor* target)
{
    string msg = "but misses.\n";

    //Determines whether the attacker hits the target
    int AtkP = getDP() + (getWeapon())->getDexBonus();
    int DefP = target->getDP() + target->getArP();
    if (randInt(AtkP) >= randInt(DefP)){
        msg = "and hits the target.\n";        //Sets msg if it hits
    }

    //If the target is hit, the target's hit points go down
    if (msg == "and hits the target.\n")
    {
        int damP = randInt(getSP() + (getWeapon())->getDamage());
        target->setHP(target->getHP() - damP);
    }

    //If the attacker uses a magic fang of sleep, there is a chance to put the target to sleep
    if ((getWeapon())->getName() == "magic fang of sleep" && msg == "and hits the target.\n")
    {
        //Sleep happens 20% of the time
        if (trueWithProbability(0.2)){
            int x = randInt(5) + 2, y = target->getSleepTime(), sleep;
            //sleepTime is set to the maximum of the old sleepTime and the new random value
            if (x>y)
                sleep=x;
            sleep=y;
            target->setSleepTime(sleep);
            msg = " and mesmerizes " + target->getName() + ".";
        }
    }
    //If the target has 0 hits points, it is dead and the msg it updated
    if (target->getHP() <= 0){
        msg = "slays "+target->getName()+".\n";
    }
    return msg;
}

string Actor::move(Dungeon* dung, Actor* player)
{
    string msg = "";
    //Player's location
    int pRow = player->getRow();
    int pCol = player->getCol();
    //If the Actor is adjacent to the player
    if ((abs(pRow - getRow()) == 1 and pCol == getCol()) or (pRow == getRow() and abs(pCol - getCol()) == 1))
    {
        //...the Actor will attack the player and update the msg string
        string atk = attack(player);
        msg += getName() + " " + getWeapon()->action() + " " +player->getName()+" "+ atk;
    }
    return msg;
}

bool Actor::isMorW(Dungeon*dung,int row, int col) const {
    if (dung->getCellStatus(row, col)==1 or dung->getCellStatus(row, col)==30 or dung->getCellStatus(row, col)==40 or dung->getCellStatus(row, col)==50 or dung->getCellStatus(row, col)==60){
        return true;
    }
    return false;
}

bool Actor::setRow(int row)
{
    if (row < 0 || row > 17){    //row out of bounds
        return false;
    }
    m_row = row;
    return true;
}
bool Actor::setCol(int col)
{
    if (col < 0 || col > 79){    //col out of bounds
        return false;
    }
    m_col = col;
    return true;
}

bool Actor::setSleepTime(int time)
{
    if (time < 0){            //time too low
        m_sleepTime = 0;
    }
    else if (time > 9){        //time too high
        m_sleepTime = 9;
    }
    else{                    //time in range
        m_sleepTime = time;
    }
    return true;
}

Player::Player(int row, int col)
    : Actor('@', "Player", row, col, 20, 2, 2, 2)
{
    //Sets weapon to a short sword
    ShortSword* playerWeapon = new ShortSword();
    //Adds the short sword to the inventory and weaponry
    setWeapon(playerWeapon);
    m_inventory.push_back(playerWeapon);
}

Player::Player(const Player& obj, int row, int col): Actor('@', "Player", -1, -1, obj.getMaxHP(), obj.getArP(), obj.getSP(), obj.getDP())
{
    setRow(row);
    setCol(col);
    setHP(obj.getHP());
    Item* newitem=new Item(obj.getWeapon()->getSymbol(),obj.getWeapon()->getName(),-1,-1);
    Weapon* newweapon=dynamic_cast<Weapon*>(newitem);
    setWeapon(newweapon);
    cout<<newweapon->getName();
    vector<Item*> *copyInventory=new vector<Item*>;
    *copyInventory = obj.m_inventory;
    m_inventory=*copyInventory;
}

string Player::pickupScroll(Item* item)
{
    string msg = "";
    //If the inventory is not full, it adds the item to the inventory
    if (m_inventory.size() <= 25){
        m_inventory.push_back(item);
        msg = getName()+" pick up " + item->getName() + "\n";
    }
    //Otherwise it says that the inventory is full
    else{
        msg = "Your bag is full; you can't pick that up.\n";
    }
    return msg;
}
string Player::pickupWeapon(Weapon* item)
{
    //If the inventory is not full, it adds the weapon to the inventory and weaponry
    m_inventory.push_back(item);
    string msg = getName()+" pick up " + item->getName() + ".\n";
    return msg;
}
string Player::selectWeapon(char choice)
{
    string msg = "";
    int ichoice=choice-'a';
    if (ichoice<0 or ichoice>m_inventory.size()-1)
        return msg;
    Weapon* chosenweapon=dynamic_cast<Weapon*>(m_inventory[ichoice]);
    if (chosenweapon!=nullptr){
        setWeapon(chosenweapon);
        msg = "You are wielding a " + chosenweapon->getName() + ".\n";
    }
    else
        msg = "You can't wield a " + chosenweapon->getName() + ".\n";
    return msg;
}

string Player::readScroll(char choice)
{
    string msg = "";
    int ichoice=choice-'a';
    if (ichoice<0 or ichoice>m_inventory.size()-1)
        return msg;
    if (m_inventory[ichoice]->isScroll()){
        msg = m_inventory[ichoice]->use(this);
        m_inventory.erase(m_inventory.begin()+ichoice);
    }
    else
        msg = "You can't read " + m_inventory[ichoice]->getName() + " that is not a scroll.\n";
    return msg;
}
void Player::displayInventory()
{
    //Clear the screen and display the title
    clearScreen();
    cout << "Inventory:" << endl;
    for (int i=0;i<m_inventory.size(); i++)
    {
        cout << " " << char('a'+i) << ". " << m_inventory[i]->getName() << endl;
    }
}
Player::~Player()
{
    //Delete all items in the inventory
    for (vector<Item*>::iterator i = m_inventory.begin(); i != m_inventory.end(); i++)
    {
        if ((*i)!=getWeapon())
            delete *i;
    }
    m_inventory.clear();
}


Bogeyman::Bogeyman(int row, int col)        //Constructor
    : Actor('B', "The Bogeyman", row, col, randInt(5,10),
    2, randInt(2,3), randInt(2,3))
{
    setWeapon(new ShortSword());
}
string Bogeyman::move(Dungeon* dung, Actor* player)
{
    //Attack the Player if the Bogeyman is next to the Player
    string msg = Actor::move(dung, player);
    //If the Bogeyman did not attack
    if (msg == "")
    {
        int DisR = player->getRow() - getRow();
        int DisC = player->getCol() - getCol();
        //If the Bogeyman is within 5 spots of the Player
        if (abs(DisR) + abs(DisC) <= 5)
        {
            bool canmoveR = true;
            bool canmoveC = true;
            int tempRow = getRow();
            int tempCol = getCol();
            //Set tempRow to a row one spot closer to the Player if it is not in the same row as the Player
            if (abs(DisR) > 0){
                tempRow += DisR / abs(DisR);
            }
            else{
                canmoveR = false;
            }
            //set tempCol to a column one spot closer to the Player if it is not in the same col as the Player
            if (abs(DisC) > 0){
                tempCol += DisC / abs(DisC);
            }
            else{
                canmoveC = false;
            }
            //If there is a wall in the way, indicate that the Bogeyman can't move that direction.
            if (isMorW(dung, tempRow, getCol())){
                canmoveR = false;
            }
            if (isMorW(dung, getRow(), tempCol))
            {
                canmoveC = false;
            }
            //if a move is determined:
            dung->setCellStatus(getRow(), getCol(), getcurrentGridState());
            //If the Bogeyman can move vertically toward the Player, move it closer
            if (canmoveC){
                setCol(tempCol);
            }
            //Otherwise, move the Bogeyman horizontally toward the Player if it can
            else if (canmoveR){
                setRow(tempRow);
            }
            //after the move
            setcurrentGridState(dung->getCellStatus(getRow(), getCol()));
            dung->setCellStatus(getRow(), getCol(), 30);
        }
    }
    return msg;
}

Weapon* Bogeyman::drop()
{
    //Return a pointer to a MagicAxe 10% of the time
    if (trueWithProbability(0.1)){
        Weapon* item = new MagicAxe(getRow(), getCol());
        return item;
    }
    else{
        return nullptr;
    }
}

Snakewoman::Snakewoman(int row, int col)
    : Actor('S', "The Snakewoman", row, col, randInt(3,6), 3, 2, 3)
{
    setWeapon(new MagicFang());
}
string Snakewoman::move(Dungeon* dung, Actor* player)
{
    //Attack the Player if the Snakewoman is next to the Player
    string msg = Actor::move(dung, player);
    //If the Snakewoman did not attack
    if (msg == "")
    {
        int DisR = player->getRow() - getRow();
        int DisC = player->getCol() - getCol();
        //If the Snakewoman is within 3 spots from the Player
        if (abs(DisR) + abs(DisC) <= 3)
        {
            bool canmoveR = true;
            bool canmoveC = true;
            int tempRow = getRow();
            int tempCol = getCol();
            //Set tempRow to a row one spot closer to the Player if it is not in the same row as the Player
            if (abs(DisR) > 0){
                tempRow += DisR / abs(DisR);
            }
            else{
                canmoveR = false;
            }
            //set tempCol to a column one spot closer to the Player if it is not in the same col as the Player
            if (abs(DisC) > 0){
                tempCol += DisC / abs(DisC);
            }
            else{
                canmoveC = false;
            }
            //If there is a wall in the way, indicate that the Bogeyman can't move that direction.
            if (isMorW(dung, tempRow, getCol())){
                canmoveR = false;
            }
            if (isMorW(dung, getRow(), tempCol))
            {
                canmoveC = false;
            }
            //if a move is determined:
            dung->setCellStatus(getRow(), getCol(), getcurrentGridState());
            //If the Bogeyman can move vertically toward the Player, move it closer
            if (canmoveC){
                setCol(tempCol);
            }
            //Otherwise, move the Bogeyman horizontally toward the Player if it can
            else if (canmoveR){
                setRow(tempRow);
            }
            setcurrentGridState(dung->getCellStatus(getRow(), getCol()));
            dung->setCellStatus(getRow(), getCol(), 40);
        }
    }
    return msg;
}
Weapon* Snakewoman::drop()
{
    int rand = randInt(3);
    //Return a pointer to a MagicFang 1/3 of the time
    if (rand == 0){
        Weapon* item = new MagicFang(getRow(), getCol());
        return item;
    }
    else{
        return nullptr;
    }
}

Dragon::Dragon(int row, int col)
    : Actor('D', "The Dragon", row, col, randInt(20,25), 4, 4, 4)
{
    setWeapon(new LongSword());
}
string Dragon::move(Dungeon* dung, Actor* player)
{
    //1/10 of the time, increase the Dragon's hit points by 1 (up to its maximum hit points)
    if (trueWithProbability(0.1))
    {
        setHP(getHP() + 1);
    }
    //Attack the Player if the Player is next to the Dragon
    return Actor::move(dung, player);
}

Item* Dragon::drop()
{
    int rand = randInt(5);
    Item* item = nullptr;
    //Return a pointer to a random scroll
    switch (rand)
    {
    case 0:
        item = new AScroll(getRow(), getCol());
        break;
    case 1:
            item = new DScroll(getRow(), getCol());
        break;
    case 2:
        item = new TScroll(getRow(), getCol());
        break;
    case 3:
            item = new SScroll(getRow(), getCol());
        break;
    case 4:
        item = new HScroll(getRow(), getCol());
        break;
    }
    return item;
}

Goblin::Goblin(int row, int col, int smellDistance)
    : Actor('G', "The Goblin", row, col, randInt(15,20), 1, 3, 1), m_smellDistance(smellDistance)
{
    setWeapon(new ShortSword());
}
Weapon* Goblin::drop()
{
    int rand = randInt(6);
    //1/6 of the time, return a pointer to a MagicFang
    if (rand == 0){
        Weapon* item = new MagicFang(getRow(), getCol());
        return item;
    }
    //1/6 of the time, return a pointer to a MagicAxe
    else if (rand == 1){
        Weapon* item = new MagicAxe(getRow(), getCol());
        return item;
    }
    else{
        return nullptr;
    }
}

string Goblin::move(Dungeon* dung, Actor* player)
{
    //If the Goblin is adjacent to the Actor, attack the Actor
    string msg = Actor::move(dung, player);
    //If the Goblin did not attack
    if (msg == "")
    {
        int dir=suggestmove(dung, player, getRow(), getCol(), m_smellDistance);
        switch (dir)
        {
        case 0:
            dung->setCellStatus(getRow(), getCol(), getcurrentGridState());
            setCol(getCol() + 1);
                cout<<"move 0";
            setcurrentGridState(dung->getCellStatus(getRow(), getCol()));
            dung->setCellStatus(getRow(), getCol(), 60);
            break;
        case 1:
            dung->setCellStatus(getRow(), getCol(), getcurrentGridState());
            setCol(getCol() - 1);
                cout<<"move 1";
            setcurrentGridState(dung->getCellStatus(getRow(), getCol()));
            dung->setCellStatus(getRow(), getCol(), 60);
            break;
        case 2:
            dung->setCellStatus(getRow(), getCol(), getcurrentGridState());
            setRow(getRow() - 1);
                cout<<"move 2";
            setcurrentGridState(dung->getCellStatus(getRow(), getCol()));
            dung->setCellStatus(getRow(), getCol(), 60);
            break;
        case 3:
            dung->setCellStatus(getRow(), getCol(), getcurrentGridState());
            setRow(getRow() + 1);
                cout<<"move 3";
            setcurrentGridState(dung->getCellStatus(getRow(), getCol()));
            dung->setCellStatus(getRow(), getCol(), 60);
            break;
        default:    //Don't move the goblin since there is not path to the player within the goblin's smelling range
                cout<<"not move";
            break;
        }
    }
    return msg;
}



int Goblin::suggestmove(Dungeon* dung, Actor* player, int row, int col, int sD){
    int prow=player->getRow(),pcol=player->getCol();
    if ((abs(prow-row)+abs(pcol-col))>sD)
        return -1;
    queue<Coord> pt;
    int grid[18][70];
    int testgrid[18][70];
    for (int i=0;i<18;i++)
        for (int j=0;j<70;j++)
            grid[i][j]=10000;
    for (int i=0;i<18;i++)
        for (int j=0;j<70;j++)
            testgrid[i][j]=0;
    Coord start(prow,pcol);
    int r=prow, c=pcol;
    pt.push(start);
    grid[prow][pcol]=0;
    while(!pt.empty()){
        r = pt.front().r();
        c = pt.front().c();
        pt.pop();
        Coord E(r,c+1);
        Coord W(r,c-1);
        Coord S(r-1,c);
        Coord N(r+1,c);
        if (!isMorW(dung,r,c+1) or (r==row and c+1==col)){
            if (grid[r][c+1]>grid[r][c]+1 or testgrid[r][c+1]==0){
                grid[r][c+1]=grid[r][c]+1;
                testgrid[r][c+1]=1;
                pt.push(E);
            }
            if (testgrid[r][c+1]==0)
                pt.push(E);
        }
        if (!isMorW(dung,r,c-1) or (r==row and c-1==col)){
            if (grid[r][c-1]>grid[r][c]+1 or testgrid[r][c-1]==0){
                grid[r][c-1]=grid[r][c]+1;
                testgrid[r][c-1]=1;
                pt.push(W);
            }
            if (testgrid[r][c-1]==0)
                pt.push(W);
        }
        if (!isMorW(dung,r-1,c) or (r-1==row and c==col)){
            if (grid[r-1][c]>grid[r][c]+1 or testgrid[r-1][c]==0){
                grid[r-1][c]=grid[r][c]+1;
                testgrid[r-1][c]=1;
                pt.push(S);
            }
            if (testgrid[r-1][c]==0)
                pt.push(S);
        }
        if (!isMorW(dung,r+1,c) or (r+1==row and c==col)){
            if (grid[r+1][c]>grid[r][c]+1 or testgrid[r+1][c]==0){
                grid[r+1][c]=grid[r][c]+1;
                testgrid[r+1][c]=1;
                pt.push(N);
            }
            if (testgrid[r+1][c]==0)
                pt.push(N);
        }
    }
    int mingrad=min(min(min(grid[row][col+1],grid[row][col-1]),grid[row-1][col]),grid[row+1][col]);
    if (mingrad==10000)
        return -1;
    else if (mingrad==grid[row][col+1])
        return 0;
    else if (mingrad==grid[row][col-1])
        return 1;
    else if (mingrad==grid[row-1][col])
        return 2;
    else
        return 3;
}




