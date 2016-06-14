#include <allegro.h>

void init();
void deinit();

int  menu();
void jogar();
void fimJogo(int &);
void creditos();
void pers_tem();
int vida ();
void morreu();

int pers_Tem = 0;
int mapa = 0;

 //SAMPLE* efeito = load_sample("efeito.wav");
 //MIDI* musica = load_midi("musica.mid");

volatile int exit_program;
void fecha_programa() { exit_program = TRUE; }
END_OF_FUNCTION(fecha_programa)

volatile int ticks;
void tick_counter() { ticks++; }
END_OF_FUNCTION(tick_counter)

volatile int milisegundos;
void msec_counter() { milisegundos++; }
END_OF_FUNCTION(msec_counter)

typedef struct Pers{
        int x;
        int y;
        int tam_x;
        int tam_y;
};

typedef struct Plata{
        int x1;
        int y1;
        int x2;
        int y2;
};

bool colide(int ax1,int ay1,int ax2,int ay2,int bx1,int by1,int bx2,int by2);
bool colideLado(int ax1,int ay1,int ax2,int ay2,int bx1,int by1,int bx2,int by2);

int main() {
init();
int opcao=0;

while (opcao!=3) {
          opcao=menu();
          if(opcao==1)
               jogar();
          else if(opcao==2)
              creditos();
               //else if(opcao==3)
                    //creditos();
    }//fim do while

deinit();
return 0;
}
END_OF_MAIN();

int  menu(){
     //int estado_anterior;


     BITMAP *buffer   = create_bitmap(SCREEN_W, SCREEN_H);
     BITMAP *botao1ns = load_bitmap("Sprites/Menu/play.bmp",NULL);
     BITMAP *botao1fs = load_bitmap("Sprites/Menu/play2.bmp",NULL);
     //BITMAP *botao2ns = load_bitmap("opcao2ns.bmp",NULL);
     //BITMAP *botao2fs = load_bitmap("opcao2fs.bmp",NULL);
     BITMAP *botao3ns = load_bitmap("Sprites/Menu/sair.bmp",NULL);
     BITMAP *botao3fs = load_bitmap("Sprites/Menu/sair2.bmp",NULL);
     BITMAP *fundo    = load_bitmap("Sprites/Menu/fundo3.bmp",NULL);
     BITMAP *fundo1    = load_bitmap("Sprites/Menu/fundo4.bmp",NULL);
     BITMAP *cred    = load_bitmap("Sprites/Menu/Creditos.bmp",NULL);
     MIDI* music = load_midi("Sprites/Sons/Game_Over.mid");
     //SAMPLE* music = load_sample("Sprites/Sons/Guilty Of Being Innocent Of Being Jack Sparrow.wav");

       // estado_anterior = mouse_b;
        //play_midi(music, TRUE);
         //if(estado_anterior == 0 && mouse_b)
       // play_sample(music, 255, 128, 1000, FALSE);
        play_midi(music, TRUE);
     while(true){


          //renderiza o menu usando as imagens sem selecao
           blit(fundo, buffer, 0,0,0,0, SCREEN_W, SCREEN_H);
           masked_blit(botao1ns, buffer,0,0,160, 300,350,75);
           masked_blit(cred, buffer,0,0,430,385,200,75);
           masked_blit(botao3ns, buffer,0,0,0,380,100,100);



          //produz o comportamento do mouse passando por cima
           if(mouse_x>160 && mouse_x<510 &&
              mouse_y>300 && mouse_y<375){
                  masked_blit(botao1fs, buffer,0,0,160, 300,350,75);
           }else if(mouse_x>430 && mouse_x<630 &&
              mouse_y>385 && mouse_y<460){
                masked_blit(cred, buffer,0,0,430,385,200,75);
           }else if(mouse_x>0 && mouse_x<100 &&
              mouse_y>380&& mouse_y<480){
                  masked_blit(botao3fs, buffer,0,0,0, 380,100,100);
           }

          //se houver o click do mouse, retornar o valor
           if(mouse_b & 1){
                if(mouse_x>160 && mouse_x<510 &&
                   mouse_y>300 && mouse_y<375){
                         return 1;
                }else if(mouse_x>430 && mouse_x<630 &&
                   mouse_y>385 && mouse_y<460){
                         return 2;
                }else if(mouse_x>0 && mouse_x<100 &&
                   mouse_y>380 && mouse_y<480){
                         return 3;
               }
           }

           show_mouse(buffer);
           blit(buffer, screen, 0,0,0,0,SCREEN_W, SCREEN_H);

     }//fim do while
     destroy_midi(music);

}//fim da funcao menu


