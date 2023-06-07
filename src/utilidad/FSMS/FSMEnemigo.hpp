#pragma once
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include "../../motor/KeyOyente.hpp"
#include "../../motor/MouseOyente.hpp"
#include<string>

class Enemigo; //forward declaration
class Boss; //forward declaration
class FSMEnemigo
{
    protected:
        std::string strnombre;
    
    public:
        virtual ~FSMEnemigo(){};
        virtual void entrar(Enemigo& player)=0;
        virtual void salir(Enemigo& player)=0;
        virtual void update_en(Enemigo& player,double dt)=0;
        virtual void update_bo(Boss& player,double dt)=0;
        std::string get_namestate()const{return strnombre;};
        Coordenadas direccion;
};

class EstadoEnemigoIDLE : public FSMEnemigo
{
    public:
        EstadoEnemigoIDLE();
        virtual ~EstadoEnemigoIDLE(){};
        void entrar(Enemigo& player);
        void salir(Enemigo& player);
        void update_en(Enemigo& player,double dt);
        void update_bo(Boss& player,double dt);

    private:
        int frames_actual_ani;
        int frames_maxim_ani;
        int frame_dt{0};
};
