//
//  Dungeon.cpp
//  CS32PJ3
//
//  Created by Zhe Wang on 5/15/20.
//  Copyright © 2020 Zhe Wang. All rights reserved.
//

#include "Actor.h"
#include "Dungeon.h"
#include "Item.h"
#include "utilities.h"
#include <iostream>
#include <string>
#include <list>
using namespace std;

Dungeon::Dungeon(int floor, int goblinSmellDistance):m_floor(floor),m_goblinSmellDistance(goblinSmellDistance)
{
    for (int i=0;i<18;i++)
        for (int j=0;j<70;j++)
            m_grid[i][j]=1;
    makeroom();
    additems();
    addactors();
    int row, col;
    getavai(row,col);
    m_player=new Player(row,col);
    m_player->setRow(importantrow());
    m_player->setCol(importantcol()-1);
}

Dungeon::Dungeon(int floor, int goblinSmellDistance, const Player& obj):m_floor(floor),m_goblinSmellDistance(goblinSmellDistance)
{
    //cout<<obj.getWeapon()->getName();
    for (int i=0;i<18;i++)
        for (int j=0;j<70;j++)
            m_grid[i][j]=1;
    makeroom();
    additems();
    addactors();
    int row, col;
    getavai(row,col);
    m_player=new Player(obj,row,col);
    m_player->setRow(importantrow());
    m_player->setCol(importantcol()-1);
}

Stair::Stair(int row, int col): m_symbol('>'){}

Dungeon::~Dungeon()
{
    delete m_player;
    //Delete all the monsters
    for (list<Actor*>::iterator i = m_monsters.begin(); i != m_monsters.end(); i++){
        delete *i;
    }
    m_monsters.clear();
    //Delete all the scrolls
    for (list<Item*>::iterator i = m_item.begin(); i != m_item.end(); i++){
        delete *i;
    }
    m_item.clear();
}

Player* Dungeon::player() const
{
    return m_player;
}

int Dungeon::floor() const
{
    return m_floor;
}
int Dungeon::importantrow() const
{
    return m_importantrow;
}

int Dungeon::importantcol() const
{
    return m_importantcol;
}

list<Actor*> Dungeon::monster()
{
    list<Actor*> *Monster=new list<Actor*>;
    *Monster=m_monsters;
    return *Monster;
}

list<Item*> Dungeon::item()
{
    list<Item*> *copyItem=new list<Item*>;
    *copyItem = m_item;
    return *copyItem;
}

int Dungeon::getCellStatus(int r, int c) const
{
    return m_grid[r][c];
}

void Dungeon::display()
{
    char disgrid[18][70];
    for (int i=0;i<18;i++)
        for (int j=0;j<70;j++){
            if (m_grid[i][j]==0)
                disgrid[i][j]=' ';
            else if (m_grid[i][j]==1)
                disgrid[i][j]='#';
            else if (m_grid[i][j]==16)
                disgrid[i][j]='&';
            else if (m_grid[i][j]==17)
                disgrid[i][j]='>';
        }
    //For each weapon, mark its location in the grid with a ')'
    for (list<Item*>::iterator i = m_item.begin(); i != m_item.end(); i++)
    {
        Item* item=*i;
        if (item->isWeapon())
            disgrid[(*i)->getRow()][(*i)->getCol()] = ')';
        else if (item->isScroll())
            disgrid[(*i)->getRow()][(*i)->getCol()] = '?';
    }
  
    //For each monster, mark its location in the grid with its corresponding symbol
    for (list<Actor*>::iterator i = m_monsters.begin(); i != m_monsters.end(); i++)
    {
        disgrid[(*i)->getRow()][(*i)->getCol()] = (*i)->getSymbol();
    }
    //Mark the location of the player with an '@'
    disgrid[m_player->getRow()][m_player->getCol()] = '@';
      // Draw the grid
    clearScreen();
    for (int r = 0; r < 18; r++)
    {
        for (int c = 0; c < 70; c++)
            cout << disgrid[r][c];
        cout << endl;
    }
    cout << "Dungeon Level: " << m_floor << ", Hit points: " << m_player->getHP() << ", Armor: " << m_player->getArP()
    << ", Strength: " << m_player->getSP() << ", Dexterity: " << m_player->getDP() << endl;
}

void Dungeon::setCellStatus(int r, int c, int status)
{
    //0 is empty
    //1 is wall
    //2-6 are weapons
    //11-15 are scrolls
    //17 is stair
    //16 is golden idol
    //20 is player
    //30 is bogeymen
    //40 is snakewomen
    //50 is dragon
    //60 is goblin
    m_grid[r][c] = status;

}

void Dungeon::getavai(int &row, int &col) const
{
    row = col = 0;
    //While the location at (row, col) in the grid is not empty
    while (m_grid[row][col] != 0)
    {
        //Get a random row and column
        row = randInt(16) + 1;
        col = randInt(68) + 1;
    }
}

