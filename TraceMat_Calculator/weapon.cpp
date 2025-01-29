#include "weapon.h"

Weapon::Weapon(const string &name, int rarity, const string &path): m_name(name),
    m_rarity(rarity), m_path(path)
{
    if(m_rarity == 5){
        m_materials = {15,12,4};
    }
    else if(m_rarity == 4){
        m_materials = {12,9,3};
    }
    else{
        m_materials = {9,6,2};
    }
}
