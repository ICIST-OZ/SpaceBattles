#pragma once
#include <SDL.h>
#include <vector>
#include <string>
#include "motor/Pipeline.hpp"
#include "objetos_dinamicos/ObjetoDinamico.hpp"
#include "motor/camaras/Camaras.hpp"
#include "utilidad/Spawns/BalaSpawner.hpp"
#include "utilidad/Spawns/EnemigoSpawner.hpp"

class SDLApp
{
    private:
        SDLApp();   
        bool on_init();
        void on_evento(SDL_Event *evento);
        void on_fisicaupdate(double dt); 
        void on_frameupdate(double dt); 
        void on_limpiar();
        void game_over(std::string prompt);

        bool esta_corriendo;
        int WIDTH;
        int HEIGHT;

        SDL_Window* vnt;
        SDL_Surface* vntsurf;
        SDL_Renderer* render;

        static SDLApp* instancia;

        Pipeline *ensamble;
    
        double maxFPS;
        double msfrecuencia;
        int fps;

        Background* background;
        Jugador* player_1;
        Jugador* player_2;
        Boss* boss;
        bool in_boss_fase;

        BalaSpawner *bala_spawner;
        EnemigoSpawner *enemigo_spawner;
        
        Camara* camara_principal;
        std::vector<Objeto*> objetos;
        std::vector<Objeto*> balas_p1;
        std::vector<Objeto*> balas_p2;
        std::vector<Objeto*> enemigos;
        std::vector<Objeto*> balas_enemigas;

    public:
        SDL_Color bg_color;
        static int on_correr();
        static void set_bgcolor(Uint8 rojo,Uint8 verde,Uint8 azul);
        static SDLApp& get();
        static bool estaCorriendo() {return get().esta_corriendo;};
};