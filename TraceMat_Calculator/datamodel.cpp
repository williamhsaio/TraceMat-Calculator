#include "datamodel.h"

#include <iostream>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>

DataModel::DataModel(QObject *parent) {
    loadCharData();
    loadWepData();
}

DataModel::~DataModel(){
    while(!charList.empty()){
        Character *c = charList.back();
        delete(c);
        charList.pop_back();
    }
    while(!wepList.empty()){
        Weapon *w = wepList.back();
        delete(w);
        wepList.pop_back();
    }
}

DataModel::Errors DataModel::addCharacter(const string &name, const string &path, int rarity){
    if(path.empty() || name.empty()){
        cout<<"Please give a valid string."<<endl;
        return Errors::FAIL_TO_ADD;
    }
    if(rarity == 5 || rarity == 4){
        const auto it = find_if(charList.begin(), charList.end(), [name](const auto *p){
            return p->getName() == name;
        });
        if(it != charList.end()){
            cout<<"Character already exists!"<<endl;
            return Errors::FAIL_TO_ADD;
        }
        else{
            Character *c = new Character(name, rarity, path);
            charList.push_back(c);

            vector<int> mats = c->getMaterials();
            saveCharData(name, path, rarity, mats[0], mats[1], mats[2]);
            return Errors::SUCCESS;
        }

    }
    else{
        cout<<"Character rarity must be 4 or 5."<<endl;
        return Errors::FAIL_TO_ADD;
    }
}

DataModel::Errors DataModel::updateCharMats(const string &name, int purples, int blues, int greens){
    if(purples < 0 || blues < 0 || greens < 0 || purples % 1 < 0 || greens % 1 < 0 || blues % 1 < 0){
        cout<<"Values must be positive integers."<<endl;
        return Errors::FAIL_TO_UPDATE;
    }
    const auto it = find_if(charList.begin(), charList.end(), [name](const auto *p){
        return p->getName() == name;
    });
    if(it != charList.end()){
        vector<int> mats = (*it)->getMaterials();
        if(mats[2] - greens < 0){
            blues += greens/3;
            greens = mats[2];
        }
        if(mats[1] - blues < 0){
            purples += blues/3;
            blues = mats[1];
        }
        if(mats[0] - purples <=0){
            purples = mats[0];
        }

        (*it)->setMaterials(mats[0]-purples, mats[1]-blues, mats[2]-greens);
        return Errors::SUCCESS;
    }
    else{
        cout<<"Character does not exist."<<endl;
        return Errors::FAIL_TO_UPDATE;
    }
}

DataModel::Errors DataModel::addWeapon(const string &name, const string &path, int rarity){
    if(path.empty() || name.empty()){
        cout<<"Please give a valid string."<<endl;
        return Errors::FAIL_TO_ADD;
    }
    if(rarity == 5 || rarity == 4 || rarity == 3){
        const auto it = find_if(wepList.begin(), wepList.end(), [name](const auto *p){
            return p->getName() == name;
        });
        if(it != wepList.end()){
            cout<<"Weapon already exists!"<<endl;
            return Errors::FAIL_TO_ADD;
        }
        else{
            Weapon *c = new Weapon(name, rarity, path);
            wepList.push_back(c);
        }
        return Errors::SUCCESS;
    }
    else{
        cout<<"Weapon rarity must be between 3 and 5 inclusive."<<endl;
        return Errors::FAIL_TO_ADD;
    }

}

DataModel::Errors DataModel::updateWepMats(const string &name, int purples, int blues, int greens){
    if(purples < 0 || blues < 0 || greens < 0 || purples % 1 < 0 || greens % 1 < 0 || blues % 1 < 0){
        cout<<"Values must be positive integers."<<endl;
        return Errors::FAIL_TO_UPDATE;
    }
    const auto it = find_if(wepList.begin(), wepList.end(), [name](const auto *p){
        return p->getName() == name;
    });
    if(it != wepList.end()){
        vector<int> mats = (*it)->getMaterials();
        if(mats[2] - greens < 0){
            blues += greens/3;
            greens = mats[2];
        }
        if(mats[1] - blues < 0){
            purples += blues/3;
            blues = mats[1];
        }
        if(mats[0] - purples <=0){
            purples = mats[0];
        }

        (*it)->setMaterials(mats[0]-purples, mats[1]-blues, mats[2]-greens);
        return Errors::SUCCESS;
    }
    else{
        cout<<"Weapon does not exist."<<endl;
        return Errors::FAIL_TO_UPDATE;
    }
}

