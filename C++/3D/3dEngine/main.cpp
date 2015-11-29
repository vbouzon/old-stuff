#include <iostream.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include <math.h>
// #include "src/tga.h"
#include "src/objet.h"
#include "src/write.h"
#include "src/map.h"
#include "src/font.h"
#include "src/texture.h"
#include "src/md2.h"

CMD2Model cyberpunk_md2;
CMD2Model weapon_md2;

CEntity cyberpunk;
CEntity weapon;

float angle[3] = {0, 0, 0};
float po[3] = {0, 0, 0};
static float temps = 0;
static float fps = 0;
SDL_Surface *screen;
int width = 0;
int height = 0;
static int warped = 0;
int up, down, left, right;
float vitesse = 2;
float map[257][257];
int arbr[50][4];
unsigned int base = 0;
short int combat = 0;
float timer, last_time = 0;
bool devant = false, derriere = false, droite = false, gauche = false;
// unsigned int texturebmp[10];

void affich2d(void)
{
	// calcul du frame rate
	static char tfps[3];
	int gg;
	CTextureManager *pTexMgr = CTextureManager::GetInstance();
	gg = pTexMgr->LoadTexture("data/images/font.tga");

	if (fps == 30)
	{
		fps = 30000.0 / ((float)SDL_GetTicks() - temps);
		cout << fps << endl;
		sprintf(tfps, "fps=%d", (int)fps);
		temps = (float)SDL_GetTicks();
		fps = 0;
	}
	fps++;
	// menu 2D
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1, 1, 1);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.1);
	glPrint(30, 38, tfps, 0, base, gg);
	char text[10];
	sprintf(text, "x=%.1f", po[0]);
	glPrint(15, 22, text, 0, base, gg);
	sprintf(text, "z=%.1f", po[1]);
	glPrint(15, 7, text, 0, base, gg);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void affichmap(void)
{
	CTextureManager *pTexMgr = CTextureManager::GetInstance();
	float cosrad, sinrad;
	cosrad = cos((angle[0]) * M_PI / 180);
	sinrad = sin((angle[0]) * M_PI / 180);

	short test[16][16];
	int jj = 0, ii = 0;
	for (ii = 0; ii <= 15; ii = ii++)
	{
		for (jj = 0; jj <= 15; jj = jj++)
		{
			test[ii][jj] = 0;
		}
	}

	glBindTexture(GL_TEXTURE_2D, pTexMgr->LoadTexture("data/images/herbe.tga"));
	glBegin(GL_TRIANGLES);
	for (float i = 0; i <= 110; i += 0.5)
	{
		for (float j = -(i * 0.001 + 0.65) * i; j <= (i * 0.001 + 0.65) * i + 1; j += 1)
		{
			jj = (int)(((-po[1] - (i - 2) * cosrad + j * sinrad) * 15) / 240);
			ii = (int)(((-po[0] + (i - 2) * sinrad + j * cosrad) * 15) / 240);
			if (!(ii < 0 || jj < 0 || ii > 15 || jj > 15))
			{
				if (test[ii][jj] == 0)
				{
					write_bloc((int)ii, (int)jj, map, (int)(pow(((int)(-po[1] / 16) - jj), 2) + pow(((int)(-po[0] / 16) - ii), 2)), test);
				}
			}
		}
	}
	write_bords(map, test);
	glEnd();
}

void afficharbr(void)
{

	CTextureManager *pTexMgr = CTextureManager::GetInstance();

	for (int k = 0; k < 50; k++)
	{
		if (arbr[k][3] == 1)
			arbre(arbr[k][0], map[arbr[k][0]][arbr[k][1]] / 25.5 - .1, arbr[k][1], arbr[k][2], pTexMgr->LoadTexture("data/images/arbre2.tga"));
		else if (arbr[k][3] == 2)
			arbre(arbr[k][0], map[arbr[k][0]][arbr[k][1]] / 25.5 - .1, arbr[k][1], pTexMgr->LoadTexture("data/images/branche.tga"));
		else
			arbre(arbr[k][0], map[arbr[k][0]][arbr[k][1]] / 25.5 - .1, arbr[k][1], arbr[k][2], pTexMgr->LoadTexture("data/images/arbre3.tga"));
	}
}

