#pragma once
#include "../../objetos/Objeto.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include "../../motor/Pipeline.hpp"
#include <SDL.h>
#include <vector>

class Enemigo;

class EnemigoSpawner {
    public:
        EnemigoSpawner(int x1, int x2, int y1, int y2, int w, int h, int sw, int sh, int cant_enm, int batches, SDL_Color color, Pipeline &p);

        void spawn(std::vector<Objeto*> *enemigos, std::vector<Objeto*> *balas);
        bool despawn(std::vector<Objeto*>*lista);
        bool update(std::vector<Objeto*>*enemigos, std::vector<Objeto*>*balas);

        int num_batches;
        
    private:
        std::vector<Objeto*> objetos;
        int cantidad_enemigos;
        int limite_x1;
        int limite_x2;
        int limite_y1;
        int limite_y2;

        std::vector<Nave*> naves_enemigas;
        int w; // ancho sprite
        int h; // alto sprite
        int sw;
        int sh;

        int objetos_activos;
        int delay;
        bool check;
        Pipeline *pipeline;
        int velocidad;
};