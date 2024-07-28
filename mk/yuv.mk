O=tmpyuv
OUTFILE=yuvTool
LIBSRC=./
UNAME=$(shell uname -s)
ALLOBJ+=$(O)/main.o \
	$(O)/yuvTool.o
	
CFLAG+=-DNODEBUG -DNDEBUG \
			 -DAEXP_CAPI \
       -Wno-write-strings -Wno-deprecated-declarations-O2 -fPIC -fvisibility=hidden \
       -I$(LIBSRC)/../src

ifeq ($(UNAME), Linux)
	CC=gcc
	CPP=g++
	LD=g++
	STRIP=strip -s
	ARCH=$(shell getconf LONG_BIT)
	ifeq ($(ARCH), 32)
		ARCH=86
		CFLAG+=-D__STDC_CONSTANT_MACROS
	endif	
	OUTPUT=$(OUTFILE)
	CFLAG+=-D__LINUX__ -D__HAS_LOG__ -DHASNOT_UUID -march=core2 -Wno-unused-result
	CPPFLAG+=-std=gnu++0x -Wno-deprecated
	LIBS += -pthread -lrt -lm -static-libgcc
	# LDFLAG+=$(CFLAG) $(CPPFLAG) -static-libstdc++ -shared -Wl,-Bsymbolic
endif

COMPILE=$(CPP) -c $(CFLAG) $(CPPFLAG) -o $(O)/$(*F).o $<
CCOMPILE=$(CC) -c $(CFLAG) -o $(O)/$(*F).o $<

$(O)/%.o : %.cpp
	$(COMPILE)

all: $(OUTPUT)

$(OUTPUT): $(O) $(ALLOBJ)
	$(LD) $(LDFLAG) -o $@ $(ALLOBJ) $(LIBS)
	@$(STRIP) $@
	@rm -rf $(O)

$(O):
	@mkdir -p $(O)
