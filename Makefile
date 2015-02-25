NAME = a.out

CUDA_PATH	= /Developer/NVIDIA/CUDA-6.5/
NVCC 		= $(CUDA_PATH)/bin/nvcc

CPPFLAGS = -L libs/ -ljpeg -L $(CUDA_PATH)/lib/ -lcurand

OBJ =	main.o \
	Jpeg.o \
	Glut.o \
	PathTracer.o \
	Image.o \
	SimpleRT.cu.o \
	CudaUtils.cu.o \
	Scene.cu.o

FRAMEWORK = -Xlinker -framework,OpenGL,-framework,GLUT

all: $(OBJ)
	$(NVCC) -arch=sm_20 $(OBJ) -o $(NAME) $(CPPFLAGS) $(FRAMEWORK)


%.o: sources/%.cpp
	$(NVCC) -x cu -arch=sm_20 -I includes -dc $< -o $@

clean:
	rm -f *.o $(NAME)

re: clean all