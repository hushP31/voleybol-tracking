#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.h"
#include "misObjetos.h"
#include "stdlib.h"

using namespace std;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Window_width,Window_height,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int UI_window_pos_x=50,UI_window_pos_y=50,UI_window_width=1080,UI_window_height=720;



float altura = 1.0;

void clear_window();
void change_projection();
void change_observer();
void draw_axis();
char seleccion = 1; // selector de las teclas que usamos para cambiar el modelo a pintar.
char antseleccion = 1;
char mode = 1;

//Objeto
vector<int> caras;
vector<int> aristas; 
vector<float> vertices; 

/*
// Tetraedro
vector<float> v_piramide; 
vector<int> c_piramide; 

//Cubo
vector<float> v_cubo; 
vector<int> c_cubo; 
*/

bool limite = true;

vector<float> v_basico1;
vector<int> c_basico1;

vector<float> v_basico2;
vector<int> c_basico2;


//*************************************************************************
//Lector de .PLY
//*************************************************************************

void leeply(char* nombreFichero, vector<float> &vertices, vector<int> &caras);
void terreno_juego(vector<float> &vertices, vector<int> &caras);
void red_juego(vector<float> &vertices, vector<int> &caras);
void campo_voleibol(vector<float> &tvertices, vector<int> &tcaras, vector<float> &rvertices, vector<int> &rcaras);
void nuevo_tetraedro(vector<float> &pvertices, vector<int> &pcaras, float altura);
void draw_points(vector<float> vertices);
void draw_aristas(vector<float> vertices, vector<int> aristas);
void draw_caras(vector<float> vertices, vector<int> caras);
void draw_solido(vector<float> vertices, vector<int> caras);
void draw_ajedrez(vector<float> vertices, vector<int> caras);
void draw_all(vector<float> vertices, vector<int> caras);
void draw_campo_juego_lines(vector<float> tvertices, vector<float> rvertices);
void draw_campo_juego_solid(vector<float> tvertices, vector<float> rvertices);

int modo=1;
int pinta=0; //0=puntos, 1=aristas, 2=solido, 3=ajedrez
bool actualiza = true;
int perfiles = 10;
int inicio_giro = 0;
int final_giro = 360;
bool cargado1 = false;

int inicio = 0;
int fin = 10;
void puntos_tracking_ball(vector <MiPunto4D> coordenadas, int inicio, int fin);
void draw_objects();
void draw_scene(void);
void change_window_size(int Ancho1,int Alto1);
void normal_keys(unsigned char Tecla1,int x,int y);
void special_keys(int Tecla1,int x,int y);
void initialize(void);
