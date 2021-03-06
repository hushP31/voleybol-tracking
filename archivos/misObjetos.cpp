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

MiPunto4D::MiPunto4D(double tiempo, MiPunto3D espacio, bool manual){
	segundos = tiempo;
	coordenada = espacio;
    creacion = manual;
}

MiPunto4D::MiPunto4D(){
	segundos = 0;
	coordenada.x = 0;
	coordenada.y = 0;
	coordenada.z = 0;
    creacion = false; 
}


Recta::Recta(){
	m = 0;
	k = 0;
}
Recta::Recta(double m1, double k1){
	m = m1;
	k = k1;
}

void ImprimeMiPunto(MiPunto p){
    cout << "(" << p.x << ", " << p.y << ")  ";
}

void ImprimeMiPunto3D(MiPunto3D p){
    cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" ;
}

void ImprimeRecta(Recta r){
    cout << "y = " << r.m << " x + " << r.k << endl;
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

    //cout << "m: " << solucion.m << " k: " << solucion.k << endl;

	return solucion;
}

MiPunto PuntoCorte(Recta r, Recta s){
	MiPunto solucion;
	
	solucion.x = ((r.k - s.k)/(s.m - r.m)*1.0);
    solucion.y = solucion.x*r.m + r.k*1.0;
    	   
    //cout << "( " << solucion.x << ", " << solucion.y << " ) \n";

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
    
    resizeWindow(camara, 640, 380);

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
        ss << p[i].creacion << "\t"; 
		ss << p[i].segundos << "\t"; 
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

vector <MiPunto> PuntosDeFuga(vector<MiPunto> puntos_campo){

    vector<Recta> rectas_campo;
    vector<MiPunto> fugas;
    Recta diagonal1, diagonal2;
    Recta auxiliar_fugas;


    for(int i=0; i<4; i++)
        rectas_campo.push_back(RectaDosPuntos(puntos_campo[i%4], puntos_campo[(i+1)%4]));

    diagonal2 = RectaDosPuntos(puntos_campo[0], puntos_campo[2]);
    diagonal1 = RectaDosPuntos(puntos_campo[1], puntos_campo[3]);

    fugas.push_back(PuntoCorte(rectas_campo[0], rectas_campo[2]));
    
    MiPunto p1, p2;
    p1.x = 0.0; p1.y = 0.0;
    p2.x = 4.0; p2.y = 0.0;

    /*
    Triangulo formado por puntos_campo[0], fugas[0], puntos_campo[3]
    */
    Recta rr = PerpendicularPunto(rectas_campo[3], fugas[0]);
    Recta aa = PerpendicularPunto(rr, fugas[0]);

    auxiliar_fugas = ParalelaPunto(aa, fugas[0]);

    fugas.push_back(PuntoCorte(auxiliar_fugas, diagonal1));
    fugas.push_back(PuntoCorte(auxiliar_fugas, diagonal2));

    fugas.push_back(PuntoCorte(diagonal1, diagonal2));
    
    //Recta perpendicular a rectas_campo[3] que pasa por fugas[0]
    Recta perpendicular_aux = PerpendicularPunto(rectas_campo[3], fugas[0]);
    fugas.push_back(PuntoCorte(rectas_campo[3], perpendicular_aux));

    return fugas;
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

    fugas = PuntosDeFuga(puntos_campo);

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
        p_aux1.y = recta_bc.m*p_aux1.x + recta_bc.k;


        p_aux2.x = ((-1*b)-raiz)/(2*a);
        p_aux2.y = recta_bc.m*p_aux2.x + recta_bc.k;

        if(DistanciaPuntos(abc[1], p_aux1) < DistanciaPuntos(abc[1], p_aux2))
            solucion = p_aux1;
        else
            solucion = p_aux2;
    }

    return solucion;
}


