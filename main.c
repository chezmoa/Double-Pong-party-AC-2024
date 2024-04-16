#include <gb/gb.h>
#include <stdint.h>
#include <stdio.h>
#include "sprites.h"

#define SPRITE_BALLE 0
#define SPRITE_VERRE 20 
#define SPRITE_J1 3
#define SPRITE_J2 6
#define SPRITE_VERRE2 9
#define J1X 10
#define J2X 150
#define SPRITE_SLIP 25
#define SPRITE_SLIP2 30

UINT8 j1y;
UINT8 j2y;

UINT8 BALL_X = 100;
UINT8 BALL_Y = 80;
INT8 BALL_DELTA_X = -1;
INT8 BALL_DELTA_Y = -1;

UINT8 SCORE_J1 = 0;
UINT8 SCORE_J2 = 0;

char *NOMJ1;
char *NOMJ2;

void bouge_j1(UINT8 y)
{
  move_sprite(SPRITE_J1, J1X, y+16);
  move_sprite(SPRITE_J1+1, J1X, y+8+16);
  move_sprite(SPRITE_J1+2, J1X, y+16+16);
}

void bouge_j2(UINT8 y)
{
  move_sprite(SPRITE_J2, J2X, y+16);
  move_sprite(SPRITE_J2+1, J2X, y+8+16);
  move_sprite(SPRITE_J2+2, J2X, y+16+16);
}

void bouge_balle(UINT8 x, UINT8 y)
{
   move_sprite(SPRITE_BALLE, x, y+16);
}

