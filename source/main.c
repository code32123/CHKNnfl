/*
-------------------------------------------------

	NightFox's Lib Template
	Dibujado de imagenes de 16 bits

	Requiere DevkitARM
	Requiere NightFox's Lib

	Codigo por NightFox
	http://www.nightfoxandco.com
	Inicio 10 de Octubre del 2009

	(c)2009 - 2011 NightFox & Co.

-------------------------------------------------
*/

/*
-------------------------------------------------
	Includes
-------------------------------------------------
*/

// Includes C
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// Includes propietarios NDS
#include <nds.h>

// Includes librerias propias
#include <nf_lib.h>

enum rotations
{
	RotStraight = 0,
	RotRight = 1,
	RotLeft = 2
};
enum destinations
{
	Destinations_VRAM = 0,
	Destinations_BackBuffer = 1
};
enum screens
{
	Screen_Top = 0,
	Screen_Bottom = 1
};
enum sprites
{
	spr1,spr2,spr3,spr4,spr5,spr6,spr7,spr8,spr9
};

bool keyAReady = true;

/*
-------------------------------------------------
	Main() - Bloque general del programa
-------------------------------------------------
*/

int main(int argc, char **argv) {

	// Pantalla de espera inicializando NitroFS
	NF_Set2D(Screen_Top,    0);
	NF_Set2D(Screen_Bottom, 0);
	consoleDemoInit();
	iprintf("\n NitroFS init.\n\n");
	swiWaitForVBlank();

	// Define el ROOT e inicializa el sistema de archivos
	NF_SetRootFolder("NITROFS");	// Define la carpeta ROOT para usar NITROFS

	// Inicializa el motor 2D en modo BITMAP
	NF_Set2D(Screen_Top, 5);				// Modo 2D_5 en ambas pantallas
	NF_Set2D(Screen_Bottom, 5);

	/*NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(Screen_Top);
	NF_InitTextSys(Screen_Top);
	NF_LoadTextFont("fnt/default", "normal", 256, 256, RotStraight);
	NF_CreateTextLayer(Screen_Top, 0, RotStraight, "normal");*/
	NF_InitRawSoundBuffers();
	NF_LoadRawSound("raw/ZOOPSFX", 0, 22050, 1);
	NF_PlayRawSound(0, 120, 64, false, 0);

	// Inicializa los fondos en modo "BITMAP"
	NF_InitBitmapBgSys(Screen_Top, Destinations_BackBuffer);
	NF_InitBitmapBgSys(Screen_Bottom, Destinations_BackBuffer);

	// Inicializa los buffers para guardar fondos en formato BITMAP
	NF_Init16bitsBgBuffers();

	// Inicializa el backbuffer
	NF_Init16bitsBackBuffer(Screen_Top);
	NF_Init16bitsBackBuffer(Screen_Bottom);

	// Habilita el backbuffer
	NF_Enable16bitsBackBuffer(Screen_Top);
	NF_Enable16bitsBackBuffer(Screen_Bottom);

	// Carga el archivo BITMAP de imagen en formato RAW a la RAM
	NF_Load16bitsBg("bmp/bgT", spr1);
	NF_Load16bitsBg("bmp/bgB", spr2);
	NF_Load16bitsImage("bmp/EGG", spr4, 32, 32);

	// Tranfiere la imagen a la VRAM de ambas pantallas
	NF_Copy16bitsBuffer(Screen_Top,    Destinations_VRAM, spr1);
	NF_Copy16bitsBuffer(Screen_Bottom, Destinations_VRAM, spr2);

	NF_Copy16bitsBuffer(Screen_Top,    Destinations_BackBuffer, spr4);
	NF_Copy16bitsBuffer(Screen_Bottom, Destinations_BackBuffer, spr4);

	NF_InitSpriteBuffers();				// Inicializa los buffers para almacenar sprites y paletas
	NF_InitSpriteSys(Screen_Top);		// Inicializa los sprites para la pantalla superior
	NF_InitSpriteSys(Screen_Bottom);	// Inicializa los sprites para la pantalla inferior

	NF_LoadSpriteGfx("sprites/CHKN", spr3, 32, 32);
	NF_LoadSpritePal("sprites/CHKN", spr3);

	NF_VramSpriteGfx(Screen_Top,    spr3, spr3, false);	// Personaje, copia todos los frames a la VRAM
	NF_VramSpritePal(Screen_Top,    spr3, spr3);

	NF_VramSpriteGfx(Screen_Bottom, spr3, spr3, false);	// Personaje, copia todos los frames a la VRAM
	NF_VramSpritePal(Screen_Bottom, spr3, spr3);

	// Variables
	s16 x = 0;
	s16 y = 0;
	bool top = true;
	int eggs = 0;

	NF_Copy16bitsBuffer(Screen_Top,    Destinations_BackBuffer, spr1);
	NF_Copy16bitsBuffer(Screen_Bottom, Destinations_BackBuffer, spr2);
	
	NF_CreateSprite(Screen_Top,    spr3, spr3, spr3, (int)x, (int)y);
	NF_CreateSprite(Screen_Bottom, spr3, spr3, spr3, (int)x, (int)y);

	srand((unsigned int)time(NULL)); //seed random number generator with system time

	// Repite para siempre
	while (1) {
		scanKeys();
		int keys = keysHeld();
		if (keys & KEY_A) {
			if (keyAReady) {
				eggs++;
				NF_PlayRawSound(0, 120, 64, false, 0);
				s16 Nx = rand() % 191;
				y = rand() % 127;
				top = rand() & 1;
				// Flip Chicken:
				if (Nx > x) {
					if (top) {
						NF_HflipSprite(Screen_Top,    spr3, false);
					}
					else {
						NF_HflipSprite(Screen_Bottom, spr3, false);
					}
				}
				//
				x = Nx;
				// Draw Egg:
				if (top) {
					NF_Draw16bitsImage(Screen_Top, spr4, x, y, true);
				}
				else {
					NF_Draw16bitsImage(Screen_Bottom, spr4, x, y, true);
				}
				//
			}
			keyAReady = false;
		}
		else {
			keyAReady = true;
		}
		// Reset On Start:
		if (keys & KEY_START) {
			eggs = 0;
			NF_Copy16bitsBuffer(Screen_Top,    Destinations_BackBuffer, spr1);
			NF_Copy16bitsBuffer(Screen_Bottom, Destinations_BackBuffer, spr2);
		}
		//

		// Draw Chicken:
		NF_ShowSprite(Screen_Top,    spr3, top);
		NF_ShowSprite(Screen_Bottom, spr3, !top);
		NF_MoveSprite(Screen_Top,    spr3, x, y);
		NF_MoveSprite(Screen_Bottom, spr3, x, y);
		//

		NF_SpriteOamSet(Screen_Top);
		NF_SpriteOamSet(Screen_Bottom);

		//NF_WriteText(Screen_Top, 0, 0, 0, "Eggs: " + (char)eggs);

		swiWaitForVBlank();

		NF_Flip16bitsBackBuffer(Screen_Top);
		NF_Flip16bitsBackBuffer(Screen_Bottom);

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}


	return 0; 

}
