#pragma once
#include "../../utilidad/Estructuras.hpp"
#include "../../objetos/Objeto.hpp"
#include "../Pipeline.hpp"
#include "../KeyOyente.hpp"
#include "../MouseOyente.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"

#include <SDL.h>
#include <vector>

class Camara
{
    public:
        Camara(int x, int y, int w, int h, SDL_Renderer &view);
        ~Camara();
        void renderizar(std::vector<Objeto*> objetos);
        void proyectar(std::vector<Objeto*> objetos);
        void render_cross();

        Coordenadas get_posicion_mundo() const {return pos_mundo;};
        void set_posicion_mundo(Coordenadas np);
        void set_posicion_centro(Coordenadas np);
        Coordenadas get_posicion_centro() const{return pos_centro;};

        /*FSM métodos*/
        void input_handle(Jugador& player_1, Jugador& player_2, KeyOyente &input, MouseOyente &mouse);
        void update();
    
    public:
        int lock{0};
        int velocidad;
        int en_transicion{0};
    
    private:
        int width;
        int height;
        Coordenadas pos_mundo;
        Coordenadas pos_centro;
        SDL_Renderer *viewport;
        Pipeline * objensamble;
        std::vector<Coordenadas> cruz;
        std::vector<Coordenadas> limder;
        std::vector<Coordenadas> limizq;
};