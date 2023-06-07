#include "sdlapp.hpp"
#include "utilidad/Tiempo.hpp"
#include "motor/KeyOyente.hpp"
#include "motor/MouseOyente.hpp"
#include <SDL_ttf.h>
#include "utilidad/RenderTexto.hpp"
#include "motor/fisica/MotorFisico.hpp"
#include <cmath>

#include <random>
#include <ctime>
#include <cstdlib>

//#define RECTANGULOS
//#define TRIANGULOS
//#define CIRCULOS

SDLApp* SDLApp::instancia = 0;

SDLApp::SDLApp()
{
    vnt = nullptr;
    vntsurf = nullptr;
    render = nullptr;
    ensamble= nullptr;
    bg_color.r = 255; 
    bg_color.g = 255; 
    bg_color.b = 255; 
    bg_color.a = 255; 

    esta_corriendo = true;
    WIDTH=1024;
    HEIGHT=720;
    maxFPS=60;
    fps=0;
};

SDLApp& SDLApp::get()
{
    if(!instancia)
        instancia = new SDLApp();
    return *instancia;
};

bool SDLApp::on_init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING)<0){return false;};

    get().vnt = SDL_CreateWindow(
        "Juego",              
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        get().WIDTH,           
        get().HEIGHT,           
        SDL_WINDOW_OPENGL);     
    
    TTF_Init();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    if(get().vnt == NULL)
    {
        printf("No se creo la ventana por: %s",SDL_GetError());
        return false;
    }
    SDL_SetHint(SDL_HINT_RENDER_BATCHING,"opengl");

    get().render = SDL_CreateRenderer(
        get().vnt,                 
        -1,                      
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if(get().render == NULL)
    {
        printf("No se creo el renderer por: %s",SDL_GetError());
        return false; 
    }

    get().ensamble = new Pipeline(*get().render);

    background = new Background("assets/sprites/mundo/background_estrellas.jpg", 
        WIDTH/2, HEIGHT/2, 38400, 21600, WIDTH, HEIGHT + 200, {0,0,0,255});
    get().ensamble->cargar_texturas(background->get_sprite());

    player_1 = new Jugador("assets/sprites/heroe/nave_naranja.png", // NAVE NARANJA
                //     hp,id,  x,    y, sW,  sH ,vW, vH, color
                        10, 1, 800, 250, 324, 362, 50, 50, {255,255,0,255}, get().ensamble, &balas_p1);
    get().ensamble->cargar_texturas(player_1->get_sprite());

    player_2 = new Jugador("assets/sprites/heroe/nave_roja.png", // NAVE ROJA
                //      hp, id, x , y, sW, sH ,vW, vH, color
                        10, 2, 800, 450, 167, 187, 50, 50, {255,0,0,255}, get().ensamble, &balas_p2);
    get().ensamble->cargar_texturas(player_2->get_sprite());

    get().camara_principal = new Camara(0,0,get().WIDTH,get().HEIGHT,*get().render);
    
    objetos.push_back(background);
    objetos.push_back(player_1);
    objetos.push_back(player_2);
                            //             x1,x2,y1, y2, w,   h, sw,sh, cant_enm, batches, color, pipeline
    enemigo_spawner = new EnemigoSpawner(-500, 0, 30, 690,501,498,50,50,   5,       3, {255, 0, 0, 255}, *get().ensamble);
    // enemigo_spawner->spawn(&enemigos, &balas_enemigas);
    // printf("\nSpawnear enemigos\n");

    in_boss_fase = false;

    SDL_SetRenderDrawColor(
        get().render,    
        get().bg_color.r,   
        get().bg_color.g,   
        get().bg_color.b,   
        SDL_ALPHA_TRANSPARENT);

    return true;
};

void SDLApp::on_evento(SDL_Event* evento)
{
    if(evento->type==SDL_QUIT) {
        get().esta_corriendo = false;
    }

    MouseOyente::get().evento_movimiento(evento);
    MouseOyente::get().evento_presionado(evento);
    MouseOyente::get().evento_scroll(evento);
    KeyOyente::get().keyPresionado(evento);
    
};