void display(float time)
{
	float cosrad, sinrad;
	cosrad = cos((angle[0]) * M_PI / 180);
	sinrad = sin((angle[0]) * M_PI / 180);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// calcul du deplacement
	float ya, yb, yc, yd, xt, zt;

	if (up)
	{
		po[0] = po[0] - vitesse * time * sinrad;
		po[1] = po[1] + vitesse * time * cosrad;
	}
	if (down)
	{
		po[0] = po[0] + vitesse * time * sinrad;
		po[1] = po[1] - vitesse * time * cosrad;
	}
	if (right)
	{
		po[0] = po[0] - vitesse * time * cosrad;
		po[1] = po[1] - vitesse * time * sinrad;
	}
	if (left)
	{
		po[0] = po[0] + vitesse * time * cosrad;
		po[1] = po[1] + vitesse * time * sinrad;
	}
	if (gauche)
		cyberpunk.RotateZ(-90 * time);
	if (droite)
		cyberpunk.RotateZ(90 * time);
	if (devant)
		cyberpunk.Translate(vitesse * time * cos(cyberpunk.GetAngleZ() * M_PI / 180), 0, vitesse * time * sin(cyberpunk.GetAngleZ() * M_PI / 180));
	if (derriere)
		cyberpunk.Translate(0, 0, -2 * time);
	xt = -po[0] + (float)(int)(po[0]);
	zt = -po[1] + (float)(int)(po[1]);
	ya = (-map[-(int)po[0]][-(int)po[1]]);
	yb = (-map[-(int)po[0] + 1][-(int)po[1]]);
	yc = (-map[-(int)po[0]][-(int)po[1] + 1]);
	yd = (-map[-(int)po[0] + 1][-(int)po[1] + 1]);
	if (xt <= zt)
	{
		po[2] = ((yb - ya) * xt + (yc - ya) * zt + ya) / 25.5 - 0.9;
	}
	else
	{
		po[2] = ((yc - yd) * (1 - xt) + (yb - yd) * (1 - zt) + yd) / 25.5 - 0.9;
	}
	glRotatef(angle[0], 0, 1.0, 0);
	glRotatef(cosrad * angle[1], 1.0, 0, 0);
	glRotatef(sinrad * angle[1], 0, 0, 1.0);
	glTranslatef(po[0], po[2] - 1, po[1]);

	// animation des modèles
	cyberpunk.Animate(0, 39, 10 * time);
	weapon.Animate(0, 39, 10 * time);

	cyberpunk.DrawEntity(-1, true, false);
	weapon.DrawEntity(-1, true, true);

	// affiche la carte
	affichmap();

	// affiche les arbres
	afficharbr();

	// affiche le menu
	affich2d();

	SDL_GL_SwapBuffers();
}

// Initialisation de l' Open GL
static int Initialise_OpenGl(int width, int height)
{
	CTextureManager *pTexMgr = CTextureManager::GetInstance();
	float ratio = (float)width / (float)height;
	GLfloat couleur_brouillard[4] = {0, 0.8, 1, 0};
	BuildFont(base, pTexMgr->LoadTexture("data/images/font.tga"));
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_ONE, GL_ONE);
	glAlphaFunc(GL_ALWAYS, 0);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glClearColor(0, .8, 1, 1);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPolygonMode(GL_FRONT, GL_FILL);
	gluPerspective(55.0, ratio, 0.1, 100.0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	SDL_ShowCursor(SDL_DISABLE);
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, couleur_brouillard);
	glFogf(GL_FOG_START, 95);
	glFogf(GL_FOG_END, 99);

	return 0;
}

// debut de gestion des evenements
// gestion du clavier appui touche
static void key_down(SDL_keysym *keysym)
{
	switch (keysym->sym)
	{
	case SDLK_RSHIFT:
		vitesse = 6;
		break;
	case SDLK_ESCAPE:
		exit(1);
		break;
	case SDLK_UP:
		up = 1;
		break;
	case SDLK_w:
		devant = true;
		break;
	case SDLK_s:
		derriere = true;
		break;
	case SDLK_a:
		gauche = true;
		break;
	case SDLK_d:
		droite = true;
		break;
	case SDLK_DOWN:
		down = 1;
		break;
	case SDLK_LEFT:
		left = 1;
		break;
	case SDLK_RIGHT:
		right = 1;
		break;
	default:
		break;
	}
}
// gestion du clavier relache touche
static void key_up(SDL_keysym *keysym)
{
	switch (keysym->sym)
	{
	case SDLK_RSHIFT:
		vitesse = 2;
		break;
	case SDLK_UP:
		up = 0;
		break;
	case SDLK_DOWN:
		down = 0;
		break;
	case SDLK_LEFT:
		left = 0;
		break;
	case SDLK_RIGHT:
		right = 0;
		break;
	case SDLK_w:
		devant = false;
		break;
	case SDLK_s:
		derriere = false;
		break;
	case SDLK_a:
		gauche = false;
		break;
	case SDLK_d:
		droite = false;
		break;
	default:
		break;
	}
}

