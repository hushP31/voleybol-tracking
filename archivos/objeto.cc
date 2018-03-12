#include "objeto.h"

//***************************************************************************
// Puntos
//***************************************************************************

Puntos_::Puntos_(){}

void Puntos_::drawPuntos(){
    glPointSize(1);
    glColor3f(255,0,0);

  	glBegin(GL_POINTS);
  	for (int i = 0; i < Vertices.size(); i++){
  		glVertex3fv((GLfloat *) &Vertices[i]);
  	}
  	glEnd();
}

//***************************************************************************
// Alambre
//***************************************************************************

Triangulo_::Triangulo_(){}

void Triangulo_::drawAlambre()
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glLineWidth(1);
    glColor3f(150,150,150);

    glBegin(GL_TRIANGLES);
  	for (int i = 0; i < Triangulos.size(); i++)
    {
  		glVertex3fv((GLfloat *) &Vertices[Triangulos[i]._0]);
  		glVertex3fv((GLfloat *) &Vertices[Triangulos[i]._1]);
  		glVertex3fv((GLfloat *) &Vertices[Triangulos[i]._2]);
  	}
  	glEnd();
}

//***************************************************************************
// Solido
//***************************************************************************

void Triangulo_::drawSolido()
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  	glColor3f(150,150,150);

  	glBegin(GL_TRIANGLES);
  	for (int i = 0; i < Triangulos.size(); i++)
  	{
    		glVertex3fv((GLfloat *) &Vertices[Triangulos[i]._0]);
    		glVertex3fv((GLfloat *) &Vertices[Triangulos[i]._1]);
    		glVertex3fv((GLfloat *) &Vertices[Triangulos[i]._2]);
  	}
  	glEnd();

}

//***************************************************************************
// Triangulos
//***************************************************************************

void Triangulo_::drawAjedrez()
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  	glBegin(GL_TRIANGLES);

  	for (int i = 0; i < Triangulos.size(); i++){
  		if(i%2==0) glColor3f(0.5, 0.5, 0.5);
  		else glColor3f(0,0,255);
  		glVertex3fv((GLfloat *) &Vertices[Triangulos[i]._0]);
  		glVertex3fv((GLfloat *) &Vertices[Triangulos[i]._1]);
  		glVertex3fv((GLfloat *) &Vertices[Triangulos[i]._2]);
  	}
  	glEnd();
}

//***************************************************************************
// DibujarTodo
//***************************************************************************

void Triangulo_::drawTodo(){
    drawPuntos();
    drawAlambre();
    drawSolido();

}

//***************************************************************************
// Leer Ply
//***************************************************************************

Ply_::Ply_(){}

void Ply_::leer_ply(char *file){

    _file_ply fichero;
    vector<float> Vertices_f;
    vector<int> triangulos_f;

    int num_ver,num_tri;
    int j,k;

    fichero.open(file);
    fichero.read(Vertices_f,triangulos_f);

    num_ver = Vertices_f.size()/3;
    num_tri = triangulos_f.size()/3;

    Vertices.resize(num_ver);
    Triangulos.resize(num_tri);
    j= 0;

    for (int i=0;i<Vertices_f.size();i=i+3){
	 	Vertices[j].x = Vertices_f[i];
        Vertices[j].y = Vertices_f[i+1];
        Vertices[j].z = Vertices_f[i+2];
        j++;
    }
    k=0;

    for (int i=0;i<triangulos_f.size();i=i+3){
		Triangulos[k]._0 = triangulos_f[i];
        Triangulos[k]._1 = triangulos_f[i+1];
        Triangulos[k]._2 = triangulos_f[i+2];
        k++;
    }
}
//***************************************************************************
// Revolucion_
//***************************************************************************

Revolucion_::Revolucion_(){}