void clean_screen(void)
{
 // printf("                                                                                                                                                                                                                                                                                                                                                                        ");
printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void setup_sprites()
{
  set_sprite_data(0, 11, sprites);  //Je charge mes sprites dans la RAM

  set_sprite_tile(SPRITE_BALLE, 0);
//sprites j2
  set_sprite_tile(SPRITE_J2, 1);
  set_sprite_tile(SPRITE_J2+1, 2);
  set_sprite_tile(SPRITE_J2+2, 3);
//sprites j1
  set_sprite_tile(SPRITE_J1, 4);
  set_sprite_tile(SPRITE_J1+1, 5);
  set_sprite_tile(SPRITE_J1+2, 6);
//sprites de score j2 en verres
  set_sprite_tile(SPRITE_VERRE2, 8);
  set_sprite_tile(SPRITE_VERRE2+1, 8);
  set_sprite_tile(SPRITE_VERRE2+2, 8);
  set_sprite_tile(SPRITE_VERRE2+3, 8);
//sprites de score j1 en verres
  set_sprite_tile(SPRITE_VERRE, 7);
  set_sprite_tile(SPRITE_VERRE+1, 7);
  set_sprite_tile(SPRITE_VERRE+2, 7);
  set_sprite_tile(SPRITE_VERRE+3, 7);
//sprites de score j1 en slip
  set_sprite_tile(SPRITE_SLIP, 9);
  set_sprite_tile(SPRITE_SLIP+1, 9);
  set_sprite_tile(SPRITE_SLIP+2, 9);
  set_sprite_tile(SPRITE_SLIP+3, 9);
  //sprites de score j2 en slip
  set_sprite_tile(SPRITE_SLIP2, 10);
  set_sprite_tile(SPRITE_SLIP2+1, 10);
  set_sprite_tile(SPRITE_SLIP2+2, 10);
  set_sprite_tile(SPRITE_SLIP2+3, 10);
}

void main(void)
{ 
  UINT8 key;
  UINT8 point_typej1; 
  UINT8 point_typej2;
  
  setup_sprites();

//On allume le son
  NR52_REG = 0x80;
  NR50_REG = 0x77;
  NR51_REG = 0xFF;

restart:
  printf("\n\n\n\n\n   Pour jouer avec\n   Xirius et Nhut\n    appuie sur A\n\n\n   Pour jouer avec\n    Fei et Marie\n    appuie sur B\n\n\n\n\n");
    key = waitpad(J_A | J_B);
    if (key & J_A) 
    {
        NOMJ1 = "Xirius";
        NOMJ2 = "Nhut";
        point_typej1 = SPRITE_VERRE;
        point_typej2 = SPRITE_VERRE2;
    }
    else if (key & J_B) 
    {
        NOMJ1 = "Fei";
        NOMJ2 = "Marie";
        point_typej1 = SPRITE_SLIP;
        point_typej2 = SPRITE_SLIP2;
    }
  clean_screen();
  printf("\n   %s VS %s", NOMJ1, NOMJ2);

  j1y=50;
  j2y=50;

  bouge_balle(100, 80);

  //setup j1 & j2
  bouge_j1(j1y);
  bouge_j2(j2y);

  SHOW_SPRITES;

    // Loop forever
    while(1) 
    {
       //mouvement j1 dans ecran
       UINT8 keys = joypad();

        if ((keys & J_UP) && (j1y>0)) 
        {
          j1y--;
          bouge_j1(j1y);
        }
        else if ((keys & J_DOWN) && (j1y<(144-24))) 
        {
          j1y++;
          bouge_j1(j1y);
        }
       //mouvement j2
        if ((keys & J_A) && (j2y>0)) 
        {
          j2y--;
          bouge_j2(j2y);
        } 
        else if ((keys & J_B) && (j2y<(144-24))) 
        {
          j2y++;
          bouge_j2(j2y);
        }
        //mouvevement balle
        BALL_X += BALL_DELTA_X;
        BALL_Y += BALL_DELTA_Y;
        bouge_balle(BALL_X, BALL_Y);

        //rebondissement balle
        if ((BALL_Y == 0) || (BALL_Y == (144-8)))
        {
          BALL_DELTA_Y = -BALL_DELTA_Y; 
        }
        //colision balle J1
        if (((BALL_X-8) <= J1X) && (BALL_DELTA_X < 0) && (j1y < BALL_Y) && (BALL_Y < j1y+24))
        {
          BALL_DELTA_X = -BALL_DELTA_X;
          NR10_REG = 0x00;
          NR11_REG = 0x81;
          NR12_REG = 0x43;
          NR13_REG = 0x73;
          NR14_REG = 0x86;
        }

        //colision balle J2
        if (((BALL_X+8) >= J2X) && (BALL_DELTA_X > 0) && (j2y < BALL_Y) && (BALL_Y < j2y+24))
        {
          BALL_DELTA_X = -BALL_DELTA_X;
          NR10_REG = 0x00;
          NR11_REG = 0x81;
          NR12_REG = 0x43;
          NR13_REG = 0x73;
          NR14_REG = 0x86;
        }

        //colision bord ecran gauche
        if (BALL_X == 0)
        {
          delay (2000);
          BALL_X = 100;
          BALL_Y = 80;
          j1y=50;
          j2y=50;
          bouge_j1(j1y);
          bouge_j2(j2y);
          SCORE_J2++;
        }

        //colision bord ecran droit
        if (BALL_X == 168)
        {
          delay (2000);
          BALL_X = 60;
          BALL_Y = 80;
          j1y=50;
          j2y=50;
          bouge_j1(j1y);
          bouge_j2(j2y);
          SCORE_J1++;
        }

        //affichage score j2
        if (SCORE_J2 == 1)        
        {
          move_sprite(point_typej2, (160-20), 1+16);
        }
        if (SCORE_J2 == 2)        
        {
          move_sprite(point_typej2+1, (160-30), 1+16);
        }
        if (SCORE_J2 == 3)        
        {
          move_sprite(point_typej2+2, (160-40), 1+16);
        }
        if (SCORE_J2 == 4)        
        {
          move_sprite(point_typej2+3, (160-50), 1+16);
        }
        if (SCORE_J2 == 5)        
        {
          clean_screen();
           if (point_typej1 == SPRITE_VERRE)
          {
          printf("\n\n      %s\nBois tes 5 shooters\n", NOMJ1);
          }
          else if (point_typej1 == SPRITE_SLIP)
          {
          printf("\n\n   A poil %s\n", NOMJ1);
          }
          HIDE_SPRITES;
          break;
        }

         //affichage score j1
        if (SCORE_J1 == 1)        
        {
          move_sprite(point_typej1, (20), 1+16);
        }
        if (SCORE_J1 == 2)        
        {
          move_sprite(point_typej1+1, (30), 1+16);
        }
        if (SCORE_J1 == 3)        
        { 
          move_sprite(point_typej1+2, (40), 1+16);
        }
        if (SCORE_J1 == 4)        
        {
          move_sprite(point_typej1+3, (50), 1+16);
        }
        if (SCORE_J1 == 5)        
        {
          clean_screen();
          if (point_typej1 == SPRITE_VERRE)
          {
          printf("\n\n      %s\nBois tes 5 shooters\n", NOMJ2);
          }
          else if (point_typej1 == SPRITE_SLIP)
          {
          printf("\n\n   A poil %s\n", NOMJ2);
          }
          HIDE_SPRITES;
          break;
        }
        
		// Game main loop processing goes here


		// Done processing, yield CPU and wait for start of next frame
        vsync();
  }
  waitpad(J_START);
  clean_screen();
  SCORE_J1 = 0;
  SCORE_J2 = 0;
  goto restart;
}
