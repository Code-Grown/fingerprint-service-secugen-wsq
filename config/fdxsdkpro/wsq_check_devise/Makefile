#*************************************************************
#*
#* Author :      SecuGen Corporation
#* Description : AUTO ON Makefile
#* Copyright(c): 2009 SecuGen Corporation, All rights reserved
#* date        person   comments
#* ======================================================
#* 12/10/2009  driley   initial release
#*
#*************************************************************

PORT = linux3
CFLAGS = -DLINUX3
CPP=g++
INCPATH = -I./ -I../include
LIBS =  -L../lib

FDU03_TARGET = ../bin/$(PORT)/wsq_check_devise_fdu03
FDU04_TARGET = ../bin/$(PORT)/wsq_check_devise_fdu04
FDU05_TARGET = ../bin/$(PORT)/wsq_check_devise_fdu05
FDU06_TARGET = ../bin/$(PORT)/wsq_check_devise_fdu06
FDU07_TARGET = ../bin/$(PORT)/wsq_check_devise_fdu07
FDU07A_TARGET = ../bin/$(PORT)/wsq_check_devise_fdu07a
FDU08_TARGET = ../bin/$(PORT)/wsq_check_devise_fdu08

all : $(FDU08_TARGET) $(FDU07_TARGET) $(FDU06_TARGET) $(FDU05_TARGET) $(FDU04_TARGET) $(FDU03_TARGET)

$(FDU08_TARGET) : $(OBJS)
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU08 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu08 -lusb -lpthread

$(FDU07A_TARGET) : $(OBJS)
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU07 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu07a -lAK38XX -lAKXU -lusb -lpthread

$(FDU07_TARGET) :
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU07 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu07 -lusb -lpthread

$(FDU06_TARGET) :
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU06 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu06 -lusb -lpthread

$(FDU05_TARGET) :
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU05 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu05 -lusb -lpthread

$(FDU04_TARGET) :
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU04 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu04 -lusb -lpthread

$(FDU03_TARGET) :
	rm -rf $(PORT)/main.o
	$(CPP) $(CFLAGS) $(INCPATH) -D_FDU03 -o $@ main.cpp $(LIBS) -lsgfplib -lsgnfiq -lsgimage -lsgfpamx -lsgfdu03 -lusb -lpthread

clean :
	rm -rf *.raw $(OBJS) $(FDU03_TARGET) $(FDU04_TARGET) $(FDU05_TARGET) $(FDU06_TARGET) $(FDU07_TARGET) $(FDU07A_TARGET) $(FDU08_TARGET) core
	rm -rf $(PORT)
	mkdir $(PORT)

