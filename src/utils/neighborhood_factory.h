#ifndef NEIGHBORHOOD_FACTORY_H
#define NEIGHBORHOOD_FACTORY_H
#include "neighborhood_strategy.h"
#include <string>

using namespace std;

//Cette classe est une fabrique de voisinage utilisée pour renvoyer le bon type de voisinage suite à une requête à la base de donnée
class NeighborhoodFactory {

public:
    // Permet de créer le type de voisinage correspondant au nom passé en paramètre
    NeighborhoodStrategy* production(const string name) const;
    NeighborhoodStrategy* production(const string name, const int radius) const;
    NeighborhoodStrategy* production(const string name, const int nbVoisins, int* dx, int* dy) const;

};

#endif // NEIGHBORHOOD_FACTORY_H
