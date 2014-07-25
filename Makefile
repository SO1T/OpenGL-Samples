all: bloc4MoreMac

bloc2: bloc2.cpp
	g++ -o bloc2 bloc2.cpp -lGL -lglut -lGLU Model/model.o 
bloc2Mac: bloc2.cpp
	g++ -o bloc2 bloc2.cpp Model/model.o -framework OpenGL -framework GLUT -Wno-deprecated

bloc3: bloc3.cpp
	g++ -o bloc3 bloc3.cpp -lGL -lglut -lGLU Model/model.o

bloc3Mac: bloc3.cpp
	g++ -o bloc3 bloc3.cpp Model/model.o -framework OpenGL -framework GLUT -Wno-deprecated

bloc4: bloc4.cpp
	g++ -o bloc4 bloc4.cpp -lGL -lglut -lGLU model.o

bloc4More: Sample4-CameraLights.cpp
	g++ -o bloc4More Sample4-CameraLights.cpp -lGL -lglut -lGLU model.o

bloc4Mac: bloc4.cpp
	 g++ -o bloc4 bloc4.cpp Model/model.o -framework OpenGL -framework GLUT -Wno-deprecated

bloc4MoreMac: bloc4More.cpp
	g++ -o bloc4More bloc4More.cpp Model/model.o -framework OpenGL -framework GLUT -Wno-deprecated


clean: 
	rm -f *.o 
