# fichero Makefile
#
# nombre del fichero ejecutable
#
TARGETS= trackingVB

# banderas para el compilador (C y C++). Se indican los directorios donde se puenden
# encontrar ficheros para incluir mediante la opcion -I
# la opcion -g sirve para permitir la depuraci�n
#
CFLAGS=    -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -DXWINDOWS -DSHM -I/usr/include -I.
CXXFLAGS=  -I/usr/local/include/opencv -I/usr/local/include/opencv2 -L/usr/local/lib/ -g -DXWINDOWS -DSHM -I/usr/include -I.

# banderas pra el enlazador
# Se indica directorios donde encontrar las funciontecas con -L. Usando -l seguido del
# nombre se indica la funcionteca que se desea enlazar.
#
LDFLAGS=  -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_stitching -lGLU -lGL -lglut

# definicion del compilador
#
CC = g++

# orden que sera ejecutada por defecto
#
default : $(TARGETS)

# regla de ejecucion
#
$(TARGETS) : archivos/misObjetos.o archivos/objeto.o archivos/file_ply_stl.o trackingVB.o
	$(CC) -o $@ $(CXXFLAGS) archivos/misObjetos.o archivos/objeto.o archivos/file_ply_stl.o trackingVB.o $(LDFLAGS)

#
#
clean:
	rm -f *.o
	rm -f $(TARGETS)

#
#
redo:
	touch *.cc
	make

#
#
superclean:
	rm -f *.o
	rm *~
	rm -f $(TARGETS)

#
#
tgz:
	rm -f *.o
	rm *~
	rm -f $(TARGETS)
	tar -zcvf $(TARGETS).tgz *
