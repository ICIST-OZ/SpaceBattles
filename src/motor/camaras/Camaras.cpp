#include "Camaras.hpp"
#include "../../utilidad/Func_aux.hpp"
#include <iostream>

Camara::Camara(int x, int y, int w, int h, SDL_Renderer &view) {

    // se considera que la camara empieza en 0,0 y es toda la pantalla
    // si se quiere centrar hay que desplazar w y h en todos los cálculos.
    width = w;
    height = h;
    pos_mundo = {x,y};
    pos_centro = {x+w/2,y+h/2};

    cruz.push_back({x,y+h/2});
    cruz.push_back({x+w,y+h/2});
    cruz.push_back({x+w/2,y});
    cruz.push_back({x+w/2,y+h});

    viewport = &view;
    objensamble = new Pipeline(view);
    velocidad = 5;
};

Camara::~Camara() {
    SDL_DestroyRenderer(viewport);
    delete objensamble;
};

void Camara::render_cross() {
    // cruz
    objensamble->lineas(cruz,{0,255,0,180});
    // caja de limites
    objensamble->lineas({{500, 175}, {1010, 175}, {500, 545}, {1010, 545}, {500, 175}, {500, 545}, {1010, 175}, {1010, 545}},{255,0,0,180});
};

void Camara::proyectar(std::vector<Objeto*> objetos) {

    for(auto &o:objetos) {
        Coordenadas posM = o->get_posicion_mundo();
        posM.x -= pos_mundo.x;
        posM.y -= pos_mundo.y;
        o->set_posicion_mundo(posM);
    }
};

void Camara::renderizar(std::vector<Objeto*>objetos) {
    for(auto& o:objetos)
        o->render(viewport);
};

void Camara::set_posicion_mundo(Coordenadas np) { pos_mundo = np; };

void Camara::set_posicion_centro(Coordenadas np) { pos_centro = np; };

void Camara::input_handle(Jugador& player_1, Jugador& player_2, KeyOyente &input, MouseOyente &mouse) {

    if (pos_centro.y < 265) {
        player_1.moving_cam_up = false;
        player_2.moving_cam_up = false;
    }
    if (pos_centro.y > 455) {
        player_1.moving_cam_down = false;
        player_2.moving_cam_down = false;
    }

    if (player_1.moving_cam_up || player_2.moving_cam_up) {
        pos_mundo = {0, -velocidad};
    } else if (player_1.moving_cam_down || player_2.moving_cam_down) {
        pos_mundo = {0, velocidad};
    } else {
        pos_mundo = {0,0};
    }
};

void Camara::update() {
    // la posición centro es muy importante porque es el indicador de los límites del mundo
    pos_centro.x += pos_mundo.x;
    pos_centro.y += pos_mundo.y;
};