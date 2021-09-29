#*************************************************************
#*
#* Author :      SecuGen Corporation
#* Description : FPLibTest Makefile
#* Copyright(c): 2009 SecuGen Corporation, All rights reserved
#* date        person   comments
#* ======================================================
#* 12/10/2009  driley   initial release
#*
#*************************************************************

PORT = linux3
CFLAGS = -D__cplusplus -Wall -O
CPP=g++
CC=gcc
INCPATH = -I./ -I../include
LIBS =  -L../lib/$(PORT)

OBJS=	main.o 

SRCS=	main.cpp 

FDU03_TARGET = ../bin/$(PORT)/wsqtest_fdu03
FDU04_TARGET = ../bin/$(PORT)/wsqtest_fdu04
FDU05_TARGET = ../bin/$(PORT)/wsqtest_fdu05
FDU06_TARGET = ../bin/$(PORT)/wsqtest_fdu06
FDU07_TARGET = ../bin/$(PORT)/wsqtest_fdu07
FDU08_TARGET = ../bin/$(PORT)/wsqtest_fdu08

all : $(FDU08_TARGET) $(FDU07_TARGET) $(FDU06_TARGET) $(FDU05_TARGET) $(FDU04_TARGET) $(FDU03_TARGET)

$(FDU08_TARGET) : $(OBJS)
	$(CPP) -o $@ $(OBJS) $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu08 -lusb -lpthread
	cp *.raw ../bin/$(PORT)
	cp *.wsq ../bin/$(PORT)

$(FDU07_TARGET) : $(OBJS)
	$(CPP) -o $@ $(OBJS) $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu07 -lusb -lpthread
	cp *.raw ../bin/$(PORT)
	cp *.wsq ../bin/$(PORT)

$(FDU06_TARGET) : $(OBJS)
	$(CPP) -o $@ $(OBJS) $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu06 -lusb -lpthread
	cp *.raw ../bin/$(PORT)
	cp *.wsq ../bin/$(PORT)

$(FDU05_TARGET) : $(OBJS)
	$(CPP) -o $@ $(OBJS) $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu05 -lusb -lpthread
	cp *.raw ../bin/$(PORT)
	cp *.wsq ../bin/$(PORT)

$(FDU04_TARGET) : $(OBJS)
	$(CPP)  -o $@ $(OBJS) $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu04 -lusb -lpthread
	cp *.raw ../bin/$(PORT)
	cp *.wsq ../bin/$(PORT)

$(FDU03_TARGET) : $(OBJS)
	$(CPP) -o $@ $(OBJS) $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu03 -lusb -lpthread
	cp *.raw ../bin/$(PORT)
	cp *.wsq ../bin/$(PORT)

all : $(TARGET)

.cc.o :
	$(CPP) $(FLAGS) $(INCPATH) -c $<

.c.o :
	$(CPP) $(FLAGS) $(INCPATH) -c $<

.cpp.o :
	$(CPP) $(FLAGS) $(INCPATH) -c $<

dep :
	gccmakedep $(INCPATH) $(SRCS)

clean :
	rm -rf $(FDU08_TARGET) $(FDU07_TARGET) $(FDU06_TARGET) $(FDU05_TARGET) $(FDU04_TARGET) $(FDU03_TARGET)
	rm -rf $(PORT)
	mkdir $(PORT)