void Revolucion_::revol_puntos(vector<_vertex3f> &rev,float lados,float angI_,float angF_){

	int p=rev.size(),k;

	_vertex3f aux;
	_vertex3i aux_tri;

	float angI = angI_;
	float angF = angF_;
	float anguloR;

	anguloR = M_PI/180*angI;
	for (int i=0; i<p; i++)
	{
		aux.x=rev[i].x*cos(anguloR) + rev[i].z*sin(anguloR);
		aux.y=rev[i].y;
		aux.z=-rev[i].x*sin(anguloR) + rev[i].z*cos(anguloR);
		Vertices.push_back(aux);
	}

	angI = M_PI/180*angI;
	angF = M_PI/180*angF;
	int lad=0;
	
	for(int j=0; j<lados+1;j++)
	{
		anguloR= (2*M_PI*j)/lados;
		if( anguloR >= angI && anguloR <= angF)
		{
			lad++;
			for (int i=0; i<p; i++)
			{
				aux.x=rev[i].x*cos(anguloR) + rev[i].z*sin(anguloR);
				aux.y=rev[i].y;
				aux.z=-rev[i].x*sin(anguloR) + rev[i].z*cos(anguloR);
				Vertices.push_back(aux);
			}
		}	
	}

	// Aristas
	for(int j=1; j<lad;j++)
	{
		for (int i=0; i<p-1; i++)
		{
			aux_tri._0=j*p+i;
			aux_tri._1=j*p+i+p;
			aux_tri._2=j*p+i+p+1;
			Triangulos.push_back(aux_tri);

			aux_tri._0=j*p+i;
			aux_tri._1=j*p+i+p+1;
			aux_tri._2=j*p+i+1;
			Triangulos.push_back(aux_tri);
		}
	}
}



 //***************************************************************************
 // DIbujar Partes Animacion
 //***************************************************************************
 ModoAnim_::ModoAnim_(){}

 void ModoAnim_::draw(int modo){
 	if (modo == 1) 	drawPuntos();
 	if (modo == 2) 	drawAlambre();
 	if (modo == 3) 	drawSolido();
 	if (modo == 4) 	drawTodo();
  	if (modo == 5)  drawAjedrez();
 }

 //***************************************************************************
 // Esfera // Cuerpo
 //***************************************************************************
  Esfera_::Esfera_(double radio, MiPunto3D p){
    double tam = radio;
    double angI = M_PI/2;
    double angF = (3*M_PI)/2;
    double lados = 70;
    double Angulo = (angF - angI)/lados;
    double j = angF;

    vector <_vertex3f> perfilEsfera;
    _vertex3f aux;

    for(int i = 0; i <= lados; ++i){
      aux.x = tam*cos(j);
      aux.y =  tam*sin(j);
      aux.z = 0;
      j+=Angulo;
      perfilEsfera.push_back(aux);
    }

    for(int i=0; i<perfilEsfera.size(); i++){
      perfilEsfera[i].x += p.x;
      perfilEsfera[i].y += p.y;
      perfilEsfera[i].z += p.z;
    }


    esferaR.revol_puntos(perfilEsfera,15,0.0,360.0);

    

  } 

 Esfera_::Esfera_(){
   double tam = 4.0;
   double angI = M_PI/2;
   double angF = (3*M_PI)/2;
   double lados = 70;
   double Angulo = (angF - angI)/lados;
   double j = angF;

   vector <_vertex3f> perfilEsfera;
   _vertex3f aux;

   for(int i = 0; i <= lados; ++i)
   {
      aux.x = tam*cos(j);
      aux.y =  tam*sin(j);
      aux.z = 0;
      j+=Angulo;
      perfilEsfera.push_back(aux);
   }
   
   esferaR.revol_puntos(perfilEsfera,15,0.0,360.0);

 }

void Esfera_::draw(int modo){
    //glPushMatrix();
    esferaR.draw(modo);
    //glPopMatrix();
}

//***************************************************************************
// Cilindro Conector
//***************************************************************************
Cilindro_::Cilindro_(){

  vector <_vertex3f> perfilCil;
  _vertex3f aux;

  aux.x=0.0; aux.y=0.5; aux.z=0.0;
  perfilCil.push_back(aux);
  aux.x=0.5; aux.y=0.5; aux.z=0.0;
	perfilCil.push_back(aux);

  aux.x=0.5; aux.y=-0.5; aux.z=0.0;
  perfilCil.push_back(aux);
  aux.x=0.0; aux.y=-0.5; aux.z=0.0;
  perfilCil.push_back(aux);

	cilindro.revol_puntos(perfilCil,12.,0.0,360.0);
}

