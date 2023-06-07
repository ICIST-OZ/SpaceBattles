#include "FSMJugador.hpp"
#include "../Func_aux.hpp"
#include<SDL.h>
#include<iostream>
#include <sstream>
#include<cmath>


EstadoJugadorIDLE::EstadoJugadorIDLE() {

    strnombre = "IDLE";
    frames_actual_ani=0;
    frames_maxim_ani=4;
    direccion = {0, 0};
};

FSMJugador* EstadoJugadorIDLE::input_handle(Jugador& player, KeyOyente& input, MouseOyente& mouse)
{
    if(input.estaPresionado(player.key_right))
        return new EstadoJugadorMOVER({1,0});
    if(input.estaPresionado(player.key_left))
        return new EstadoJugadorMOVER({-1,0});
    if(input.estaPresionado(player.key_up))
        return new EstadoJugadorMOVER({0,-1});
    if(input.estaPresionado(player.key_down))
        return new EstadoJugadorMOVER({0,1});

    return NULL;
};

void EstadoJugadorIDLE::entrar(Jugador& player) {
    frames_actual_ani=0;
    frames_maxim_ani=4;
};

void EstadoJugadorIDLE::salir(Jugador& player) {};

void EstadoJugadorIDLE::update(Jugador& player, double dt) {

    // ANIMACIÓN
    player.get_sprite()->play_frame(0, 1);
    if (frame_dt>7) { // este es el delay, qué tantos frames durará cada fotograma
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};


EstadoJugadorMOVER::EstadoJugadorMOVER(Coordenadas dir) {

    strnombre = "MOVER";
    frames_actual_ani=0;
    frames_maxim_ani=4;
    direccion = dir;
};

FSMJugador* EstadoJugadorMOVER::input_handle(Jugador& player, KeyOyente& input, MouseOyente& mouse) {

    /*
    if(input.estaPresionado(player.key_up)) {
        if (input.estaPresionado(player.key_right))
            return new EstadoJugadorMOVER({1,-1});
        if (input.estaPresionado(player.key_left))
            return new EstadoJugadorMOVER({-1,-1});
        return new EstadoJugadorMOVER({0,-1});
    }

    if(input.estaPresionado(player.key_down)) {
        if (input.estaPresionado(player.key_right))
            return new EstadoJugadorMOVER({1,1});
        if (input.estaPresionado(player.key_left))
            return new EstadoJugadorMOVER({-1,1});
        return new EstadoJugadorMOVER({0,1});
    }

    if(input.estaPresionado(player.key_right))
        return new EstadoJugadorMOVER({1,0});

    if(input.estaPresionado(player.key_left))
        return new EstadoJugadorMOVER({-1,0});
    */
   //buggy

    if(input.estaPresionado(player.key_down) & input.estaPresionado(player.key_right)) 
        return new EstadoJugadorMOVER({1,1});
    if(input.estaPresionado(player.key_down) & input.estaPresionado(player.key_left)) 
        return new EstadoJugadorMOVER({-1,1});
    if(input.estaPresionado(player.key_up) & input.estaPresionado(player.key_right)) 
        return new EstadoJugadorMOVER({1,-1});
    if(input.estaPresionado(player.key_up) & input.estaPresionado(player.key_left)) 
        return new EstadoJugadorMOVER({-1,-1});
    
    if(input.nadaPresionado())
        return new EstadoJugadorIDLE();

    return NULL;
};

void EstadoJugadorMOVER::entrar(Jugador& player) {
    frames_actual_ani=0;
    frames_maxim_ani=4;
};

void EstadoJugadorMOVER::salir(Jugador& player){};

void EstadoJugadorMOVER::update(Jugador& player, double dt) {
    
    Coordenadas p = player.get_posicion_mundo();

    // horizonatal limits
    if ((p.x > 500 || player.get_estado()->direccion.x == 1) && (p.x < 1000 || player.get_estado()->direccion.x == -1))
        p.x += (player.vel_x * direccion.x);

    // vertical limits
    if ((p.y > 30 || player.get_estado()->direccion.y == 1) && (p.y < 690 || player.get_estado()->direccion.y == -1))
        p.y += (player.vel_y * direccion.y);

    player.set_posicion_mundo(p);

    // ANIMACIÓN
    player.get_sprite()->play_frame(frames_actual_ani % frames_maxim_ani, 0); 
    if (frame_dt>4) {
        frame_dt=0;
        frames_actual_ani++;
    }
    frame_dt++;
};