void SDLApp::on_fisicaupdate(double dt) {
    // camara_principal->lock_objeto(*player_1);
    // camara_principal->unlock_objeto();

    // logs
    // printf("\n / CAM POS CENTRO: (%d, %d)", camara_principal->get_posicion_centro().x, camara_principal->get_posicion_centro().y);
    // printf(" \nPLAYER 1 POS MUND: (%d, %d)", player_1->get_posicion_mundo().x, player_1->get_posicion_mundo().y);

    //render cuadro colisión players
    if(KeyOyente::get().estaPresionado(SDL_SCANCODE_C)) {
        player_1->render_colbox = (player_1->render_colbox) ? false : true;
        player_2->render_colbox = (player_2->render_colbox) ? false : true;
    }
    
    player_1->input_handle(KeyOyente::get(), MouseOyente::get());
    player_1->update(dt);

    player_2->input_handle(KeyOyente::get(), MouseOyente::get());
    player_2->update(dt);

    enemigo_spawner->spawn(&enemigos, &balas_enemigas);

    // checar colisiones de balas enemigas en jugadores
    for(auto &p : objetos) {
        if(p == player_1 || p == player_2) {
            for(auto& be : balas_enemigas) {
                MotorFisico2D::get().diag_overlap(*p,*be);
                bool pc = MotorFisico2D::get().aabb_colision(*p->get_colbox(),*be->get_colbox());
                // actualizar los estados de las balas y jugadoresa pc
                p->en_colision = pc;

                be->en_colision = pc;
                be->se_borra = pc;

                if (pc) {
                    for (int i = 0; i < balas_enemigas.size(); i++) {
                        if (balas_enemigas.at(i)->se_borra) {
                            delete balas_enemigas.at(i);
                            balas_enemigas.erase(std::next(balas_enemigas.begin()+i-1));
                        }
                    }
                    p->set_hp(p->get_hp()-1);
                    // printf("\n / HP PLAYER 1: %d", p->get_hp());

                    if(p->get_hp() <= 0){
                        game_over("GAME OVER");
                        p->se_borra = pc;
                        p->en_colision = pc;
                    }
                }
            }
        }
    }

    // checar colisiones de balas en enemigos
    for(auto& e : enemigos) {
        Enemigo* en = (Enemigo*) e;
        e->update(dt);
        for(auto& bp1 : balas_p1) {
            MotorFisico2D::get().diag_overlap(*e,*bp1);
            bool pc = MotorFisico2D::get().aabb_colision(*e->get_colbox(),*bp1->get_colbox());
            e->en_colision = pc;
            // e->se_borra = pc;

            bp1->en_colision = pc;
            bp1->se_borra = pc;

            if(pc){
                player_1->set_score(player_1->get_score()+1);
                e->set_hp(e->get_hp()-1);

                /* Sprite para colision
                if(e->get_hp() <= 0) {
                    e->set_sprite("assets/sprites/enemigos/explosion.png", 1, 0, 0, 0, 0, 0, 0, {255, 0, 0, 255}, get().ensamble);
                }
                */

                // printf("\n / SCORE PLAYER 1: %d", player_1->get_score());
            }
            player_1->get_bala_spawn()->despawn(&balas_p1);
        }

        for(auto& bp2 : balas_p2) {
            MotorFisico2D::get().diag_overlap(*e,*bp2);
            bool pc = MotorFisico2D::get().aabb_colision(*e->get_colbox(),*bp2->get_colbox());
            e->en_colision = pc;
            // e->se_borra = pc;

            bp2->en_colision = pc;
            bp2->se_borra = pc;

            if(pc) {
                player_2->set_score(player_2->get_score()+1);
                // printf("\n / SCORE PLAYER 2: %d", player_2->get_score());
            }
            player_2->get_bala_spawn()->despawn(&balas_p2);
        }

        // en->update_balas();
    }

    if (enemigo_spawner->num_batches == 0 && enemigos.size() == 0) {
        in_boss_fase = true;
        std::mt19937 rng(std::time(nullptr));
        std::uniform_int_distribution<int> distribution_x(-500, 0);    
        std::uniform_int_distribution<int> distribution_y(50, 600);
        int x = distribution_x(rng);
        int y = distribution_y(rng);
        // boss = new Boss(15, x, y, {255, 0, 255, 255}, ensamble, &balas_enemigas);
        boss = new Boss("assets/sprites/enemigos/boss.png", 10, x, y, 1371, 1264, 60, 60, {255, 0, 0, 255}, get().ensamble, &balas_enemigas);

        // e->balas->clear();
        get().ensamble->cargar_texturas(boss->get_sprite());
        // get().ensamble->figuras(boss->get_avatar());
        enemigos.push_back(boss);
    }

    if(in_boss_fase && boss->se_borra) {
        if (boss->get_hp() == 0) {
            game_over("YOU WON!");
        } else {
            game_over("GAME OVER");
        }
    }

    camara_principal->input_handle(*player_1, *player_2, KeyOyente::get(), MouseOyente::get());
    camara_principal->update();
    camara_principal->proyectar(objetos);
    camara_principal->proyectar(balas_p1);
    camara_principal->proyectar(balas_p2);
    camara_principal->proyectar(balas_enemigas);
    camara_principal->proyectar(enemigos);
};

