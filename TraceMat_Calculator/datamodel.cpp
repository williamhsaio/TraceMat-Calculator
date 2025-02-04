#include "datamodel.h"

#include <iostream>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <unordered_map>

static unordered_map<int, matsPerLevel> otherCharMats;
static unordered_map<int, matsPerLevel> basicCharMats;

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

void DataModel::initializeCharMap(int rarity){
    if(rarity == 5){
        otherCharMats[1] = {0,0,0};
        otherCharMats[2] = {0,0,3};
        otherCharMats[3] = {0,3,0};
        otherCharMats[4] = {0,5,0};
        otherCharMats[5] = {0,7,0};
        otherCharMats[6] = {3,0,0};
        otherCharMats[7] = {5,0,0};
        otherCharMats[8] = {8,0,0};
        otherCharMats[9] = {14,0,0};

        basicCharMats[1] = {0,0,3};
        basicCharMats[2] = {0,3,0};
        basicCharMats[3] = {0,5,0};
        basicCharMats[4] = {3,0,0};
        basicCharMats[5] = {8,0,0};
    }
    else{
        otherCharMats[1] = {0,0,0};
        otherCharMats[2] = {0,0,2};
        otherCharMats[3] = {0,2,0};
        otherCharMats[4] = {0,4,0};
        otherCharMats[5] = {0,6,0};
        otherCharMats[6] = {2,0,0};
        otherCharMats[7] = {4,0,0};
        otherCharMats[8] = {6,0,0};
        otherCharMats[9] = {11,0,0};

        basicCharMats[1] = {0,0,2};
        basicCharMats[2] = {0,2,0};
        basicCharMats[3] = {0,4,0};
        basicCharMats[4] = {2,0,0};
        basicCharMats[5] = {6,0,0};
    }
}

vector<int> DataModel::findCharMats(int currLevel, int finLevel){
    vector<int> totMats;
    int purples = 0;
    int blues = 0;
    int greens = 0;

    for(int i = currLevel; i < finLevel; i++){
        matsPerLevel mats = otherCharMats[i];
        purples += mats.purples;
        blues += mats.blues;
        greens += mats.greens;
    }
    totMats.push_back(purples);
    totMats.push_back(blues);
    totMats.push_back(greens);

    return totMats;
}
vector<int> DataModel::findCharBasic(int currLevel, int finLevel){
    vector<int> totMats;
    int purples = 0;
    int blues = 0;
    int greens = 0;

    for(int i = currLevel; i < finLevel; i++){
        cout<<"Current Level: "<<i<<endl;
        matsPerLevel mats = basicCharMats[i];
        cout<<"Struct purples: "<<mats.purples<<endl;
        purples += mats.purples;
        blues += mats.blues;
        cout<<"Struct blues: "<<mats.blues<<endl;
        greens += mats.greens;
        cout<<"Struct greens: "<<mats.greens<<endl;
    }
    totMats.push_back(purples);
    totMats.push_back(blues);
    totMats.push_back(greens);

    return totMats;
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
            delete(*it);
            return Errors::FAIL_TO_ADD;
        }
        else{
            Character *c = new Character(name, rarity, path);
            charList.push_back(c);

            saveCharData(name, path, rarity, c->getMaterials());
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
        updateCharData(name, (*it)->getMaterials());
        return Errors::SUCCESS;
    }
    else{
        cout<<"Character does not exist."<<endl;
        delete(*it);
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
            delete(*it);
            return Errors::FAIL_TO_ADD;
        }
        else{
            Weapon *c = new Weapon(name, rarity, path);
            wepList.push_back(c);

            saveWepData(name, path, rarity, c->getMaterials());
            return Errors::SUCCESS;
        }
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
        updateWepData(name, (*it)->getMaterials());
        return Errors::SUCCESS;
    }
    else{
        cout<<"Weapon does not exist."<<endl;
        delete(*it);
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
    delete(*it);
    return nullptr;
}
Weapon* DataModel::getWep(const string &name) const{
    const auto it = find_if(wepList.begin(), wepList.end(), [name](const auto *p){
        return p->getName() == name;
    });
    if(it != wepList.end()){
        return *it;
    }
    delete(*it);
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
DataModel::DBErrors DataModel::saveCharData(const string &name, const string &path, int rarity, const vector<int> &materials){
    if(charExists(name)){
        return DBErrors::KEY_EXISTS;
    }
    QSqlQuery query;
    query.prepare("insert into character_data(name, path, rarity) "
                  "values(:name, :path, :rarity)");
    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":path", QString::fromStdString(path));
    query.bindValue(":rarity", rarity);

    if(!query.exec()){
        qDebug()<<"Query failed to execute: "<<query.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }

    QSqlQuery query2;
    query2.prepare("insert into character_mats(name, purples, blues, greens) "
                   "values(:name, :purples, :blues, :greens)");
    query2.bindValue(":name", QString::fromStdString(name));
    query2.bindValue(":purples", materials[0]);
    query2.bindValue(":blues", materials[1]);
    query2.bindValue(":greens", materials[2]);

    if(!query2.exec()){
        qDebug()<<"Mats Query failed to execute: "<<query2.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }
    return DBErrors::SUCCESS;
}