MiPunto PuntoDistanciaRecta(MiPunto p, Recta r, float distancia, int direccion){

    //direccion = 1 --> punto a la izquierda de p
    //direccion = 0 --> punto a la derecha de p

    MiPunto solucion;
    float A, B, C, x1, x2, raiz;
    float xaux;

    A = 1+(r.m*r.m);
    B = (-2*p.x) + (2*r.m*r.k) - (2*p.y*r.m);
    C = (p.x*p.x) + (r.k*r.k) + (p.y*p.y) - (2*p.y*r.k) - (distancia*distancia);

    raiz = sqrt(B*B - 4*A*C);

    x1 = (-1*B + raiz)/(2*A);
    x2 = (-1*B + raiz)/(2*A);

    if((x2 < x1)){
        xaux = x1;
        x1 = x2;
        x2 = xaux;
    }
    
    if(direccion%2 == 0)
        solucion.x = x2;
    else
        solucion.x = x1;

    solucion.y = solucion.x*r.m + r.k;

    return solucion;
}

void Cuadricula(Mat &cameraFeed, int posicion, int divisiones, vector <MiPunto> campo, 
                        vector <MiPunto> fuga, vector <MiPunto> abc, vector <Recta> lineas_campo){

    /**************************************
        Posicion:
            Si posicion == 1 -> Vertical
            Si posicion == 0 -> Horizontal
            Si posicion == 2 -> Ambos;
    ***************************************/
    vector <MiPunto> cortes_bc, cortes_1, cortes_3;
    int x1, x3, y1, y3;

    //Puntos de corte en la Recta(abc[1], abc[2])
    Recta bc = RectaDosPuntos(abc[1], abc[2]);
    MiPunto c_superior;
    c_superior.x = abc[2].x;
    c_superior.y = abc[2].y - divisiones;

    Recta c9 = RectaDosPuntos(abc[2], c_superior);

    float desplazamiento_corte = divisiones/divisiones;

    Recta aux_paralela = RectaDosPuntos(c_superior, abc[1]);
    MiPunto c_aux;
    Recta corte;
    c_aux = abc[2];

    for(int i=0; i<divisiones; i++){
        c_aux.y -= desplazamiento_corte;
        cortes_bc.push_back(PuntoCorte(bc, ParalelaPunto(aux_paralela, c_aux)));
    }

    if(posicion == 1){

        for(int i=0; i<divisiones; i++){
            cortes_1.push_back(PuntoCorte(lineas_campo[1], RectaDosPuntos(fuga[0], cortes_bc[i])));
            cortes_3.push_back(PuntoCorte(lineas_campo[3], RectaDosPuntos(fuga[0], cortes_bc[i])));
        }

        for(int i=0; i<divisiones; i++){
            x1 = (int)cortes_1[i].x; y1 = (int)cortes_1[i].y;
            x3 = (int)cortes_3[i].x; y3 = (int)cortes_3[i].y;

            line(cameraFeed, Point(x1, y1), Point(x3, y3), Scalar(100, 30, 30), 2);
            putText(cameraFeed, doubleToString(i+1) , Point(x3,y3 + 30), 1, 2, Scalar(0,0,100), 1);
        }
    }

    cortes_1.clear(); // lateral izquierdo. lineas_campo[0]
    cortes_3.clear(); // lateral derecho.   lineas_campo[2]

    if(posicion == 0){

        for(int i=0; i<divisiones; i++){
            cortes_1.push_back(PuntoCorte(RectaDosPuntos(fuga[1], PuntoCorte(RectaDosPuntos(fuga[0], cortes_bc[i]), lineas_campo[3])), lineas_campo[0]));
            cortes_3.push_back(PuntoCorte(RectaDosPuntos(fuga[1], PuntoCorte(RectaDosPuntos(fuga[0], cortes_bc[i]), lineas_campo[1])), lineas_campo[2]));
        }

        for(int i=0; i<divisiones; i++){
            x1 = (int)cortes_1[i].x; y1 = (int)cortes_1[i].y;
            x3 = (int)cortes_3[i].x; y3 = (int)cortes_3[i].y;

            line(cameraFeed, Point(x1, y1), Point(x3, y3), Scalar(100, 30, 30), 1);
            putText(cameraFeed, doubleToString(i+1) , Point(x1 - 30,y1), 1, 1, Scalar(0,0,100), 1);
        }
    }

    

}


void PintaLinea(Mat &cameraFeed, MiPunto p, MiPunto q, int r, int g, int b, int tam){
    line(cameraFeed, Point((int)p.x, (int)p.y), Point((int)q.x, (int)q.y), Scalar(r,g,b), tam);
}

