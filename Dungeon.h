//
//  Dungeon.h
//  CS32PJ3
//
//  Created by Zhe Wang on 5/15/20.
//  Copyright © 2020 Zhe Wang. All rights reserved.
//

#ifndef Dungeon_h
#define Dungeon_h

#include <string>
#include <list>
#include <vector>
using namespace std;

class Player;
class Actor;
class Item;
struct Room;
class Stair;

class Dungeon
{
  public:
      // Constructor/destructor
    Dungeon(int floor, int goblinSmellDistance);
    Dungeon(int floor, int goblinSmellDistance, const Player& obj);
    ~Dungeon();

      // Accessors
    int     floor() const;
    Player* player() const;
    int     getCellStatus(int r, int c) const;
    void    display();
    int     importantrow() const;
    int     importantcol() const;
    list<Actor*> monster();
    list<Item*>  item();
    void getavai(int &row, int &col) const;
    

      // Mutators
    void setCellStatus(int r, int c, int status);
    string movePlayer(char direction);
    string movemonsters(Dungeon* dung);
    bool gameover=false;
    bool quit=false;
    void newlevel(int floor);
    

  private:
    int      m_grid[18][70];
    int      m_floor;
    Player*  m_player;
    list<Actor*> m_monsters;
    list<Item*> m_item;
    Stair*   m_stair;
    int      m_turns;
    int      m_goblinSmellDistance;
    int      m_importantrow;
    int      m_importantcol;
    
    
      // Helper functions
    void makeroom();
    void addactors();
    void additems();
    void drawcorridor(Room room0, vector<Room> rooms);
    bool isRoomAvai(Room room0, vector<Room> rooms);
    void drawroom(Room room0);
};
struct Room{
    int m_row,m_col,m_length,m_width;
};
class Stair
{
public:
    Stair(int row, int col);
    int m_row;        //Row in Dungeon grid
    int m_col;        //Col in Dungeon grid
    char m_symbol;    //'>' if the Exit is a staircase and '&' if the Exit is a golden idol
};
#endif /* Dungeon_h */
