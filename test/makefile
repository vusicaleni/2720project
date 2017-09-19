##
## C++ MAKEFILE 
## AUTHOR: VUSI KA-CALENI
## DATE: FEB 2015
##
##COMPILER
CC = g++
##LIBRARIES
LIBS = -lallegro -lallegro_main -lallegro_dialog -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_font -lallegro_ttf -lallegro_image  -lcppunit -ldl
##OBJECT FILES
OBJS = main.o 
OBJS += ../src/allegro.o ../src/gamemanager.o ../src/ship.o ../src/bullet.o ../src/enemy.o ../src/enemy_bullet.o ../src/boss.o ../src/particle.o ../src/item.o ../src/missile.o 
##CFLAGS
CFLAGS = -c -std=c++11 -Wall
##CTOR
TEST_CORE: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LIBS)

%.o : %.cpp
	$(CC) $(CFLAGS) -o $@ $<
	$(CC) $(CFLAGS) -MM -MP -MT $@ $< > $(basename $@).d

.PHONY : clean
clean :
	rm -f *.o *~ *.d
## Include the generated dependency files
-include $(addsuffix .d,$(basename $(OBJS)))