float AlturaBalon(Mat &cameraFeed, MiPunto low_ball, MiPunto high_ball, vector <MiPunto> frontal, 
                                        vector<Recta> f_limite, vector<MiPunto> fuga, vector<MiPunto> abc, bool frente){


        MiPunto dcha, izda, inf, sup, p_x, p_y, p_proyeccion_balon;
        Recta r_bc, r_horizontal, r_vertical;
        float anchura, d_relativa, altura = 0, anchura_actual, altura_actual;
        float DD = (DistanciaPuntos(abc[1], abc[2]));

        if(frente) anchura = 9.0;
        else anchura = 18.0;

        //cout << "SUELO: " << low_ball.x << ", " << low_ball.y << endl;
        
        r_bc = RectaDosPuntos(abc[1], abc[2]);
        d_relativa = (DD*(9-low_ball.x))/(9.0);

        p_x = PuntoDistanciaRecta(abc[1], r_bc, d_relativa, 1);
        //cout << "Distancia: " << DD << "\t d_relativa: " << d_relativa << "\t";
        
        d_relativa = (DD*low_ball.y)/(18.0);
        p_y = PuntoDistanciaRecta(abc[1], r_bc, d_relativa, 1);
        //cout << d_relativa << endl;
        
        dcha = PuntoCorte(f_limite[2], RectaDosPuntos(fuga[2], PuntoCorte(f_limite[3], RectaDosPuntos(p_y, fuga[0]))));
        izda = PuntoCorte(f_limite[0], RectaDosPuntos(fuga[2], PuntoCorte(f_limite[1], RectaDosPuntos(p_y, fuga[0]))));
        //PintaLinea(cameraFeed, dcha, izda, 40, 0, 10, 3);

        inf = PuntoCorte(f_limite[3], RectaDosPuntos(p_x, fuga[0]));
        sup = PuntoCorte(f_limite[1], RectaDosPuntos(p_x, fuga[0]));
        //PintaLinea(cameraFeed, inf, sup, 40, 0, 10, 3);

        r_horizontal = RectaDosPuntos(dcha, izda);
        r_vertical = RectaDosPuntos(inf , sup);

        p_proyeccion_balon = PuntoCorte(r_horizontal, r_vertical);

        altura_actual = DistanciaPuntos(p_proyeccion_balon, high_ball);
        anchura_actual = DistanciaPuntos(dcha, izda);

        altura = anchura*altura_actual/anchura_actual;

        //cout << "\nALTURA: " << altura << endl;

        //Dibujar la red:
        float inc = anchura_actual*2.24/9.0;

        dcha.y -= inc;
        izda.y -= inc;

        //PintaLinea(cameraFeed, dcha, izda, 10, 30, 0, 4);
        
        return altura;
}