void Cilindro_::draw(int modo){

   //Ojo grande
   glPushMatrix();
   glTranslatef(0.0,5.0,2.3);
   glScalef(1.0,1.0,0.5);
   glRotatef(90.0,1,0,0);
   cilindro.draw(modo);
   glPopMatrix();

   //Conector
   glPushMatrix();
   glTranslatef(0.0,4.5,0.0);
   glScalef(4.5,1.0,4.9);
   cilindro.draw(modo);
   glPopMatrix();


}


//***************************************************************************
// Cilindro OJO
//***************************************************************************
CilindroOjo_::CilindroOjo_(){

  vector <_vertex3f> perfilCil;
  _vertex3f aux;

  aux.x=0.0; aux.y=0.5; aux.z=0.0;
  perfilCil.push_back(aux);
  aux.x=0.5; aux.y=0.5; aux.z=0.0;
	perfilCil.push_back(aux);

  aux.x=0.5; aux.y=-0.5; aux.z=0.0;
  perfilCil.push_back(aux);
  aux.x=0.0; aux.y=-0.5; aux.z=0.0;
  perfilCil.push_back(aux);

	cilindro1.revol_puntos(perfilCil,12.,0.0,360.0);
}

void CilindroOjo_::draw(int modo){

   //Ojo pequeño
   glPushMatrix();
   glTranslatef(1.0,5.0,2.3);
   glScalef(0.5,0.5,0.60);
   glRotatef(90.0,1,0,0);
   cilindro1.draw(modo);
   glPopMatrix();
}


//***************************************************************************
// SemiEsfera Cabeza // Antenas
//***************************************************************************
Semiesfera_::Semiesfera_(){

  double tam = 2.5;
  double angI = M_PI/2;
  double angF = 0.0;
  double lados = 20;
  double Angulo = (angF - angI)/lados;
  double j = angF;

  vector <_vertex3f> perfilSemiesfera;
  _vertex3f aux;

  for(int i = 0; i <= lados; ++i)
  {
     aux.x = tam*cos(j);
     aux.y =  tam*sin(j);
     aux.z = 0;
     j+=Angulo;
     perfilSemiesfera.push_back(aux);
  }

  semiesferaR.revol_puntos(perfilSemiesfera,12,0.0,360.0);
}

void Semiesfera_::draw(int modo){

  //Antena Mas Alta
  glPushMatrix();
  glTranslatef(1.0,7.0,0.0);
  glScalef(0.01,1.0,0.0);
  antena.draw(modo);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.5,7.0,0.0);
  glScalef(0.01,0.5,0.0);
  antena.draw(modo);
  glPopMatrix();

  //Cabeza
  glPushMatrix();
  glTranslatef(0.0,4.5,0.0);
  glRotatef(180.0,1,0,0);
  semiesferaR.draw(modo);
  glPopMatrix();

}

//***************************************************************************
// Parpado
//***************************************************************************
Parpado_::Parpado_(){

  double tam = 0.5;
  double angI = M_PI/2;
  double angF = 0.0;
  double lados = 10;
  double Angulo = (angF - angI)/lados;
  double j = angF;

  vector <_vertex3f> perfilSemiesfera;
  _vertex3f aux;

  for(int i = 0; i <= lados; ++i)
  {
     aux.x = tam*cos(j);
     aux.y =  tam*sin(j);
     aux.z = 0;
     j+=Angulo;
     perfilSemiesfera.push_back(aux);
  }

  semiesferaR.revol_puntos(perfilSemiesfera,12,0.0,360.0);

}

void Parpado_::draw(int modo){

  glPushMatrix();
  glTranslatef(0.0,5.0,1.8);
  glRotatef(270.0,1,0,0);
  glScalef(1.0,1.0,1.0);
  semiesferaR.draw(modo);
  glPopMatrix();

}

//***************************************************************************
// BB8
//***************************************************************************
int inim=0;
int inio=0;

BB8_::BB8_(){
  m_parpado = 0.0;
  m_cuerpo = 0.0;
  gradoc = 0;
  gradocc=0;
}

void BB8_::parpadeo(){
  inim++;
  if(inim%2!=0){
    m_parpado = 0.0;
    inim=1;
  }
  if(inim%2==0){
    m_parpado = 0.5;
    inim=0;
  }
}

