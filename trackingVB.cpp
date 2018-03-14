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

#include "opencv2/imgproc/imgproc_c.h"
//#include <opencv/core/MatOfPoint2f.hpp>

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

bool limite = true;

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************


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
 

MiPunto searchForMovement(Mat thresholdImage, Mat &cameraFeed, char camara, vector<MiPunto> terrero, vector<MiPunto> &anteriores, vector<float> &media_area, vector<MiPunto> &puntos_desechables){
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
   	int maximo = 2500;
   	MiPunto p_anterior;
   	std::vector<int> v_index_color;
   	Scalar color( 10, 255, 255);
   	float area_balon, distancia_solucion, MAXcolor, altura_minima;
   	float div = 3.0;
   	float distancia_minima = 10;
   	MiPunto aux, p2, p3;
	float d2, d3;

   	MAXcolor = 0;

   	if(camara == 'L'){
   		minimo = 100;
   		maximo = 1200;

   		div = 6.0;
   	}

   	if(terrero.size() == 4){
   		float d = (DistanciaPuntos(terrero[1], terrero[2]))/div;;
   		Recta r = RectaDosMiPunto(terrero[1], terrero[2]);
   		int x = DistanciaPuntos(terrero[1], terrero[2])/2;
   		altura_minima = r.m*x+r.k;
   		altura_minima-=d;

		line(cameraFeed, Point((int)0, (int)altura_minima), Point((int)1900,(int)altura_minima),Scalar(0,255,0),3);
   	}
	else{
		altura_minima = -1;
	}

   	if(anteriores.size() == 0){
   		p_anterior.x = -1;
   		p_anterior.y = -1;
   	}
   	else{
   		p_anterior = anteriores[anteriores.size()-1];
   	}


   	if(media_area.size()>150){

   		float avgarea = 0.0;
    	for(int i=0; i<media_area.size(); i++){
    		avgarea = avgarea + media_area[i];
    	}
    	avgarea/=media_area.size();

    	avgarea = Trunca(2, avgarea);
    	//cout << "Actualizo maximos y minimos\n";
    	minimo = (avgarea/2)-((avgarea/8));
    	maximo = avgarea*2+((avgarea/8));
   	}

    //find contours of filtered image using openCV findContours function
    findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );// retrieves all contours
    
    Mat labels = cv::Mat::zeros(temp.size(), CV_8UC1);     
	vector<float> cont_avgs(contours.size(), 0.f); // This contains the averages of each contour
	std::vector<int> idx_conts, idx_conts_2, idx_conts_3;
    
    //1. Cribo los contornos por tamaños, seleccionando los posibles valores para un balon en movimiento
    for(int idx = 0; idx >= 0; idx = hierarchy[idx][0])
        if((contourArea(contours[idx]) < maximo) && (contourArea(contours[idx]) > minimo))
        	idx_conts.push_back(idx);
    
    //2. Calcula la media de los contornos seleccionados en paso 1.
    cv::drawContours(labels, contours, -1, cv::Scalar(255), CV_FILLED);
    for (int i = 0; i < idx_conts.size(); i++){
	    cv::Rect roi = cv::boundingRect(contours[idx_conts[i]]);
	    cv::Scalar mean = cv::mean(cameraFeed(roi), labels(roi));
	    cont_avgs[i] = mean[0];
	}

   	for(int i=0; i<idx_conts.size(); i++)
   		if((contourArea(contours[idx_conts[i]]) >= minimo) && (contourArea(contours[idx_conts[i]]) <= maximo)){ // && (hierarchy[i][3] == -1)){
   			cont_avgs_1.push_back(cont_avgs[idx_conts[i]]);
   			idx_conts_2.push_back(idx_conts[i]);
   		}

	/****

		3. Descartar lo que no pueda ser un balón. Formas muy rectangulares
   		
			Busco:
				- Punto mas a la derecha
				- Punto mas a la izda
				- Punto mas alto
				- Punto mas bajo

			Si la relacion altura-anchura es menor al 30%, lo descarto.

			Además, si el punto del centro no está incluido en el contorno, se excluye.
			(Esto excluye formas lineales parabólicas)
   	****/
	
	double dist_x, dist_y, rel_x, rel_y;

	for(int i=0; i<idx_conts_2.size(); i++){
        objectBoundingRectangleAUX = boundingRect(contours.at(idx_conts_2[i]));
        double w_pos = objectBoundingRectangleAUX.width;
        double h_pos = objectBoundingRectangleAUX.height;
        
        rel_x = w_pos/h_pos;
        rel_y = h_pos/w_pos;

		if((rel_x < 3) && (rel_y < 3)){
			Point p = Point(objectBoundingRectangleAUX.x+objectBoundingRectangleAUX.width/2,objectBoundingRectangleAUX.y+objectBoundingRectangleAUX.height/2);
			Mat m = Mat(contours[idx_conts_2[i]]);
			double r = pointPolygonTest(m,p, false);

			if(r == 1){
        		cont_avgs_2.push_back(cont_avgs_1[i]);
        		idx_conts_3.push_back(idx_conts_2[i]);//Indices de los contornos que pueden ser soluciones (el balon)
			}
		}
	}
	
	// **Pinto los contornos seleccionados
	for(int idx = 0; idx < idx_conts_3.size(); idx++){
		Scalar color( rand()%255, rand()%255, rand()%255);
        drawContours( cameraFeed, contours, idx_conts_3[idx], color, CV_FILLED, 100, hierarchy );
	}
	
	//4. Selecciono el indice de los contornos que tienen una media de color parecido a blanco.
	//   (Parecido a blanco es un numero menor a 1.)
    if(idx_conts_3.size()>0){
    	for(int i=0; i< cont_avgs_2.size(); i++){ // Color (ball) is less than 1.
    		if(MAXcolor == cont_avgs_2[i])
    			v_index_color.push_back(i);
    	}
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

    	area_balon = contourArea(contours.at(idx_conts_3[idx_conts_3.size()-1]));

    	objectBoundingRectangle = boundingRect(contours.at(idx_conts_3[idx_conts_3.size()-1]));
    	//objectBoundingRectangle = boundingRect(largestContourVec.at(v_index_color.at(v_index_color.size()-1)));
		p_actual.x = objectBoundingRectangle.x + objectBoundingRectangle.width/2.0;
		p_actual.y = objectBoundingRectangle.y + objectBoundingRectangle.height/2.0;
/*
		if(p_anterior.x == -1){
			p_anterior.x = p_actual.x;
			p_anterior.y = p_actual.y;
		}
*/
    	//distancia_solucion = DistanciaPuntos(p_anterior, p_actual);
    	

    	//if(camara == 'L') cout << distancia_solucion << camara << endl;

    	if((anteriores.size() < 50)){
    		point_sol.x = p_actual.x;
	    	point_sol.y = p_actual.y;
	    	area_balon = contourArea(contours.at(idx_conts_3[idx_conts_3.size()-1]));
    	}
    	else{

    		/***
    		6.	Para todoslos contornos posibles:
					Si-->	Contorno alto?
							¿Está cerca del anterior balon?
								si->actualizar posicion
								no->iguiente
					No-->
						Buscar contorno mas proximo a las 3 ultimas posiciones.
			****/

    		
    		p2 = anteriores.at(anteriores.size()-2);
			p3 = anteriores.at(anteriores.size()-3);

    		if((p_actual.y < altura_minima && !Desechable(p_actual, puntos_desechables))){
    			//Escocger entre todos el mas cercano al balon anterior
    			//Similar a cuando está por debajo de la linea
				if(PuntosIguales(p_anterior, p2) && PuntosIguales(p_anterior, p3))
					puntos_desechables.push_back(p_anterior);
			}
    		else{
    			p_actual.x = -1;
    			p_actual.y = -1;

    			//cout << endl << camara << " - " << p_anterior.x << "," << p_anterior.y << " " << p2.x << "," << p2.y << " " << p3.x << "," << p3.y;

    			if(PuntosIguales(p_anterior, p2) && PuntosIguales(p_anterior, p3))
					puntos_desechables.push_back(p_anterior);
				else{

		    		while((p_actual.x == -1) && (distancia_minima < 500)){
			    		for(int i=0; i<idx_conts_3.size(); i++){
			    			objectBoundingRectangle = boundingRect(contours.at(idx_conts_3[i]));

							aux.x = objectBoundingRectangle.x + objectBoundingRectangle.width/2.0;
							aux.y = objectBoundingRectangle.y + objectBoundingRectangle.height/2.0;
			    			distancia_solucion = DistanciaPuntos(p_anterior, aux);
			    			d2 = DistanciaPuntos(p2, aux);
			    			d3 = DistanciaPuntos(p3, aux);

		    				if(d3 < distancia_minima){
			    				//cout << "- actualizo ";
			    				p_actual = aux;
			    				area_balon = contourArea(contours.at(idx_conts_3[i]));
			    				distancia_minima = d3;
			    			}
			    			if(d2 < distancia_minima){
			    				//cout << "- actualizo ";
			    				p_actual = aux;
			    				area_balon = contourArea(contours.at(idx_conts_3[i]));
			    				distancia_minima = d2;
			    			}
			    			if(distancia_solucion < distancia_minima){
			    				//cout << "- actualizo ";
			    				p_actual = aux;
			    				area_balon = contourArea(contours.at(idx_conts_3[i]));
			    				distancia_minima = distancia_solucion;
			    			}
			    			distancia_minima+=5;
			    		}
		    		}
		    	}
    		}
    	}
		//update the objects positions by changing the 'theObject' array values

		point_sol.x = p_actual.x;
	    point_sol.y = p_actual.y;

	    media_area.push_back(area_balon);
	    anteriores.push_back(point_sol);

    }
    else{
	    point_sol.x = -1;
	    point_sol.y = -1;
    }
    //make some temp x and y variables so we dont have to type out so much
    putText(cameraFeed,"- Area: " + doubleToString(area_balon) , Point(50,900), 2, 1, Scalar(240,240,240), 2);
    putText(cameraFeed,"- Distancia: " + doubleToString(distancia_solucion) , Point(50,950), 2, 1, Scalar(240,240,240), 2);
    putText(cameraFeed,"- Actual: " + doubleToString(point_sol.x) + ", " + doubleToString(point_sol.y) , Point(50,1000), 2, 1, Scalar(240,240,240), 2);
    putText(cameraFeed,"- Anterior: " + doubleToString(p_anterior.x) + ", " + doubleToString(p_anterior.y) , Point(50,1050), 2, 1, Scalar(240,240,240), 2);


    return point_sol;
}




