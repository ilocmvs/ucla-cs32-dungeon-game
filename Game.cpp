// Game.cpp

#include "Game.h"
#include "utilities.h"
#include <iostream>
#include "Dungeon.h"
#include "Actor.h"
#include "Item.h"
#include <list>
#include <string>
using namespace std;

// Implement these and other Game member functions you may have added.

Game::Game(int goblinSmellDistance):m_goblinSmellDistance(goblinSmellDistance),m_floor(0)
{
    m_dung = new Dungeon(m_floor,m_goblinSmellDistance);
}

void Game::play()
{
    cout<<"Type in the name of your player:"<<endl;
    char name[100];
    cin.get(name,100);
    m_dung->player()->setName(name);
    m_dung->display();
    while (!m_dung->gameover){
        string playermsg=playerTurn(getCharacter());
        string monstermsg=m_dung->movemonsters(m_dung);
        m_dung->display();
        cout<<endl<<playermsg<<endl<<monstermsg<<endl;
    }
    if (m_dung->gameover){
        cout << "Press q to exit game." << endl;
        while (getCharacter() != 'q')
            ;
    }
}

string Game::playerTurn(char command)
{
    //10% of the time, increase the player's hit points by 1 (up to its maximum hit points)
    if (trueWithProbability(0.1)){
        m_dung->player()->setHP(m_dung->player()->getHP() + 1);
    }
    if (m_dung->player()->getSleepTime() > 0){
        m_dung->player()->setSleepTime(m_dung->player()->getSleepTime() - 1);
        return "";
    }
    //Carry out the command
    switch (command)
    {
    //If the command is an indicator for the player to move
    case ARROW_UP:
    case ARROW_DOWN:
    case ARROW_RIGHT:
    case ARROW_LEFT:
        //Move the player in the given direction
        return m_dung->movePlayer(command);
        break;
    //Get an object
    case 'g':
        //If there is a golden idol at the player's location, indicate that the player has won
        if (m_dung->player()->getRow() == m_dung->importantrow() && m_dung->player()->getCol() == m_dung->importantcol() && m_dung->floor()==4)
        {
            m_dung->gameover = true;
            return "You pick up the golden idol\nCongratulations, you won!\n";
        }
        //If there is a scroll at the player's location, add it to the player's inventory and erase it from the Dungeon
            for (list<Item*>::iterator p = m_dung->item().begin(); p != m_dung->item().end(); p++){
                string msg="";
            if ((*p)->getRow() == m_dung->player()->getRow() && (*p)->getCol() == m_dung->player()->getCol()){
                if ((*p)->isScroll())
                    msg = m_dung->player()->pickupScroll(*p);
                else if ((*p)->isWeapon()){
                    Weapon* q=dynamic_cast<Weapon*>(*p);
                    msg = m_dung->player()->pickupWeapon((q));
                }
                m_dung->item().erase(p);
                return msg;
            }
        }
        break;
    //Wield a weapon
    case 'w':
        //Display the inventory
        m_dung->player()->displayInventory();
        //Wield the weapon if a weapon is chosen from the inventory
        return m_dung->player()->selectWeapon(getCharacter());
        break;
    //Read a scroll
    case 'r':
        //Display the inventory
        m_dung->player()->displayInventory();
        if (true){
            //Read the scroll if a scroll is chosen from the inventory
            string msg = m_dung->player()->readScroll(getCharacter());
            //If the scroll read is a scroll of teleportation, teleport the player
            if (msg == "teleport"){
                int row, col;
                m_dung->getavai(row, col);
                m_dung->player()->setRow(row);
                m_dung->player()->setCol(col);
                return "You read a scroll of teleportation. You feel your body wrenched in space and time.\n";
            }
            else{
                return msg;
            }
        }
        break;
    //Display the inventory
    case 'i':
        //Have the player display the inventory
        m_dung->player()->displayInventory();
        //Return to normal after another key is pressed
        getCharacter();
        break;
    //Move down a level
    case '>':
        //If there is a staircase at the player's location, move down a level and generate a new level
        if (m_dung->player()->getRow() == m_dung->importantrow() && m_dung->player()->getCol() == m_dung->importantcol() && m_dung->floor()!=4){
            //carry on with new player
            //Player newplayer(*(m_dung->player()),m_dung->player()->getRow(),m_dung->player()->getCol());
            //delete m_dung;
            //cout<<m_dung->player()->getWeapon()->getName();
            //m_dung=nullptr;
            m_floor++;
            m_dung->newlevel(m_floor);
            //m_dung = new Dungeon(m_floor,m_goblinSmellDistance,newplayer);
        }
        break;
    //Quit the game
    case 'q':
        //Indicate that the game is over and has been quit
        m_dung->gameover= true;
        m_dung->quit = true;
        break;
    //Cheat
    case 'c':
        //Set the player's strength points to 9 and its hit point and max hit points to 50
        m_dung->player()->setSP(9);
        m_dung->player()->setMaxHP(50);
        m_dung->player()->setHP(50);
    //If the character is not a valid command, do nothing
    default:
        break;
    }
    return "";
}

// You will presumably add to this project other .h/.cpp files for the
// various classes of objects you need to play the game:  player, monsters,
// weapons, etc.  You might have a separate .h/.cpp pair for each class
// (e.g., Player.h, Boegeyman.h, etc.), or you might put the class
// declarations for all actors in Actor.h, all game objects in GameObject.h,
// etc.
