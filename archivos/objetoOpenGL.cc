#include "objetoOpenGL.h"

/******************** - OPENGL - **********************/

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
}
//**************************************************************************
//  Creacion de los  vectores necesarios para la creacion de un modelo
//**************************************************************************

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

void terreno_juego(vector<float> &vertices, vector<int> &caras){
  //0
  vertices.push_back(0);
  vertices.push_back(0);
  vertices.push_back(0);
  //1
  vertices.push_back(9);
  vertices.push_back(0);
  vertices.push_back(0);
  //2
  vertices.push_back(9);
  vertices.push_back(0);
  vertices.push_back(6);
  //3
  vertices.push_back(9);
  vertices.push_back(0);
  vertices.push_back(9);
  //4
  vertices.push_back(9);
  vertices.push_back(0);
  vertices.push_back(12);
  //5
  vertices.push_back(9);
  vertices.push_back(0);
  vertices.push_back(18);
  //6
  vertices.push_back(0);
  vertices.push_back(0);
  vertices.push_back(18);
  //7
  vertices.push_back(0);
  vertices.push_back(0);
  vertices.push_back(12);
  //8
  vertices.push_back(0);
  vertices.push_back(0);
  vertices.push_back(9);
  //9
  vertices.push_back(0);
  vertices.push_back(0);
  vertices.push_back(6);

  for(int i=0; i<vertices.size(); i+=3){
    vertices[i]-=4.5;
    vertices[i+2]-=9;
  }


  caras.push_back(0);
  caras.push_back(1);
  caras.push_back(2);

  caras.push_back(0);
  caras.push_back(2);
  caras.push_back(9);

  caras.push_back(9);
  caras.push_back(2);
  caras.push_back(3);

  caras.push_back(9);
  caras.push_back(3);
  caras.push_back(8);

  caras.push_back(8);
  caras.push_back(3);
  caras.push_back(4);

  caras.push_back(8);
  caras.push_back(4);
  caras.push_back(7);

  caras.push_back(7);
  caras.push_back(4);
  caras.push_back(5);

  caras.push_back(7);
  caras.push_back(5);
  caras.push_back(6);
}

void red_juego(vector<float> &vertices, vector<int> &caras){
  //vertices
  //0
  vertices.push_back(9);
  vertices.push_back(1.43);
  vertices.push_back(9);
  //1
  vertices.push_back(9);
  vertices.push_back(2.43);
  vertices.push_back(9);
  //2
  vertices.push_back(0);
  vertices.push_back(1.43);
  vertices.push_back(9);
  //3
  vertices.push_back(0);
  vertices.push_back(2.43);
  vertices.push_back(9);

  for(int i=0; i<vertices.size(); i+=3){
    vertices[i]-=4.5;
    vertices[i+2]-=9;
  }

  //caras
  //0
  caras.push_back(0);
  caras.push_back(1);
  caras.push_back(3);
  //1
  caras.push_back(0);
  caras.push_back(3);
  caras.push_back(2);
}

void campo_voleibol(vector<float> &tvertices, vector<int> &tcaras, vector<float> &rvertices, vector<int> &rcaras){
  terreno_juego(tvertices, tcaras);
  red_juego(rvertices, rcaras);
}

//*************************************************************************
// Tetraedro NUEVO
//*************************************************************************

void nuevo_tetraedro(vector<float> &pvertices, vector<int> &pcaras, float altura){

  //Vertices a mi libre eleccion
  pvertices.push_back(0);
  pvertices.push_back(-1*altura/2.0);
  pvertices.push_back(altura/2.0);

  pvertices.push_back(altura/2.0);
  pvertices.push_back(-1*altura/2.0);
  pvertices.push_back(-1*altura/2.0);
  
  pvertices.push_back(-1*altura/2.0);
  pvertices.push_back(-1*altura/2.0);
  pvertices.push_back(-1*altura/2.0);
  
  pvertices.push_back(0);
  pvertices.push_back(altura/2.0);
  pvertices.push_back(0);

  //Caras

  pcaras.push_back(0);
  pcaras.push_back(1);
  pcaras.push_back(2);

  pcaras.push_back(0);
  pcaras.push_back(1);
  pcaras.push_back(3);
  
  pcaras.push_back(1);
  pcaras.push_back(2);
  pcaras.push_back(3);
  
  pcaras.push_back(2);
  pcaras.push_back(0);
  pcaras.push_back(3);


}
//************************************************************************
//Funciones que crean los vertices y caras de revolucion
//************************************************************************




