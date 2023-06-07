#pragma once
#include "../objetos/Objeto.hpp"
#include "../motor/KeyOyente.hpp"
#include "../motor/MouseOyente.hpp"
#include "../utilidad/FSMS/FSMJugador.hpp"
#include "../utilidad/FSMS/FSMEnemigo.hpp"
#include "../utilidad/Spawns/BalaSpawner.hpp"
#include<SDL.h>
#include<string>
#include "../motor/Pipeline.hpp"

class FSMJugador;//forward declaration
class FSMEnemigo;//forward declaration
class BalaSpawner;//forward declaration

class ObjetoDinamico : public Objeto {
    public:
        virtual ~ObjetoDinamico(){};
        virtual void update(double dt)=0;
};

class Background : public ObjetoDinamico {
    public:
        virtual ~Background(){};
        Background(std::string path_sprite, int x, int y, int w, int h, int sw, int sh, SDL_Color color);
        void update(double dt);
        void input_handle(KeyOyente& input,MouseOyente& mouse);

     private:
        int velocidad;
};

class Bala : public ObjetoDinamico {
    public:
        virtual ~Bala(){};
        Bala(int dmg, int vel, int x, int y, int w, int h, SDL_Color color);
        int get_velocidad()const{return velocidad;};
        void update(double dt){};
    private:
        int dmg;
        int velocidad;
};

class Nave : public ObjetoDinamico {

    public:
        virtual ~Nave(){};
        std::vector<Objeto*> get_balas()const{return *balas;};
        float vel_x;
        float vel_y;
        std::vector<Objeto*> *balas;
        BalaSpawner* get_bala_spawn()const{return bala_spawn;};
    
    protected:
        int shooting_delay;
        // std::vector<Bala*> balas;  // vector de balas
        BalaSpawner* bala_spawn;

        virtual void disparar()=0;
};


class Jugador : public Nave {
    public:
        virtual ~Jugador(){};
        Jugador(std::string path_sprite, int vida, int id, int x, int y, int w, int h, int sw, int sh, SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos);
        void update(double dt);
        void input_handle(KeyOyente& input, MouseOyente& mouse);

        std::string get_strEstado();
        void set_estado(FSMJugador* estado);
        FSMJugador* get_estado();

        void disparar();
        void update_balas();

        int get_score()const{return score;};
        void set_score(int s){score=s;};
        
        int key_up;
        int key_down;
        int key_right;
        int key_left;
        int key_shoot;

        bool moving_cam_up;
        bool moving_cam_down;

    private:
        int score;
        FSMJugador* estado_actual;
        // BalaSpawner* bala_spawn;
        Pipeline *ensamble;
};

class Enemigo : public Nave {
    public:
        virtual ~Enemigo(){};
        // std::string path_sprite, float vida, int x, int y, int w, int h, int sw, int sh, SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos
        Enemigo(std::string path_sprite, int vida, int x, int y, int w, int h, int sw, int sh, SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos);
        Enemigo(int vida, int x, int y,SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos);

        void update(double dt);
        // void input_handle(KeyOyente& input, MouseOyente& mouse);

        std::string get_strEstado();
        void set_estado(FSMEnemigo* estado);
        FSMEnemigo* get_estado();

        void disparar();
        void update_balas();

        int shooting_delay;
        bool passed_limits;

    private:
        int contador;
        double timer;
        double past_time;
        int delay;
        FSMEnemigo* estado_actual;
        // BalaSpawner* bala_spawn;
        Pipeline *ensamble;
};

class Boss : public Nave {
    public:
        virtual ~Boss(){};
        Boss(std::string path_sprite, int vida, int x, int y, int w, int h, int sw, int sh, SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos);
        Boss(int vida, int x, int y,SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos);

        void update(double dt);
        // void input_handle(KeyOyente& input, MouseOyente& mouse);

        std::string get_strEstado();
        void set_estado(FSMEnemigo* estado);
        FSMJugador* get_estado();

        void disparar();
        void update_balas();

        int shooting_delay;

    private:
        FSMEnemigo* estado_actual;
        // BalaSpawner* bala_spawn;
        Pipeline *ensamble;
        int bala_dmg;
        int bala_vel;
        std::vector<int> turn_points;
        int timer;
};