void jogar(){

    Pers personagem;
    personagem.x=0;
    personagem.y=0;
    personagem.tam_x=35;
    personagem.tam_y=45;

    //plata [10]
    Plata pl1;
    pl1.x1=360;
    pl1.y1=380;
    pl1.x2= pl1.x1 + 287;
    pl1.y2= pl1.y1 + 47;



    ticks = 0;
    LOCK_FUNCTION(tick_counter);
    LOCK_VARIABLE(ticks);
    install_int_ex(tick_counter, BPS_TO_TIMER(60));

    milisegundos = 0;
    LOCK_FUNCTION(msec_counter);
    LOCK_VARIABLE(milisegundos);
    install_int_ex(msec_counter, MSEC_TO_TIMER(1.5));

    ///BITMAPS
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);


    //BITMAP* ameboss = load_bitmap("Sprites/Inimigos/ameba.bmp", NULL);
    BITMAP* chao  = load_bitmap("Sprites/Fundo/chao.bmp", NULL);
    BITMAP* jack1 = load_bitmap("Sprites/Personagem/jack_d.bmp", NULL);
    BITMAP* jack2 = load_bitmap("jack_e.bmp", NULL);
    BITMAP* jackIdle = load_bitmap("Sprites/Personagem/jack_parado.bmp", NULL);
    BITMAP* jackIdle2 = load_bitmap("Sprites/Personagem/jack_parado_e.bmp", NULL);
    BITMAP* jackAtk = load_bitmap("Sprites/Personagem/jack_atk4.bmp", NULL);
    BITMAP* jackAtke = load_bitmap("Sprites/Personagem/jack_atk_e.bmp", NULL);//---------------
    BITMAP* jackesquerda = load_bitmap("Sprites/Personagem/jackinverso.bmp", NULL);
    BITMAP* jackPulo = load_bitmap("Sprites/Personagem/jack_pulando.bmp", NULL);
    BITMAP* jackPuloe = load_bitmap("Sprites/Personagem/jack_pulando_e.bmp", NULL);//----------
    BITMAP* jackCai = load_bitmap("Sprites/Personagem/jack_cai.bmp", NULL);
    BITMAP* jackCaie = load_bitmap("Sprites/Personagem/jack_cai_e.bmp", NULL);//-------------
    BITMAP* star = load_bitmap("Sprites/Fundo/star.bmp", NULL);
    BITMAP* plat = load_bitmap("Sprites/Fundo/plat.bmp", NULL);
    BITMAP* fundoxd[4];

    fundoxd[0] = load_bitmap("Sprites/Fundo/dark2.bmp", NULL);
    fundoxd[1] = load_bitmap("Sprites/Fundo/dark1.bmp", NULL);
    fundoxd[2] = load_bitmap("Sprites/Fundo/amarelo.bmp", NULL);
    fundoxd[3] = load_bitmap("Sprites/Fundo/voce_venceu.bmp", NULL);

    ///SOM
     MIDI* music2 = load_midi("Sprites/Sons/EDCred.mid");
