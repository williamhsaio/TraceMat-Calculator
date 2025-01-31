#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "character.h"
#include "weapon.h"

#include <QObject>
#include <string>

using namespace std;

class DataModel: public QObject
{
    Q_OBJECT
public:
    enum class Errors{
        SUCCESS,
        FAIL_TO_ADD,
        FAIL_TO_UPDATE
    };
    enum class DBErrors{
        SUCCESS,
        SELECT_FAIL,
        DB_WRITE_FAIL,
        DB_READ_FAIL,
        KEY_EXISTS
    };

    explicit DataModel(QObject *parent = nullptr);
    ~DataModel();

    Errors addCharacter(const string &name, const string &path, int rarity);
    Errors updateCharMats(const string &name, int purples, int blues, int greens);
    Errors addWeapon(const string &name, const string &path, int rarity);
    Errors updateWepMats(const string &name, int purples, int blues, int greens);

    DBErrors loadCharData();
    DBErrors loadWepData();
    DBErrors saveCharData(const string &name, const string &path, int rarity, const vector<int> &materials);
    DBErrors saveWepData(const string &name, const string &path, int rarity, const vector<int> &materials);
    DBErrors updateCharData(const string &name, const vector<int> &materials);
    DBErrors updateWepData(const string &name, const vector<int> &materials);

    bool charExists(const string &name);
    bool wepExists(const string &name);

    Character* getChar(const string &name) const;
    Weapon* getWep(const string &name) const;
    vector<Character*> getCharList() const{
        return charList;
    }
    vector<Weapon*> getWepList() const{
        return wepList;
    }

private:
    vector<Character*> charList;
    vector<Weapon*> wepList;
};

#endif // DATAMODEL_H