void BB8_::rotarOjo(){

 if(m_ojo == 0){
    gradocc++;
    if(gradocc > 2)
      m_ojo = 1;

  }
  if(m_ojo == 1){
    gradocc--;
    if(gradocc < -2)
      m_ojo = 0;
  }
}

void BB8_::rotarCuerpo(){
   m_cuerpo+=7;
}

void BB8_::rotarCuerpo_b(){
   m_cuerpo-=7;
}

int m_cabeza = 0;
void BB8_::girarCabeza(){

  if(m_cabeza == 0){
    gradoc++;
    if(gradoc > 60)
      m_cabeza = 1;

  }

  if(m_cabeza == 1){
    gradoc--;
    if(gradoc < -60)
      m_cabeza = 0;

  }
}

void BB8_::draw(int modo){

	glPushMatrix();
		glRotatef(m_cuerpo,1,0,0);
			esfera.draw(modo);
	glPopMatrix();

	glPushMatrix();
		glRotatef(gradoc,0,1,0);
			cilindro.draw(modo);
			semiesfera.draw(modo);
		glTranslatef(0.0,0.0,m_parpado);
			parpado.draw(modo);
		glPushMatrix();
			glRotatef(gradocc,1,0,0);
				cilindro1.draw(modo);
		glPopMatrix();
	glPopMatrix();

}


//***************************************************************************
// Cubo
//***************************************************************************
Cubo_::Cubo_(float t){

  //Vertices
  Vertices.resize(8);
  Vertices[0].x=-t;Vertices[0].y=-t;Vertices[0].z=t;
  Vertices[1].x=t;Vertices[1].y=-t;Vertices[1].z=t;
  Vertices[2].x=t;Vertices[2].y=t;Vertices[2].z=t;
  Vertices[3].x=-t;Vertices[3].y=t;Vertices[3].z=t;
  Vertices[4].x=-t;Vertices[4].y=-t;Vertices[4].z=-t;
  Vertices[5].x=t;Vertices[5].y=-t;Vertices[5].z=-t;
  Vertices[6].x=t;Vertices[6].y=t;Vertices[6].z=-t;
  Vertices[7].x=-t;Vertices[7].y=t;Vertices[7].z=-t;

  //Triangulos
  Triangulos.resize(12);
  Triangulos[0]._0=0;Triangulos[0]._1=1;Triangulos[0]._2=2;
  Triangulos[1]._0=0;Triangulos[1]._1=2;Triangulos[1]._2=3;
  Triangulos[2]._0=1;Triangulos[2]._1=5;Triangulos[2]._2=6;
  Triangulos[3]._0=1;Triangulos[3]._1=6;Triangulos[3]._2=2;
  Triangulos[4]._0=5;Triangulos[4]._1=4;Triangulos[4]._2=7;
  Triangulos[5]._0=5;Triangulos[5]._1=7;Triangulos[5]._2=6;

  Triangulos[6]._0=4;Triangulos[6]._1=0;Triangulos[6]._2=3;
  Triangulos[7]._0=4;Triangulos[7]._1=3;Triangulos[7]._2=7;
  Triangulos[8]._0=3;Triangulos[8]._1=2;Triangulos[8]._2=6;
  Triangulos[9]._0=3;Triangulos[9]._1=6;Triangulos[9]._2=7;
  Triangulos[10]._0=4;Triangulos[10]._1=5;Triangulos[10]._2=1;
  Triangulos[11]._0=4;Triangulos[11]._1=1;Triangulos[11]._2=0;
}

//***************************************************************************
// Tetraedro
//***************************************************************************
Tetraedro_::Tetraedro_(){

  float t = 1.5;
  //Vertices
  Vertices.resize(4);
  Vertices[0].x=0;Vertices[0].y=-t;Vertices[0].z=t;
  Vertices[1].x=t;Vertices[1].y=-t;Vertices[1].z=-t;
  Vertices[2].x=0;Vertices[2].y=t;Vertices[2].z=0;
  Vertices[3].x=-t;Vertices[3].y=-t;Vertices[3].z=-t;

  //Triangulos
  Triangulos.resize(4);
  Triangulos[0]._0=0;Triangulos[0]._1=1;Triangulos[0]._2=2;
  Triangulos[1]._0=3;Triangulos[1]._1=2;Triangulos[1]._2=0;
  Triangulos[2]._0=1;Triangulos[2]._1=3;Triangulos[2]._2=2;
  Triangulos[3]._0=0;Triangulos[3]._1=3;Triangulos[3]._2=1;

}


