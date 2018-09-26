#include <allegro5/allegro.h>
#include "allegro5/allegro_image.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const float FPS = 20;
const int SCREEN_W = 500;
const int SCREEN_H = 550;

enum MYKEYS
{
    KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_P, KEY_S, KEY_N, KEY_SPACE, KEY_R
};

//matriz definindo mapa do jogo: 1 representa paredes, 0 representa corredor
char MAPA[26][26] =
{	
    "1111111111111111111111111",
    "1400000001111111000000041",
    "1011111101111111011111101",
    "1011111100000000011111101",
    "1000000001111111000000001",
    "1110111101111111011110111",
    "1000111100001000011110001",
    "1010111111101011111110101",
    "1010000111001001110000101",
    "1011110111011101110111101",
    "1011110100000000010111101",
    "1000110001110111000110001",
    "1110111101260721011110111",
    "8000000001260721000000009",
    "1011111101110111011111101",
    "1011100000000000000011101",
    "1000001111110111111000001",
    "1111101110000000111011111",
    "1111101110111110111011111",
    "1000000000000000000000001",
    "1011101111110111111011101",
    "1000001111110111111000001",
    "1011100000010100000011101",
    "1400001111000001111000041",
    "1111111111111111111111111",
};

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *mapa   = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;
ALLEGRO_FONT *fonte = NULL, *ganhou= NULL, *fsair= NULL;
ALLEGRO_BITMAP *pacmanl   = NULL, *pacmanr   = NULL, *pacmanu = NULL, *pacmand   = NULL, *pacmanf   = NULL;
ALLEGRO_BITMAP *fanta  = NULL, *fantl   = NULL, *fantt   = NULL, *fantk   = NULL , *fantass = NULL;
ALLEGRO_BITMAP *ponto  = NULL, *pilu = NULL;
ALLEGRO_BITMAP *inicial  = NULL;
ALLEGRO_BITMAP *inicialc  = NULL;

int i = 20, j = 12, c[4], l[4];	//posição inicial do Pacman e do fantasma na matriz
int q = 20; 	//tamanho de cada célula no mapa
int posy = i*q;
int posx = j*q;	
int contu = 0, contd = 0, contl = 0, contr = 0;
int contfu[4], contfd[4], contfl[4], contfr[4];
int posxfant[4], posyfant[4];
int pontos=0,vidas=3;
int contstart = 0, contass = 0, contmorte = 0;

int r1, r2, r3, r4=2, pont=0, cont=0;

bool key[4] = { false, false, false, false };
bool redraw = true;
bool sair = false, quersair = false, start = false;
bool esquerda[4], direita[4], cima[4], baixo[4], U = false, D = false, L = false, R = false;
bool telainicial = true, teste = true, pause = false, conts = false, respaw = true, pos2 = true, contasss = false, perdeu = true;
bool ivulneravel[4];

