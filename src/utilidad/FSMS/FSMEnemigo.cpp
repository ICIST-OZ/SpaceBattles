#include "FSMEnemigo.hpp"
#include "../Func_aux.hpp"
#include<SDL.h>
#include<iostream>
#include <sstream>
#include<cmath>


EstadoEnemigoIDLE::EstadoEnemigoIDLE() {

    strnombre = "IDLE";
    frames_actual_ani=0;
    frames_maxim_ani=1;
    direccion = {0, 0};
};

void EstadoEnemigoIDLE::entrar(Enemigo& player) {
    frames_actual_ani=0;
    frames_maxim_ani=1;
};

void EstadoEnemigoIDLE::salir(Enemigo& player) {};

void EstadoEnemigoIDLE::update_en(Enemigo& player, double dt) {
    // ANIMACIÓN
    player.get_sprite()->play_frame(0, 0);
    if (frame_dt>7) { // este es el delay, qué tantos frames durará cada fotograma
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};

void EstadoEnemigoIDLE::update_bo(Boss& player, double dt) {
    // ANIMACIÓN
    player.get_sprite()->play_frame(0, 0);
    if (frame_dt>7) { // este es el delay, qué tantos frames durará cada fotograma
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};


