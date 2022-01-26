// Game.h

#ifndef GAME_INCLUDED
#define GAME_INCLUDED

#include "utilities.h"
#include <iostream>
#include "Dungeon.h"
#include "Actor.h"
#include "Item.h"
#include <list>
#include <string>
using namespace std;

// You may add data members and other member functions to this class.

class Game
{
public:
    Game(int goblinSmellDistance);
    void play();
    string playerTurn(char command);
private:
    Dungeon*  m_dung;
    Player*   m_player;
    int       m_floor;
    
    int m_goblinSmellDistance;
};

#endif // GAME_INCLUDED
