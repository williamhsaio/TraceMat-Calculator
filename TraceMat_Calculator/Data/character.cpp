#include "character.h"

Character::Character(const string &name, int rarity, const string &path): m_name(name),
    m_rarity(rarity), m_path(path), m_materials({0,0,0})
{}
