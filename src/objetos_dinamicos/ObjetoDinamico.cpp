#include "ObjetoDinamico.hpp"
#include "../objetos/Figuras.hpp"
#include "../utilidad/Spawns/BalaSpawner.hpp"
#include "../utilidad/Tiempo.hpp"
#include <ctime>

Bala::Bala(int dmg, int vel, int x, int y, int w, int h, SDL_Color c) {
    se_borra = false;
    posicion_mundo.x = x;
    posicion_mundo.y = y;

    avatar = new Rectangulo(x, y, w, h, c);

    avatar->set_rellenocolor(c);
    avatar->set_serellena(true);

    this->dmg = dmg; // daño de la bala

    this->velocidad = vel; // velocidad de la bala

    col_box = new Rectangulo(x,y,w,h,c);

    col_box->set_serellena(false);
    this->tiene_fisica = true;
    this->en_colision = false;
    this->sprite = nullptr;
};


Jugador::Jugador(std::string path_sprite, int vida, int id, int x, int y, int w, int h, int sw, int sh, SDL_Color c, Pipeline *pipeline, std::vector<Objeto*>* objetos)
    : Nave{}
{
    se_borra = false;
    balas = objetos;
    posicion_mundo.x = x;
    posicion_mundo.y = y;

    avatar = new Rectangulo(x, y, 75, 75, c);
    c.a = 150;
    avatar->set_rellenocolor(c);
    avatar->set_serellena(true);

    color = c;
    hp = vida;
    score = 0;
    vel_x = 5;
    vel_y = 5;

    ensamble = pipeline;

    (id == 1) ? key_up = SDL_SCANCODE_W : key_up = SDL_SCANCODE_UP;
    (id == 1) ? key_down = SDL_SCANCODE_S : key_down = SDL_SCANCODE_DOWN;
    (id == 1) ? key_right = SDL_SCANCODE_D : key_right = SDL_SCANCODE_RIGHT;
    (id == 1) ? key_left = SDL_SCANCODE_A : key_left = SDL_SCANCODE_LEFT;
    (id == 1) ? key_shoot = SDL_SCANCODE_F : key_shoot = SDL_SCANCODE_RALT; //SDL_SCANCODE_RCTRL

    col_box = new Rectangulo(x, y, 75 + 10, 75 + 10, c);
    col_box->set_serellena(false);

    tiene_fisica = true;
    en_colision = false;

    estado_actual = new EstadoJugadorIDLE();

    sprite = new Sprite(path_sprite, posicion_mundo, w, h, sw, sh);
    bala_spawn = new BalaSpawner(10, 10, 5, {0, 0, 255, 255}, *pipeline);
};

std::string Jugador::get_strEstado()
{
    return estado_actual->get_namestate();
};

FSMJugador *Jugador::get_estado()
{
    return estado_actual;
};

void Jugador::set_estado(FSMJugador *estado)
{
    estado_actual->salir(*this);
    delete estado_actual;
    estado_actual = (FSMJugador *)estado;
    estado_actual->entrar(*this);
};

void Jugador::update(double dt)
{
    shooting_delay++;
    if (!estado_actual)
        return;

    // colisión color
    if (en_colision){
        avatar->set_rellenocolor({255, 0, 0, 255});
        hp--;
    }
    else{
        avatar->set_rellenocolor(color);
    }

    // limits for camera-movement-freedom
    if (get_posicion_mundo().y < 175 && get_posicion_mundo().y > 165 && get_estado()->direccion.y == -1)
    {
        moving_cam_up = true;
    }
    else if (get_posicion_mundo().y > 545 && get_posicion_mundo().y < 555 && get_estado()->direccion.y == 1)
    {
        moving_cam_down = true;
    }
    else
    {
        moving_cam_up = false;
        moving_cam_down = false;
    }

    estado_actual->update(*this, dt);
    update_balas();
    // reset
    en_colision = false;
};

void Jugador::update_balas() {
    for(auto &b : *balas) { 
        if(b->get_posicion_mundo().x >= -2 && b->get_posicion_mundo().x <= 1024) {
            Bala* b2 = (Bala*) b;
            int tempx = b2->get_posicion_mundo().x;
            tempx -= b2->get_velocidad();
            b->set_posx(tempx);
            //el movimiento funciona bien
            // printf("Posicion de la bala: %d\n", b->get_posx());
        } else {
            b->se_borra = true;
        }
    }
};

void Jugador::input_handle(KeyOyente &input, MouseOyente &mouse)
{
    if (!estado_actual)
        return;
    FSMJugador *estado = estado_actual->input_handle(*this, input, mouse);
    if (estado)
        set_estado(estado);

    // shooting
    if (input.estaPresionado(key_shoot) && shooting_delay >= 30)
        disparar();
};

