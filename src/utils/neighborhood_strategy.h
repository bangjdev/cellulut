//
// Created by Téva Ducoté on 23/04/2021.
//

#ifndef LO21_NEIGHBORHOODSTRATEGY_H
#define LO21_NEIGHBORHOODSTRATEGY_H
#include<utility>
#include "cell.h"
#include "grid.h"
#include "cell_state.h"

class NeighborhoodStrategy{
public:
    virtual Cell*** getNeighborhood() const = 0;
    virtual unsigned int getNbNeighbors()const=0 ;
};

class VonNeumannNeighborhood: public NeighborhoodStrategy{
    Cell*** neighborhood;
    unsigned int nb_neighbors=4;
    unsigned int nb_cells;
public:
    VonNeumannNeighborhood(const Grid g );
    ~VonNeumannNeighborhood(){ for(unsigned int i=0; i < nb_cells;i++ ) delete[] neighborhood[i];
        delete[] neighborhood;}
    unsigned int getNbNeighbors() const { return nb_neighbors;}
    Cell*** getNeighborhood() const {return neighborhood;}
};

class VonNeumannNeighborhoodGeneralized: public NeighborhoodStrategy{
    Cell*** neighborhood;
    unsigned int radius;
    unsigned int nb_neighbors;
    unsigned int nb_cells;
public:
    VonNeumannNeighborhoodGeneralized(const Grid g, unsigned int radius=1);
    ~VonNeumannNeighborhoodGeneralized(){
        for(unsigned int i = 0; i < nb_cells; i++ ) delete[] neighborhood[i];
        delete[] neighborhood;}
    unsigned int getNbNeighbors() const { return nb_neighbors;}
    Cell*** getNeighborhood() const {return neighborhood;}
};
class MooreNeighborhood: public NeighborhoodStrategy{
    Cell*** neighborhood;
    unsigned int nb_neighbors=8;
    unsigned int nb_cells;
public:
    MooreNeighborhood(const Grid g);
    ~MooreNeighborhood(){
        for(unsigned int i=0;i<nb_cells;i++ ) delete[] neighborhood[i];
        delete[] neighborhood;}
    unsigned int getNbNeighbors() const { return nb_neighbors;}
    Cell*** getNeighborhood() const {return neighborhood;}
};

class MooreNeighborhoodGeneralized: public NeighborhoodStrategy{
    Cell*** neighborhood;
    unsigned int radius;
    unsigned int nb_neighbors;
    unsigned int nb_cells;
public:
    MooreNeighborhoodGeneralized(const Grid g, unsigned int radius=1);
    ~MooreNeighborhoodGeneralized(){
        for(unsigned int i=0;i<nb_cells;i++ ) delete[] neighborhood[i];
        delete[] neighborhood;
    }
    unsigned int getNbNeighbors() const { return nb_neighbors;}
    Cell*** getNeighborhood() const {return neighborhood;}
};

class ArbitraryNeighborhood: public NeighborhoodStrategy{
    Cell*** neighborhood;
    unsigned int nb_neighbors;
    unsigned int nb_cells;
public:
    ArbitraryNeighborhood(const Grid g);
    ~ArbitraryNeighborhood(){
        for(size_t i=0;i<nb_cells;i++ ) delete[] neighborhood[i];
        delete[] neighborhood;}
    unsigned int getNbNeighbors() const { return nb_neighbors;}
    Cell*** getNeighborhood() const {return neighborhood;}
};

#endif //LO21_NEIGHBORHOODSTRATEGY_H