void SDLApp::on_frameupdate(double dt)
{
    // limpiar frame
    SDL_RenderClear(get().render);

    //Renderizar todo a través de la camara
    camara_principal->renderizar(objetos);

    //Renderizar las balas a través de la camara
    camara_principal->renderizar(balas_p1);
    camara_principal->renderizar(balas_p2);

    //Renderizar los enemigos a través de la camara
    camara_principal->renderizar(enemigos);
    camara_principal->renderizar(balas_enemigas);

    if(enemigo_spawner->update(&enemigos, &balas_enemigas)) {
        game_over("GAME OVER");
    }
 
    // camara_principal->render_cross();
    
    // GUI (Score y vida de cada jugador)
    std::string score_player_1 = "PLAYER 1: " + std::to_string(player_1->get_score()) + " POINTS";
    std::string hp_player_1 = "PLAYER 1: " + std::to_string(player_1->get_hp()) + " HP";

    std::string score_player_2 = "PLAYER 2: " + std::to_string(player_2->get_score()) + " POINTS";
    std::string hp_player_2 = "PLAYER 2: " + std::to_string(player_2->get_hp()) + " HP";

    RenderTexto::get().render_texto(get().render, 20, 20, score_player_1, 180, 20, SDL_Color{255, 127, 0, 255});
    RenderTexto::get().render_texto(get().render, 20, 50, hp_player_1, 180, 20, SDL_Color{255, 127, 0, 255});

    RenderTexto::get().render_texto(get().render, 20, 100, score_player_2, 180, 20, SDL_Color{255, 0, 0, 255});
    RenderTexto::get().render_texto(get().render, 20, 130, hp_player_2, 180, 20, SDL_Color{255, 0, 0, 255});

    //Actualizar
    SDL_RenderPresent(get().render);

    //resetear color del frame
    SDL_SetRenderDrawColor(
        get().render,
        get().bg_color.r,
        get().bg_color.g,
        get().bg_color.b,
        SDL_ALPHA_TRANSPARENT);
};

void SDLApp::on_limpiar()
{
    SDL_DestroyRenderer(get().render);
    SDL_FreeSurface(get().vntsurf);
    SDL_DestroyWindow(get().vnt);

    delete ensamble;
    delete player_1;
    delete player_2;
    delete boss;
    SDL_Quit();
};

int SDLApp::on_correr()
{
    if(get().on_init()==false){return -1;}

    SDL_Event eventos;
    double dt=0;
    double frecuencia = 1/get().maxFPS;
    get().msfrecuencia = frecuencia*1000;

    while(get().estaCorriendo())
    {
        double inicio = Tiempo::get_tiempo();

        while(SDL_PollEvent(&eventos)) {
            get().on_evento(&eventos);
        }

        get().on_fisicaupdate(dt);
        get().on_frameupdate(dt);

        dt = (Tiempo::get_tiempo() - inicio)/frecuencia*1000;
        
        inicio=dt;
        if(dt<get().msfrecuencia) {
            SDL_Delay(floor(get().msfrecuencia-dt));
        }
    }
    get().on_limpiar();
    return 0;
};

void SDLApp::game_over(std::string prompt) {
    SDL_Color color = {255, 255, 255, 255};
    // RenderTexto* rt = new RenderTexto("assets/fonts/Roboto-Regular.ttf", 50, "GAME OVER", color, *get().render);
    RenderTexto::render_texto(get().render, 400, 300, prompt, 40, color, "onuava");
    SDL_RenderPresent(get().render);
    SDL_Delay(3000);
    get().esta_corriendo = false;
};



