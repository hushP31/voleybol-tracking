#include "misObjetos.h"
#include <vector>
#include <stdlib.h>
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>

using namespace std;
using namespace cv;




MiPunto::MiPunto(){
	x = 0;
	y = 0;
}
MiPunto::MiPunto(double x1, double y1){
	x = x1;
	y = y1;
}

MiPunto3D::MiPunto3D(){
	x = 0;
	y = 0;
	z = 0;
}
MiPunto3D::MiPunto3D(double x1, double y1, double z1){
	x = x1;
	y = y1;
	z = z1;
}

MiPunto4D::MiPunto4D(double tiempo, MiPunto3D espacio){
	segundos = tiempo;
	coordenada = espacio;
}

MiPunto4D::MiPunto4D(){
	segundos = 0;
	coordenada.x = 0;
	coordenada.y = 0;
	coordenada.z = 0;
}


Recta::Recta(){
	m = 0;
	k = 0;
}
Recta::Recta(double m1, double k1){
	m = m1;
	k = k1;
}



double Trunca(int n, double d){
	int aux = d * pow(10, n);
	double salida = aux / pow(10, n);

	return salida;
}

vector<MiPunto> PuntoEquidistanteRecta(Recta recta, MiPunto punto, double distancia){
    
    double y_ = recta.m*punto.x + recta.k;
    vector<MiPunto> puntos;

    double ai, bi, ti, ki, ci, mi;
    double Ai, Bi, CCi;

    ai = punto.x; 
    bi = punto.y; 
    ti = distancia; 
    ki = recta.k; 
    mi = recta.m;

    Ai = 1+mi*mi;
    Bi = 2*(mi*ki - ai - bi*mi);
    CCi = -1*(ti*ti - ai*ai - bi*bi - ki*ki + 2*bi*ki);

    double x1_aux = (-1*Bi + sqrt(Bi*Bi - 4*Ai*CCi)) / (2*Ai);
    double x2_aux = (-1*Bi - sqrt(Bi*Bi - 4*Ai*CCi)) / (2*Ai);
    double x1 = x1_aux;
    double x2 = x2_aux;
    
    double y1 = mi*x1 + ki;
    double y2 = mi*x2 + ki;

    MiPunto pcorte1;
    MiPunto pcorte2;

    pcorte1.x = x1;
    pcorte1.y = y1;

    pcorte2.x = x2;
    pcorte2.y = y2;

        

    puntos.push_back(pcorte1);
    puntos.push_back(pcorte2);

    return puntos;

}


Recta RectaDosMiPunto(MiPunto p1, MiPunto p2){
	Recta solucion;

    solucion.m = (1.0*(p2.y - p1.y)) / (1.0*(p2.x-p1.x));
    solucion.k =  -1.0*(p1.x*((p2.y-p1.y)/(1.0*(p2.x-p1.x))))+p1.y;
    
	return solucion;
}

Recta RectaDosPuntos( MiPunto p1, MiPunto p2){
	Recta solucion;

    solucion.m = (1.0*(p2.y - p1.y)) / (1.0*(p2.x-p1.x));
    solucion.k =  -1.0*(p1.x*((p2.y-p1.y)/(1.0*(p2.x-p1.x))))+p1.y;
    
	return solucion;
}

MiPunto PuntoCorte (Recta r, Recta s){
	MiPunto solucion;
	
	solucion.x = ((r.k - s.k)/(s.m - r.m)*1.0);
    solucion.y = solucion.x*r.m + r.k*1.0;
    	
	return solucion;
}

void OrdenaPuntos(vector<MiPunto> &V){
	MiPunto aux;
	for (int i=0; i<V.size()-1; i++)
		for (int j=i+1; j<V.size(); j++)
			if(V[i].x>V[j].x){
				aux = V[i];
				V[i] = V[j];
				V[j] = aux;
			}
}