void Jugador::disparar() {
    shooting_delay = 0;
    Bala* b = new Bala(1, 10, get_posx(), get_posy(), 10, 5, {0, 255, 0, 255});
    bala_spawn->spawn(balas, b); // spawnear bala
    // printf("\n / Se creo una bala. BALAS: %d", balas->size());
};

Enemigo::Enemigo(std::string path_sprite, int vida, int x, int y, int w, int h, int sw, int sh, SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos) {
    estado_actual = new EstadoEnemigoIDLE();
    int vw = w/10;
    int vh = h/10;

    timer = 0;
    past_time = 0;
    contador = 0;
    delay = 1;
    shooting_delay = rand() % 3 + 1;

    se_borra = false;
    hp = 1;
    balas = objetos;
    posicion_mundo.x = x;
    posicion_mundo.y = y;

    avatar = new Rectangulo(x, y, sw, sh, c);
    c.a = 150;
    avatar->set_rellenocolor(c);
    avatar->set_serellena(false);

    color = c;
    hp = vida;
    vel_x = 2;

    ensamble = p;

    col_box = new Rectangulo(x, y, sw, sh, c);
    col_box->set_serellena(false);

    tiene_fisica = true;
    en_colision = false;

    sprite = new Sprite(path_sprite, posicion_mundo, w, h, sw, sh);
    bala_spawn = new BalaSpawner(10, 10, 5, {0, 0, 255, 255}, *p);
};

Enemigo::Enemigo(int vida, int x, int y , SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos) {
    
    timer = 0;
    past_time = 0;
    contador = 0;
    delay = 1;
    shooting_delay = rand() % 2 + 1;

    se_borra = false;
    hp = vida;
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    avatar = new Rectangulo(x,y,75,75,c);
    c.a=150;
    color = c;
    avatar->set_rellenocolor(c);
    avatar->set_serellena(true);
    col_box = new Rectangulo(x,y,75+10,75+10,c);
    col_box->set_serellena(false);
    tiene_fisica = true;
    en_colision = false;
    sprite = nullptr;
    vel_x = 2;
    ensamble = p;
    balas = objetos;
    bala_spawn = new BalaSpawner(10, 10, 5, {0, 0, 255, 255}, *p);
};

void Enemigo::update(double dt) {
    // mover la nave enemiga hacia el jugador (+x)
    posicion_mundo.x += vel_x;
    if(posicion_mundo.x >= 1024) {
        se_borra = true;
        passed_limits = true;
    }
    if(en_colision) {
        // printf("Entra a la colision\n");
        hp--;
        if(hp <= 0) {
            se_borra = true;
        }
    }
    
    timer += Tiempo::get_tiempo() - timer;
    if((int)timer%delay ==0 && (int)timer!=0 && (int)timer > past_time)
    {
        contador++;
        past_time = timer;
    }
    // shooting delay por tiempo (practica de enferemdades)
    //  disparo de naves enemigas (TO POLISH)
    if(contador > shooting_delay) {
        shooting_delay = rand() % 2 + 1;
        disparar();
        contador = 0;
    }
    en_colision = false;

    estado_actual->update_en(*this, dt);
    update_balas();
};

void Enemigo::disparar() {
    // disparar una bala
    Bala* b = new Bala(1, 3, get_posx(), get_posy(), 10, 5, {255, 0, 0, 255});
    bala_spawn->spawn(balas, b); // spawnear bala
    // printf("\n / Se creo una bala. BALAS: %d", balas->size());
};

void Enemigo::update_balas() {
    for(auto &b : *balas) { 
        if(b->get_posicion_mundo().x > 0 && b->get_posicion_mundo().x < 1024) {
            Bala* b2 = (Bala*) b;
            int tempx = b2->get_posicion_mundo().x;

            //movimiento hacia la derecha
            tempx += b2->get_velocidad();
            b->set_posx(tempx);
            //el movimiento funciona bien
            // printf("Posicion de la bala: %d\n", b->get_posx());
        } else {
            b->se_borra = true;
            get_bala_spawn()->despawn(balas);
            // bala_spawn->despawn(balas);
        }
        // bala_spawn->despawn(balas);
    }
};