/*********************************************
	Creacion de R2D2
*********************************************/
//***************************************************************************
// Pie
//***************************************************************************
Pie_::Pie_(){

	float lados = 4.0;

	char PLY_pie[23] = "plybasicos/pieR2D2.ply";
	Ply_ ply_pie;
	//Revolucion_ pie_;

	ply_pie.leer_ply(PLY_pie);
	pie_.revol_puntos(ply_pie.Vertices, lados, 0.0, 360.0);
}

void Pie_::draw(int modo){

	glPushMatrix();
		glTranslatef(0.0,0.8,0.0);
		glScalef(0.5,0.8,1.0);
		glRotatef(45.0,0,1,0);
		pie_.draw(modo);
	glPopMatrix();

}

//***************************************************************************
// Tibia
//***************************************************************************
Tibia_::Tibia_(){

	float lados = 4.0;

	char PLY_tibia[23] = "plybasicos/basico1.ply"; //cilindro abierto
	Ply_ ply_tibia;

	ply_tibia.leer_ply(PLY_tibia);
	tibia_.revol_puntos(ply_tibia.Vertices, lados, 0.0, 360.0);
}

void Tibia_::draw(int modo){

	glPushMatrix();
		glTranslatef(0.0,3.6,0.0);
		glScalef(0.4,2.0,0.8);
		glRotatef(45.0,0,1,0);
		tibia_.draw(modo);
	glPopMatrix();

}

//***************************************************************************
// Rodilla
//***************************************************************************
Rodilla_::Rodilla_(){

	float ladosC = 50.0;
	float ladosD = 4.0;

	char PLY_cilindro[23] = "plybasicos/basico4.ply"; //Cilindro cerrado
	Ply_ cilindroR;
	Ply_ cajaR;


	cilindroR.leer_ply(PLY_cilindro);
	cilindroR_.revol_puntos(cilindroR.Vertices, ladosC, 0.0, 360.0);

	cajaR.leer_ply(PLY_cilindro);
	cajaR_.revol_puntos(cajaR.Vertices, ladosD, 0.0, 360.0);
}

void Rodilla_::draw(int modo){

	glPushMatrix();
		glTranslatef(0.0,7.6,0.0);
		glScalef(0.5,1.0,1.0);
		glRotatef(90.0,0,0,1);
		cilindroR_.draw(modo);
	glPopMatrix();
	
	glPushMatrix();
		glTranslatef(0.0,6.6,0.0);
		glScalef(0.7,1.0,1.4);
		glRotatef(45.0,0,1,0);
		cajaR_.draw(modo);
	glPopMatrix();

}

//***************************************************************************
// Pierna
//***************************************************************************
Pierna_::Pierna_(){

}

void Pierna_::draw(int modo){
	pieR2D2.draw(modo);
	tibiaR2D2.draw(modo);
	rodillaR2D2.draw(modo);
}


//***************************************************************************
// Tronco_
//***************************************************************************
Tronco_::Tronco_(){

	float lados = 60.0;

	char PLY_tronco[23] = "plybasicos/basico4.ply"; //cilindro abierto
	Ply_ ply_tronco;
	char PLY_base[23] = "plybasicos/basico2.ply"; //taza invertida
	Ply_ ply_base;

	ply_tronco.leer_ply(PLY_tronco);
	tronco_.revol_puntos(ply_tronco.Vertices, lados, 0.0, 360.0);

	ply_base.leer_ply(PLY_base);
	base_.revol_puntos(ply_base.Vertices, lados, 0.0, 360.0);
}

void Tronco_::draw(int modo){

	glPushMatrix();
		glTranslatef(0.0,5.6,0.0);
		glScalef(2.0,3.0,2.0);
		glRotatef(0.0,0,0,0);
		tronco_.draw(modo);
	glPopMatrix();
	glPushMatrix();
		glTranslatef(0.0,2.1,0.0);
		glScalef(1.0,0.5,1.0);
		glRotatef(0.0,0,0,0);
		base_.draw(modo);
	glPopMatrix();

}

