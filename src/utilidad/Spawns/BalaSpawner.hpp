#pragma once
#include "../../objetos/Objeto.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include "../../motor/Pipeline.hpp"
#include <SDL.h>
#include <vector>

class Bala;

class BalaSpawner {
    public:
        BalaSpawner(int vel, int w, int h, SDL_Color color, Pipeline &p);

        void spawn(std::vector<Objeto*> *lista, Bala* b);
        void despawn(std::vector<Objeto*>*lista);
        // void update(std::vector<Bala*>*lista);

    private:
        std::vector<Objeto*> objetos;
        SDL_Color colordebug;
        int w; // ancho sprite
        int h; // alto sprite
        int objetos_activos;
        int delay;
        bool check;
        Pipeline *pipeline;
        int velocidad;
};