int main(int argc, char* argv[]) {

	string video_trackingF = "../videos/videos_sinc/fondo.mp4";
	string video_trackingL = "../videos/videos_sinc/lateral.mp4";

	string nombre_archivo = "Femenino.txt";

	int contador_frame, num_frame_segundo;
	int ayuda = 0;

	char F = 'F';
	char L = 'L';

	string Front = "threshold frontal";
	string Lat = "threshold lateral";

    std::vector<float> avg_frontal, avg_lateral;
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
	std::vector<float> avg_color_f, avg_color_l;

	vector<MiPunto> frontal_points, lateral_points, desechables_f, desechables_l;

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
    Mat frame1,frame2,thresholdImage;
    //their grayscale images (needed for absdiff() function)
    //video capture object.
    VideoCapture capture;
    Mat frame1L,frame2L, thresholdImageL;
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

	  	while(pause == true){
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

    	captureL.read(frame1L);
    	captureL.read(frame2L);
	    
        //Frontal -->
        thresholdImage = ConvertImageThreshold(frame1, frame2, debugMode, Front);
        // -----------
        //Lateral -->
        thresholdImageL = ConvertImageThreshold(frame1L, frame2L, debugMode, Lat);
        // -----------
        
        //if tracking enabled, search for contours in our thresholded image
        if(trackingEnabled){

            point = searchForMovement(thresholdImage,frame1, F, puntos, frontal_points, avg_frontal, desechables_f);
            pointL = searchForMovement(thresholdImageL,frame1L, L, puntosL, lateral_points, avg_lateral, desechables_l);


            if(!imprime && point.y > 0 && pointL.y > 0){

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


				DrawPlayGround(frame1, puntos, 1);
				DrawPlayGround(frame1L, puntosL, 0);

                DrawObject(frame1, point, coordenada_balon);
                DrawObject(frame1L, pointL, coordenada_balon);
                
                if(coordenada_balon.y >= 0 ){
                	MiPunto4D p(tiempo_frame, coordenada_balon);
              		coordenadas_balon.push_back(p);
                }

                //Vista_Frontal();
        		Vista_Lateral(coordenadas_balon);
				
                line(frame1L,Point((int)corte_basesL[0].x, (int)corte_basesL[0].y), 
                								Point((int)corte_basesL[1].x,(int)corte_basesL[1].y),Scalar(0,255,0),1);
                line(frame1, Point((int)corte_basesF[0].x, (int)corte_basesF[0].y), 
                								Point((int)corte_basesF[1].x,(int)corte_basesF[1].y),Scalar(0,255,0),1);
            }
        }
		
		/*******************************************************************
        							VISUALIZACION
    	********************************************************************/

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
		
        



    	/*************************************************************************
										MENU
    	**************************************************************************/
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
    	
    	if(puntos.empty()){
	        //Inicializar campo de juego
	        //Frontal
		    puntos.push_back(MiPunto(190, 842));
		    puntos.push_back(MiPunto(1634, 875));
		    puntos.push_back(MiPunto(1180, 668));
		    puntos.push_back(MiPunto(660, 658));

		    //Lateral
		    puntosL.push_back(MiPunto(163, 821));
		    puntosL.push_back(MiPunto(348, 684));
		    puntosL.push_back(MiPunto(1855, 750));
		    puntosL.push_back(MiPunto(1470, 645));
		}
        
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

    cout << "\n\nTotal Puntos laterales: " << lateral_points.size();
    cout << "\nTotal Puntos Frontales: " << frontal_points.size() << endl;

    //Encontrar puntos maximos y minimos relativos.
    vector <MiPunto> lateral_relativos = MaxMinRelativos(lateral_points);
    vector <MiPunto> frontal_relativos = MaxMinRelativos(frontal_points);

    for(int i=0; i<lateral_relativos.size(); i++){
    	cout << "(" << lateral_relativos[i].x << ", " << lateral_relativos[i].y << ")\t ";
    }
    cout << endl;

    //DrawParabola(lateral_relativos);

    ImprimeArchivo(nombre_archivo, coordenadas_balon);

    ABC(puntos);
    ABC(puntosL);

    return 0;
}

