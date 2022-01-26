//
//  Item.h
//  CS32PJ3
//
//  Created by Zhe Wang on 5/15/20.
//  Copyright © 2020 Zhe Wang. All rights reserved.
//

#ifndef Item_h
#define Item_h

#include <string>
using namespace std;

class Actor;

class Item
{
public:
    Item(char symbol, string name, int row, int col);
    //Constructor
    virtual ~Item() {}
    //Destructor
    virtual string use(Actor* target){ return ""; }
    //Will return the result of using the Item on the target Actor
    bool isWeapon() const;
    //Returns true if the Item is a weapon. Returns false otherwise.
    bool isScroll() const;
    //Returns true if the Item is a scroll. Returns false otherwise.

    //Accessor functions
    char getSymbol() const { return m_symbol; }
    string getName() const { return m_name; }
    int getRow() const { return m_row; }
    int getCol() const { return m_col; }
private:
    char m_symbol;        //Character displayed when the Item is displayed in a Dungeon.
    string m_name;        //Name of the Item
    int m_row;            //Row in Dungeon grid
    int m_col;            //Column in Dungeon grid
};

class TScroll : public Item
{
public:
    TScroll(int row = -1, int col = -1);
    //Constructs a scroll of teleportation at row "row" and column "col"
    virtual string use(Actor* target);
    //Returns "teleport", indicating that the Dungeon should teleport the Player
};

class AScroll : public Item
{
public:
    AScroll(int row = -1, int col = -1);
    //Constructs a scroll of improve armor at row "row" and column "col"
    virtual string use(Actor* target);
    //Increases the target's armor by 1-3 points and returns a string detailing the change.
};

class SScroll : public Item
{
public:
    SScroll(int row = -1, int col = -1);
    //Constructs a scroll or raise strength at row "row" and column "col"
    virtual string use(Actor* target);
    //Increases the target's strength by 1-3 points and returns a string detailing the change
};

class HScroll : public Item
{
public:
    HScroll(int row = -1, int col = -1);
    //Constructs a scroll of enhance health at row "row" and column "col"
    virtual string use(Actor* target);
    //Increases the target's max hit points 3-8 points and returns a string detailing the change
};

class DScroll : public Item
{
public:
    DScroll(int row = -1, int col = -1);
    //Constructs a scroll of enhance dexterity at row "row" and column "col"
    virtual string use(Actor* target);
    //Increases the target's dexterity by 1 and returns a string detailing the change
};

class Weapon : public Item
{
public:
    Weapon(string name, int row, int col, int dexbonus, int damage);
    Weapon(Weapon& obj);
    //Constructor
    virtual ~Weapon() {}
    //Destructor
    virtual string action() const =0;
    //Will return a string detailing how the weapon is used to attack a target

    //Accessor Functions
    int getDexBonus() const { return m_dexbonus; }
    int getDamage() const { return m_damage; }

    virtual string use(Actor* target) { return ""; }
    //Returns an empty string as the Weapon needs a wielder and a target
private:
    int m_dexbonus;        //Dexterity bonus of the Weapon
    int m_damage;        //Damage the Weapon does
};

class Mace : public Weapon
{
public:
    Mace(int row = -1, int col = -1);
    //Constructs a mace at row "row" and column "col"
    virtual string action() const;
    //Returns a string detailing how the mace is used
};

class ShortSword : public Weapon
{
public:
    ShortSword(int row = -1, int col = -1);
    //Constructs a short sword at row "row" and column "col"
    virtual string action() const;
    //Returns a string detailing how the short sword is used.
};

class LongSword : public Weapon
{
public:
    LongSword(int row = -1, int col = -1);
    //Constructs a long sword at row "row" and column "col"
    virtual string action() const;
    //Returns a string detailing how the long sword is used.
};

class MagicAxe : public Weapon
{
public:
    MagicAxe(int row = -1, int col = -1);
    //Constructs a magic axe at row "row" and column "col"
    virtual string action() const;
    //Returns a string detailing how the magic axe is used.
};

class MagicFang : public Weapon
{
public:
    MagicFang(int row = -1, int col = -1);
    //Constructs a magic fang of sleep at row "row" and column "col"
    virtual string action() const;
    //Returns a string detailing how the magic fang of sleep is used.
};

class Goldenidol : public Item
{
public:
    Goldenidol(int row = -1, int col = -1);
    //Constructs a scroll of teleportation at row "row" and column "col"
    virtual string use();
};

#endif /* Item_h */
