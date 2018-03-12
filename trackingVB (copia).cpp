/*
	Programa de estudio del comportamiento del balón en 
	un entrenamiento de voleybol usando técnicas de tracking
	mediante la biblioteca OpenCV.
*/
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include "archivos/objeto.h"
#include "stdlib.h"

#include <ctype.h>
#include "archivos/vertex.h"
#include <cmath>
#include "archivos/file_ply_stl.h"

#include <unistd.h>

using namespace std;
using namespace cv;

/*********** Vars Globals ************/

vector<MiPunto> puntos, puntosL;
vector <Recta> pgroundF, pgroundL;
vector <MiPunto> puntos_fugaL, puntos_fugaF;
MiPunto fuga_left_inicio, fuga_left_final;
MiPunto fuga_left_inicioL, fuga_left_finalL;

vector <MiPunto4D> coordenadas_balon;
vector <MiPunto4D> coordenadas_balon_final;
/****************************/

bool imprime_linea_puntos = true;



/******************** - OPENGL - **********************/

// tamaño de los ejes
//const int AXIS_SIZE=5000;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Window_width,Window_height,Front_plane,Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int UI_window_pos_x=50,UI_window_pos_y=50,UI_window_width=1080,UI_window_height=720;



float altura = 1.0;
//**************************************************************************
//
//***************************************************************************

void clear_window()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}


