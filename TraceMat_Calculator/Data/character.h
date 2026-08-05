#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <vector>

using namespace std;

class Character
{
public:
    Character(const string &name, int rarity, const string &path);

    string getName() const{
        return m_name;
    }
    int getRarity() const{
        return m_rarity;
    }
    string getPath() const{
        return m_path;
    }
    vector<int> getMaterials() const{
        return m_materials;
    }

    void setName(string name){
        m_name = name;
    }
    void setRarity(int rarity){
        m_rarity = rarity;
    }
    void setPath(string path){
        m_path = path;
    }
    void setMaterials(int purples, int blues, int greens){
        m_materials[0] = purples;
        m_materials[1] = blues;
        m_materials[2] = greens;
    }

private:
    string m_name;
    string m_path;
    int m_rarity;
    vector<int> m_materials;
};

#endif // CHARACTER_H
