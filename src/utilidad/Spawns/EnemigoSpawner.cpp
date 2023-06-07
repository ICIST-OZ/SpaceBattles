#include "EnemigoSpawner.hpp"
#include "../Tiempo.hpp"
#include "../Func_aux.hpp"
#include<iostream>
#include <random>
#include <ctime>
#include<cstdlib>
#include "../../motor/fisica/MotorFisico.hpp"

EnemigoSpawner::EnemigoSpawner(int x1, int x2, int y1, int y2, int w, int h, int sw, int sh, int cant_enm, int batches, SDL_Color color, Pipeline &p)
{
    limite_x1 = x1;
    limite_x2 = x2;
    limite_y1 = y1;
    limite_y2 = y2;
    cantidad_enemigos = cant_enm;
    num_batches = batches;
    this->w = w;
    this->h = h;
    this->sw = w/7;
    this->sh = h/7;
    this->pipeline = &p;
};

void EnemigoSpawner::spawn(std::vector<Objeto*>*enemigos, std::vector<Objeto*>*balas) {
    if(num_batches > 0 && enemigos->size() == 0) {
        num_batches--;

        // set up random seed
        std::mt19937 rng(std::time(nullptr));
        std::uniform_int_distribution<int> distribution_x(limite_x1,limite_x2);    
        std::uniform_int_distribution<int> distribution_y(limite_y1, limite_y2);    
        int x;
        int y;

        for(int i = 0; i < cantidad_enemigos; i++){
            x = distribution_x(rng);
            y = distribution_y(rng);
            // printf("Crea los rands\n");
            // std::string path_sprite, float vida, int x, int y, int w, int h, int sw, int sh, SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos
            Enemigo *e = new Enemigo("assets/sprites/enemigos/ssblack.png", 1, x,y, w, h, sw, sh, {255, 0, 0, 255}, pipeline, balas);
            // Enemigo *e = new Enemigo(1, x,y,{255, 0, 0, 255}, pipeline, balas);

            for(int j = 0; j < i; j++) {
                while(MotorFisico2D::aabb_colision(*e->get_avatar(), *enemigos->at(j)->get_avatar())) {
                    x = distribution_x(rng);
                    y = distribution_y(rng);
                    e->set_posx(x);
                    e->set_posy(y);
                }
            }
            pipeline->cargar_texturas(e->get_sprite());
            // pipeline->figuras(e->get_avatar());

            enemigos->push_back(e);
            naves_enemigas.push_back(e);
            objetos_activos++;
        }
    }
};

bool EnemigoSpawner::despawn(std::vector<Objeto*>*lista) {
   for(int i = 0; i < lista->size(); i++) {
        if(lista->at(i)->se_borra && lista->at(i)->get_hp() <= 0) {
            delete lista->at(i);
            lista->erase(std::next(lista->begin()+i-1));
            return false;
        } else if(lista->at(i)->se_borra) {
            delete lista->at(i);
            lista->erase(std::next(lista->begin()+i-1));
            return true;
        }
   }
   return false;
};

bool EnemigoSpawner::update(std::vector<Objeto*>*enemigos, std::vector<Objeto*>*balas) {
    if(naves_enemigas.size() == 0){
        spawn(enemigos, balas);
        return false;
    }
    else{
        /*
        for(int i = 0; i < enemigos->size(); i++){
            if(enemigos->at(i)->get_hp() <= 0 || enemigos->at(i)->get_posx() > 500){
                despawn(enemigos);
                // naves_enemigas->erase(std::next(naves_enemigas->begin()+i-1));
            }
        }
        */
       return despawn(enemigos);
    } 
};