///Variáveis
    int num_frames = 6;
    int frame_w2 = jack1->w / num_frames;
    ///int frame_w3 = jack2->w / num_frames;
    int frame_atual;
    int tempo_troca = 100;

    float pos_y=400;
	float pos_x=50;

	bool andando = false;
	bool olhando_esquerda = false;
	bool parado = true;
	bool parado_e = false;
    bool pulando = false;
    bool pulandoAtras = false;
    bool atacando = false;
    bool tanochao = false;
    bool descer = false;
    bool descer_e = false;//--------------------
    bool atacando_e = false;//------------------

    bool level1 = true; // primeira tela



	///MAPA

	bool moveu = false;
	bool podepassar = false;
    bool lv1 = true;
    bool fim = true;

    play_midi(music2, TRUE);
    while(!key[KEY_ESC] && fim ){
     ///CÓDIGO DE MUDANÇA DE TELAS
        num_frames = 6;

        if(pos_x > 650){
            mapa++;
            moveu = true;
        }
        if (mapa == 3){
            fim = false;
        }
        frame_atual = (milisegundos / tempo_troca) % num_frames;

        draw_sprite(buffer ,fundoxd[mapa], 0,0);
        draw_sprite(buffer ,chao, 0,0);


        if(moveu){
            pos_x=50;
         moveu = false;
         podepassar = false;
         level1 = false;
        }

        if(level1 == true){
        if(pos_y == 200)
            podepassar = true;
        }

        ///CÓDIGO DO PULO
    if(pos_y == 400){
        tanochao = true;
        descer = false;
    }
    else tanochao = false;

        if(pulando){
            pos_y--;

            if(pos_y < 275){
                pulando = false;
                descer = true;

            }
        }

        if(!pulando && descer)
            pos_y++;

    if(mapa == 0){
        if(pos_x > 370 && pos_x < 412 && pos_y == 300){
            lv1 = false;
            podepassar = true;
        }
}
        ///CÓDIGO DA MOVIMENTAÇAO

        if (key[KEY_LEFT] && key[KEY_RIGHT]){
            parado = true;
            andando = false;
        }

        if (!key[KEY_RIGHT]){
            parado = true;
            andando = false;
        }

        if (!key[KEY_LEFT]){
            andando = false;
            parado = true;
        }

        if (key[KEY_LEFT] !=0 && pos_x > 0 && !key[KEY_RIGHT]){
            //colide(pos_x,pos_y,pos_x+30,pos_y+40,pl1.x1,pl1.y1,pl1.x2,pl1.y2);
            pos_x = pos_x - 0.75;

            andando = true;
            olhando_esquerda = true;
            parado_e = true;
            parado = false;
        }


        if (key[KEY_RIGHT] !=0 && !key[KEY_LEFT]){
            pos_x = pos_x + 0.75;

            andando = true;
            olhando_esquerda = false;
            parado_e = false;
            parado = false;
        }


        /*if (key[KEY_DOWN]  && pos_y < 400){
            //pos_y = pos_y + 0.75;
            andando = true;
            parado = false;
        }*/

        if (key[KEY_UP]  && tanochao && !key[KEY_Z]){

            atacando = false;
            if (parado_e){
                pulandoAtras = true;
            }
            else {
                pulandoAtras = false;
            }
            pulando = true;

        }
        else {
            if (!tanochao){
                descer = true;
                if (olhando_esquerda){ //key[KEY_LEFT]
                    descer_e = true;
                }
                else descer_e = false;
            }
            else descer = false;
        }

        if (key[KEY_Z] ){
            parado = false;
            andando = false;
            atacando = true;
            if (olhando_esquerda){
                atacando_e = true;
            }
            else atacando_e = false;
        }else atacando = false;


        if(level1){
            //colide(pos_x,pos_y,pos_x+30,pos_y+40,pl1.x1,pl1.y1,pl1.x2,pl1.y2);
            draw_sprite(buffer, plat, pl1.x1, pl1.y1);
           // draw_sprite(buffer, ameboss, pl1.x1-200, pl1.y1);






            if(lv1){

                draw_sprite(buffer ,star, 400,300);
            }
        }

        if (atacando && atacando_e && !pulando && !descer){

            draw_sprite (buffer, jackAtke, pos_x, pos_y);
        }

        if (atacando && !atacando_e && !pulando && !descer){

            draw_sprite (buffer, jackAtk, pos_x, pos_y);
        }

        if(pulando && !pulandoAtras){
            andando = false;
            descer = false;
            parado = false;
            num_frames = 10;
            masked_blit(jackPulo,buffer, frame_atual * frame_w2, 0, pos_x, pos_y, frame_w2, jack1->h);
        }

        if (pulando && pulandoAtras){
            andando = false;
            descer = false;
            parado = false;
            num_frames = 10;
            masked_blit(jackPuloe,buffer, frame_atual * frame_w2, 0, pos_x, pos_y, frame_w2, jack1->h);
        }

        if (descer && descer_e){
            andando = false;
            parado = false;
            draw_sprite (buffer, jackCaie, pos_x, pos_y);
        }

        if(descer && !descer_e){
            andando = false;
            parado = false;
            draw_sprite (buffer, jackCai, pos_x, pos_y);
        }


        if (andando && olhando_esquerda){

            masked_blit(jackesquerda, buffer, frame_atual * frame_w2, 0, pos_x, pos_y, frame_w2, jack1->h);
        }

        if (andando && !olhando_esquerda){

            masked_blit(jack1, buffer, frame_atual * frame_w2, 0, pos_x, pos_y, frame_w2, jack1->h);
        }

        if(parado){
            if (parado_e)
                masked_blit(jackIdle2, buffer, frame_atual * frame_w2, 0, pos_x, pos_y, frame_w2, jack1->h);
            else masked_blit(jackIdle, buffer, frame_atual * frame_w2, 0, pos_x, pos_y, frame_w2, jack1->h);

        }

        textprintf_ex(buffer, font, 530, 0, makecol(255,1,255), -1, "posicao X: %f", pos_x);//mostra a posiçao do x
        textprintf_ex(buffer, font, 530, 10, makecol(255,1,255), -1, "posicao Y: %f", pos_y);//mostra a posiçao do y
        textprintf_ex(buffer, font, 531, 1, makecol(255,255,255), -1, "posicao X: %f", pos_x);//mostra a posiçao do x
        textprintf_ex(buffer, font, 531, 11, makecol(255,255,255), -1, "posicao Y: %f", pos_y);//mostra a posiçao do y

           draw_sprite(screen ,buffer,0,0);

        clear_to_color(buffer, makecol(255,255,255));

            ticks--;
     }
     fimJogo(mapa);
    destroy_midi(music2);
};

