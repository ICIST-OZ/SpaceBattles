#include "BalaSpawner.hpp"
#include "../Tiempo.hpp"
#include "../Func_aux.hpp"
#include<iostream>
#include<cstdlib>

BalaSpawner::BalaSpawner(int vel, int w, int h, SDL_Color color, Pipeline &p)
{
    this->w = w;
    this->h = h;
    this->colordebug = color;
    this->objetos_activos = 0;
    this->delay = 1000;
    this->check = false;
    this->pipeline = &p;
};

void BalaSpawner::spawn(std::vector<Objeto*>*lista, Bala* b) {
    pipeline->figuras(b->get_avatar());
    lista->push_back(b);
    // printf("Created a bullet\n");
    objetos_activos++;
};

void BalaSpawner::despawn(std::vector<Objeto*>*lista) {
    /*
    int id = lista->size()-objetos_activos;
    delete lista->at(id);
    objetos_activos--;
    lista->erase(std::next(lista->begin()+id-1));
    */
    for(int i = 0; i < lista->size(); i++) {
        if(lista->at(i)->se_borra) {
            delete lista->at(i);
            lista->erase(std::next(lista->begin()+i-1));
            // printf("Deleted a bullet\n");
        }
   }
};