int inicializa() {
	
	for(int z=0;z<4;z++){
		contfu[z]=0;
		contfd[z]=0;
		contfl[z]=0;
		contfr[z]=0;
		esquerda[z]=false;
		direita[z]=false;
		cima[z]=false;
		baixo[z]=false;
	}
	
	ivulneravel[0] = false; ivulneravel[1] = false; ivulneravel[2] = false; ivulneravel[3] = false;
	
	c[0]=10; l[0]=13;
	c[1]=10; l[1]=12;
	c[2]=14; l[2]=13;
	c[3]=14; l[3]=12;
	
	posxfant[0]=c[0]*q; posyfant[0]=l[0]*q;
	posxfant[1]=c[1]*q; posyfant[1]=l[1]*q;
	posxfant[2]=c[2]*q; posyfant[2]=l[2]*q;
	posxfant[3]=c[3]*q; posyfant[3]=l[3]*q;
	
	
    if(!al_init())
    {
        cout << "Falha ao carregar Allegro" << endl;
        return 0;
    }

    if(!al_install_keyboard())
    {
        cout << "Falha ao inicializar o teclado" << endl;
        return 0;
    }

    timer = al_create_timer(1.0 / FPS);
    if(!timer)
    {
        cout << "Falha ao inicializar o temporizador" << endl;
        return 0;
    }
	
	al_init_font_addon();											//FONTEEEEEE
    if (!al_init_ttf_addon())
    {
        cout << "Falha ao inicializar add-on allegro_ttf.\n";
        return 0;
    }
	
	fonte = al_load_font("gameover.TTF", 70, 0);
	ganhou = al_load_font("gameover.TTF", 200, 0);
	fsair = al_load_font("gameover.TTF", 150, 0);
    if (!fonte || !ganhou || !fsair)
    {
        cout << "Falha ao carregar fonte.\n";
        return 0;
    }
	
																	//MUSICAAAAAAAAAAAAA
	if(!al_install_audio())
    {
        cout << (stderr, "Falha ao inicializar áudio.\n");
        return false;
    }
 
    if(!al_init_acodec_addon())
    {
        cout << (stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }
 
    if(!al_reserve_samples(1))
    {
        cout << (stderr, "Falha ao alocar canais de áudio.\n");
        return false;
	}
	
    if(!al_init_image_addon())
    {
        cout <<"Falha ao iniciar al_init_image_addon!" << endl;
        return 0;
    }
	
	musica = al_load_audio_stream("proerd.ogg", 4, 1024);
    if(!musica)
    {
        cout << "Falha ao inicializar a musica" << endl;
        return 0;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        cout << "Falha ao inicializar a tela" << endl;
        al_destroy_timer(timer);
        return 0;
    }

    mapa = al_load_bitmap("map.bmp");
    if(!mapa)
    {
        cout << "Falha ao carregar o mapa!" << endl;
        al_destroy_display(display);
        return 0;
	}
	
	inicial = al_load_bitmap("inicial.bmp");
    if(!inicial)
    {
        cout << "Falha ao carregar a tela inicial!" << endl;
        al_destroy_display(display);
        return 0;
	}
	
	inicialc = al_load_bitmap("inicialc.bmp");
    if(!inicialc)
    {
        cout << "Falha ao carregar o a tela inicial!" << endl;
        al_destroy_display(display);
        return 0;
    }
	
	al_draw_bitmap(inicialc,0,0,0);

    al_draw_bitmap(mapa,0,0,0);

    pacmanl = al_load_bitmap("pacmanl.tga");
	pacmanu = al_load_bitmap("pacmanu.tga");
	pacmanr = al_load_bitmap("pacmanr.tga");
	pacmand = al_load_bitmap("pacmand.tga");
	pacmanf = al_load_bitmap("pacmanf.tga");
    if(!pacmanl || !pacmanu || !pacmand || !pacmanr || !pacmanf)
    {
        cout << "Falha ao carregar o pacman!" << endl;   //CARRENGANDO OS PACMANS E O FANTASMA
        al_destroy_display(display);
        return 0;
    }
	
	fanta = al_load_bitmap("fanta.tga");
	fantl = al_load_bitmap("fantl.tga");
	fantk = al_load_bitmap("fantk.tga");
	fantt = al_load_bitmap("fantt.tga");
	fantass = al_load_bitmap("fantass.tga");
    if(!fanta || !fantass)
    {
        cout << "Falha ao carregar o fantasma!" << endl; //FANSTASMAAAAAAA
        al_destroy_display(display);
        return 0;
    }
	

	ponto = al_load_bitmap("ponto.tga");
	pilu = al_load_bitmap("pilu.tga");
	if(!ponto || !pilu)
    {
        cout << "Falha ao carregar os pontos!" << endl;
        al_destroy_display(display);
        return 0;
    }
	
    al_draw_bitmap(pacmanl,posx,posy,0);
	al_draw_bitmap(fanta,posxfant[0],posyfant[0],0);
	al_draw_bitmap(fantl,posxfant[1],posyfant[1],0);
	al_draw_bitmap(fantk,posxfant[2],posyfant[2],0);
	al_draw_bitmap(fantt,posxfant[3],posyfant[3],0);
	
	for(int t=0;t<26;t++)
		for(int u=0;u<26;u++)
			if(MAPA[u][t]=='0')
				al_draw_bitmap(ponto,t*20,u*20,0);
	

    event_queue = al_create_event_queue();
	
    if(!event_queue)
    {
        cout << "Falha ao criar a fila de eventos" << endl;
        al_destroy_display(display);
        al_destroy_timer(timer);
        return 0;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
    al_start_timer(timer);

	srand(time(0));

	
    return 1;
}

int main(int argc, char **argv)
{
    if(!inicializa()) return -1;
	
	al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
	al_set_audio_stream_playing(musica,true);

    while(!sair)       //ONDE O JOGO COMEÇA...
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
		
	
			if(cima[0] && MAPA[l[0]-1][c[0]] != '1' && start && perdeu)				//FANTASMA 1
            {
				/*
				l--;
                posyfanta = l*q;	//MOVIMENTAÇÃO ANTIGA PARA 4FPS (CASO DE ERRO)
				posy -= 4;
				contu++;
				if(contu==5)
					contu=0;
					i-=1;
					*/
				posyfant[0]-=4;
				contfu[0]++;
				if(contfu[0]==5){
					contfu[0]=0;
					l[0]--;
				}
            }

            else if(baixo[0] && MAPA[l[0]+1][c[0]] != '1' && start && perdeu)
            {
                posyfant[0]+=4;
				contfd[0]++;
				if(contfd[0]==5){
					contfd[0]=0;
					l[0]++;
				}
            }

            else if(esquerda[0] && MAPA[l[0]][c[0]-1] != '1' && MAPA[l[0]][c[0]-1] != '6' && start && perdeu)
            {
                posxfant[0]-=4;
				contfl[0]++;
				if(contfl[0]==5){
					contfl[0]=0;
					c[0]--;
				}
            }

            else if(direita[0] && MAPA[l[0]][c[0]+1] != '1' && MAPA[l[2]][c[2]+1] != '7' && start && perdeu)
            {
                posxfant[0]+=4;
				contfr[0]++;
				if(contfr[0]==5){
					contfr[0]=0;
					c[0]++;
				}
            }
		
		if(cima[0] && MAPA[l[0]-1][c[0]] == '1' || baixo[0] && MAPA[l[0]+1][c[0]] == '1' || esquerda[0]  && MAPA[l[0]][c[0]-1] == '1' || esquerda[0] && MAPA[l[0]][c[0]-1] == '6' ||
		direita[0] && MAPA[l[0]][c[0]+1] == '1' || direita[0] && MAPA[l[0]][c[0]+1] == '7'){
			r1=rand()%4;
			cima[0] = baixo[0] = esquerda[0] = direita[0] = false;
		}
		
		if(r1==0)
			esquerda[0]=true;
		if(r1==1)
			direita[0]=true;
		if(r1==2)
			cima[0]=true;
		if(r1==3)
			baixo[0]=true;
		
		
		
			if(cima[1] && MAPA[l[1]-1][c[1]] != '1' && start && perdeu)				//FANTASMA 2
            {
				/*
				l--;
                posyfanta = l*q;	//MOVIMENTAÇÃO ANTIGA PARA 4FPS (CASO DE ERRO)
				posy -= 4;
				contu++;
				if(contu==5)
					contu=0;
					i-=1;
					*/
				posyfant[1]-=4;
				contfu[1]++;
				if(contfu[1]==5){
					contfu[1]=0;
					l[1]--;
				}
            }

            else if(baixo[1] && MAPA[l[1]+1][c[1]] != '1' && start && perdeu)
            {
                posyfant[1]+=4;
				contfd[1]++;
				if(contfd[1]==5){
					contfd[1]=0;
					l[1]++;
				}
            }

            else if(esquerda[1] && MAPA[l[1]][c[1]-1] != '1' && MAPA[l[1]][c[1]-1] != '6' && start && perdeu)
            {
                posxfant[1]-=4;
				contfl[1]++;
				if(contfl[1]==5){
					contfl[1]=0;
					c[1]--;
				}
            }

            else if(direita[1] && MAPA[l[1]][c[1]+1] != '1' && MAPA[l[1]][c[1]+1] != '7' && start && perdeu)
            {
                posxfant[1]+=4;
				contfr[1]++;
				if(contfr[1]==5){
					contfr[1]=0;
					c[1]++;
				}
            }

	
		if(cima[1] && MAPA[l[1]-1][c[1]] == '1' || baixo[1] && MAPA[l[1]+1][c[1]] == '1' || esquerda[1]  && MAPA[l[1]][c[1]-1] == '1'  || esquerda[1] && MAPA[l[1]][c[1]-1] == '6' ||
		direita[1] && MAPA[l[1]][c[1]+1] == '1' || direita[1] && MAPA[l[1]][c[1]+1] == '7'){
			r2=rand()%4;
			cima[1] = baixo[1] = esquerda[1] = direita[1] = false;
		}
		
		if(r2==0)
			esquerda[1]=true;
		if(r2==1)
			direita[1]=true;
		if(r2==2)
			cima[1]=true;
		if(r2==3)
			baixo[1]=true;
		
		
			if(cima[2] && MAPA[l[2]-1][c[2]] != '1' && start && perdeu)				//FANTASMA 3
            {
				/*
				l--;
                posyfanta = l*q;	//MOVIMENTAÇÃO ANTIGA PARA 4FPS (CASO DE ERRO)
				posy -= 4;
				contu++;
				if(contu==5)
					contu=0;
					i-=1;
					*/
				posyfant[2]-=4;
				contfu[2]++;
				if(contfu[2]==5){
					contfu[2]=0;
					l[2]--;
				}
            }

            else if(baixo[2] && MAPA[l[2]+1][c[2]] != '1' && start && perdeu)
            {
                posyfant[2]+=4;
				contfd[2]++;
				if(contfd[2]==5){
					contfd[2]=0;
					l[2]++;
				}
            }

            else if(esquerda[2] && MAPA[l[2]][c[2]-1] != '1' && MAPA[l[2]][c[2]-1] != '6' && start && perdeu)
            {
                posxfant[2]-=4;
				contfl[2]++;
				if(contfl[2]==5){
					contfl[2]=0;
					c[2]--;
				}
            }

            else if(direita[2] && MAPA[l[2]][c[2]+1] != '1' && MAPA[l[2]][c[2]+1] != '7' && start && perdeu)
            {
                posxfant[2]+=4;
				contfr[2]++;
				if(contfr[2]==5){
					contfr[2]=0;
					c[2]++;
				}
            }

		if(cima[2] && MAPA[l[2]-1][c[2]] == '1' || baixo[2] && MAPA[l[2]+1][c[2]] == '1' || esquerda[2]  && MAPA[l[2]][c[2]-1] == '1' || esquerda[2] && MAPA[l[2]][c[2]-1] == '6' ||
		direita[2] && MAPA[l[2]][c[2]+1] == '1' || direita[2] && MAPA[l[2]][c[2]+1] == '7'){
			r3=rand()%4;
			cima[2] = baixo[2] = esquerda[2] = direita[2] = false;
		}
		
		if(r3==0)
			esquerda[2]=true;
		if(r3==1)
			direita[2]=true;
		if(r3==2)
			cima[2]=true;
		if(r3==3)
			baixo[2]=true;
		

			if(cima[3] && MAPA[l[3]-1][c[3]] != '1' && start && perdeu)				//FANTASMA inteligente
            {

				posyfant[3]-=4;
				contfu[3]++;
				if(contfu[3]==5){
					contfu[3]=0;
					l[3]--;
				}
            }

            else if(baixo[3] && MAPA[l[3]+1][c[3]] != '1' && start && perdeu)
            {
                posyfant[3]+=4;
				contfd[3]++;
				if(contfd[3]==5){
					contfd[3]=0;
					l[3]++;
				}
            }

            else if(esquerda[3] && MAPA[l[3]][c[3]-1] != '1' && start && perdeu)
            {
                posxfant[3]-=4;
				contfl[3]++;
				if(contfl[3]==5){
					contfl[3]=0;
					c[3]--;
				}
            }

            else if(direita[3] && MAPA[l[3]][c[3]+1] != '1' && start && perdeu)
            {
                posxfant[3]+=4;
				contfr[3]++;
				if(contfr[3]==5){
					contfr[3]=0;
					c[3]++;
				}
            }
			
			if(esquerda[3]  && MAPA[l[3]][c[3]-1] == '1'){
				esquerda[3] = false;
			}
			if(direita[3] && MAPA[l[3]][c[3]+1] == '1'){
				direita[3] = false;
			}
			if(cima[3] && MAPA[l[3]-1][c[3]] == '1'){
				cima[3] = false;
			}
			if(baixo[3] && MAPA[l[3]+1][c[3]] == '1'){
				baixo[3] = false;
			}		
					
				if(j!=c[3] && i!=l[3] && (MAPA[l[3]][c[3]-1] == '1' || MAPA[l[3]][c[3]+1] == '1')){     //CODIGOS RELACIONADOS A INTELIGENCIA DO FANTASMA
					if(j-c[3]<0){
						if(MAPA[l[3]-1][c[3]] == '1' && MAPA[l[3]][c[3]-1] == '1')
							r4=3;
						else if(MAPA[l[3]+1][c[3]] == '1' && MAPA[l[3]][c[3]-1] == '1')
							r4=2;
						else
							r4=0;
					}
				
					else if(j-c[3]>0){
						if(MAPA[l[3]-1][c[3]] == '1' && MAPA[l[3]][c[3]+1] == '1')
							r4=3;
						else if(MAPA[l[3]+1][c[3]] == '1' && MAPA[l[3]][c[3]+1] == '1')
							r4=1;
						else
							r4=1;
					}
					cout << 111 << endl;
				}
				else if(i!=l[3] && j!=c[3] && (MAPA[l[3]-1][c[3]] == '1' || MAPA[l[3]+1][c[3]] == '1')){
					if(i-l[3]<0){
						if(MAPA[l[3]+1][c[3]] == '1' && MAPA[l[3]][c[3]+1] == '1')
							r4=0;
						else if(MAPA[l[3]+1][c[3]] == '1' && MAPA[l[3]][c[3]-1] == '1')
							r4=1;
						else
							r4=2;
					}
					else if(i-l[3]>0){
						if(MAPA[l[3]-1][c[3]] == '1' && MAPA[l[3]][c[3]+1] == '1')
							r4=0;
						else if(MAPA[l[3]-1][c[3]] == '1' && MAPA[l[3]][c[3]-1] == '1')
							r4=1;
						else
							r4=3;
					}
					cout << 222 << endl;
				}
				else if(j==c[3] && i!=l[3]){
					if(MAPA[l[3]-1][c[3]] == '1')
						r4=0;
					if(MAPA[l[3]+1][c[3]] == '1')
						r4=1;
				}
				else if(i==l[3] && i!=l[3]){
					if(MAPA[l[3]][c[3]-1] == '1')
						r4=2;
					if(MAPA[l[3]][c[3]+1] == '1')
						r4=3;
				}
				else if(j==c[3] && (MAPA[l[3]][c[3]-1] == '1' || MAPA[l[3]][c[3]+1] == '1')){
					if(i-l[3]<0)
						r4=2;
					else if(i-l[3]>0)
						r4=3;
					cout << 333 << endl;
				}
				else if(i==l[3] && (MAPA[l[3]-1][c[3]] == '1' || MAPA[l[3]+1][c[3]] == '1')){
					if(j-c[3]<0)
						r4=0;
					else if(j-c[3]>0)
						r4=1;
					cout << 444 << endl;
				}

			
			if(r4==0)
				esquerda[3]=true;
			if(r4==1)
				direita[3]=true;
			if(r4==2)
				cima[3]=true;
			if(r4==3)
				baixo[3]=true;




        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            if(key[KEY_UP] && MAPA[i-1][j] != '1'  && start && respaw && perdeu)
            {
				if(posx%20==0){
					posy -= 4;
					contu++;
					if(contu==5 ){
						contu=0;
						i--;
					}
					
					U = true;
					D = L = R = false;
				}
            }

            if(key[KEY_DOWN] && MAPA[i+1][j] != '1'  && start && respaw && perdeu)
            {
				if(posx%20==0){
					posy += 4;
					contd++;
					if(contd==5){
						contd=0;
						i++;
					}
					D = true;
					U = L = R = false;
				}
            }

            if(key[KEY_LEFT] && MAPA[i][j-1] != '1'  && start && respaw && perdeu)
            {
				if(posy%20==0){
					posx -= 4;
					contl++;
					if(contl==5){
						contl=0;
						j--;
					}
					L = true;
					U = D = R = false;
				}
            }

            if(key[KEY_RIGHT] && MAPA[i][j+1] != '1' && start && respaw && perdeu)
            {
				if(posy%20==0){
					posx += 4;
					contr++;
					if(contr==5){
						contr=0;
						j++;
					}
					R = true;
					U = D = L = false;
				}
            }

            redraw = true;
        }
        else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            break;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {		
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
					key[KEY_UP] = true;
                break;

            case ALLEGRO_KEY_DOWN:
					key[KEY_DOWN] = true;
                break;

            case ALLEGRO_KEY_LEFT:
					key[KEY_LEFT] = true;
                break;

            case ALLEGRO_KEY_RIGHT:
					key[KEY_RIGHT] = true;
                break;
			
			case ALLEGRO_KEY_P:		//PAUSA DO GAME
				pause=true;
				break;
			
			case ALLEGRO_KEY_SPACE:
				conts=true;
				telainicial = false;
				break;
			
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_UP:
		
                key[KEY_DOWN] = false;
				key[KEY_UP] = true;				
				key[KEY_LEFT] = false;
				key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_DOWN:
			
                key[KEY_DOWN] = true;
				key[KEY_UP] = false;				
				key[KEY_LEFT] = false;
				key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_LEFT:

                key[KEY_DOWN] = false;
				key[KEY_UP] = false;				
				key[KEY_LEFT] = true;
				key[KEY_RIGHT] = false;
                break;

            case ALLEGRO_KEY_RIGHT:
			

                key[KEY_DOWN] = false;
				key[KEY_UP] = false;				
				key[KEY_LEFT] = false;
				key[KEY_RIGHT] = true;
                break;

            case ALLEGRO_KEY_ESCAPE:
                quersair = true;
                break;
							
            }
        }
		
		
		
		if(((i==l[0] && j==c[0]) || (i==l[1] && j==c[1]) || (i==l[2] && j==c[2]) || (i==l[3] && j==c[3])) && ivulneravel[0]==false && ivulneravel[1]==false
		&& ivulneravel[2]==false && ivulneravel[3]==false){			//SE ENCOSTAR NO FANTASMA 
			vidas--;
			respaw = false;
		}
		
		if(respaw==false){
			key[KEY_DOWN] = key[KEY_UP] = key[KEY_LEFT] = key[KEY_RIGHT] = false;	//VOLTA A POSIÇAO INICIAL SE MORRER (E CONSERTA UM BUG QUANDO RENASCE)
			contu = contd = contr = contl = 0;
			i = 20 ; j = 12;
			posy=i*q; posx=j*q;
			contmorte++;
			if(contmorte==20){
				respaw=true;
				contmorte=0;
			}
		}
		
		if(MAPA[i][j]=='8'){	//TUNELLL
			key[KEY_DOWN] = key[KEY_UP] = key[KEY_LEFT] = key[KEY_RIGHT] = false;
			contu = contd = contr = contl = 0;
			i = 13 ; j = 23;
			posy=i*q; posx=j*q;
		}
		if(MAPA[i][j]=='9'){	//TUNELLL
			key[KEY_DOWN] = key[KEY_UP] = key[KEY_LEFT] = key[KEY_RIGHT] = false;
			contu = contd = contr = contl = 0;
			i = 13 ; j = 1;
			posy=i*q; posx=j*q;
		}
		
		
        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
			
            al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_bitmap(mapa,0,0,0);
			
			
			if(ivulneravel[0]==false)
				al_draw_bitmap(fanta,posxfant[0],posyfant[0],0);	//FANTASMAS
			if(ivulneravel[1]==false)
				al_draw_bitmap(fantl,posxfant[1],posyfant[1],0);
			if(ivulneravel[2]==false)
				al_draw_bitmap(fantk,posxfant[2],posyfant[2],0);
			if(ivulneravel[3]==false)
				al_draw_bitmap(fantt,posxfant[3],posyfant[3],0);
			
			
			if(ivulneravel[0])
				al_draw_bitmap(fantass,posxfant[0],posyfant[0],0);  //FANTASMAS VULNERAVEIS
			if(ivulneravel[1])
				al_draw_bitmap(fantass,posxfant[1],posyfant[1],0);
			if(ivulneravel[2])
				al_draw_bitmap(fantass,posxfant[2],posyfant[2],0);
			if(ivulneravel[3])
				al_draw_bitmap(fantass,posxfant[3],posyfant[3],0);
				
			if(contasss)
				contass++;
			
				if(contass==120){
					ivulneravel[0]=false;
					ivulneravel[1]=false;
					ivulneravel[2]=false;
					ivulneravel[3]=false;
					contasss = false;
					contass=0;
				}
				if(i==l[0] && j==c[0]){			//SE ENCOSTAR NO FANTASMA VULNERAVEL
					l[0] = 13;
					c[0] = 10;
					posyfant[0]=l[0]*q;
					posxfant[0]=c[0]*q;
					direita[0] = true;
					ivulneravel[0]=false;
					pontos+=100;
				}
				if(i==l[1] && j==c[1]){			//SE ENCOSTAR NO FANTASMA VULNERAVEL
					l[0] = 12;
					c[0] = 10;
					posyfant[1]=l[1]*q;
					posxfant[1]=c[1]*q;
					direita[1] = true;	
					ivulneravel[1]=false;
					pontos+=100;
				}
				if(i==l[2] && j==c[2]){			//SE ENCOSTAR NO FANTASMA VULNERAVEL
					l[2] = 13;
					c[2] = 14;
					posyfant[2]=l[2]*q;
					posxfant[2]=c[2]*q;
					direita[2] = true;	
					ivulneravel[2]=false;
					pontos+=100;
				}
				if(i==l[3] && j==c[3]){			//SE ENCOSTAR NO FANTASMA VULNERAVEL
					l[3] = 12;
					c[3] = 14;
					posyfant[3]=l[3]*q;
					posxfant[3]=c[3]*q;
					direita[3] = true;
					ivulneravel[3]=false;
					pontos+=100;
				}
			
			if(vidas<1){	//CONTAGEM DE VIDAS
				perdeu = false;
				start = false;
				al_draw_textf(fsair, al_map_rgb(255, 255, 255), SCREEN_H / 2.2, 150, ALLEGRO_ALIGN_CENTRE, "GAME OVER");
			}
			
			for(int t=0;t<26;t++)
				for(int u=0;u<26;u++)	{	//COLOCA AS BOLINHAS
					if(MAPA[u][t]=='0')
						al_draw_bitmap(ponto,t*20,u*20,0);
					if(MAPA[u][t]=='4')
						al_draw_bitmap(pilu,t*20,u*20,0);
				}
					
			MAPA[12][12]='3';
			
			//al_draw_bitmap(pacman,posx,posy,0);
			al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_H / 7, 505, ALLEGRO_ALIGN_CENTRE, "PONTOS: %d",pontos);
			al_draw_textf(fonte, al_map_rgb(255, 255, 255), SCREEN_H / 1.3, 505, ALLEGRO_ALIGN_CENTRE, "VIDAS: %dx",vidas);
						
			if(pontos > 2000)
				al_draw_text(ganhou, al_map_rgb(255, 255, 255), SCREEN_H / 2.3, 200, ALLEGRO_ALIGN_CENTRE, "YOU WIN!!!");
			
			if(cont < 5 || cont>10 && cont<15){
				if(U)
					al_draw_bitmap(pacmanu,posx,posy,0); //ABRE E FECHA A BOCA
				if(D)
					al_draw_bitmap(pacmand,posx,posy,0);
				if(L)
					al_draw_bitmap(pacmanl,posx,posy,0);
				if(R)
					al_draw_bitmap(pacmanr,posx,posy,0);
			}
			else if(cont<10 || cont<20)
				al_draw_bitmap(pacmanf,posx,posy,0);
			
			
			if(MAPA[posy/20][posx/20]=='0'){	 //COME AS BOLINHAS
				MAPA[posy/20][posx/20]='2';
				pontos++;
			}
			
			if(MAPA[posy/20][posx/20]=='4'){ 	//PILULA
				MAPA[posy/20][posx/20]='2';
				pontos+=30;
				contasss=true;
				ivulneravel[0] = ivulneravel[1] = ivulneravel[2] = ivulneravel[3] = true;
			}
			
			
			if(telainicial && cont<10)  //TELA INICIAL DO JOGO
				al_draw_bitmap(inicialc,0,0,0);
			else if(telainicial && cont<20)
				al_draw_bitmap(inicial,0,0,0);
			
			cont++;  //CONTADOR USADO PARA ABRIR A BOCA, TELA INICIAL, ETC
			
			if(cont==20)
				cont=0;

			
			if(conts)
				contstart++;
			if(contstart>=10 && contstart<30)
				al_draw_textf(fsair, al_map_rgb(255, 255, 255), SCREEN_H / 2.2, 150, ALLEGRO_ALIGN_CENTRE, "3"); 	//3, 2, 1, start!
			if(contstart>=30 && contstart<50)
				al_draw_textf(fsair, al_map_rgb(255, 255, 255), SCREEN_H / 2.2, 150, ALLEGRO_ALIGN_CENTRE, "2");
			if(contstart>=50 && contstart<70)
				al_draw_textf(fsair, al_map_rgb(255, 255, 255), SCREEN_H / 2.2, 150, ALLEGRO_ALIGN_CENTRE, "1");
			if(contstart>=70 && contstart<90)
				al_draw_textf(fsair, al_map_rgb(255, 255, 255), SCREEN_H / 2.2, 150, ALLEGRO_ALIGN_CENTRE, "START!!!");
			if(contstart>90){
				conts=false;
				start=true;
			}
			//al_draw_textf(fsair, al_map_rgb(255, 255, 255), SCREEN_H / 2.2, 150, ALLEGRO_ALIGN_CENTRE, "PAUSADO");
							
			while(pause){
				al_wait_for_event(event_queue, &ev);
				ev.type == ALLEGRO_EVENT_KEY_DOWN;
				if(ev.type == ALLEGRO_EVENT_KEY_DOWN){	//PAUSA O GAME
					switch(ev.keyboard.keycode){
						case ALLEGRO_KEY_P:
						pause=false;
						break;
					}
				}
			}
			
		
			if(quersair){
				al_draw_textf(fsair, al_map_rgb(255, 255, 255), SCREEN_H / 2.2, 150, ALLEGRO_ALIGN_CENTRE, "DESEJA SAIR?");
				al_draw_textf(fsair, al_map_rgb(255, 255, 255), SCREEN_H / 2.2, 200, ALLEGRO_ALIGN_CENTRE, "S/N");
				ALLEGRO_EVENT ev;
				al_wait_for_event(event_queue, &ev);
				ev.type == ALLEGRO_EVENT_KEY_UP;
					switch(ev.keyboard.keycode){
						case ALLEGRO_KEY_S:
						sair=true;
						break;
						
						case ALLEGRO_KEY_N:
						quersair=false;
						break;
					}
				
			}

			
            al_flip_display();
			
			//cout << "posx: " << posx << " " << "posy: " << posy << endl;
			
        }
    }
	al_destroy_bitmap(inicial);
	al_destroy_bitmap(inicialc);
    al_destroy_bitmap(mapa);
	al_destroy_bitmap(pacmanf);
    al_destroy_bitmap(pacmanr);
	al_destroy_bitmap(pacmanu);
	al_destroy_bitmap(pacmand);
	al_destroy_bitmap(pacmanl);
	al_destroy_bitmap(fanta);
	al_destroy_bitmap(fantt);
	al_destroy_bitmap(fantk);
	al_destroy_bitmap(fantl);
	al_destroy_bitmap(ponto);
	al_destroy_bitmap(pilu);
	al_destroy_bitmap(fantass);
    al_destroy_timer(timer);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
