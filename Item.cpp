//
//  Item.cpp
//  CS32PJ3
//
//  Created by Zhe Wang on 5/15/20.
//  Copyright © 2020 Zhe Wang. All rights reserved.
//

#include "Item.h"
#include "Actor.h"
#include "utilities.h"
#include <string>
using namespace std;

Item::Item(char symbol, string name, int row, int col)
    :m_symbol(symbol), m_name(name), m_row(row), m_col(col)
{}

bool Item::isWeapon() const
{
    //If the object's name is that of a weapon, return true
    return (getName() == "mace" || getName() == "short sword" || getName() == "long sword"
        || getName() == "magic axe" || getName() == "magic fang of sleep");
}
bool Item::isScroll() const
{
    //If the object's name is that of a scroll, return true
    return (getName() == "a scroll of teleportation" || getName() == "a scroll of improve armor"
        || getName() == "a scroll of raise strength" || getName() == "a scroll of enhance dexterity"
        || getName() == "a scroll of enhance health");
}

TScroll::TScroll(int row, int col)
    :Item('?', "a scroll of teleportation", row, col)
{}
string TScroll::use(Actor* target)
{
    return "teleport";
}

AScroll::AScroll(int row, int col)
    :Item('?', "a scroll of improve armor", row, col)
{}
string AScroll::use(Actor* target)
{
    //Increase the target's hit points by 1-3 points and display a message
    target->setArP(target->getArP() + randInt(3) + 1);
    string msg = "You read " + getName() + ". Your armor glows blue.\n";
    return msg;
}

SScroll::SScroll(int row, int col)
    :Item('?', "a scroll of raise strength", row, col)
{}
string SScroll::use(Actor* target)
{
    //Increase the target's strength points by 1-3 points and display a message
    target->setSP(target->getSP() + randInt(3) + 1);
    string msg = "You read " + getName() + ". Your muscles bulge.\n";
    return msg;
}

HScroll::HScroll(int row, int col)
    :Item('?', "a scroll of enhance health", row, col)
{}
string HScroll::use(Actor* target)
{
    //Increase the target's maximum hit points by 3-8 points and display a message
    target->setMaxHP(target->getMaxHP() + randInt(6) + 3);
    string msg = "You read " + getName() + ". You feel your heart beating stronger.\n";
    return msg;
}

DScroll::DScroll(int row, int col)
    :Item('?', "a scroll of enhance dexterity", row, col)
{}
string DScroll::use(Actor* target)
{
    //Increase the target's dexterity points by 1 and display a message
    target->setDP(target->getDP() + 1);
    string msg = "You read " + getName() + ". You feel like less of a klutz.\n";
    return msg;
}

Weapon::Weapon(string name, int row, int col, int dexbonus, int damage)    //Sets the member values of Weapon
    : Item(')', name, row, col), m_dexbonus(dexbonus), m_damage(damage)
{}
Weapon::Weapon(Weapon& obj): Item(obj.getSymbol(), obj.getName(), obj.getRow(), obj.getCol()), m_dexbonus(obj.getDexBonus()), m_damage(obj.getDamage())
{}

//Simple constructors and functions
Mace::Mace(int row, int col)
    : Weapon("mace", row, col, 0, 2)
{}
string Mace::action() const
{
    return "swings mace at";
}

ShortSword::ShortSword(int row, int col)
    : Weapon("short sword", row, col, 0, 2)
{}
string ShortSword::action() const
{
    return "slashes short sword at";
}

LongSword::LongSword(int row, int col)
    : Weapon("long sword", row, col, 2, 4)
{}
string LongSword::action() const
{
    return "swings long sword at";
}

MagicAxe::MagicAxe(int row, int col)
    : Weapon("magic axe", row, col, 5, 5)
{}
string MagicAxe::action() const
{
    return "chops magic axe at";
}

MagicFang::MagicFang(int row, int col)
    : Weapon("magic fang of sleep", row, col, 3, 2)
{}
string MagicFang::action() const
{
    return "strikes magic fangs at";
}

Goldenidol::Goldenidol(int row, int col): Item('?', "a golden idol", row, col){}

string Goldenidol::use(){
    string msg="A mysterious treasure.";
    return msg;
}
