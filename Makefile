CC=g++ -Werror
TESTADOR=bin/PNPNTest
SIMULATOR=bin/PNPNeuron
CONFIG_FILE=config/defaults.cfg config/physics_constants.cfg config/output.cfg

OBJETOS=src/RenderView.o src/Standarts.o src/main.o src/ModelLoad.o src/MainEngine.o src/Membrana.o src/EletricPotentialComputer.o src/GradientComputer.o src/Config.o
TEST_OBJ=src/mainTest.o

LD_FLAGS=-lSDL -lSDL_image -lpng -lz -lGL -lm

STRIP=strip
RUN=
ifdef debug
    CC_FLAGS = -gstabs+ -ggdb
    RUN=gdb
    CONFIG_FILE=
    STRIP=du -h
else
    CC_FLAGS = -Ofast -pipe -mtune=native -march=native -mfpmath=both -msse -msse2 -msse3 -msse4.1 -msse4.2 -m3dnow -mmmx
    RUN=time
endif

ifdef profile
    CC_FLAGS = -ggdb -gstabs+ -pg
    STRIP=du -h
endif

ifdef test
    TESTDEFINE= -D__TEST__
endif

DEFINES = $(TESTDEFINE)

all: clean $(TESTADOR) $(SIMULATOR) run
clean: cleandocs
	rm -rf src/*.o $(SIMULATOR) $(TESTADOR) gmon.out results src/*~ config/*~ log.txt
run:
	@printf "running app\n"
	@mkdir results
	$(RUN) ./$(SIMULATOR) $(CONFIG_FILE)
	@printf "\n\n\tapp end\n"

%.o:%.cpp
	$(CC) -I. $(CC_FLAGS) -c $+ $(DEFINES) -o $@

docs:
	doxygen src/doxy.template

cleandocs:
	rm -rf html

$(SIMULATOR): $(OBJETOS)
	@printf "Building application\n"
	$(CC) $(CC_FLAGS) $(OBJETOS) "bin/libmgl.a" -o $(SIMULATOR) $(LD_FLAGS)
	$(STRIP) $(SIMULATOR)
	@printf "All build done\n\n"

$(TESTADOR): $(TEST_OBJ)
	@printf "Testig source code...\n"
	$(CC) $(CC_FLAGS) $(TEST_OBJ) -o $(TESTADOR) $(LD_FLAGS)
	./$(TESTADOR)
	@printf "Test passed sucessfull\n\n\n\n"