Recta ParalelaPunto(Recta r, MiPunto p){
    Recta nueva_recta;
    
    nueva_recta.k = -1.0*(r.m*p.x) + p.y;
    nueva_recta.m = r.m;

    return nueva_recta;
}

Recta PerpendicularPunto(Recta r, MiPunto p){
	Recta nueva_recta;
    
    nueva_recta.m = (-1.0)/(1.0*r.m);
    nueva_recta.k = -1.0*nueva_recta.m*p.x + p.y;
    

    return nueva_recta;
}

double DistanciaPuntos(MiPunto p1, MiPunto p2){
    double d = 1.0*sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*1.0*(p1.y - p2.y));

    return d;
}

double DistanciaPoint(cv::Point p1, cv::Point p2){
    double d = 1.0*sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*1.0*(p1.y - p2.y));

    if(p1.x < p2.x)
    	d=-1.0*d;

    return d;
}

void ExtensionLateral(vector <MiPunto> &points3m){
    MiPunto c1;
    Recta r_base_mayor = RectaDosPuntos(points3m[0], points3m[3]);
    Recta r_base_menor = RectaDosPuntos(points3m[1], points3m[2]);
    vector <Recta> limites, d;
    vector <MiPunto> pf, p_extension;

    for(int i=0; i<points3m.size(); i++){
        Recta recta_aux = RectaDosPuntos(points3m[i], points3m[(i+1)%4]);
        limites.push_back(recta_aux);

        if(d.size()<2){
            Recta recta_d = RectaDosPuntos(points3m[i], points3m[(i+2)%4]);
            d.push_back(recta_d);
        }
    }

    c1 = PuntoCorte(limites[0], limites[2]);
    Recta t = ParalelaPunto(limites[1], c1);
    
    pf.push_back(PuntoCorte(t, d[0])); // punto de fuga derecha
    pf.push_back(PuntoCorte(t, d[1])); // punto de fuga izquierda

    //p_extension[0] = Punto de corte entre recta pfd-points3m[1] y r_base_mayor
    p_extension.push_back(PuntoCorte(RectaDosPuntos(pf[0], points3m[1]), r_base_mayor));

    //p_extension[1] = Punto de corte entre pfi-points3m[0] y r_base_menor
    p_extension.push_back(PuntoCorte(RectaDosPuntos(pf[1], points3m[0]), r_base_menor));

    //p_extension[2] = Punto de corte entre pfd-points3m[3] y r_base_menor
    p_extension.push_back(PuntoCorte(RectaDosPuntos(pf[0], points3m[3]), r_base_menor));

    //p_extension[3] = Punto de corte entre pfi-points3m[2] y r_base_mayor
    p_extension.push_back(PuntoCorte(RectaDosPuntos(pf[1], points3m[2]), r_base_mayor));

    for(int i=0; i<p_extension.size(); i++)
    	points3m[i] = p_extension[i];
    
}

