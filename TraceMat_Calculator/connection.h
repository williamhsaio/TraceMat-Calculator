#ifndef CONNECTION_H
#define CONNECTION_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

static const char *db_file_path = "/Users/billhsaio/Personal_Work/starrailmats.db";

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(db_file_path);
    if (!db.open()) {
        QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
                              QObject::tr("Unable to establish a database connection.\n"
                                          "This example needs SQLite support. Please read "
                                          "the Qt SQL driver documentation for information how "
                                          "to build it.\n\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    QSqlQuery query;
    query.exec("Pragma foreign_keys = ON");
    query.exec("create table character_data (characterID integer primary key, "
               "name text not null, "
               "path text not null, "
               "rarity int not null)");
    query.exec("create table weapon_data (wepID integer primary key, "
               "name text not null, "
               "path text not null, "
               "rarity int not null)");
    // query.exec("create table character_mats (characterID integer primary key, "
    //            "name text not null, "
    //            "path text not null, "
    //            "rarity int not null)");
    query.exec("create table character_mats (characterID integer primary key, "
               "name text not null, "
               "purples int not null, "
               "blues int not null, "
               "greens int not null)");/*, "
               "foreign key (name) references character_data(name))");*/
    query.exec("create table wep_mats (characterID integer primary key, "
               "name text not null, "
               "purples int not null, "
               "blues int not null, "
               "greens int not null, "
               "foreign key (name) references wep_data(name))");
    /*query.exec("create table wep_mats (wepID integer primary key, "
               "name text not null, "
               "purples int not null, "
               "blues int not null, "
               "greens int not null, "
               "foreign key (name) references weapon_data(name))");*/
    //qDebug() << "exec :" << query.lastError();
    return true;
}

#endif // CONNECTION_H