Character* DataModel::getChar(const string &name) const{
    const auto it = find_if(charList.begin(), charList.end(), [name](const auto *p){
        return p->getName() == name;
    });
    if(it != charList.end()){
        return *it;
    }
    return nullptr;
}
Weapon* DataModel::getWep(const string &name) const{
    const auto it = find_if(wepList.begin(), wepList.end(), [name](const auto *p){
        return p->getName() == name;
    });
    if(it != wepList.end()){
        return *it;
    }
    return nullptr;
}

bool DataModel::charExists(const string &name){
    bool nameExists = false;
    QSqlQuery query;
    query.prepare("select* from character_data where name=:name");
    query.bindValue(":name", QString::fromStdString(name));
    query.exec();

    nameExists = query.next();
    return nameExists;
}

bool DataModel::wepExists(const string &name){
    bool nameExists = false;
    QSqlQuery query;
    query.prepare("select* from weapon_data where name=:name");
    query.bindValue(":name", QString::fromStdString(name));
    query.exec();

    nameExists = query.next();
    return nameExists;
}
/*insertDataTable(const string &table, const string &name, const string &path, int rarity){
QSqlQuery query;
query.prepare("insert into table_name (name, path, rarity) "
              "values(:name, :path, :rarity)");
query.bindValue(":name", QString::fromStdString(name));
query.bindValue(":path", QString::fromStdString(path));
query.bindValue(":rarity", rarity);
int charID = query.value("characterID").toInt();
if(!query.exec()){
    qDebug()<<"Query failed to execute: "<<query.lastError();
    return DBErrors::DB_WRITE_FAIL;
}
}*/
DataModel::DBErrors DataModel::saveCharData(const string &name, const string &path, int rarity, int purples, int blues, int greens){
    if(charExists(name)){
        return DBErrors::KEY_EXISTS;
    }
    QSqlQuery query;
    query.prepare("insert into character_data(name, path, rarity) "
                  "values(:name, :path, :rarity)");
    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":path", QString::fromStdString(path));
    query.bindValue(":rarity", rarity);
    int charID = query.value("characterID").toInt();
    if(!query.exec()){
        qDebug()<<"Query failed to execute: "<<query.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }
    query.prepare("insert into char_mats(characterID, purples, blues, greens) "
                  "values(:characterID, :purples, :blues, :greens)");
    query.bindValue(":characterID", charID);
    query.bindValue(":purples", purples);
    query.bindValue(":blues", blues);
    query.bindValue(":greens", greens);
    if(!query.exec()){
        qDebug()<<"Query failed to execute: "<<query.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }
    return DBErrors::SUCCESS;
}

DataModel::DBErrors DataModel::loadCharData(){
    QSqlQuery query;
    if(!query.exec("select * from character_data "
                    "join char_mats "
                    "on character_data.characterID = char_mats.characterID")){
        qDebug()<<"Select failed: "<< query.lastError();
        return DBErrors::SELECT_FAIL;
    }
    while(query.next()){
        string name = query.value("name").toString().toStdString();
        string path = query.value("path").toString().toStdString();
        int rarity = query.value("rarity").toInt();
        int purples = query.value("purples").toInt();
        int blues = query.value("blues").toInt();
        int greens = query.value("greens").toInt();
        addCharacter(name, path, rarity);
        for(auto c: getCharList()){
            c->setMaterials(purples, blues, greens);
        }
    }
    return DBErrors::SUCCESS;
}

DataModel::DBErrors DataModel::loadWepData(){
    QSqlQuery query;
    if(!query.exec("select * from weapon_data "
                    "join wep_mats "
                    "on weapon_data.wepID = wep_mats.wepID")){
        qDebug()<<"Select failed: "<< query.lastError();
        return DBErrors::SELECT_FAIL;
    }
    while(query.next()){
        string name = query.value("name").toString().toStdString();
        string path = query.value("path").toString().toStdString();
        int rarity = query.value("rarity").toInt();
        int purples = query.value("purples").toInt();
        int blues = query.value("blues").toInt();
        int greens = query.value("greens").toInt();
        addWeapon(name, path, rarity);
        for(auto w: getWepList()){
            w->setMaterials(purples, blues, greens);
        }
    }
    return DBErrors::SUCCESS;
}
