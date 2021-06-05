#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include "../constants.h"
#include "automata_manager.h"

#include <QSqlQuery>


class DBException{
    std::string info;
public:
    DBException(const std::string& s) : info(s){}
    const std::string& getInfo() const{return info;}
};

class DBManager{
    DBManager(const QString&);
    QSqlDatabase db;
    DBManager(const DBManager*) = delete;
    DBManager operator=(const DBManager*) = delete;
    static DBManager* DBManInstance;

public:
    ~DBManager();
    static DBManager& getDB();
    static void closeDB();

    void loadAutomatasFromDB() const;

    void DBaddNeighborhood(const NeighborhoodStrategy* neighborhood);
    NeighborhoodStrategy** loadNeighborhood() const;
    void loadStatesFromDB() const;
    void loadTransitionsFromDB();

};


#endif // DATABASE_H
