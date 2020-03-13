#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.147                       #
#------------------------------------------------------------------------------#


WORKDIR = `pwd`

CC = gcc
CXX = g++
AR = ar
LD = g++
WINDRES = windres

INC = 
CFLAGS = -Wall
RESINC = 
LIBDIR = 
LIB = 
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf



INC_RELEASE = $(INC)
CFLAGS_RELEASE = $(CFLAGS) -O2
RESINC_RELEASE = $(RESINC)
RCFLAGS_RELEASE = $(RCFLAGS)
LIBDIR_RELEASE = $(LIBDIR)
LIB_RELEASE = $(LIB)
LDFLAGS_RELEASE = $(LDFLAGS) -s
OBJDIR_RELEASE = obj/Release
DEP_RELEASE = 
OUT_RELEASE = ./Wonsz1.x


OBJ_RELEASE = $(OBJDIR_RELEASE)/spider.o $(OBJDIR_RELEASE)/snake.o $(OBJDIR_RELEASE)/particle.o $(OBJDIR_RELEASE)/main.o $(OBJDIR_RELEASE)/level.o $(OBJDIR_RELEASE)/game_gui.o $(OBJDIR_RELEASE)/ANZ_Image.o $(OBJDIR_RELEASE)/entitySpawner.o $(OBJDIR_RELEASE)/apple.o $(OBJDIR_RELEASE)/AbstractEntity.o $(OBJDIR_RELEASE)/ANZ_Sprite.o



before_release: 
	test -d . || mkdir -p .
	test -d $(OBJDIR_RELEASE) || mkdir -p $(OBJDIR_RELEASE)

after_release: 

all: before_release out_release after_release

out_release: before_release $(OBJ_RELEASE) $(DEP_RELEASE)
	$(LD) $(LIBDIR_RELEASE) -o $(OUT_RELEASE) $(OBJ_RELEASE)  $(LDFLAGS_RELEASE) $(LIB_RELEASE)

$(OBJDIR_RELEASE)/spider.o: spider.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c spider.c -o $(OBJDIR_RELEASE)/spider.o

$(OBJDIR_RELEASE)/snake.o: snake.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c snake.c -o $(OBJDIR_RELEASE)/snake.o

$(OBJDIR_RELEASE)/particle.o: particle.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c particle.c -o $(OBJDIR_RELEASE)/particle.o

$(OBJDIR_RELEASE)/main.o: main.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c main.c -o $(OBJDIR_RELEASE)/main.o

$(OBJDIR_RELEASE)/level.o: level.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c level.c -o $(OBJDIR_RELEASE)/level.o

$(OBJDIR_RELEASE)/game_gui.o: game_gui.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c game_gui.c -o $(OBJDIR_RELEASE)/game_gui.o

$(OBJDIR_RELEASE)/ANZ_Image.o: ANZ_Image.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ANZ_Image.c -o $(OBJDIR_RELEASE)/ANZ_Image.o

$(OBJDIR_RELEASE)/entitySpawner.o: entitySpawner.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c entitySpawner.c -o $(OBJDIR_RELEASE)/entitySpawner.o

$(OBJDIR_RELEASE)/apple.o: apple.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c apple.c -o $(OBJDIR_RELEASE)/apple.o

$(OBJDIR_RELEASE)/AbstractEntity.o: AbstractEntity.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c AbstractEntity.c -o $(OBJDIR_RELEASE)/AbstractEntity.o

$(OBJDIR_RELEASE)/ANZ_Sprite.o: ANZ_Sprite.c
	$(CC) $(CFLAGS_RELEASE) $(INC_RELEASE) -c ANZ_Sprite.c -o $(OBJDIR_RELEASE)/ANZ_Sprite.o

clean: 
	rm -f $(OBJ_RELEASE) $(OUT_RELEASE)
	rm -rf $(OBJDIR_RELEASE)

.PHONY: before_debug after_debug clean_debug before_release after_release clean_release