//***************************************************************************
// Cabeza_
//***************************************************************************
Cabeza_::Cabeza_(){

  double tam = 1.0;
  double angI = M_PI/2;
  double angF = 0.0;
  double lados = 60.0;
  double Angulo = (angF - angI)/lados;
  double j = angF;

  vector <_vertex3f> perfilSemiesfera;
  _vertex3f aux;

  for(int i = 0; i <= lados; ++i)
  {
     aux.x = tam*cos(j);
     aux.y =  tam*sin(j);
     aux.z = 0;
     j+=Angulo;
     perfilSemiesfera.push_back(aux);
  }

  cabeza_.revol_puntos(perfilSemiesfera,lados,0.0,360.0);
}

void Cabeza_::draw(int modo){
  //Cabeza
  glPushMatrix();
	  glTranslatef(0.0,8.6,0.0);
	  glScalef(2.0,2.7,2.0);
	  glRotatef(180.0,1,0,0);
	  cabeza_.draw(modo);
  glPopMatrix();
}

//***************************************************************************
// Ojo_
//***************************************************************************
Ojo_::Ojo_(){

	float lados = 20.0;

	char PLY_ojo[23] = "plybasicos/basico4.ply"; //cilindro cerrado
	Ply_ ply_ojo;

	ply_ojo.leer_ply(PLY_ojo);
	ojo_.revol_puntos(ply_ojo.Vertices, lados, 0.0, 360.0);
}

void Ojo_::draw(int modo){

	glPushMatrix();
		glTranslatef(0.3,9.0,2.4);
		glScalef(0.15,0.15,0.45);
		glRotatef(90.0,1,0.0,0);
		ojo_.draw(modo);
	glPopMatrix();

}



R2D2_::R2D2_(){
	gira_tronco = 0;
	gira_cabeza = 0;
	examina = 0;
	dcha = true;
	delante = true;
	arriba = true;
	desplazamiento = 0;
	gira_cadera = 0;
	desplazamiento_inclinacion = 0;
	despl_altura_ojo = 0;
	altura_rodilla = 7.6;
	altura_ojo = 9.0;
	desplazamiento_altura = 0;
	a1 = 0;
	b1 = 0;
	puedeAvanzar = false;
	puedeRetroceder = false;
}

void R2D2_::giraCuerpo(){
   gira_tronco+=7;
}

void R2D2_::radar(){
	if(gira_cabeza < 60 && dcha){
		gira_cabeza+=5;
	}
	else{
		gira_cabeza-=5;
	}
	if(gira_cabeza == 60)
		dcha = false;
	
	if(gira_cabeza == -60)
		dcha = true;
}

void R2D2_::examinar(){
	
	if(examina < 30 && arriba){
		examina+=3;
	}
	else{
		examina-=3;
	}
	if(examina == 30)
		arriba = false;
	
	if(examina == -30)
		arriba = true;

	float h=9.0;
	float d=2.4;

	float alpha = examina*M_PI/180.0;
	float beta = atan(d/h);
	float angulo = alpha+beta;
	float distancia = sqrt(h*h+d*d);

	float h1 = distancia*cos(angulo);
	float d1 = distancia*sin(angulo);

	float aux_despl = 


	a1=d*cos(alpha)-d1;
	b1=(h-h1)-sin(alpha); //correcto
}

void R2D2_::inclinarD(){ //ir hacia atrás
	if(gira_cadera != -30){
		gira_cadera-=3;
	}

	float anguloR = M_PI/180*gira_cadera;
	float h = altura_rodilla;
	desplazamiento_altura = h - (cos(anguloR)*h);
	desplazamiento_inclinacion = sin(anguloR)*h;

}

void R2D2_::inclinarA(){ //ir hacia atrás
	if(gira_cadera != 30){
		gira_cadera+=3;
	}

	float anguloR = M_PI/180*gira_cadera;
	int h = altura_rodilla;
	desplazamiento_altura = h - (cos(anguloR)*h);
	desplazamiento_inclinacion = sin(anguloR)*h;
}