MiPunto3D PuntoTransformadoSuelo(vector <MiPunto> frontal, vector <MiPunto> lateral, vector <Recta> f_limite, vector <Recta> l_limite, 
                                        vector <MiPunto> f_abc, vector <MiPunto> l_abc, MiPunto balon_frontal, MiPunto balon_lateral,
                                                vector <MiPunto> f_fuga, vector <MiPunto> l_fuga, Mat &f_cameraFeed, Mat &l_cameraFeed )
{
    /*
    cout << "F_ABC:\n";
    for(int i=0; i<f_abc.size(); i++)
        ImprimeMiPunto(f_abc[i]);
    cout << endl;

    cout << "F_FUGAS:\n";
    for(int i=0; i<f_fuga.size(); i++)
        ImprimeMiPunto(f_fuga[i]);
    cout << endl;
    */
    
    MiPunto solucion;
    MiPunto3D solucion3D;
    MiPunto suelo_balon;
    Recta f_referencia_9, f_referencia_par;
    Recta l_referencia_18, l_referencia_par;
    MiPunto f_ref_aux, l_ref_aux;
    Recta f_balon, l_balon;

    Recta f_bc = RectaDosPuntos(f_abc[1], f_abc[2]);
    Recta l_bc = RectaDosPuntos(l_abc[1], l_abc[2]);

    MiPunto p_aux;
    MiPunto p_aux2;

    p_aux.x = 1.0;
    p_aux.y = 2.0;

    p_aux2.x = 2.0;
    p_aux2.y = 2.0;

    Recta paralela_aux_f_l = f_limite[3];
    
    /** FRONTAL **/
    //0.A. Crear recta de referencia real desde f_abc[2] (C).
    f_ref_aux.x = f_abc[2].x + 0.01;
    f_ref_aux.y = f_abc[2].y - 9.0;

    f_referencia_9 = RectaDosPuntos(f_abc[2], f_ref_aux);
    
    //0.B. Crear recta desde ref_aux hasta f_abc[1] (B)
    f_referencia_par = RectaDosPuntos(f_abc[1], f_ref_aux);

    // 1. Crear recta perpendicular al campo que pase por el punto del balón 
    //    y calcular los puntos de corte de dicha recta con las bases del 
    //    trapecio que forma el campo.
    f_balon = ParalelaPunto(RectaDosPuntos(f_fuga[0], f_fuga[4]), balon_frontal);
    MiPunto f_c1 = PuntoCorte(f_balon, f_limite[1]);
    MiPunto f_c3 = PuntoCorte(f_balon, f_limite[3]);
    
    int x11 = (int)f_c1.x;
    int y11 = (int)f_c1.y;
    int x33 = (int)f_c3.x;
    int y33 = (int)f_c3.y;
    line(f_cameraFeed,Point(x11,y11),Point(x33,y33),Scalar(0,255,255),1);

    //2. Hallar el punto real de f_c1 y f_c3. --> f_c1_real, f_c3_real

    //  2.A. crear rectas desde el punto de fuga 0 y f_c1 y f_c3.
        Recta f_c1_0 = RectaDosPuntos(f_fuga[0], f_c1);
        Recta f_c3_0 = RectaDosPuntos(f_fuga[0], f_c3);

    //  2.B. hallar puntos de corte entre las rectas anteriores y f_bc
        MiPunto f_c1_bc = PuntoCorte(f_c1_0, f_bc);
        MiPunto f_c3_bc = PuntoCorte(f_c3_0, f_bc);

    //  2.C. Crear recta paralela a f_referencia_par que pase por los puntos de corte anteriores
        Recta f_c1_par = ParalelaPunto( f_referencia_par, f_c1_bc);
        Recta f_c3_par = ParalelaPunto( f_referencia_par, f_c3_bc);

    //  2.D. Hallar puntos de corte de las rectas anteriores con la recta f_referencia_9
        MiPunto f_c1_real = PuntoCorte(f_c1_par, f_referencia_9);
        MiPunto f_c3_real = PuntoCorte(f_c3_par, f_referencia_9);

    //  Extrapolar
        f_c1_real.y = f_abc[2].y - f_c1_real.y;
        f_c1_real.x = 18;

        f_c3_real.y = f_abc[2].y - f_c3_real.y;
        f_c3_real.x = 0;

        putText(f_cameraFeed, doubleToString(f_c1_real.y) , Point(x11,y11 + 10), 1, 1, Scalar(240,240,240), 1);
        putText(f_cameraFeed, doubleToString(f_c3_real.y) , Point(x33,y33 - 10), 1, 1, Scalar(240,240,240), 1);

    /** LATERAL **/
    //0.A. Crear recta de referencia real desde l_abc[2] (C).
    l_ref_aux.x = l_abc[2].x + 0.01;
    l_ref_aux.y = l_abc[2].y - 18.0;
    l_referencia_18 = RectaDosPuntos(l_abc[2], l_ref_aux);
    
    //0.B. Crear recta desde ref_aux hasta f_abc[1] (B)
    l_referencia_par = RectaDosPuntos(l_abc[1], l_ref_aux);

    // 1. Crear recta perpendicular al campo que pase por el punto del balón 
    //    y calcular los puntos de corte de dicha recta con las bases del 
    //    trapecio que forma el campo.


    l_balon = ParalelaPunto(RectaDosPuntos(l_fuga[0], l_fuga[4]), balon_lateral);
    MiPunto l_c1 = PuntoCorte(l_balon, l_limite[1]);
    MiPunto l_c3 = PuntoCorte(l_balon, l_limite[3]);

    int x1 = (int)l_c1.x;
    int y1 = (int)l_c1.y;
    int x3 = (int)l_c3.x;
    int y3 = (int)l_c3.y;
    line(l_cameraFeed,Point(x1,y1),Point(x3,y3),Scalar(0,255,255),1);

    //2. Hallar el punto real de l_c1 y l_c3. --> l_c1_real, l_c3_real

    //  2.A. crear rectas desde el punto de fuga 0 y l_c1 y l_c3.
        Recta l_c1_0 = RectaDosPuntos(l_fuga[0], l_c1);
        Recta l_c3_0 = RectaDosPuntos(l_fuga[0], l_c3);
    
    //  2.B. hallar puntos de corte entre las rectas anteriores y l_bc
        MiPunto l_c1_bc = PuntoCorte(l_c1_0, l_bc);
        MiPunto l_c3_bc = PuntoCorte(l_c3_0, l_bc);

    //  2.C. Crear recta paralela a l_referencia_par que pase por los puntos de corte anteriores
        Recta l_c1_par = ParalelaPunto(l_referencia_par, l_c1_bc);
        Recta l_c3_par = ParalelaPunto(l_referencia_par, l_c3_bc);

    //  2.D. Hallar puntos de corte de las rectas anteriores con la recta l_referencia_9
        MiPunto l_c1_real = PuntoCorte(l_c1_par, l_referencia_18);
        MiPunto l_c3_real = PuntoCorte(l_c3_par, l_referencia_18);

    //  Extrapolar
        l_c1_real.y = l_abc[2].y - l_c1_real.y;
        l_c1_real.x = 0;

        l_c3_real.y = l_abc[2].y - l_c3_real.y;
        l_c3_real.x = 9;
    
        putText(l_cameraFeed, doubleToString(l_c1_real.y) , Point(x1,y1 + 10), 1, 1, Scalar(240,240,240), 1);
        putText(l_cameraFeed, doubleToString(l_c3_real.y) , Point(x3,y3 - 10), 1, 1, Scalar(240,240,240), 1);

    
        /********************* creacion de recta de corte **/
        //FRONTAL

        //puntos lateral: 
        //  l_c1_real.x = 0
        //  l_c1_real.y = xxx
        //  l_c3_real.x = 9
        //  l_c3_real.y = xxx

        //1. Convertir puntos laterales al plano frontal.
        MiPunto lc_c1_real, lc_c3_real, lc_c1, lc_c3, lc_solucion;
        Recta lc_c1_bc, lc_c3_bc, lc_sol_1;

        lc_c1_real.x = f_abc[2].x;
        lc_c1_real.y = f_abc[2].y - (l_c1_real.y)/2.0;

        lc_c3_real.x = f_abc[2].x;
        lc_c3_real.y = f_abc[2].y - (l_c3_real.y)/2.0;

        lc_c1_bc = ParalelaPunto(f_referencia_par, lc_c1_real);
        lc_c3_bc = ParalelaPunto(f_referencia_par, lc_c3_real);

        lc_c1 = PuntoCorte(RectaDosPuntos(f_abc[1], f_abc[2]), lc_c1_bc);
        lc_c3 = PuntoCorte(RectaDosPuntos(f_abc[1], f_abc[2]), lc_c3_bc);

        lc_c1_bc = RectaDosPuntos(lc_c1, f_fuga[0]);
        lc_c3_bc = RectaDosPuntos(lc_c3, f_fuga[0]);

        lc_c1 = PuntoCorte(lc_c1_bc, f_limite[3]);
        lc_c3 = PuntoCorte(lc_c3_bc, f_limite[1]);

        lc_c1_bc = RectaDosPuntos(f_fuga[1], lc_c1);
        lc_c3_bc = RectaDosPuntos(f_fuga[1], lc_c3);

        lc_c1 = PuntoCorte(lc_c1_bc, f_limite[0]);
        lc_c3 = PuntoCorte(lc_c3_bc, f_limite[2]);

        lc_solucion = PuntoCorte(f_balon, RectaDosPuntos(lc_c1, lc_c3));
        suelo_balon = lc_solucion;

        /*
        suelo del balon para poder calcular la altura del balon
        Se hace desde el plano frontal ya que es el que menor error puede 
        producir por la situacion de la camara
        */
        

        lc_sol_1 = RectaDosPuntos(lc_solucion, f_fuga[0]);

        x3 = (int)lc_solucion.x;
        y3 = (int)lc_solucion.y;
        x1 = (int)f_fuga[0].x;
        y1 = (int)f_fuga[0].y;

        lc_solucion = PuntoCorte(lc_sol_1, RectaDosPuntos(f_abc[1], f_abc[2]));
        lc_sol_1 = ParalelaPunto(f_referencia_par, lc_solucion);

        lc_solucion = PuntoCorte(lc_sol_1, f_referencia_9);
        

        /****************** SOLUCION.X ******************/
        solucion.x = Trunca(2, f_abc[2].y - lc_solucion.y);
        /************************************************/

        suelo_balon.x = x3;
        suelo_balon.y = y3;


        x1 = (int)balon_frontal.x;
        y1 = (int)balon_frontal.y;
        rectangle(f_cameraFeed, Point(x3-15, y3-10), Point(x3+15, y3+10), Scalar(240,0,0), 1);
        line(f_cameraFeed,Point(x1,y1),Point(x3,y3),Scalar(255,30,255),2);

        x1 = (int)lc_c1.x;
        y1 = (int)lc_c1.y;
        x3 = (int)lc_c3.x;
        y3 = (int)lc_c3.y;
        line(f_cameraFeed,Point(x1,y1),Point(x3,y3),Scalar(0,255,255),2);


        //2. Convertir puntos frontales al plano lateral.

        lc_c1_real.x = l_abc[2].x;
        lc_c1_real.y = l_abc[2].y - (9-f_c1_real.y)*2.0;

        lc_c3_real.x = l_abc[2].x;
        lc_c3_real.y = l_abc[2].y - (9-f_c3_real.y)*2.0;

        lc_c1_bc = ParalelaPunto(l_referencia_par, lc_c1_real);
        lc_c3_bc = ParalelaPunto(l_referencia_par, lc_c3_real);

        lc_c1 = PuntoCorte(RectaDosPuntos(l_abc[1], l_abc[2]), lc_c1_bc);
        lc_c3 = PuntoCorte(RectaDosPuntos(l_abc[1], l_abc[2]), lc_c3_bc);

        lc_c1_bc = RectaDosPuntos(lc_c1, l_fuga[0]);
        lc_c3_bc = RectaDosPuntos(lc_c3, l_fuga[0]);

        lc_c1 = PuntoCorte(lc_c1_bc, l_limite[1]);
        lc_c3 = PuntoCorte(lc_c3_bc, l_limite[3]);

        lc_c1_bc = RectaDosPuntos(l_fuga[1], lc_c1);
        lc_c3_bc = RectaDosPuntos(l_fuga[1], lc_c3);

        lc_c1 = PuntoCorte(lc_c1_bc, l_limite[2]);
        lc_c3 = PuntoCorte(lc_c3_bc, l_limite[0]);

        lc_solucion = PuntoCorte(l_balon, RectaDosPuntos(lc_c1, lc_c3));
        lc_sol_1 = RectaDosPuntos(lc_solucion, l_fuga[0]);

        x3 = (int)lc_solucion.x;
        y3 = (int)lc_solucion.y;
        x1 = (int)l_fuga[0].x;
        y1 = (int)l_fuga[0].y;

        lc_solucion = PuntoCorte(lc_sol_1, RectaDosPuntos(l_abc[1], l_abc[2]));
        lc_sol_1 = ParalelaPunto(l_referencia_par, lc_solucion);

        lc_solucion = PuntoCorte(lc_sol_1, l_referencia_18);
        

        suelo_balon.y = lc_solucion.y;
        /****************** SOLUCION.X ******************/
        solucion.y = Trunca(2, l_abc[2].y - lc_solucion.y);
        /************************************************/

        x1 = (int)balon_lateral.x;
        y1 = (int)balon_lateral.y;
        rectangle(l_cameraFeed, Point(x3-15, y3-10), Point(x3+15, y3+10), Scalar(240,0,0), 1);
        line(l_cameraFeed,Point(x1,y1),Point(x3,y3),Scalar(0,255,255),1);

        x1 = (int)lc_c1.x;
        y1 = (int)lc_c1.y;
        x3 = (int)lc_c3.x;
        y3 = (int)lc_c3.y;
        line(l_cameraFeed,Point(x1,y1),Point(x3,y3),Scalar(100,100,0),1);

        /**********************************************************************/
        //Recta de ayuda.
        Recta t = RectaDosPuntos(l_fuga[0], l_fuga[3]);
        MiPunto aux1 = PuntoCorte(t, l_limite[1]);
        MiPunto aux2 = PuntoCorte(t, l_limite[3]);

        x1 = (int)aux1.x;
        y1 = (int)aux1.y;
        x3 = (int)aux2.x;
        y3 = (int)aux2.y;
        //line(l_cameraFeed,Point(x1,y1),Point(x3,y3),Scalar(0,255,255),2);

        for(int i=0; i<lateral.size(); i++){
            x1 = (int)lateral[i].x;
            y1 = (int)lateral[i].y;
            x3 = (int)lateral[(i+1)%4].x;
            y3 = (int)lateral[(i+1)%4].y;

            line(l_cameraFeed,Point(x1,y1),Point(x3,y3),Scalar(0,255,255),2);
        }

        
        //Recta de ayuda.
        t = RectaDosPuntos(f_fuga[0], f_fuga[3]);
        aux1 = PuntoCorte(t, f_limite[1]);
        aux2 = PuntoCorte(t, f_limite[3]);

        x1 = (int)aux1.x;
        y1 = (int)aux1.y;
        x3 = (int)aux2.x;
        y3 = (int)aux2.y;
        //line(f_cameraFeed,Point(x1,y1),Point(x3,y3),Scalar(0,255,255),2);

        for(int i=0; i<frontal.size(); i++){
            x1 = (int)frontal[i].x;
            y1 = (int)frontal[i].y;
            x3 = (int)frontal[(i+1)%4].x;
            y3 = (int)frontal[(i+1)%4].y;

            line(f_cameraFeed,Point(x1,y1),Point(x3,y3),Scalar(0,255,255),2);
        }

    int xx = f_cameraFeed.size().width;
    putText(f_cameraFeed,"- X: " + doubleToString(solucion.x) + " Y: " + doubleToString(solucion.y) , Point(xx-450,260), 2, 1, Scalar(240,240,240), 1);
    xx = l_cameraFeed.size().width;
    putText(l_cameraFeed,"- X: " + doubleToString(solucion.x) + " Y: " + doubleToString(solucion.y) , Point(xx-450,260), 2, 1, Scalar(240,240,240), 1);

    //cout << "Suelo balon : " << suelo_balon.x << ", " << suelo_balon.y << " - " << balon_frontal.x << ", " << balon_frontal.y << endl;
    solucion3D.x = solucion.x;
    solucion3D.z = solucion.y;
    solucion3D.y = AlturaBalon(f_cameraFeed, solucion, balon_frontal, frontal, f_limite, f_fuga, f_abc, true);


    return solucion3D;
}