Boss::Boss(std::string path_sprite, int vida, int x, int y, int w, int h, int sw, int sh, SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos) {
    estado_actual = new EstadoEnemigoIDLE();
    int vw = w/13;
    int vh = h/13;

    se_borra = false;
    hp = vida;
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    shooting_delay = 20;
    avatar = new Rectangulo(x,y,vw,vh,c);
    c.a=150;
    color = c;
    avatar->set_rellenocolor(c);
    avatar->set_serellena(true);

    col_box = new Rectangulo(x,y,vw,vh,c);
    col_box->set_serellena(false);
    tiene_fisica = true;
    en_colision = false;
    sprite = nullptr;
    vel_x = 1;
    vel_y = 1;
    ensamble = p;
    balas = objetos;
    bala_spawn = new BalaSpawner(10, 10, 5, {0, 0, 255, 255}, *p);
    bala_dmg = 1;
    bala_vel = 10;

    srand((unsigned) time(NULL));
    timer = 0; // el timer se incrementa en el update
    for (int i = 0; i < 7; i++) {
        turn_points.push_back(rand() % 1500);
    }

    sprite = new Sprite(path_sprite, posicion_mundo, w, h, vw, vh);
};

Boss::Boss(int vida, int x, int y , SDL_Color c, Pipeline* p, std::vector<Objeto*>* objetos) {
    se_borra = false;
    hp = vida;
    posicion_mundo.x=x;
    posicion_mundo.y=y;
    shooting_delay = 20;
    avatar = new Rectangulo(x,y,75,75,c);
    c.a=150;
    color = c;
    avatar->set_rellenocolor(c);
    avatar->set_serellena(true);
    col_box = new Rectangulo(x,y,75+10,75+10,c);
    col_box->set_serellena(false);
    tiene_fisica = true;
    en_colision = false;
    sprite = nullptr;
    vel_x = 1;
    vel_y = 1;
    ensamble = p;
    balas = objetos;
    bala_spawn = new BalaSpawner(10, 10, 5, {0, 0, 255, 255}, *p);
    bala_dmg = 1;
    bala_vel = 10;

    srand((unsigned) time(NULL));
    timer = 0; // el timer se incrementa en el update
    for (int i = 0; i < 12; i++) {
        turn_points.push_back(rand() % 1500);
    }
};

void Boss::update(double dt) {
    // mover la nave enemiga hacia el jugador (+x)
    posicion_mundo.x += vel_x;
    posicion_mundo.y += vel_y;
    if(posicion_mundo.x > 1024) {
        se_borra = true;
    }
    if(en_colision) {
        // printf("Entra a la colision\n");
        hp--;
        printf("\n%d", hp);
        if(hp <= 0) {
            se_borra = true;
        }
    }
    // manejar el delay de balas
    /*
    if(posicion_mundo.x > 0)
        shooting_delay--;
    */

    timer++;
    for (auto &tp : turn_points) {
        if (timer == tp) {
            vel_y *= -1;
        }
    }

    if (posicion_mundo.y < 50 || posicion_mundo.y > 670) {
        vel_y *= -1;
    }

    shooting_delay--;
    // shooting delay por tiempo (practica de enferemdades)
    //  disparo de naves enemigas (TO POLISH)
    if(shooting_delay <= 0) {
        disparar();
        // printf("Dispara nave\n");
        shooting_delay = rand() % 50 + 20;
    }
    en_colision = false;

    estado_actual->update_bo(*this, dt);
    update_balas();
};

void Boss::disparar() {
    // disparar una bala
    Bala* b = new Bala(bala_dmg, bala_vel, get_posx(), get_posy(), 10, 5, {255, 0, 0, 255});
    bala_spawn->spawn(balas, b); // spawnear bala
    // printf("\n / Se creo una bala. BALAS: %d", balas->size());
};

void Boss::update_balas() {
    for(auto &b : *balas) { 
        if(b->get_posicion_mundo().x > 0 && b->get_posicion_mundo().x < 1024) {
            Bala* b2 = (Bala*) b;
            int tempx = b2->get_posicion_mundo().x;

            //movimiento hacia la derecha
            tempx += b2->get_velocidad();
            b->set_posx(tempx);
        } else {
            b->se_borra = true;
            get_bala_spawn()->despawn(balas);
            // bala_spawn->despawn(balas);
        }
        // bala_spawn->despawn(balas);
    }
};


Background::Background(std::string path_sprite, int x, int y, int w, int h, int sw, int sh, SDL_Color color)
    : ObjetoDinamico{}
{
    posicion_mundo.x = x;
    posicion_mundo.y = y;
    posicion_camara = {0, 0};
    avatar = new Rectangulo(x, y, sw, sh, color);
    avatar->set_rellenocolor(color);
    avatar->set_serellena(false);
    col_box = nullptr;
    tiene_fisica = false;
    en_colision = false;
    sprite = new Sprite(path_sprite, posicion_mundo, w, h, sw, sh);
    velocidad = 5;
};

void Background::input_handle(KeyOyente &input, MouseOyente &mouse) {}

void Background::update(double dt){};

