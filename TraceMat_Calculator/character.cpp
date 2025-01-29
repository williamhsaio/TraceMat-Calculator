#include "character.h"

Character::Character(const string &name, int rarity, const string &path): m_name(name),
    m_rarity(rarity), m_path(path)
{
    if(rarity == 5){
        m_materials = {139,69,18};
    }
    else{
        m_materials = {114,60,14};
    }
}