//**************************************************************************
// Funcion que dibuja los objetos
//**************************************************************************


void draw_points(vector<float> vertices)
{

  int i;
  int n_puntos = vertices.size()/3;


  glColor3f(0,1,0);
  glPointSize(4);

  glBegin(GL_POINTS);
    
    for (i=0;i<n_puntos;i++){
      glVertex3f(vertices[i*3], vertices[i*3+1], vertices[i*3+2]);
    }

  glEnd();

}

void draw_aristas(vector<float> vertices, vector<int> aristas){

  glBegin(GL_LINES);
  
  for(int i=0; i<aristas.size()/2; i++){
    
    glVertex3f(vertices[aristas[i*2]*3], vertices[aristas[i*2]*3+1], vertices[aristas[i*2]*3+2]);
    glVertex3f(vertices[aristas[i*2+1]*3], vertices[aristas[i*2+1]*3+1], vertices[aristas[i*2+1]*3+2]);
  
  }

  glEnd();

}


void draw_caras(vector<float> vertices, vector<int> caras){

  glColor3f(0,0.8,0.2);
  
  glBegin (GL_LINES);
    
    for (int i=0; i<caras.size(); i+=3) {
    
    int vertice1 = caras[i];
    int vertice2 = caras[i+1];
    int vertice3 = caras[i+2];
    
    glVertex3f (vertices[vertice1*3], vertices[vertice1*3+1], vertices[vertice1*3+2]);
    glVertex3f (vertices[vertice2*3], vertices[vertice2*3+1], vertices[vertice2*3+2]);
    
    glVertex3f (vertices[vertice2*3], vertices[vertice2*3+1], vertices[vertice2*3+2]);
    glVertex3f (vertices[vertice3*3], vertices[vertice3*3+1], vertices[vertice3*3+2]);
    
    glVertex3f (vertices[vertice3*3], vertices[vertice3*3+1], vertices[vertice3*3+2]);
    glVertex3f (vertices[vertice1*3], vertices[vertice1*3+1], vertices[vertice1*3+2]);
    
  }

    glEnd();


}


void draw_solido(vector<float> vertices, vector<int> caras){
  
  glColor3f(0.2,0.2,0.8);
  
  glBegin (GL_TRIANGLES);
    
    for (int i=0; i<caras.size(); i+=3) {
    
    int vertice1 = caras[i], vertice2 = caras[i+1], vertice3 = caras[i+2];
    
    glVertex3f (vertices[vertice1*3], vertices[vertice1*3+1], vertices[vertice1*3+2]);
    glVertex3f (vertices[vertice2*3], vertices[vertice2*3+1], vertices[vertice2*3+2]);
    glVertex3f (vertices[vertice3*3], vertices[vertice3*3+1], vertices[vertice3*3+2]);
  }
      
    glEnd();

}

void draw_ajedrez(vector<float> vertices, vector<int> caras){


  glBegin (GL_TRIANGLES);
    
    for (int i=0; i<caras.size(); i+=3) {
    
    if (i%2 == 0)
      glColor3f(5,1,0.2);
    else 
      glColor3f(0,1,0);
    
    int vertice1 = caras[i], vertice2 = caras[i+1], vertice3 = caras[i+2];
    
    glVertex3f (vertices[vertice1*3], vertices[vertice1*3+1], vertices[vertice1*3+2]);
    glVertex3f (vertices[vertice2*3], vertices[vertice2*3+1], vertices[vertice2*3+2]);
    glVertex3f (vertices[vertice3*3], vertices[vertice3*3+1], vertices[vertice3*3+2]);
  }
      
    glEnd();
}


//Pinta todo


void draw_all(vector<float> vertices, vector<int> caras){

    draw_solido(vertices, caras);
    draw_points(vertices);
    draw_caras(vertices, caras);
}