void R2D2_::avanzar(){

	if(!puedeAvanzar){
		if(gira_cadera != -30){
			gira_cadera-=3;
		}
		else{
			puedeAvanzar = true;
			puedeRetroceder = false;
		}
		float anguloR = M_PI/180*gira_cadera;
		float h = altura_rodilla;
		desplazamiento_altura = h - (cos(anguloR)*h);
		desplazamiento_inclinacion = sin(anguloR)*h;
	}

	if(puedeAvanzar){
		desplazamiento += 1.0;
	}
}

void R2D2_::retroceder(){
	if(!puedeRetroceder){
		if(gira_cadera != 30){
			gira_cadera+=3;
		}
		else{
			puedeAvanzar = false;
			puedeRetroceder = true;
		}

		float anguloR = M_PI/180*gira_cadera;
		float h = altura_rodilla;
		desplazamiento_altura = h - (cos(anguloR)*h);
		desplazamiento_inclinacion = sin(anguloR)*h;
	}

	if(puedeRetroceder){
		desplazamiento -= 1.0;
	}
}

void R2D2_::draw(int modo){
	
	glPushMatrix();
		glTranslatef(0.0,desplazamiento_altura,desplazamiento-desplazamiento_inclinacion);
		glScalef(1.0,1.0,1.0);
		glRotatef(gira_cadera,1,0,0);
		tronco_.draw(modo);
		glPushMatrix();
			glTranslatef(0.0,0.0,0.0);
			glScalef(1.0,1.0,1.0);
			glRotatef(gira_cabeza,0,1,0);
			cabeza_.draw(modo);
			glPushMatrix();
				glTranslatef(0.0,b1,a1);
				glScalef(1.0,1.0,1.0);
				glRotatef(examina,1,0,0);
				ojo_.draw(modo);
			glPopMatrix();
  		glPopMatrix();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-2.5,0.0,desplazamiento);
		glScalef(1.0,1.0,1.0);
		glRotatef(0.0,0,0,0);
		piernaDcha.draw(modo);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(2.5,0.0,desplazamiento);
		glScalef(1.0,1.0,1.0);
		glRotatef(180.0,0,1,0);
		piernaIzda.draw(modo);
	glPopMatrix();
}



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
    glLineWidth(4.0);

    glVertex3f (tvertices[0], tvertices[1]+0.01, tvertices[2]); //0
    glVertex3f (tvertices[3], tvertices[4]+0.01, tvertices[5]); //1

    glVertex3f (tvertices[9*3], tvertices[9*3+1]+0.01, tvertices[9*3+2]); //9
    glVertex3f (tvertices[2*3], tvertices[2*3+1]+0.01, tvertices[2*3+2]); //2
    
    glVertex3f (tvertices[3*3], tvertices[3*3+1]+0.01, tvertices[3*3+2]); //3
    glVertex3f (tvertices[8*3], tvertices[8*3+1]+0.01, tvertices[8*3+2]); //8

    glVertex3f (tvertices[4*3], tvertices[4*3+1]+0.01, tvertices[4*3+2]); //4
    glVertex3f (tvertices[7*3], tvertices[7*3+1]+0.01, tvertices[7*3+2]); //7
    
    glVertex3f (tvertices[5*3], tvertices[5*3+1]+0.01, tvertices[5*3+2]); //5
    glVertex3f (tvertices[6*3], tvertices[6*3+1]+0.01, tvertices[6*3+2]); //6

    glVertex3f (tvertices[0], tvertices[1]+0.01, tvertices[2]); //0
    glVertex3f (tvertices[6*3], tvertices[6*3+1]+0.01, tvertices[6*3+2]); //6
    
    glVertex3f (tvertices[3], tvertices[4]+0.01, tvertices[5]); //1
    glVertex3f (tvertices[5*3], tvertices[5*3+1]+0.01, tvertices[5*3+2]); //5

    //RED
    glVertex3f (rvertices[0], rvertices[1], rvertices[2]); //0
    glVertex3f (rvertices[3], rvertices[4], rvertices[5]); //1

    glVertex3f (rvertices[3], rvertices[4], rvertices[5]); //1
    glVertex3f (rvertices[3*3], rvertices[3*3+1], rvertices[3*3+2]); //3

    glVertex3f (rvertices[2*3], rvertices[2*3+1], rvertices[2*3+2]); //2
    glVertex3f (rvertices[3*3], rvertices[3*3+1], rvertices[3*3+2]); //3

    glVertex3f (rvertices[2*3], rvertices[2*3+1], rvertices[2*3+2]); //2
    glVertex3f (rvertices[0*3], rvertices[0*3+1], rvertices[0*3+2]); //0

  glEnd();

  glLineWidth(1.0);
}