void Dungeon::addactors()
{
    int n_monsters = 2 + randInt(5 * (m_floor + 1));
    for (int i = 0; i < n_monsters; i++)
    {
        int row, col;
        getavai(row, col);
        //Determine which monsters are placed
        int level;
        if (m_floor < 2){        //If level < 2, only the Goblin and Snakewoman are placed
            level = randInt(2);
        }
        else if (m_floor < 3){    //If level < 3, only the Goblin, Snakewoman, and Bogeyman are placed
            level = randInt(3);
        }
        else{                    //If level >= 4, place any of the monsters
            level = randInt(4);
        }
        //Add a monster to the dungeon
        switch (level)
        {
        case 0:
                m_monsters.push_back(new Bogeyman(row, col));
                setCellStatus(row, col, 30);
            break;
        case 1:
                m_monsters.push_back(new Snakewoman(row, col));
                setCellStatus(row, col, 40);
            break;
        case 2:
                m_monsters.push_back(new Dragon(row, col));
                setCellStatus(row, col, 50);
            break;
        case 3:
                m_monsters.push_back(new Goblin(row, col, m_goblinSmellDistance));
                setCellStatus(row, col, 60);
            break;
        }
    }
}

void Dungeon::additems(){
    for (int i = 0; i < randInt(2) + 2; i++)
    {
        int row, col;
        getavai(row, col);
        //Add a random weapon or scroll to the Dungeon
        int itemtype = randInt(7);
        switch (itemtype)
        {
        case 0:
            m_item.push_back(new Mace(row, col));
            break;
        case 1:
            m_item.push_back(new ShortSword(row, col));
            break;
        case 2:
            m_item.push_back(new LongSword(row, col));
            break;
        case 3:
            m_item.push_back(new AScroll(row, col));
            break;
        case 4:
                m_item.push_back(new SScroll(row, col));
            break;
        case 5:
            m_item.push_back(new HScroll(row, col));
            break;
        case 6:
            m_item.push_back(new DScroll(row, col));
            break;
        }
    }
}

string Dungeon::movePlayer(char direction)
{
    string msg="";
    int row = m_player->getRow();
    int col = m_player->getCol();
    //Set "row" and "col" as the row and column of where the player should move next
    switch (direction)
    {
    case ARROW_UP:
        row -= 1;
        break;
    case ARROW_DOWN:
        row += 1;
        break;
    case ARROW_RIGHT:
        col += 1;
        break;
    case ARROW_LEFT:
        col -= 1;
        break;
    }
    //Cycle through all the monsters
    for (list<Actor*>::iterator i = m_monsters.begin(); i != m_monsters.end(); i++)
    {
        //If there is a monster at this new location, attack the monster
        if (row == (*i)->getRow() && col == (*i)->getCol()){
            msg = m_player->getName()+" "+m_player->getWeapon()->action()+" "+(*i)->getName()+" "+m_player->attack(*i)+" Now the "+(*i)->getName()+" has "+to_string((*i)->getHP())+" hit points left.";
            return msg;
        }
    }
    //Otherwise, if there isn't a wall in the way, move the player to the new location
    if (m_grid[row][col] != 1){
        m_player->setRow(row);
        m_player->setCol(col);
    }
    return msg;
}

string Dungeon::movemonsters(Dungeon* dung)
{
    string msg="";
    //If the game is over, do nothing
    if (!gameover){
    //Cycle through all the monsters
    for (list<Actor*>::iterator i = m_monsters.begin(); i != m_monsters.end(); i++)
    {
        Actor* monster=*i;
        if (monster->getHP()<=0)
        {
            monster->setHP(0);
            msg+= monster->getName()+" dies.";
            //Check if there are weapons or scrolls or goldenidol at the monster's location
            bool canLoot = true;
            for (list<Item*>::iterator j = m_item.begin(); j != m_item.end(); j++){
                if ((monster->getRow() == (*j)->getRow() && monster->getCol() == (*j)->getCol()) or (monster->getRow()==importantrow() && monster->getCol()==importantcol())){
                    canLoot = false;
                }
            }
            //Drop a scroll or weapon if there is no item at the monster's location
            if (canLoot)
            {
                Item* lootItem = monster->drop();
                if (lootItem != nullptr){
                    m_item.push_back(lootItem);
                    msg+=" And it drops a "+lootItem->getName()+" .";
                }
            }
            //Erase the monster from the Dungeon
            m_monsters.erase(i);
        }
        //If the monster is asleep, decrement its sleepTime and have it do nothing
        if (monster->getSleepTime() > 0)
        {
            monster->setSleepTime(monster->getSleepTime() - 1);
        }
        //If the monster is not asleep
        else{
            msg+= monster->move(dung,m_player);
        }
    }
    //If the player has no hit points, indicate that the game is over
    if (m_player->getHP() <= 0){
        gameover = true;
    }
    }
    return msg;
}

