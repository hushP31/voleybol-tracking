#include <vector>
#include <stdlib.h>
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace cv;
using namespace std;


class MiPunto{
	public:
		double x;
		double y;
	public:
		MiPunto();
		MiPunto(double x1, double y1);
};

class MiPunto3D{
public:
	double x; 
	double y; 
	double z;

	MiPunto3D();
	MiPunto3D(double x1, double y1, double z1);
};

class MiPunto4D{
public:
	double segundos;
	MiPunto3D coordenada;

	MiPunto4D();
	MiPunto4D(double tiempo, MiPunto3D espacio);
};

class Recta{
public:
	double m; // pendiente
	double k; // 

	Recta();
	Recta(double m1, double k1);
};


//FUNCIONES

Recta ParalelaPunto(Recta r, MiPunto p);
Recta PerpendicularPunto(Recta r, MiPunto p);
Recta RectaDosMiPunto(MiPunto p1, MiPunto p2);
Recta RectaDosPuntos( MiPunto p1, MiPunto p2);

MiPunto PuntoCorte (Recta r, Recta s);
vector<MiPunto> PuntoEquidistanteRecta(Recta recta, MiPunto punto, double distancia);

void OrdenaPuntos(vector<MiPunto> &V);
void ExtensionLateral(vector <MiPunto> &points3m);
//void PlaygroundHorizontal(int event, int x, int y, int flags, void* userdata);
//void PlaygroundLateral(int event, int x, int y, int flags, void* userdata);
void DrawObject(Mat &cameraFeed, MiPunto p, MiPunto3D suelo);
void DrawPlayGround(Mat &cameraFeed, vector<MiPunto> playg, int plano);
void ImprimeArchivo(string nombre, vector<MiPunto4D> p);


double Altura(vector<MiPunto> puntos, MiPunto point, MiPunto suelo);
double DistanciaPuntos(MiPunto p1, MiPunto p2);
double DistanciaPoint(cv::Point p1, cv::Point p2);
double Trunca(int n, double d);
double DistanciaPuntos4D(MiPunto4D p, MiPunto4D q);


Mat ConvertImageThreshold(Mat frame1, Mat frame2, bool debugMode, String camara);

string intToString(int number);
string doubleToString(double number);

MiPunto4D PuntoMedio(MiPunto4D p, MiPunto4D q);

void Axis_lateral(Mat &camera);
void Vista_Lateral(vector<MiPunto4D> v);
void Vista_Frontal();
bool PuntosIguales(MiPunto p1, MiPunto p2);
bool Desechable(MiPunto p, vector<MiPunto> v);

vector<MiPunto> MaxMinRelativos(vector<MiPunto> v);
void DrawParabola(vector<MiPunto> v);
vector<MiPunto> ABC(vector<MiPunto> puntos_campo);
vector<MiPunto> PuntosDeFuga(vector<MiPunto> puntos_campo);
MiPunto3D PuntoTransformadoSuelo(vector <MiPunto> frontal, vector <MiPunto> lateral, 
                                    vector <Recta> f_limite, vector <Recta> l_limite, 
                                        vector <MiPunto> f_abc, vector <MiPunto> l_abc,
                                            MiPunto balon_frontal, MiPunto balon_lateral,
                                                vector <MiPunto> f_fuga, vector <MiPunto> l_fuga, 
                                                	Mat &f_cameraFeed, Mat &l_cameraFeed);
MiPunto Extrapolacion(float d, vector<MiPunto> abc);
void ImprimeMiPunto(MiPunto p);
void ImprimeRecta(Recta r);