double Altura(vector<MiPunto> puntos, MiPunto point, MiPunto suelo){
	
    double altura = 0;
	vector<MiPunto> fugas, corte; //0-> centro, 1->izda, 2->dcha 
	vector <Recta> campo_juego;
	MiPunto cero_relativo;
	double distancia_relativa, distancia_extra, metro_relativo;
	MiPunto punto_equis, corte_lateral_izda, corte_lateral_dcha, suelo_relativo;
	Recta r_aux_corte, r_aux_paralela, r_aux_perpendicular;



	//rectas del campo
	for(int i=0; i<puntos.size(); i++)
		campo_juego.push_back(RectaDosPuntos(puntos[i], puntos[(i+1)%4]));

	//Puntos de fuga
	fugas.push_back(PuntoCorte(campo_juego[0], campo_juego[2])); //principal, central

	Recta r_aux = ParalelaPunto(campo_juego[1], fugas[0]);

	fugas.push_back(PuntoCorte(r_aux, RectaDosPuntos(puntos[1], puntos[3])));//fuga izda
	fugas.push_back(PuntoCorte(r_aux, RectaDosPuntos(puntos[0], puntos[2])));//fuga izda

	//Punto cero relativo
	cero_relativo = PuntoCorte(campo_juego[1], RectaDosPuntos(puntos[0], fugas[1]));

	distancia_relativa = DistanciaPuntos(cero_relativo, puntos[0]);

	distancia_extra = (18.0 / suelo.y) * distancia_relativa;

	//Calcular el punto de la recta campo_juego[1] que dista "distancia_extra" a "cero_relativo"
	// El punto es el mas a la derecha

	double ai, bi, ti, ki, ci, mi;
    double Ai, Bi, CCi;

    ai = cero_relativo.x; 
    bi = cero_relativo.y; 
    ti = distancia_extra; 
    ki = campo_juego[1].k; 
    mi = campo_juego[1].m;

    Ai = 1+mi*mi;
    Bi = 2*(mi*ki - ai - bi*mi);
    CCi = -1*(ti*ti - ai*ai - bi*bi - ki*ki + 2*bi*ki);

    double x1_aux = (-1*Bi + sqrt(Bi*Bi - 4*Ai*CCi)) / (2*Ai);
    double x2_aux = (-1*Bi - sqrt(Bi*Bi - 4*Ai*CCi)) / (2*Ai);
    double x1 = x1_aux;
    double x2 = x2_aux;
    
    double y1 = mi*x1 + ki;
    double y2 = mi*x2 + ki;

    if(x1 > x2){
    	punto_equis.x = x1;
    	punto_equis.y = y1;
    }
    else{
    	punto_equis.x = x2;
    	punto_equis.y = y2;
    }


    //Recta que pasa por fugas[1] y punto_equis
    r_aux_corte = RectaDosPuntos(fugas[1], punto_equis);

    //Punto de corte entre recta anterior y campo_juego[0]
    corte_lateral_izda = PuntoCorte(r_aux_corte, campo_juego[0]);
    
    //Recta paralela a campo_juego[3] que pasa por corte_lateral_izda
    r_aux_paralela = ParalelaPunto(campo_juego[3], corte_lateral_izda);
    corte_lateral_dcha = PuntoCorte(r_aux_paralela, campo_juego[2]);

    metro_relativo = DistanciaPuntos(corte_lateral_izda, corte_lateral_dcha)/9.0;

    //Recta perpendicular a recta anterior que pasa por 'point'
    r_aux_perpendicular = PerpendicularPunto(r_aux_paralela, point);

    //Punto de corte entre rcta anteior y paralela
    suelo_relativo = PuntoCorte(r_aux_paralela, r_aux_perpendicular);

    distancia_relativa = DistanciaPuntos(point, suelo_relativo);

    altura = Trunca(2, distancia_relativa/metro_relativo);


	return altura;
}


void DrawObject(Mat &cameraFeed, MiPunto p, MiPunto3D suelo){

	int x = (int)p.x;
	int y = (int)p.y;

    //draw some crosshairs around the object
    circle(cameraFeed,Point(x,y),20,Scalar(0,255,0),1);
    line(cameraFeed,Point(x,y),Point(x,y-25),Scalar(0,255,255),1);
    line(cameraFeed,Point(x,y),Point(x,y+25),Scalar(0,255,255),1);
    line(cameraFeed,Point(x,y),Point(x-25,y),Scalar(0,255,255),1);
    line(cameraFeed,Point(x,y),Point(x+25,y),Scalar(0,255,255),1);
 	
    //write the position of the object to the screen
    putText(cameraFeed,"- Ball ( " + doubleToString(suelo.x) + ", " + doubleToString(suelo.y)+", " + doubleToString(suelo.z) + " ) ", Point(x,y), 1, 1, Scalar(240,240,240), 1.5);

}