vector<float> Parabola(MiPunto a, MiPunto b, MiPunto c){
    vector<float> solucion;

    float A, B, C, div1, div2;

    div1 = (c.x-a.x)*(b.y-a.y) - (b.x-a.x)*(c.y-a.y);
    div2 = (b.x*b.x - a.x*a.x)*(c.x-a.x) - (c.x*c.x - a.x*a.x)*(b.x-a.x);
    A = div1/div2;

    div1 = c.y-a.y - A*(c.x*c.x - a.x*a.x);
    div2 = c.x - a.x;
    B = div1/div2;

    C = a.y - A*a.x*a.x - B*a.x;

    solucion.push_back(A);
    solucion.push_back(B);
    solucion.push_back(C);

    return solucion;
}


float DistanciaPuntosParabola(vector<float> ParX, vector<float> ParZ, MiPunto3D inicial, MiPunto3D final, int precision){

    MiPunto iniX, iniZ, finX, finZ;
    MiPunto x_aux, x_siguiente_aux;
    MiPunto z_aux, z_siguiente_aux;
    float distX, distZ, dist_Real, incX, incZ;
    Recta recta_cortes, recta_aux, recta_aux_paralela;
    vector<MiPunto> centrales, centrales_parabola;

    distX = 0;
    distZ = 0;
    iniX.x = inicial.x; iniX.y = inicial.y;
    iniZ.x = inicial.z; iniZ.y = inicial.y; 
    finX.x = final.x; finX.y = final.y;
    finZ.x = final.z; finZ.y = final.y; 


    //Parabola X
    recta_cortes = RectaDosPuntos(iniX, finX);

    x_aux = finX;
    x_aux.y += precision;
    recta_aux_paralela = RectaDosPuntos(iniX, x_aux);
    

    for(int i=0; i<precision+1; i++){
        x_aux.y = finX.y + i;
        recta_aux = ParalelaPunto(recta_aux_paralela, x_aux);
        centrales.push_back(PuntoCorte(recta_cortes, recta_aux));
    }
    
/*
    for(int i=0; i<centrales.size(); i++){
        ImprimeMiPunto(centrales[i]);
        cout << " - "; 
    }
*/
    for(int i=0; i<centrales.size(); i++){
        x_aux = centrales[i];
        x_aux.y = ParX[0]*x_aux.x*x_aux.x + ParX[1]*x_aux.x + ParX[2];
        centrales_parabola.push_back(x_aux);
    }

    //Calcular la distancia entre los puntos consecutivos de centrales_parabola
    for(int i=0; i<centrales_parabola.size()-1; i++)
        distX += DistanciaPuntos(centrales_parabola[i], centrales_parabola[i+1]);


    //Parabola Z
    centrales.clear();
    centrales_parabola.clear();

    recta_cortes = RectaDosPuntos(iniZ, finZ);

    x_aux = finZ;
    x_aux.y += precision;
    recta_aux_paralela = RectaDosPuntos(iniZ, x_aux);
    

    for(int i=precision; i>1; i--){
        x_aux.y = finZ.y + i;
        recta_aux = ParalelaPunto(recta_aux_paralela, x_aux);
        centrales.push_back(PuntoCorte(recta_cortes, recta_aux));
    }
    centrales.push_back(finZ);

    for(int i=0; i<centrales.size(); i++){
        x_aux = centrales[i];
        x_aux.y = ParZ[0]*x_aux.x*x_aux.x + ParZ[1]*x_aux.x + ParZ[2];
        centrales_parabola.push_back(x_aux);
    }

    //Calcular la distancia entre los puntos consecutivos de centrales_parabola
    for(int i=0; i<centrales_parabola.size()-1; i++)
        distZ += DistanciaPuntos(centrales_parabola[i], centrales_parabola[i+1]);



    dist_Real = sqrt((distX*distX) + (distZ*distZ));

    return dist_Real;

}