void draw_campo_juego_solid(vector<float> tvertices, vector<float> rvertices){

  glColor3f(0.459,0.537,0.75);
  glBegin (GL_POLYGON);
  //SUELO
    glVertex3f (tvertices[0], tvertices[1], tvertices[2]); //0
    glVertex3f (tvertices[3], tvertices[4], tvertices[5]); //1
    
    glVertex3f (tvertices[5*3], tvertices[5*3+1], tvertices[5*3+2]); //5
    glVertex3f (tvertices[6*3], tvertices[6*3+1], tvertices[6*3+2]); //6

  glEnd();

  glColor3f(0.777,0.237,0.475);
  glBegin (GL_POLYGON);
  //RED
    glVertex3f (rvertices[0], rvertices[1], rvertices[2]); //0
    glVertex3f (rvertices[3], rvertices[4], rvertices[5]); //1

    glVertex3f (rvertices[3*3], rvertices[3*3+1], rvertices[3*3+2]); //3
    glVertex3f (rvertices[2*3], rvertices[2*3+1], rvertices[2*3+2]); //2

  glEnd();

}

void puntos_tracking_ball(vector <MiPunto4D> coordenadas, int inicio, int fin){

  int i;
  int n_puntos = coordenadas.size();

  unsigned int microseconds = 3*1000000/24.0;;
  //cout << microseconds << " ";

  //Defino un material Verde
    GLfloat mat_ambient_esfera[] = {0.1, 0.1, 0.1, 1.0f}; 
    GLfloat mat_diffuse_esfera[] = {0.0, 0.7, 0.0, 1.0f};
    GLfloat mat_specular_esfera[] = {0.8, 0.8, 0.8, 1.0f};
  
  for (i=inicio; i<fin; i++){
    
    glColor3f(0.03,1,0.7);
    glPointSize(i%10);
    /*
    Sintaxis : void glutSolidSphere(GLdouble radius,GLint slices, GLint stacks), 
      radius = radio de la esfera, 
      slice = numero de subdiviciones alrededor del eje Z, 
      staks = numero de subdivisiones a lo largo del eje Z, 
      en nuestra escena la esfera es glutSolidSphere (1.0, 16, 16) y 
      situada en (-1.0,1.0, 0.0) con respecto al origen glTranslatef (-1.0, 1.0, 0.0).
  */
    glMaterialfv (GL_FRONT, GL_AMBIENT, mat_ambient_esfera);
    glMaterialfv (GL_FRONT, GL_DIFFUSE, mat_diffuse_esfera);
    glMaterialfv (GL_FRONT, GL_SPECULAR, mat_specular_esfera);
    glMaterialf (GL_FRONT, GL_SHININESS, 100.0f);

    glPushMatrix ();
      glTranslatef (coordenadas[i].coordenada.x - 4.5, coordenadas[i].coordenada.y, coordenadas[i].coordenada.z - 9.0); 
      glutSolidSphere (((i%10)/100.0), 20, 20);
    glPopMatrix ();

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


void Rejilla( int eje){
  glLineWidth(0.5);
  glColor3f(0,1,0);

  for(int i=1; i<10; i++){


    glBegin (GL_LINES);

      if(eje == 1 || eje == 4){
        //Z
        glVertex3f (i, 0.02, 10); //0
        glVertex3f (i, 0.02, -10); //1

        glVertex3f (-i, 0.02, 10); //0
        glVertex3f (-i, 0.02, -10); //1
      }
      if(eje == 2 || eje == 4){
        //X
        glVertex3f (10, 0.02, i); //0
        glVertex3f (-10, 0.02, i); //1

        glVertex3f (10, 0.02, -i); //0
        glVertex3f (-10, 0.02, -i); //1
      }

      if(eje == 3 || eje == 4){
        //Y
        glVertex3f (-10, i, 0); //0
        glVertex3f (10, i, 0); //1

        glVertex3f (0, i, -10); //0
        glVertex3f (0, i, 10); //1
      }
    glEnd();
  }
}