void fimJogo(int &mapax){

        BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
        BITMAP* win = load_bitmap("Sprites/Fundo/voce_venceu.bmp", NULL);
        while (!key[KEY_ENTER]){
             draw_sprite(buffer ,win, 0,0);
             draw_sprite(screen,buffer,0,0);
        }
        mapax = 0;
        menu();
}

void pers_tem(){ // animacao inimigo
     if(pers_Tem==0) pers_Tem=1;
     else pers_Tem=0;
}

void creditos(){
    int cont = 0;
        BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
        BITMAP* creds = load_bitmap("Sprites/Menu/creds.bmp", NULL);

      while(cont < 2000){
         draw_sprite(buffer ,creds, 0,0);
         draw_sprite(screen,buffer,0,0);
         cont++;

     }
     menu();

};

int vida(){
    BITMAP* buffer = create_bitmap(SCREEN_W, SCREEN_H);
    BITMAP* rum = load_bitmap("Sprites/Fundo/rum_golinho.bmp", NULL);
    int hp = 3, dist_hp=0;
    for (int i=0; i<hp; i++){
        masked_blit(rum, buffer, 0, 0,dist_hp, 10, 50, 50);
        dist_hp+=55;
    }
    //colisão de dano recebido
}

bool colideLado(int ax1,int ay1,int ax2,int ay2,int bx1,int by1,int bx2,int by2){
    if(abs(ax2-bx1)<10){
        if(by1>ay1 && by1<ay2) return true;
        if(by2>ay1 && by2<ay2) return true;
        if(ay1>by1 && ay1<by2) return true;
    }
    return false;
}

bool colide(int ax1,int ay1,int ax2,int ay2,int bx1,int by1,int bx2,int by2){
    if(colideLado(ax1,ay1,ax2,ay2,bx1,by1,bx2,by2)) return true;
    if(colideLado(bx1,by1,bx2,by2,ax1,ay1,ax2,ay2)) return true;
    if(colideLado(ay1,ax1,ay2,ax2,by1,bx1,by2,bx2)) return true;
    if(colideLado(by1,bx1,by2,bx2,ay1,ax1,ay2,ax2)) return true;
    return false;
}



void init() {
int depth, res;
allegro_init();
depth = desktop_color_depth();
if (depth == 0) depth = 32;
set_color_depth(depth);
res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
if (res != 0) {
allegro_message(allegro_error);
exit(-1);
}

install_timer();
install_keyboard();
install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);//
install_mouse();
/* add other initializations here */
}

void deinit() {
clear_keybuf();
/* add other deinitializations here */
}