void ControlMenu(bool &pause, bool &debugMode, bool &trackingEnabled, bool &exit_program, bool &salir_tracking, bool &one_frame){

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
                                cout << "Code Resumed" << endl;
                            break;
                            case 39:
                                pause = false;
                            break;
                            case 27: //'esc' key has been pressed, exit program.
                                salir_tracking = false;
                            break;
                            case 110: //'n' key has been pressed, next frame.
                                pause = false;
                                one_frame = true;
                            break;
                        }
                    }

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
                                cout << "Code Resumed" << endl;
                            break;
                            case 39:
                                pause = false;
                            break;
                            case 27: //'esc' key has been pressed, exit program.
                                salir_tracking = false;
                            break;
                            case 110: //'n' key has been pressed, next frame.
                                pause = false;
                                one_frame = true;
                            break;
                        }
                    }
                }
            break;
        }

        if(one_frame) pause = true;
        one_frame = false;
}

float DistanciaPuntos3D(MiPunto3D a, MiPunto3D b){

    MiPunto ax, az, bx, bz;
    float dist_x, dist_z, solucion;

    ax.x = a.x;
    ax.y = a.y;
    az.x = a.z;
    az.y = a.y;

    bx.x = b.x;
    bx.y = b.y;
    bz.x = b.z;
    bz.y = b.y;

    dist_x = DistanciaPuntos(ax, bx);
    dist_z = DistanciaPuntos(az, bz);

    solucion = sqrt(dist_z*dist_z + dist_x*dist_x);

    return solucion;

}


float Velocidad2Puntos(MiPunto4D a, MiPunto4D b){

    float distancia = DistanciaPuntos3D(a.coordenada, b.coordenada);
    float dif_tiempo = a.segundos - b.segundos;
    float velocidad = distancia/dif_tiempo;

    return velocidad;
}