void DrawPlayGround(Mat &cameraFeed, vector<MiPunto> playg, int plano){
	/************************************************
		Draw in cameraFeed the playground's lines 
        plano: 
            1 - Frontal
            0 - Lateral
	*************************************************/
	int x,y;
	int x_,y_;
    Recta aux;
    vector<MiPunto> fugas;
    vector <Recta> diagonal;
    MiPunto A, B;
    std::vector<MiPunto> puntos_seccion_low, puntos_seccion_high;
    int divisiones = 2;

    if(plano == 0) divisiones=3;

    //Limites
	for(int i=0; i<playg.size(); i++){
		x = (int)playg[i%4].x;
		y = (int)playg[i%4].y;

		x_ = (int)playg[(i+1)%4].x;
		y_ = (int)playg[(i+1)%4].y;
        //Draw Limits Lines
		line(cameraFeed,Point(x,y),Point(x_,y_),Scalar(0,255,255), 3);
	}

    //Diagonales
    diagonal.push_back(RectaDosMiPunto(playg[0], playg[2]));
    diagonal.push_back(RectaDosMiPunto(playg[1], playg[3]));

    aux = RectaDosMiPunto(playg[1], playg[2]);

    //Puntos Fuga 
    fugas.push_back(PuntoCorte(RectaDosMiPunto(playg[0], playg[1]), RectaDosMiPunto(playg[2],playg[3])));
    aux = ParalelaPunto(aux, fugas[0]);
    fugas.push_back(PuntoCorte(diagonal[1], aux)); //fuga izda
    fugas.push_back(PuntoCorte(diagonal[0], aux)); //fuga derecha

    //cout << "\nCentro" << fugas[0].x << "Izda" << fugas[1].x << "Dcha" << fugas[2].x << endl;

    //Puntos de ayuda para dividir el campo
    A = playg[2];
    B = PuntoCorte(RectaDosMiPunto(playg[1], playg[2]), RectaDosMiPunto(fugas[2], playg[3]));
    float longitud_segmento = DistanciaPuntos(B, A);
    longitud_segmento = longitud_segmento/divisiones;
    std::vector<MiPunto> vp;
    


    for(int i=0; i<(divisiones-1); i++){
        vp = PuntoEquidistanteRecta(RectaDosMiPunto(playg[1], playg[2]), A, longitud_segmento);
        
        if(vp[0].x > vp[1].x)
            A = vp[0];
        else
            A = vp[1];

        puntos_seccion_low.push_back(PuntoCorte(RectaDosMiPunto(fugas[2], A), RectaDosMiPunto(playg[0], playg[3])));
        //line(cameraFeed,Point((int)fugas[2].x,(int)fugas[2].y),Point((int)A.x,(int)A.y),Scalar(0,0,255), 1); //A
    }

    for(int i=0; i<puntos_seccion_low.size(); i++)
        puntos_seccion_high.push_back(PuntoCorte(RectaDosMiPunto(puntos_seccion_low[i], fugas[0]), 
                                                                    RectaDosMiPunto(playg[1], playg[2])));
    
    for(int i=0; i<puntos_seccion_low.size(); i++)
        line(cameraFeed, Point((int)puntos_seccion_low[i].x, (int)puntos_seccion_low[i].y), 
                                Point((int)puntos_seccion_high[i].x, (int)puntos_seccion_high[i].y), Scalar(50,150, 230), 2);


}