DataModel::DBErrors DataModel::saveWepData(const string &name, const string &path, int rarity, const vector<int> &materials){
    if(wepExists(name)){
        return DBErrors::KEY_EXISTS;
    }
    QSqlQuery query;
    query.prepare("insert into weapon_data(name, path, rarity) "
                  "values(:name, :path, :rarity)");
    query.bindValue(":name", QString::fromStdString(name));
    query.bindValue(":path", QString::fromStdString(path));
    query.bindValue(":rarity", rarity);

    if(!query.exec()){
        qDebug()<<"Insertion into weapon_data failed: "<<query.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }

    QSqlQuery query2;
    query2.prepare("insert into wep_mats(name, purples, blues, greens) "
                   "values(:name, :purples, :blues, :greens)");
    query2.bindValue(":name", QString::fromStdString(name));
    query2.bindValue(":purples", materials[0]);
    query2.bindValue(":blues", materials[1]);
    query2.bindValue(":greens", materials[2]);

    if(!query2.exec()){
        qDebug()<<"Insertion into wep_mats failed: "<<query2.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }
    return DBErrors::SUCCESS;
}

DataModel::DBErrors DataModel::loadCharData(){
    QSqlQuery query;
    if(!query.exec("select * from character_data "
                    "join character_mats "
                    "on character_data.name = character_mats.name")){
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
                    "on weapon_data.name = wep_mats.name")){
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

DataModel::DBErrors DataModel::updateCharData(const string &name, const vector<int> &materials){
    QSqlQuery query;
    if(!query.prepare("update character_mats "
                       "set purples=:purples, "
                       "blues=:blues, "
                       "greens=:greens "
                       "where name=:name")){
        qDebug()<<"Prep failed: "<<query.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }
    query.bindValue(":purples", materials[0]);
    query.bindValue(":blues", materials[1]);
    query.bindValue(":greens", materials[2]);
    query.bindValue(":name", QString::fromStdString(name));

    if(!query.exec()){
        qDebug()<<"Failed to update character_mats: "<<query.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }
    return DBErrors::SUCCESS;
}

DataModel::DBErrors DataModel::updateWepData(const string &name, const vector<int> &materials){
    QSqlQuery query;
    if(!query.prepare("update wep_mats "
                       "set purples=:purples, "
                       "blues=:blues, "
                       "greens=:greens "
                       "where name=:name")){
        qDebug()<<"Prep failed: "<<query.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }
    query.bindValue(":purples", materials[0]);
    query.bindValue(":blues", materials[1]);
    query.bindValue(":greens", materials[2]);
    query.bindValue(":name", QString::fromStdString(name));

    if(!query.exec()){
        qDebug()<<"Failed to update wep_mats: "<<query.lastError();
        return DBErrors::DB_WRITE_FAIL;
    }
    return DBErrors::SUCCESS;
}
