#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "file_ply_stl.h"
#include "misObjetos.h"
#include "stdlib.h"
#include <unistd.h>
#include <GL/glut.h>

using namespace std;

const float AXIS_SIZE=5000;

//***************************************************************************
// Clase Puntos_
//***************************************************************************

class Puntos_{
public:
	Puntos_();
	void drawPuntos();
	vector<_vertex3f> Vertices;
};

//***************************************************************************
// Clase Triangulo_
//***************************************************************************

class Triangulo_: public Puntos_{
public:
	Triangulo_();
	void drawAlambre();
	void drawSolido();
	void drawAjedrez();
	void drawTodo();
  vector<_vertex3i> Triangulos;
};


//***************************************************************************
// Clase Revolucion_
//***************************************************************************

class ModoAnim_: public Triangulo_{
public:
	ModoAnim_();
	void draw(int modo);
};

//***************************************************************************
// Clase Ply_
//***************************************************************************

class Ply_: public ModoAnim_{
public:
	Ply_();
	void leer_ply(char *file);
};

//***************************************************************************
// Clase Revolucion_
//***************************************************************************

class Revolucion_: public Ply_{
public:
	Revolucion_();
	void revol_puntos(vector<_vertex3f> &rev,float lados,float angI, float angF);
};

//***************************************************************************
// Clase Cubo_
//***************************************************************************
class Cubo_: public ModoAnim_{
public:
	Cubo_(float t=1);
};

//***************************************************************************
//  											PARTES ANIMACION
//***************************************************************************

//***************************************************************************
// Clase Esfera
//***************************************************************************
class Esfera_: public Triangulo_{
public:
	Esfera_(double radio, MiPunto3D p);
	Esfera_();
	void draw(int modo);
//protected:
	Revolucion_ esferaR;

};



//***************************************************************************
// Clase Perfil Cilindro
//***************************************************************************
class Cilindro_: public Triangulo_{
public:
	Cilindro_();
	void draw(int modo);
protected:
	Revolucion_ cilindro;

};

//***************************************************************************
// Clase Perfil CilondroOjo_
//***************************************************************************
class CilindroOjo_: public Triangulo_{
public:
	CilindroOjo_();
	void draw(int modo);
protected:
	Revolucion_ cilindro1;

};



//***************************************************************************
// Clase Semiesfera
//***************************************************************************
class Semiesfera_: public Triangulo_{
public:
	Semiesfera_();
	void draw(int modo);
protected:
	Revolucion_ semiesferaR;
	Esfera_ baseEsfera;
	Cubo_ antena;
};

//***************************************************************************
// Clase Parpado_
//***************************************************************************
class Parpado_: public Triangulo_{
public:
	Parpado_();
	void draw(int modo);
protected:
	Revolucion_ semiesferaR;
};


//***************************************************************************
// Clase BB8
//***************************************************************************
class BB8_{
public:
	BB8_();
	double m_parpado;
	int m_cuerpo;
	double gradoc;
	int m_ojo;
	double gradocc;

  	void draw(int modo);
	void rotarCuerpo();
	void rotarCuerpo_b();
	void girarCabeza();
	void parpadeo();
	void rotarOjo();


private:
	Semiesfera_ semiesfera;
	Esfera_ esfera;
	Cilindro_ cilindro;
	CilindroOjo_ cilindro1;
	Parpado_ parpado;


};

//***************************************************************************
// Clase Tetraedro
//***************************************************************************
class Tetraedro_: public ModoAnim_{
public:
	Tetraedro_();
};





//***************************************************************************
// Clase Pie_
//***************************************************************************
class Pie_: public Triangulo_{
public:
	Pie_();
	void draw(int modo);
protected:
	Revolucion_ pie_;
};

//***************************************************************************
// Clase Tibia_
//***************************************************************************
class Tibia_: public Triangulo_{
public:
	Tibia_();
	void draw(int modo);
protected:
	Revolucion_ tibia_;
};

//***************************************************************************
// Clase Rodilla_
//***************************************************************************
class Rodilla_: public Triangulo_{
public:
	Rodilla_();
	void draw(int modo);
protected:
	Revolucion_ cilindroR_;
	Revolucion_ cajaR_;
};


//***************************************************************************
// Clase Pierna_
//***************************************************************************
class Pierna_: public Triangulo_{
public:
	Pierna_();
	void draw(int modo);
protected:
	Pie_ pieR2D2;
	Tibia_ tibiaR2D2;
	Rodilla_ rodillaR2D2;
};

//***************************************************************************
// Clase Tronco_
//***************************************************************************
class Tronco_: public Triangulo_{
public:
	Tronco_();
	void draw(int modo);
protected:
	Revolucion_ tronco_, base_;
};

//***************************************************************************
// Clase Cabeza_
//***************************************************************************
class Cabeza_: public Triangulo_{
public:
	Cabeza_();
	void draw(int modo);
protected:
	Revolucion_ cabeza_;
};

//***************************************************************************
// Clase Ojo_
//***************************************************************************
class Ojo_: public Triangulo_{
public:
	Ojo_();
	void draw(int modo);
protected:
	Revolucion_ ojo_;
};


//***************************************************************************
// Clase R2D2_
//***************************************************************************
class R2D2_: public Triangulo_{
public:
	R2D2_();
	int gira_tronco;
	int gira_cabeza, gira_cadera;
	float desplazamiento, examina, desplazamiento_inclinacion, despl_inclinacion_ojo;
	float altura_rodilla, desplazamiento_altura, altura_ojo, despl_altura_ojo;
	bool dcha, arriba, delante, puedeAvanzar, puedeRetroceder;
	float a1, b1;
	void draw(int modo);
	void giraCuerpo();
	void radar();
	void examinar();
	void avanzar();
	void retroceder();
	void inclinarA();
	void inclinarD();
protected:
	Pierna_ piernaDcha, piernaIzda;
	Tronco_ tronco_;
	Cabeza_ cabeza_;
	Ojo_ ojo_;
};



void terreno_juego(vector<float> &vertices, vector<int> &caras);
void red_juego(vector<float> &vertices, vector<int> &caras);
void campo_voleibol(vector<float> &tvertices, vector<int> &tcaras, vector<float> &rvertices, vector<int> &rcaras);

//*************************************************************************
// Tetraedro NUEVO
//*************************************************************************

void nuevo_tetraedro(vector<float> &pvertices, vector<int> &pcaras, float altura);

void draw_points(vector<float> vertices);

void draw_aristas(vector<float> vertices, vector<int> aristas);

void draw_caras(vector<float> vertices, vector<int> caras);

void draw_solido(vector<float> vertices, vector<int> caras);

void draw_ajedrez(vector<float> vertices, vector<int> caras);

void draw_all(vector<float> vertices, vector<int> caras);

void draw_campo_juego_lines(vector<float> tvertices, vector<float> rvertices);

void draw_campo_juego_solid(vector<float> tvertices, vector<float> rvertices);

void puntos_tracking_ball(vector <MiPunto4D> coordenadas, int inicio, int fin);

void Rejilla(int eje);