//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// formato(x_minimo,x_maximo, y_minimo, y_maximo,Front_plane, plano_traser)
	//  Front_plane>0  Back_plane>PlanoDelantero)
	glFrustum(-Window_width,Window_width,-Window_height,Window_height,Front_plane,Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{
	// posicion del observador
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-Observer_distance-4.5);
	glRotatef(Observer_angle_x,1,0,0);
	glRotatef(Observer_angle_y,0,1,0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{	
	glPushMatrix();
	glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
			glBegin(GL_LINES);
				// eje X, color rojo
				glColor3f(1,0,0);
				glVertex3f(-AXIS_SIZE,0,0);
				glVertex3f(AXIS_SIZE,0,0);
				// eje Y, color verde
				glColor3f(0,1,0);
				glVertex3f(0,-AXIS_SIZE,0);
				glVertex3f(0,AXIS_SIZE,0);
				// eje Z, color azul
				glColor3f(0,0,1);
				glVertex3f(0,0,-AXIS_SIZE);
				glVertex3f(0,0,AXIS_SIZE);
			glEnd();
	glPopAttrib();
    glPopMatrix();
}
//**************************************************************************
//  Creacion de los  vectores necesarios para la creacion de un modelo
//**************************************************************************

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

void leeply(char* nombreFichero, vector<float> &vertices, vector<int> &caras){
	
	_file_ply *lector;
	lector = new _file_ply();
	lector->open(nombreFichero);
	lector->read(vertices, caras);

}


//  Fin
//**************************************************************************

//************************************************************************
//Funciones que crean los vertices y caras de revolucion
//************************************************************************




//**************************************************************************
// Funcion que dibuja los objetos
//**************************************************************************

int modo=1;
int pinta=0; //0=puntos, 1=aristas, 2=solido, 3=ajedrez
bool actualiza = true;
int perfiles = 10;
int inicio_giro = 0;
int final_giro = 360;
bool cargado1 = false;

int inicio = 0;
int fin = 10;


bool aux_pause = false;
bool rejilla_draw = true;
int n_eje = 4;


void draw_objects(){

	puntos_tracking_ball(coordenadas_balon, inicio, fin);
    
	if(seleccion == 6){
		imprime_linea_puntos = !imprime_linea_puntos;
		seleccion = 7;
	}
	

    if(imprime_linea_puntos){
	    inicio++;
	    fin++;
	}

	if(seleccion==11){
		n_eje = 4;
		seleccion = 7;
		rejilla_draw = !rejilla_draw;
	}
	if(seleccion==12){
		n_eje = 1;
		seleccion = 7;
		rejilla_draw = !rejilla_draw;
	}
	if(seleccion==13){
		n_eje = 2;
		seleccion = 7;
		rejilla_draw = !rejilla_draw;
	}
	if(seleccion==14){
		n_eje = 3;
		seleccion = 7;
		rejilla_draw = !rejilla_draw;
	}

	if(rejilla_draw){
		glPushMatrix();
	    glPushAttrib( GL_LIGHTING_BIT );
		    glDisable( GL_LIGHTING );
					Rejilla(n_eje);
			glPopAttrib();
	    glPopMatrix();
	}

	glutPostRedisplay();
}

//***************************************************************************
//
//***************************************************************************


void draw_scene(void)
{

	clear_window();
	change_observer();
	draw_axis();

	if(!cargado1){
		campo_voleibol(v_basico1, c_basico1, v_basico2, c_basico2);
		cargado1 = true;
	}

	//Defino un material Rojo
    GLfloat mat_ambient_toro[] = {0.05, 0.05, 0.05, 1.0f}; 
    GLfloat mat_diffuse_toro[] = { 0.8, 0.0, 0.0, 1.0f};
    GLfloat mat_specular_toro[] = {0.9, 0.8, 0.8, 1.0f};
 
    //Defino un material Azul
    GLfloat mat_ambient_cubo[] = {0.1, 0.1, 0.1, 1.0f}; 
    GLfloat mat_diffuse_cubo[] = {0.0, 0.0, 0.8, 1.0f};
    GLfloat mat_specular_cubo[] = {0.9, 0.9, 0.9, 1.0f};
   
    //Defino un material Verde
    GLfloat mat_ambient_esfera[] = {0.1, 0.1, 0.1, 1.0f}; 
    GLfloat mat_diffuse_esfera[] = {0.0, 0.7, 0.0, 1.0f};
    GLfloat mat_specular_esfera[] = {0.8, 0.8, 0.8, 1.0f};

    glPushMatrix();
    glPushAttrib( GL_LIGHTING_BIT );
	    glDisable( GL_LIGHTING );
				draw_campo_juego_lines(v_basico1, v_basico2);
				draw_campo_juego_solid(v_basico1, v_basico2);
		glPopAttrib();
    glPopMatrix();

	draw_objects();
	
	glutSwapBuffers();
}



//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1,int Alto1)
{
	change_projection();
	glViewport(0,0,Ancho1,Alto1);
	glutPostRedisplay();
}


//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_keys(unsigned char Tecla1,int x,int y)
{
	
	switch (toupper(Tecla1)) {
			case '1': mode = 1; break; // Elige cilindro abierto
			
			case 'P': seleccion = 6; 

			break; // Pinta puntos
			case 'A': seleccion = 7; break; // Pinta aristas
			case 'S': seleccion = 8; break; // Pinta solido
			case 'C': seleccion = 9; break; // Pinta solido ajedrez
			case 'L': seleccion = 10; break; // Pinta todo
			case 'R': seleccion = 11; break; // Pinta todo
			case 'X': seleccion = 13; break; // Pinta todo
			case 'Y': seleccion = 14; break; // Pinta todo
			case 'Z': seleccion = 12; break; // Pinta todo
	
	}

	if (toupper(Tecla1)=='Q') exit(0);
}

//***************************************************************************
// Funcion llamada cuando se produce aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_keys(int Tecla1,int x,int y)
{

switch (Tecla1){
		case GLUT_KEY_LEFT:
			Observer_angle_y--;
		break;
		case GLUT_KEY_RIGHT:Observer_angle_y++;break;
		case GLUT_KEY_UP:
			if(Observer_angle_x > 0)
				Observer_angle_x--;
		break;
		case GLUT_KEY_DOWN:
			if(Observer_angle_x < 180)
				Observer_angle_x++;
		break;
		case GLUT_KEY_PAGE_UP:Observer_distance*=1.1;break;
		case GLUT_KEY_PAGE_DOWN:Observer_distance/=1.1;break;
	}
	
	glutPostRedisplay();
}



//***************************************************************************
// Funcion de incializacion
//***************************************************************************

void initialize(void)
{
	// se inicalizan la ventana y los planos de corte
	Window_width=.5;
	Window_height=.5;
	Front_plane=10;
	Back_plane=1000;

	// se inicia la posicion del observador, en el eje z
	Observer_distance=3*Front_plane;
	Observer_angle_x=0;
	Observer_angle_y=0;

	// se indica cual sera el color para limpiar la ventana	(r,v,a,al)
	// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
	glClearColor(1,1,1,1);

	GLfloat light_ambient[] = { 0.75, 0.75, 0.75, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { 0.0, 0.0, 1.0, 0.0 };

    glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv (GL_LIGHT0, GL_POSITION, light_position);
    
    glEnable (GL_LIGHTING);
    glEnable (GL_LIGHT0);
    glDepthFunc(GL_LESS);

	// se habilita el z-bufer
	glEnable(GL_DEPTH_TEST);
	//
	change_projection();
	//
	glViewport(0,0,UI_window_width,UI_window_height);
}


//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************



/*******************************************/

/*******************************************/


/****************************************************************/
void PlaygroundHorizontal(int event, int x, int y, int flags, void* userdata){

	if(event == EVENT_LBUTTONDOWN){		
		MiPunto p;
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		p.x = x;
		p.y = y;
		puntos.push_back(p);
	}
	else if(event == EVENT_RBUTTONDOWN){
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if(event == EVENT_MBUTTONDOWN){
		cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if(event == EVENT_MOUSEMOVE){
		//cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
	}
}

void PlaygroundLateral(int event, int x, int y, int flags, void* userdata){	

	if(event == EVENT_LBUTTONDOWN){		
		MiPunto p;
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		
		p.x = x;
		p.y = y;
		puntosL.push_back(p);
	}
	else if(event == EVENT_RBUTTONDOWN){
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if(event == EVENT_MBUTTONDOWN){
		cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if(event == EVENT_MOUSEMOVE){
		//cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
	}
}

/****************************************************************/



//Sensibilidad para la funcion absdiff

//Valor del desenfoque para suavizar la salida de la imagen en absdiff()
const static int BLUR_SIZE = 5;
//EL objeto a buscar (el balon) y mantener su posicion de recorrido (tracking)
double theObject[2] = {0,0};
//Rectangulo del objeto. Se usa su centro como posicion
Rect objectBoundingRectangle = Rect(0,0,0,0);
Rect objectBoundingRectangleAUX = Rect(0,0,0,0);
 

MiPunto searchForMovement(Mat thresholdImage, Mat &cameraFeed, char camara, MiPunto p_frontal, MiPunto p_lateral){
    //notice how we use the '&' operator for objectDetected and cameraFeed. This is because we wish
    //to take the values passed into the function and manipulate them, rather than just working with a copy.
    //eg. we draw to the cameraFeed to be displayed in the main() function.
    bool objectDetected = false;
    Mat temp, temp_frame;
    thresholdImage.copyTo(temp);
    cameraFeed.copyTo(temp_frame);

    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<int> n_whites_points; //Contar los puntos blancos de un contorno.
    int cont_white = 0;
    MiPunto point_sol;
    vector<int> indices_hierarchy;
    vector<float> cont_avgs_1, cont_avgs_2;
    vector< vector<Point> > largestContourVec;
    vector< vector<Point> > contours_balls, contours_balls_2;
   	int indice = 0;
   	int minimo = 90; 
   	int maximo = 1500;
   	MiPunto p_anterior = p_frontal;
   	std::vector<int> v_index_color;

   	if(camara == 'L'){
   		minimo = 60;
   		maximo = 800;
   		p_anterior = p_lateral;
   	}


    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
    
    Mat labels = cv::Mat::zeros(temp.size(), CV_8UC1);     
	vector<float> cont_avgs(contours.size(), 0.f); // This contains the averages of each contour

    
    for(int idx = 0; idx >= 0; idx = hierarchy[idx][0]){
    	Scalar color( 0, 0, 0);
        if(contourArea(contours[idx]) < maximo && contourArea(contours[idx]) > minimo){
        	drawContours( cameraFeed, contours, idx, color, CV_FILLED, 100, hierarchy );

	        cv::drawContours(labels, contours, -1, cv::Scalar(255), CV_FILLED);
	        for (size_t i = 0; i < contours.size(); ++i)
			{
			    cv::Rect roi = cv::boundingRect(contours[i]);
			    cv::Scalar mean = cv::mean(cameraFeed(roi), labels(roi));
			    cont_avgs[i] = mean[0];
			    //cout << cont_avgs[i] <<" - color " << endl; 
			}
		}
    }
    

	/*******************************************************************************
		Controlar si ese objeto es de tal color para saber que se trata de un balon	
	********************************************************************************/

    //the largest contour is found at the end of the contours vector
    //we will simply assume that the biggest contour is the object we are looking for.
    

   	/***********************************************************
   		Ordeno de menor a mayor contours_balls
   	************************************************************/
   	vector <Point> aux;

   	for(int i=0; i<contours.size(); i++){
   		if((contourArea(contours[i]) >= minimo) && (contourArea(contours[i]) <= maximo)){ // && (hierarchy[i][3] == -1)){
   			contours_balls.push_back(contours[i]);
   			cont_avgs_1.push_back(cont_avgs[i]);
   		}
	}
	/****
		Descartar lo que no pueda ser un balón. Formas muy rectangulares
   		
			Busco:
				- Punto mas a la derecha
				- Punto mas a la izda
				- Punto mas alto
				- Punto mas bajo

			Si la relacion altura-anchura es menor al 30%, lo descarto.
   	****/

	vector<Point> v_aux; // 0->, 1->izquierda, 2->arriba, 3->abajo
	double dist_x, dist_y, rel_x, rel_y;


	for(int i=0; i<contours_balls.size(); i++){
		v_aux.push_back(contours_balls[i][0]);
		v_aux.push_back(contours_balls[i][0]);
		v_aux.push_back(contours_balls[i][0]);
		v_aux.push_back(contours_balls[i][0]);

		for(int j=0; j<contours_balls[i].size(); j++){
			if(v_aux[0].x < contours_balls[i][j].x)
				v_aux[0] = contours_balls[i][j];
			if(v_aux[1].x > contours_balls[i][j].x)
				v_aux[1] = contours_balls[i][j];
			if(v_aux[2].y < contours_balls[i][j].y)
				v_aux[2] = contours_balls[i][j];
			if(v_aux[3].y > contours_balls[i][j].y)
				v_aux[3] = contours_balls[i][j];
		}

		dist_x = v_aux[0].x - v_aux[1].x;
		dist_y = v_aux[2].y - v_aux[3].y;

		rel_x = dist_x / dist_y;
		rel_y = dist_y / dist_x;

		Point centro1, centro2;
		Vec3b intensity =  (0,0,0);
		Vec3b intensity2 =  (200,200,200);
		
		centro1.x = (int)(v_aux[0].x + v_aux[1].x)/2;
		centro1.y = (int)(v_aux[0].y + v_aux[1].y)/2;
		centro2.x = (int)(v_aux[2].x + v_aux[3].x)/2;
		centro2.y = (int)(v_aux[2].y + v_aux[3].y)/2;

        objectBoundingRectangleAUX = boundingRect(contours_balls.at(i));
        double xpos1 = objectBoundingRectangleAUX.x+objectBoundingRectangleAUX.width/2.0;
        double ypos1 = objectBoundingRectangleAUX.y+objectBoundingRectangleAUX.height/2.0;
        
        //rel_x = objectBoundingRectangleAUX.width / objectBoundingRectangleAUX.height;
        //rel_y = objectBoundingRectangleAUX.height / objectBoundingRectangleAUX.width;

		if(rel_x > 0.30 && rel_y > 0.30){
        	contours_balls_2.push_back(contours_balls[i]);
        	cont_avgs_2.push_back(cont_avgs_1[i]);
		}

		v_aux.clear();
	}
	/*
	for(int i=0; i<contours_balls.size(); i++){
		contours_balls_2.push_back(contours_balls[i]);
	    cont_avgs_2.push_back(cont_avgs_1[i]);
    }
	//cout << "Posibles contornos: " << contours_balls_2.size() << endl;
   	*/

   	if(contours_balls_2.size() != 0)
     	for(int i=0; i< contours_balls_2.size(); i++)
        	largestContourVec.push_back(contours_balls_2.at(i));
	else
		cout << "contours_balls vacio" << endl;

    if(contours_balls_2.size()>0){
    	for(int i=0; i< cont_avgs_2.size(); i++) // Color (ball) is less than 1.
    		if(1 > cont_avgs_2[i])
    			v_index_color.push_back(i);
    	
    	if(!v_index_color.empty())
			objectDetected = true;
		else
			objectDetected = false;
    }
    else 
    	objectDetected = false;
 
    if(objectDetected){
		//make a bounding rectangle around the largest contour then find its centroid
		//this will be the object's final estimated position.


    	//De todos los contornos cuyo Average_color < 1, calcular el de menor distancia al anterior (p_frontal, p_lateral)
    	MiPunto p_actual_aux, p_actual;
    	int indice_ball;

    	objectBoundingRectangle = boundingRect(largestContourVec.at(v_index_color.at(v_index_color.size()-1)));
		p_actual.x = objectBoundingRectangle.x + objectBoundingRectangle.width/2.0;
		p_actual.y = objectBoundingRectangle.y + objectBoundingRectangle.height/2.0;

		if(p_anterior.x == -1){
			p_anterior.x = p_actual.x;
			p_anterior.y = p_actual.y;
		}

    	float distancia_solucion = DistanciaPuntos(p_anterior, p_actual);

    	for(int k=0; k<v_index_color.size(); k++){

			objectBoundingRectangle = boundingRect(largestContourVec.at(v_index_color.at(k)));
			p_actual_aux.x = objectBoundingRectangle.x + objectBoundingRectangle.width/2.0;
			p_actual_aux.y = objectBoundingRectangle.y + objectBoundingRectangle.height/2.0;

			float distancia_p = DistanciaPuntos(p_anterior, p_actual);

			if(distancia_p < distancia_solucion){
				indice_ball = k;
				distancia_solucion = distancia_p;
				p_actual.x = p_actual_aux.x;
				p_actual.y = p_actual_aux.y;

				cout << "\nCOLOR BALON : " << cont_avgs_2.at(k) << camara << endl;
			}

		}
		//update the objects positions by changing the 'theObject' array values
		point_sol.x = p_actual.x;
	    point_sol.y = p_actual.y;
    }
    else{
	    point_sol.x = -1;
	    point_sol.y = -1;
    }
    
    //make some temp x and y variables so we dont have to type out so much
    return point_sol;
}




int main(int argc, char* argv[]) {

	string video_trackingF = "../videos/video/fondo720.mp4";
	string video_trackingL = "../videos/video/lateral720.mp4";
	string nombre_archivo = "Femenino.txt";

	int contador_frame, num_frame_segundo;
	int ayuda = 0;

	char F = 'F';
	char L = 'L';

	double altura_balon;
	double tiempo_frame;
	double d_lateral_fondo;
	double dieciocho_metros;
	double distancia_extrapolada_left;
	double distancia_extrapolada_right;

	bool exit_program = false;
	bool una_iteracion = false;
	bool salir_tracking = true;
	bool imprime = true;

	MiPunto point_lateral_extrapolado_1, point_lateral_extrapolado_2;
	MiPunto point_frontal_extrapolado_1, point_frontal_extrapolado_2;
	MiPunto point, pointL, suelo, point_aux, pointL_aux;

	MiPunto3D coordenada_balon;
	
	Recta perp_pgroundF, perp_pgroundL;
	
	point.x = -1;
	point.y = -1;
	pointL.x = -1;
	pointL.y = -1;

	point_aux.x = -1;
	point_aux.y = -1;
	pointL_aux.x = -1;
	pointL_aux.y = -1;


	//the two frames we will be comparing
    Mat frame1,frame2,frame3,thresholdImage;
    //their grayscale images (needed for absdiff() function)
    //video capture object.
    VideoCapture capture;
    Mat frame1L,frame2L,frame3L, thresholdImageL;
    VideoCapture captureL;

    //some boolean variables for added functionality
    bool objectDetected = false;
    //these two can be toggled by pressing 'd' or 't'
    bool debugMode = false;
    bool trackingEnabled = false;
    //pause and resume code
    bool pause = false;
    //set up the matrices that we will need
    bool extender = false;

    contador_frame = 0;
	num_frame_segundo = 24;

    if (argc < 4 ){
        video_trackingF = "../videos/videos_sinc/fondo.mp4";
        video_trackingL = "../videos/videos_sinc/lateral.mp4";

        if(argc == 2)
	        if(atoi(argv[1]) == 1)
	        	extender = true;
    }
    else{
    	if(atoi(argv[1]) == 1)
        	extender = true;

        video_trackingF = argv[2];
        video_trackingL = argv[3];
    }

    limite = true;
	
    //capture.open("Lateral10min.mp4");
    capture.open(video_trackingF);

    if(!capture.isOpened()){
        cout<<"ERROR ACQUIRING VIDEO FEED (F)\n";
        getchar();
        return -1;
    }
    
    //capture.open("Lateral10min.mp4");
    captureL.open(video_trackingL);

    if(!captureL.isOpened()){
        cout<<"ERROR ACQUIRING VIDEO FEED (L)\n";
        getchar();
        return -1;
    }
    

    //check if the video has reach its last frame.
    //we add '-1' because we are reading two frames from the video at a time.
    //if this is not included, we get a memory error!
    while(((capture.get(CV_CAP_PROP_POS_FRAMES) < capture.get(CV_CAP_PROP_FRAME_COUNT)-1) || 
    			     (captureL.get(CV_CAP_PROP_POS_FRAMES) < captureL.get(CV_CAP_PROP_FRAME_COUNT)-1)) && salir_tracking)
    {
	  	//Calcular el tiempo exacto del frame a analizar.
	  	contador_frame++;
	  	tiempo_frame = contador_frame / (num_frame_segundo*1.0);

	  	if(una_iteracion)
	  		pause = true;

	  	while (pause == true){
            //stay in this loop until 
            switch (waitKey()){
                //a switch statement inside a switch statement? Mind blown.
		        case 32: 
		            //change pause back to false
		            pause = false;
		            una_iteracion = false;
		            cout << "Code Resumed" << endl;
	            break;
	            case 49:
	            	una_iteracion = true;
	            	pause = false;
	            break;
		        case 27: //'esc' key has been pressed, exit program.
				      salir_tracking = false;
		        break;
			}
		}

	  	capture.read(frame1);
    	capture.read(frame2);
    	capture.read(frame3);

    	captureL.read(frame1L);
    	captureL.read(frame2L);
    	captureL.read(frame3L);
	    
    	//cout << "Total frames: " << indice_auxuliar_frames << "\ncapture_frames: " << capturador_frames.size() << endl;

        //Frontal -->
        thresholdImage = ConvertImageThreshold(frame1, frame2, debugMode);
        // -----------
        //Lateral -->
        thresholdImageL = ConvertImageThreshold(frame1L, frame2L, debugMode);
        // -----------
        
        //if tracking enabled, search for contours in our thresholded image
        if(trackingEnabled){

            point = searchForMovement(thresholdImage,frame1, F, point_aux, pointL_aux);
            pointL = searchForMovement(thresholdImageL,frame1L, L, point_aux, pointL_aux);

            if(point.x > -1 && pointL.x > -1){
            	point_aux = point;
            	pointL_aux = pointL;
            }

            if(!imprime && point.x > 0 && pointL.x > 0){

            	//Transformar puntos
            	//1. Perpendicular a pground[3] que pasa por point
            	perp_pgroundF = PerpendicularPunto(pgroundF[3], point);
            	perp_pgroundL = PerpendicularPunto(pgroundL[3], pointL);

            	//2. Puntos de corte entre perp_pground y bases del pground.
            	vector <MiPunto> corte_basesF, corte_basesL;

            	for(int i=0; i<2; i++){
            		corte_basesF.push_back(PuntoCorte(perp_pgroundF, pgroundF[i*2+1]));
            		corte_basesL.push_back(PuntoCorte(perp_pgroundL, pgroundL[i*2+1]));
            	}

            	//3. Calculo de distancias proporcionales
            	
            	//Lateral
            	d_lateral_fondo = DistanciaPuntos(puntosL[1], corte_basesL[0]);
            	dieciocho_metros = DistanciaPuntos(puntosL[1], puntosL[2]);
            	distancia_extrapolada_left = d_lateral_fondo*18.0/dieciocho_metros;

            	point_lateral_extrapolado_1.y = Trunca(2, distancia_extrapolada_left);
            	point_lateral_extrapolado_1.x = 0.0;


            	d_lateral_fondo = DistanciaPuntos(puntosL[0], corte_basesL[1]);
            	dieciocho_metros = DistanciaPuntos(puntosL[0], puntosL[3]);
            	distancia_extrapolada_right = d_lateral_fondo*18.0/dieciocho_metros;

            	point_lateral_extrapolado_2.y = Trunca(2, distancia_extrapolada_right);
            	point_lateral_extrapolado_2.x = 9.0;


            	//Frontal
            	d_lateral_fondo = DistanciaPuntos(puntos[1], corte_basesF[0]);
            	dieciocho_metros = DistanciaPuntos(puntos[1], puntos[2]);
            	distancia_extrapolada_left = d_lateral_fondo*9.0/dieciocho_metros;

            	point_frontal_extrapolado_1.x = Trunca(2, distancia_extrapolada_left);
            	point_frontal_extrapolado_1.y = 18.0;


            	d_lateral_fondo = DistanciaPuntos(puntos[0], corte_basesF[1]);
            	dieciocho_metros = DistanciaPuntos(puntos[0], puntos[3]);
            	distancia_extrapolada_right = d_lateral_fondo*9.0/dieciocho_metros;

            	point_frontal_extrapolado_2.x = Trunca(2, distancia_extrapolada_right) ;
            	point_frontal_extrapolado_2.y = 0.0;

            	Recta frntl = RectaDosMiPunto(point_frontal_extrapolado_1, point_frontal_extrapolado_2);
            	Recta ltrl = RectaDosMiPunto(point_lateral_extrapolado_1, point_lateral_extrapolado_2);

            	suelo = PuntoCorte(frntl, ltrl);

            	suelo.x = Trunca(2, suelo.x);
            	suelo.y = Trunca(2, suelo.y);


            	altura_balon = Altura(puntos, point, suelo);

            	coordenada_balon.x = suelo.x;
            	coordenada_balon.y = abs(altura_balon); 
            	coordenada_balon.z = suelo.y;

            	/************************************
            	/*
            	/*	Usar dos vectores, uno a modo de solucion y otro auxiliar.

        			Solucion: 
            	/*
            	/*
            	/************************************/


				
				DrawPlayGround(frame1, puntos);
				DrawPlayGround(frame1L, puntosL);

                DrawObject(frame1, point, coordenada_balon);
                DrawObject(frame1L, pointL, coordenada_balon);
                
                if(coordenada_balon.y >= 0 ){
                	MiPunto4D p(tiempo_frame, coordenada_balon);
              		coordenadas_balon.push_back(p);
                }
				
                line(frame1L,Point((int)corte_basesL[0].x, (int)corte_basesL[0].y), 
                								Point((int)corte_basesL[1].x,(int)corte_basesL[1].y),Scalar(0,255,0),1);
                line(frame1, Point((int)corte_basesF[0].x, (int)corte_basesF[0].y), 
                								Point((int)corte_basesF[1].x,(int)corte_basesF[1].y),Scalar(0,255,0),1);
            }
        }
			
		namedWindow("Horizontal", WINDOW_NORMAL);
        namedWindow("Lateral", WINDOW_NORMAL);
   		
        resizeWindow("Horizontal", 1080, 720);
        resizeWindow("Lateral", 1080, 720);
   		
		//set the callback function for any mouse event
		setMouseCallback("Horizontal", PlaygroundHorizontal, NULL);
		setMouseCallback("Lateral", PlaygroundLateral, NULL);
   			
        //show our captured frame
        imshow("Horizontal",frame1);
        imshow("Lateral",frame1L);
		


        switch(waitKey(10)){
		        case 27: //'esc' key has been pressed, exit program.
		        	exit_program = true;
		            salir_tracking = false;
	        break;
		        case 116: //'t' has been pressed. this will toggle tracking
		            trackingEnabled = !trackingEnabled;
		            if(trackingEnabled == false) cout << "Tracking disabled." << endl;
		            else cout << "Tracking enabled." << endl;
	            break;
		        case 100: //'d' has been pressed. this will debug mode
		            debugMode = !debugMode;
		            if(debugMode == false) cout << "Debug mode disabled." << endl;
		            else cout << "Debug mode enabled." << endl;
	            break;
	            case 49:
		        una_iteracion = true;
	        break;
		        case 32: //'space' has been pressed. this will pause/resume the code.
		            pause = !pause;
		            if(pause == true){ 
	                	cout << "Code paused, press 'p' again to resume" << endl;
				        while (pause == true){
				            //stay in this loop until 
				            switch (waitKey()){
				                //a switch statement inside a switch statement? Mind blown.
						        case 116: //'t' has been pressed. this will toggle tracking
				  		            trackingEnabled = !trackingEnabled;
				  		            if(trackingEnabled == false)
				  		            	cout << "Tracking disabled." << endl;
				  		            else
				  		            	cout << "Tracking enabled." << endl;
			  		            break;
						        case 32: 
						            //change pause back to false
						            pause = false;
						            una_iteracion = false;
						            cout << "Code Resumed" << endl;
					            break;
					            case 49:
					            	una_iteracion = true;
					            	pause = false;
					            break;
						        case 27: //'esc' key has been pressed, exit program.
								    salir_tracking = false;
		        				break;
							}
						}
		            }
            break;
        }
    	
        //Inicializar campo de juego
	    puntos.push_back(MiPunto(206, 843));
	    puntos.push_back(MiPunto(1617, 873));
	    puntos.push_back(MiPunto(1166, 666));
	    puntos.push_back(MiPunto(679, 657));

	    puntosL.push_back(MiPunto(170, 820));
	    puntosL.push_back(MiPunto(344, 689));
	    puntosL.push_back(MiPunto(1829, 751));
	    puntosL.push_back(MiPunto(1475, 648));

        
        if((puntos.size() == 4) && (puntosL.size() == 4) && imprime){
          	//Ordenar puntos de x0 <= x1 <= x2 <= x3
			OrdenaPuntos(puntos);
			OrdenaPuntos(puntosL);

			if(extender)
				ExtensionLateral(puntosL);

			//Limites de los campos de juego
			for(int t=0; t<puntos.size(); t++){
				pgroundF.push_back(RectaDosPuntos(puntos[t], puntos[(t+1)%4]));
				pgroundL.push_back(RectaDosPuntos(puntosL[t], puntosL[(t+1)%4]));
			}
		
			//Puntos de fuga Frontal Y Lateral
			puntos_fugaF.push_back(PuntoCorte(pgroundF[0], pgroundF[2]));
			puntos_fugaL.push_back(PuntoCorte(pgroundL[0], pgroundL[2]));

			Recta r_aux = ParalelaPunto(pgroundF[1], puntos_fugaF[0]);
			Recta r_aux_2 = ParalelaPunto(pgroundL[1], puntos_fugaL[0]);

			puntos_fugaF.push_back(PuntoCorte(r_aux, RectaDosPuntos(puntos[1], puntos[3])));
			puntos_fugaF.push_back(PuntoCorte(r_aux, RectaDosPuntos(puntos[0], puntos[2])));

			puntos_fugaL.push_back(PuntoCorte(r_aux_2, RectaDosPuntos(puntosL[1], puntosL[3])));
			puntos_fugaL.push_back(PuntoCorte(r_aux_2, RectaDosPuntos(puntosL[0], puntosL[2])));

			//Rectas auxiliares usando los puntos de fuga
			Recta aux_fuga_left = RectaDosPuntos(puntos_fugaF[1], puntos[0]);

			fuga_left_inicio = PuntoCorte(aux_fuga_left, pgroundF[1]);
			fuga_left_final = puntos[1];


			aux_fuga_left = RectaDosPuntos(puntos_fugaL[1], puntosL[0]);
			fuga_left_inicioL = PuntoCorte(aux_fuga_left, pgroundF[1]);
			fuga_left_finalL = puntosL[1];

			imprime = false;
        }
	     	
	}
    capture.release();
    captureL.release();

/*
    //Elimino puntos imposibles x+-5, z+-5
    for(int i=0; i<coordenadas_balon.size(); i++){
    	if((coordenadas_balon[i].coordenada.x > 14.0) || (coordenadas_balon[i].coordenada.x < -5.0)){
    		cout << "erase x ";
    		coordenadas_balon.erase(coordenadas_balon.begin()+i); //Eliminar elemento
    	}
		if((coordenadas_balon[i].coordenada.z > 23.0) || (coordenadas_balon[i].coordenada.z < -5.0)){
			cout << "erase z ";
    		coordenadas_balon.erase(coordenadas_balon.begin()+i); //Eliminar elemento
		}
    	if((coordenadas_balon[i].coordenada.y > 15.0) || (coordenadas_balon[i].coordenada.y < 0)){
    		cout << "erase y ";
    		coordenadas_balon.erase(coordenadas_balon.begin()+i); //Eliminar elemento
    	}
    }

    //release the capture before re-opening and looping again.
    int final_for = coordenadas_balon.size()-2;
	for(int i=1; i < final_for; i++){
		//cout << "i: " << i << "/" << coordenadas_balon.size()-1 << endl;
    	if((DistanciaPuntos4D(coordenadas_balon[i], coordenadas_balon[i-1]) > 5.0) && (DistanciaPuntos4D(coordenadas_balon[i], coordenadas_balon[i+1]) > 5.0))
			coordenadas_balon.erase(coordenadas_balon.begin()+i);
    }
*/
    ImprimeArchivo(nombre_archivo, coordenadas_balon);


    // se llama a la inicialización de glut
/***********************************************************


	
// se indica las caracteristicas que se desean para la visualización con OpenGL
// Las posibilidades son:
// GLUT_SIMPLE -> memoria de imagen simple
// GLUT_DOUBLE -> memoria de imagen doble
// GLUT_INDEX -> memoria de imagen con color indizado
// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
// GLUT_DEPTH -> memoria de profundidad o z-bufer
// GLUT_STENCIL -> memoria de estarcido

// inicio del bucle de eventos
// inicio del bucle de eventos
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// posicion de la esquina inferior izquierdad de la ventana
	glutInitWindowPosition(UI_window_pos_x,UI_window_pos_y);

	// tamaño de la ventana (ancho y alto)
	glutInitWindowSize(UI_window_width,UI_window_height);

	// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
	// al bucle de eventos)
	glutCreateWindow("Práctica 1");

    // asignación de la funcion llamada "dibujar" al evento de dibujo
	glutDisplayFunc(draw_scene);
	// asignación de la funcion llamada "cambiar_tamanio_ventana" al evento correspondiente
	glutReshapeFunc(change_window_size);
	// asignación de la funcion llamada "tecla_normal" al evento correspondiente
	glutKeyboardFunc(normal_keys);
	// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
	glutSpecialFunc(special_keys);

	// funcion de inicialización
	initialize();


	glutMainLoop();
		
/*********************************************************************/	

    return 0;
}