void draw_campo_juego_lines(vector<float> tvertices, vector<float> rvertices){

  glColor3f(1,1,1);
  glLineWidth(4.0);
  glBegin (GL_LINES);
  
    glVertex3f (tvertices[0], tvertices[1], tvertices[2]); //0
    glVertex3f (tvertices[3], tvertices[4], tvertices[5]); //1

    glVertex3f (tvertices[9*3], tvertices[9*3+1], tvertices[9*3+2]); //9
    glVertex3f (tvertices[2*3], tvertices[2*3+1], tvertices[2*3+2]); //2
    
    glVertex3f (tvertices[3*3], tvertices[3*3+1], tvertices[3*3+2]); //3
    glVertex3f (tvertices[8*3], tvertices[8*3+1], tvertices[8*3+2]); //8

    glVertex3f (tvertices[4*3], tvertices[4*3+1], tvertices[4*3+2]); //4
    glVertex3f (tvertices[7*3], tvertices[7*3+1], tvertices[7*3+2]); //7
    
    glVertex3f (tvertices[5*3], tvertices[5*3+1], tvertices[5*3+2]); //5
    glVertex3f (tvertices[6*3], tvertices[6*3+1], tvertices[6*3+2]); //6

    glVertex3f (tvertices[0], tvertices[1], tvertices[2]); //0
    glVertex3f (tvertices[6*3], tvertices[6*3+1], tvertices[6*3+2]); //6
    
    glVertex3f (tvertices[3], tvertices[4], tvertices[5]); //1
    glVertex3f (tvertices[5*3], tvertices[5*3+1], tvertices[5*3+2]); //5

    glEnd();

    glLineWidth(1.0);
}

void draw_campo_juego_solid(vector<float> tvertices, vector<float> rvertices){

  glColor3f(0.459,0.537,0.75);
  
  glBegin (GL_POLYGON);
  
    glVertex3f (tvertices[0], tvertices[1], tvertices[2]); //0
    glVertex3f (tvertices[3], tvertices[4], tvertices[5]); //1
    
    glVertex3f (tvertices[5*3], tvertices[5*3+1], tvertices[5*3+2]); //5
    glVertex3f (tvertices[6*3], tvertices[6*3+1], tvertices[6*3+2]); //6

    glEnd();
}


void puntos_tracking_ball(vector <MiPunto4D> coordenadas, int inicio, int fin){

  int i;
  int n_puntos = coordenadas.size();

  unsigned int microseconds = 3*1000000/24.0;;
  //cout << microseconds << " ";

  
  
  for (i=inicio; i<fin; i++){
    
    glColor3f(0.03,1,0.7);
    glPointSize(i%10);
    
    glBegin(GL_POINTS);
      if(fin < coordenadas.size()){
        glPointSize(i%10);

        glVertex3f(coordenadas[i].coordenada.x - 4.5 , coordenadas[i].coordenada.y, coordenadas[i].coordenada.z - 9.0);
      }
    glEnd();

  }

  glBegin(GL_LINES);
    
    if(fin < coordenadas.size()){
      for (i=inicio; i<fin-1; i++){
        
        glVertex3f(coordenadas[i].coordenada.x - 4.5 , coordenadas[i].coordenada.y, coordenadas[i].coordenada.z - 9.0);
        glVertex3f(coordenadas[i+1].coordenada.x - 4.5 , coordenadas[i+1].coordenada.y, coordenadas[i+1].coordenada.z - 9.0);
      }
    }
  glEnd();

  usleep(microseconds);
}

bool aux_pause = false;

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
  draw_campo_juego_lines(v_basico1, v_basico2);
  draw_campo_juego_solid(v_basico1, v_basico2);
  
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
    case GLUT_KEY_LEFT:Observer_angle_y--;break;
    case GLUT_KEY_RIGHT:Observer_angle_y++;break;
    case GLUT_KEY_UP:Observer_angle_x--;break;
    case GLUT_KEY_DOWN:Observer_angle_x++;break;
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

  // se indica cual sera el color para limpiar la ventana (r,v,a,al)
  // blanco=(1,1,1,1) rojo=(1,0,0,1), ...
  glClearColor(1,1,1,1);

  // se habilita el z-bufer
  glEnable(GL_DEPTH_TEST);
  //
  change_projection();
  //
  glViewport(0,0,UI_window_width,UI_window_height);
}