Mat ConvertImageThreshold(Mat frame1, Mat frame2, bool debugMode, String camara){
	const static int SENSITIVITY_VALUE = 10;
	const static int SENSITIVITY_VALUE_L = 10;

	Mat thresholdImage;
	Mat grayImage1, grayImage2, differenceImage;
	Mat imgHSV;

	//convert frame1 to gray scale for frame differencing
    cv::cvtColor(frame1,grayImage1,COLOR_BGR2GRAY);

    //convert frame2 to gray scale for frame differencing
    cv::cvtColor(frame2,grayImage2,COLOR_BGR2GRAY);
    //perform frame differencing with the sequential images. This will output an "intensity image"
    //do not confuse this with a threshold image, we will need to perform thresholding afterwards.
    cv::absdiff(grayImage1,grayImage2,differenceImage);
    //threshold intensity image at a given sensitivity value
    //cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);
    //morphological opening (remove small objects from the foreground)
    erode(differenceImage, differenceImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
	dilate( differenceImage, differenceImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

	//morphological closing (fill small holes in the foreground)
	dilate( differenceImage, differenceImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
	erode(differenceImage, differenceImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );

	cv::threshold(differenceImage,thresholdImage,SENSITIVITY_VALUE,255,THRESH_BINARY);// | CV_THRESH_OTSU);
	cv::blur(thresholdImage, thresholdImage, Size(3, 3));

    //erode(differenceImage, differenceImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );
    //dilate( differenceImage, differenceImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

    //morphological closing (fill small holes in the foreground)
    //dilate( thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
    //erode(thresholdImage, thresholdImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)) );

    namedWindow(camara, WINDOW_NORMAL);
    
    resizeWindow(camara, 1080, 720);

	if(debugMode==true){
        //show the difference image and threshold image
        //cv::imshow("Difference Image",differenceImage);
        //cv::imshow("Difference Image LAT",differenceImageL);
        cv::imshow(camara, thresholdImage);
        //cv::imshow("Threshold Image LAT", thresholdImageL);
        //cv::imshow("Canny Image", cannyImage);
        //cv::imshow("Canny Image LAT", cannyImageL);
    }
    else{
        //if not in debug mode, destroy the windows so we don't see them anymore
        //cv::destroyWindow("Difference Image");
        //cv::destroyWindow("Difference Image LAT");
        cv::destroyWindow(camara);
        //cv::destroyWindow("Threshold Image LAT");
        //cv::destroyWindow("Canny Image");
        //cv::destroyWindow("Canny Image LAT");
    }
	return thresholdImage;
}


//int to string Aux-function
string intToString(int number){
    //this function has a number input and string output
    std::stringstream ss;
    ss << number;
    return ss.str();
}

//int to string Aux-function
string doubleToString(double number){
    //this function has a number input and string output
    std::stringstream ss;
    ss << number;
    return ss.str();
}


void ImprimeArchivo(string nombre, vector<MiPunto4D> p){
    
	std::stringstream ss;

	for(int i = 0; i < p.size(); ++i){
		ss << p[i].segundos << " "; 
		ss << p[i].coordenada.x;
		ss << " ";
		ss << p[i].coordenada.y;
		ss << " ";
		ss << p[i].coordenada.z;
		ss << endl;
	}

	std::string s = ss.str();

	//cout << s;

    char *cstr = new char[nombre.length() + 1];
	strcpy(cstr, nombre.c_str());

    ofstream fs(cstr); 

	fs << s;

	fs.close();
}


MiPunto4D PuntoMedio(MiPunto4D p, MiPunto4D q){
	MiPunto4D c;

	c.segundos = (p.segundos + q.segundos)/2.0;

	c.coordenada.x = p.coordenada.x + 0.5 *(q.coordenada.x - p.coordenada.x);
	c.coordenada.y = p.coordenada.y + 0.5 *(q.coordenada.y - p.coordenada.y);
	c.coordenada.z = p.coordenada.z + 0.5 *(q.coordenada.z - p.coordenada.z);

	return c;
}


double DistanciaPuntos4D(MiPunto4D p, MiPunto4D q){
	MiPunto3D c;
	double distancia;

	c.x = p.coordenada.x + 0.5 *(q.coordenada.x - p.coordenada.x);
	c.y = p.coordenada.y + 0.5 *(q.coordenada.y - p.coordenada.y);
	c.z = p.coordenada.z + 0.5 *(q.coordenada.z - p.coordenada.z);

	distancia = sqrt(c.x*c.x + c.y*c.y + c.z*c.z);

	return distancia;
}


void Axis_lateral(Mat &camera){
    

    //Coordenates axis
    line(camera, Point(50,   50), Point(1030,   50), Scalar(0,0,255), 2);//X
    line(camera, Point(50,  670), Point(1030,  670), Scalar(0,0,255), 2);//X
    line(camera, Point(1030, 50), Point(1030,  670), Scalar(0,0,255), 2);//Y
    line(camera, Point(50,   50), Point(50,    670), Scalar(0,0,255), 2);//Y

    //Auxiliar axis X
    for(int i=(112); i<670; i+=62){//y
        line(camera, Point(45,  i), Point(55,  i), Scalar(100,100,100), 1);
        line(camera, Point(1025,i), Point(1035,i), Scalar(100,100,100), 1);
    }
    //Auxiliar Axis Y
    for(int i=99; i<1030; i+=49){
        line(camera, Point(i, 45), Point(i, 55), Scalar(100,100,100), 1);//Y
        line(camera, Point(i,665), Point(i,675), Scalar(100,100,100), 1);//Y
    }

    //Playground Limits
    line(camera, Point((50+1030)/2, 670), Point((50+1030)/2, 50), Scalar(255,0,0), 1.2);//Centro
    line(camera, Point(99, 670), Point(99, 50), Scalar(255,0,0), 1.2);//Fondo Izda
    line(camera, Point(980, 670), Point(980, 50), Scalar(255,0,0), 1.2);//Fondo Dcha

    //Grid 2.42 meters
    float centimetro = (720-100)/10;
    float h_grid = 2.42*centimetro;

    line(camera, Point(50, 670-h_grid), Point(1030, 670-h_grid), Scalar(255,0,0), 1.2);//Fondo Dcha


}

void Vista_Lateral(vector<MiPunto4D> v){

    cv::Size imageSize(1080,720); // your window size
    cv::Mat vista_lateral(imageSize, CV_8UC3, Scalar::all(255));
    cv::Mat vista_frontal(imageSize, CV_8UC3, Scalar::all(255));

    int inicio = v.size()-10;
    int final = v.size();

    if(v.size() < 10)
        inicio = 0;

    if (vista_lateral.empty() || vista_frontal.empty()){ //check whether the image is valid or not
        std::cout << "Error : Image cannot be created..!!" << std::endl;
        system("pause"); //wait for a key press
    }

    Axis_lateral(vista_lateral);

    for(int i=inicio; i<final; i++){
        int z = (int)v[i].coordenada.z;
        int y = (int)v[i].coordenada.y;

        float zz = (((1030-50)/18.0)*z)+50;
        float yy = (((670-50)/10.0)*y)+50;

        z = (int)zz;
        y = (int)yy;

        circle(vista_lateral,Point(z,y),5,Scalar(0,255,0),1);
    }


    namedWindow("V", WINDOW_NORMAL);
    imshow("V", vista_lateral);

}

void Vista_Frontal(){


}

bool PuntosIguales(MiPunto p1, MiPunto p2){
    bool iguales = false;
    float d1;

    d1 = DistanciaPuntos(p1, p2);

    if(d1 < 5)
        iguales = true;

    return iguales;
}

bool Desechable(MiPunto p, vector<MiPunto> v){
    bool desechable = false;
    float d;
    if(v.size() != 0)
        for(int i=0; (i<v.size()) && (!desechable); i++)
            if(DistanciaPuntos(p, v[i]) < 5)
                desechable = true;

    return desechable;
}

vector<MiPunto> MaxMinRelativos(vector<MiPunto> v){
    vector<MiPunto> solucion;
    MiPunto min, max;
    bool maximo = true;
    bool minimo = true;

    for(int i=0; i<v.size()-1; i++){
        if(v[i].y < v[i+1].y)
            if(minimo){
                minimo = false;
                maximo = true;
                solucion.push_back(v[i]);
            }

        if(v[i].y > v[i+1].y)
            if(maximo){
                maximo = false;
                minimo = true;
                solucion.push_back(v[i]);
            }
    }

    return solucion;
}

void DrawParabola(vector<MiPunto> v){

}


vector<MiPunto> ABC(vector<MiPunto> puntos_campo){

    vector<Recta> rectas_campo;
    vector<MiPunto> fugas;
    vector<MiPunto> abc; //SOLUCION
    Recta diagonal1, diagonal2;
    Recta auxiliar_fugas;
    Recta r, t;
    MiPunto a, b, c;
    float error = 1, margen;
    float ab, ac;
    int contador = 0;    
    bool err = false;
    bool err_aux = false;


    for(int i=0; i<4; i++)
        rectas_campo.push_back(RectaDosPuntos(puntos_campo[i%4], puntos_campo[(i+1)%4]));

    diagonal2 = RectaDosPuntos(puntos_campo[0], puntos_campo[2]);
    diagonal1 = RectaDosPuntos(puntos_campo[1], puntos_campo[3]);

    fugas.push_back(PuntoCorte(rectas_campo[0], rectas_campo[2]));
    auxiliar_fugas = ParalelaPunto(rectas_campo[1], fugas[0]);

    fugas.push_back(PuntoCorte(auxiliar_fugas, diagonal1));
    fugas.push_back(PuntoCorte(auxiliar_fugas, diagonal2));

    fugas.push_back(PuntoCorte(RectaDosPuntos(puntos_campo[1], fugas[2]), 
                                    RectaDosPuntos(puntos_campo[2], fugas[1])));

    //Recta que contiene a A.
    r = RectaDosPuntos(fugas[0], fugas[3]);
    a.y = fugas[0].y - 50;
    a.x = (a.y - r.k)/r.m;

    //b, c, t

    t = ParalelaPunto(rectas_campo[1], a);
    b = PuntoCorte(rectas_campo[2], t);
    c = PuntoCorte(rectas_campo[0], t);

    ab = DistanciaPuntos(a,b);
    ac = DistanciaPuntos(a,c);

    error = abs(ab - ac);
    margen = 0.1;

    while(error > 0.00001){
        if(ab > ac){
            b.y+=margen;
            err = false;
        }
        else{
            b.y-=margen;
            err = true;
        }

        b.x = (b.y - rectas_campo[2].k)/rectas_campo[2].m;
        t = RectaDosPuntos(a, b);
        c = PuntoCorte(t, rectas_campo[0]);

        ab = DistanciaPuntos(a,b);
        ac = DistanciaPuntos(a,c);

        error = abs(ab - ac);
        
        if(error > 10)
            margen = abs(error/4.0);
        else{
            if(contador < 10)
                margen = 0.01;
            else
                margen = margen/2.0;
        }

        if(err_aux != err){
            contador ++;
            err_aux = err;
        }

    }

    abc.push_back(a);
    abc.push_back(b);
    abc.push_back(c);

    return abc;
}

MiPunto Extrapolacion(float d, vector<MiPunto> abc){

    MiPunto solucion;

    float BC = DistanciaPuntos(abc[1], abc[2]);
    Recta recta_bc = RectaDosPuntos(abc[1], abc[2]);
    float d_trans = (d*BC)/9.0;
    float a = (1+recta_bc.m*recta_bc.m);
    float b = (2*recta_bc.m*recta_bc.k - 2*abc[2].x - 2*abc[2].y*recta_bc.m);
    float c = -1*(d_trans*d_trans-abc[2].x*abc[2].x-abc[2].y*abc[2].y);
    float aux = b*b - 4*a*c;

    if(aux > 0 || aux == 0){
        float raiz = sqrt(aux);  
            
        MiPunto p_aux1, p_aux2;

        p_aux1.x = ((-1*b)+raiz)/(2*a);
        p_aux1.y = recta_bc.m*p_aux1 + recta_bc.k;


        p_aux2.x = ((-1*b)-raiz)/(2*a);
        p_aux2.y = recta_bc.m*p_aux2 + recta_bc.k;

        if(DistanciaPuntos(abc[1], p_aux1) < DistanciaPuntos(abc[1], p_aux2))
            solucion = p_aux1;
        else
            solucion = p_aux2;
    }

    return solucion;
}


MiPunto PuntoTransformadoSuelo(vector <MiPunto> frontal, vector <MiPunto> lateral, 
                                    vector <Recta> f_limite, vector <Recta> l_limite, 
                                        vector <MiPunto> f_abc, vector <MiPunto> l_abc,
                                            MiPunto p_frontal, MiPunto p_lateral,
                                                vector <MiPunto> f_fuga, vector <MiPunto> l_fuga){

    
    MiPunto solucion;
    Recta f_balon_bases, l_balon_bases, f_BC, l_BC;
    MiPunto p_aux;
    MiPunto f_corte_1b, f_corte_2B;
    MiPunto l_corte_1b, l_corte_2B;

    
    //Frontal
    p_aux.x = p_frontal.x;
    p_aux.y = p_frontal.y + 20;

    f_balon_bases = RectaDosPuntos(p_frontal, p_aux);

    f_corte_1b = PuntoCorte(f_balon_bases, f_limite[1]);
    f_corte_2B = PuntoCorte(f_balon_bases, f_limite[3]);

    //Recta BC
    f_BC = RectaDosPuntos(f_abc[1], f_abc[2]);

    Recta f_aux_1b = RectaDosPuntos(f_corte_1b, f_fuga[0]);
    Recta f_aux_2B = RectaDosPuntos(f_corte_2B, f_fuga[0]);

    MiPunto f_bc_1b = PuntoCorte(f_aux_1b, f_BC);
    MiPunto f_bc_2B = PuntoCorte(f_aux_2B, f_BC);

    float f_C_bc1b = DistanciaPuntos(f_abc[2], f_bc_1b);
    float f_C_bc2B = DistanciaPuntos(f_abc[2], f_bc_2B);

    float f_dBC = DistanciaPuntos(f_abc[1], f_abc[2]);

    float f_real_1b = (9.0*f_C_bc1b)/f_dBC;
    float f_real_2B = (9.0*f_C_bc2B)/f_dBC;



    //Lateral
    p_aux.x = p_lateral.x;
    p_aux.y = p_lateral.y + 20;

    l_balon_bases = RectaDosPuntos(p_lateral, p_aux);

    l_corte_1b = PuntoCorte(l_balon_bases, l_limite[1]);
    l_corte_2B = PuntoCorte(l_balon_bases, l_limite[3]);

    //Recta BC
    l_BC = RectaDosPuntos(l_abc[1], l_abc[2]);

    Recta l_aux_1b = RectaDosPuntos(l_corte_1b, l_fuga[0]);
    Recta l_aux_2B = RectaDosPuntos(l_corte_2B, l_fuga[0]);

    MiPunto l_bc_1b = PuntoCorte(l_aux_1b, l_BC);
    MiPunto l_bc_2B = PuntoCorte(l_aux_2B, l_BC);

    float l_C_bc1b = DistanciaPuntos(l_abc[2], l_bc_1b);
    float l_C_bc2B = DistanciaPuntos(l_abc[2], l_bc_2B);

    float l_dBC = DistanciaPuntos(l_abc[1], l_abc[2]);


    //Considero el campo como un cuadrado de 9x9, para suelo en la solucion doblar el valor lateral.
    float l_real_1b = (9.0*l_C_bc1b)/l_dBC;
    float l_real_2B = (9.0*l_C_bc2B)/l_dBC;


    //Extrapolar l_real_XY al campo frontal

    // Calcular los puntos cuya distancia a f_abc[2] (C) sea l_real_1b y l_real_2B
    // y además esté dentro del segmento que forman f_abc[1] y f_abc[2]
    
    MiPunto f_bc_l1b = Extrapolacion(l_real_1b, f_abc);
    MiPunto f_bc_l2B = Extrapolacion(l_real_2B, f_abc);

    




    return solucion;
}