// deplacement souris
static void mouse_motion(float x, float y)
{
	if (!warped && x > (width - 2))
	{
		warped = 1;
		SDL_WarpMouse(0, (int)y);
	}
	if (!warped && x < 1)
	{
		warped = 1;
		SDL_WarpMouse((width - 1), (int)y);
	}
	if (x <= (width - 2) && x >= 1)
	{
		warped = 0;
	}
	/*angle[0]=360*x/width;
	angle[1]=360*y/height;*/
	angle[0] = x * 360.0 / ((float)width);
	angle[1] = (y - 300) * 90 / ((float)height / 2);
}

// ecoute les evenement clavier et souris
static void process_events(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			key_down(&event.key.keysym);
			break;
		case SDL_KEYUP:
			key_up(&event.key.keysym);
			break;
		case SDL_MOUSEMOTION:
			mouse_motion(event.motion.x, event.motion.y);
			break;
		case SDL_QUIT:
			exit(1);
			break;
		}
	}
}

int Initialise_Ressources()
{
	make_map("data/sdl.map", 1, map);

	load_map("data/sdl.map", map);
	load_arbre("data/arbre.ras", arbr);

	CTextureManager *pTexMgr = CTextureManager::GetInstance();
	pTexMgr->Initialize();

	pTexMgr->LoadTexture("data/images/font.tga");
	pTexMgr->LoadTexture("data/images/herbe.tga");
	pTexMgr->LoadTexture("data/images/arbre3.tga");
	pTexMgr->LoadTexture("data/images/arbre2.tga");
	pTexMgr->LoadTexture("data/images/branche.tga");

	// Chargement des modèles
	cyberpunk_md2.LoadModel("models/tris.md2");
	cyberpunk_md2.LoadTexture("models/viciious.tga");
	weapon_md2.LoadModel("models/weapon.md2");
	weapon_md2.LoadTexture("models/weapon.tga");

	// Initialisation des entités

	cyberpunk.SetModel(&cyberpunk_md2);
	cyberpunk.SetScale(0.04f);

	weapon.SetModel(&weapon_md2);
	weapon.SetScale(0.04f);

	cyberpunk.SetChild(&weapon);

	cyberpunk.Translate(30, map[30][30] + 1.05, 30);

	return 0;
}

int Initialise_SDL()
{
	const SDL_VideoInfo *info = NULL;

	if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1))
	{
		cout << "Erreur d'initialisation d'un periferique SDL: " << SDL_GetError() << endl;
		SDL_Quit();
		return -1;
	}

	atexit(SDL_Quit);
	info = SDL_GetVideoInfo();

	if (!info)
	{
		cout << "Erreur video: " << SDL_GetError() << endl;
		SDL_Quit();
		return -1;
	}
	int bpp = info->vfmt->BitsPerPixel;
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	screen = SDL_SetVideoMode(width, height, bpp, SDL_OPENGL | SDL_RESIZABLE | SDL_FULLSCREEN);
	// Initialisation du mode video
	if (screen == 0)
	{
		cout << "Impossible d initialiser le mode " << width << "x" << height << " " << info->vfmt->BitsPerPixel << endl;
		SDL_Quit();
		return -1;
	}

	return 0;
}
int main(int argc, char *argv[])
{

	width = 1024;
	height = 768;

	// Initialisation et test de SDL
	cout << "Initialisation de SDL..." << endl;
	if (Initialise_SDL() == 0)
	{
		cout << "Reussie :)" << endl;
	}
	else
	{
		return -1;
	}

	cout << "Initialisation de Open Gl..." << endl;
	if (Initialise_OpenGl(width, height) == 0)
	{
		cout << "Reussie :)" << endl;
	}
	else
	{
		return -1;
	}

	// Initialisation des ressources.
	cout << "Initialisation des ressources..." << endl;
	if (Initialise_Ressources() == 0)
	{
		cout << "Reussie :)" << endl;
	}
	else
	{
		return -1;
	}

	while (1)
	{
		timer = (SDL_GetTicks() - last_time) / 1000;
		last_time = SDL_GetTicks();

		process_events();
		display(timer);
	}

	system("PAUSE");
	return 0;
}