void Dungeon::makeroom(){
    int roomNum=randInt(4, 6);
    vector<Room> rooms;
    for (int i = 0; i < roomNum; i++)
    {
        Room room0;                //The room to be added
        bool roomAvai = false;    //Determines whether a room can be placed without overlapping other rooms
        //While the room has not been placed and a placement attempt has not been tried more than 10,000 tiems
        while (!roomAvai)
        {
            //Set the row and col to a random value in the grid
            room0.m_row = randInt(1,13);
            room0.m_col = randInt(1,69);
            room0.m_length = randInt(5,7);
            room0.m_width = randInt(7,18);
            //If the room would extend pas the bottom of the grid, don't place it
            if (isRoomAvai(room0,rooms))
                roomAvai = true;
        }

        //If there is at least one room already in the dungeon, draw the corridor
        //Add the new room to the rooms array
        rooms.push_back(room0);
        drawcorridor(room0, rooms);
        //Draw the new room
        drawroom(room0);
    }
    //Place the staircase or idol at an empty location
    int row=0, col=0;
    getavai(row, col);
    m_importantrow=row;
    m_importantcol=col;
    if (m_floor != 4){        //Place a staircase if the level < 4
        m_stair=new Stair(row,col);
        setCellStatus(row, col, 17);
    }
    else{                    //Place an idol if the level == 4
        m_item.push_back(new Goldenidol(row, col));
        setCellStatus(row, col, 16);
    }
}

bool Dungeon::isRoomAvai(Room room0, vector<Room> rooms){
    if (room0.m_row + room0.m_length > 16)
    {
        return false;
    }
    //If the room would extend past the right side of the grid, don't place it
    if (room0.m_col + room0.m_width > 68)
    {
        return false;
    }
    //If the room overlaps with another room, don't place it
    for (int i = 0; i < rooms.size(); i++)
    {
        Room temp = rooms[i];
        bool overlapsHorizontally = false;
        bool overlapsVertically = false;
        //Check whether the room would overlap vertically with other rooms
        if ((temp.m_row <= room0.m_row && (temp.m_row + temp.m_length + 1) >= room0.m_row)
            || (room0.m_row <= temp.m_row && (room0.m_row + room0.m_length + 1) >= temp.m_row))
        {
            overlapsVertically = true;
        }
        //Check whether the room would overlap horizontally with other rooms
        if ((temp.m_col <= room0.m_col && (temp.m_col + temp.m_width + 1) >= room0.m_col)
            || (room0.m_col <= temp.m_col && (room0.m_col + room0.m_width + 1) >= temp.m_col))
        {
            overlapsHorizontally = true;
        }
        //If it would overlap, don't place the room
        if (overlapsHorizontally && overlapsVertically)
        {
            return false;
        }
    }
    return true;
}

void Dungeon::drawcorridor(Room room0, vector<Room> rooms){
    Room firstroom = rooms[0];
    //Find the square of the dminance between the new room and the first room
    int dmin = (firstroom.m_col - room0.m_col)*(firstroom.m_col - room0.m_col) + (firstroom.m_row - room0.m_row)*(firstroom.m_row - room0.m_row);
    //Cycle through all the rooms and find the room that is the shortest dminance away from the new room
    for (int i = 0; i < rooms.size()-1; i++)
    {
        int dtemp = (rooms[i].m_col - room0.m_col)*(rooms[i].m_col - room0.m_col) + (rooms[i].m_row - room0.m_row)*(rooms[i].m_row - room0.m_row);
        if (dtemp < dmin)
        {
            firstroom = rooms[i];
            dmin = dtemp;
        }
    }
    //Find the middle column and row of the closest room and of the new room
    int firstroomColMid = firstroom.m_col + firstroom.m_width / 2;
    int firstroomRowMid = firstroom.m_row + firstroom.m_length / 2;
    int room0ColMid = room0.m_col + room0.m_width / 2;
    int room0RowMid = room0.m_row + room0.m_length/ 2;
    //Draw a corridor vertically from the middle row and middle column of the old room to the middle row of the new room
    for (int i = firstroomRowMid; i != room0RowMid;)
    {
        m_grid[i][firstroomColMid] = 0;
        if (i < room0RowMid){
            i++;
        }
        else if (i > room0RowMid){
            i--;
        }
    }
    //From the end of that corridor, draw another corridor horizontally until it reaches the middle column of the new room
    for (int i = firstroomColMid; i != room0ColMid;)
    {
        m_grid[room0RowMid][i] = 0;
        if (i < room0ColMid){
            i++;
        }
        else if (i > room0ColMid){
            i--;
        }
    }
}

void Dungeon::drawroom(Room room0){
    //Set the Dungeon's grid to ' ' for every space the room occupies
    for (int i = room0.m_row; i < (room0.m_row + room0.m_length); i++){
        for (int j = room0.m_col; j < (room0.m_col + room0.m_width); j++){
            m_grid[i][j] = 0;
        }
    }
}

void Dungeon::newlevel(int floor){
    m_monsters.clear();
    m_item.clear();
    for (int i=0;i<18;i++)
        for (int j=0;j<70;j++)
            m_grid[i][j]=1;
    m_floor=floor;
    makeroom();
    additems();
    addactors();
    int row, col;
    getavai(row,col);
    m_player->setRow(importantrow());
    m_player->setCol(importantcol()-1);
}
