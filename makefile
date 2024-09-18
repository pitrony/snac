all: snac.exe

snac.exe: snac_v1.o
	gcc -o snac.exe snac_v1.o

snac_v1.o:  
	gcc -c --std=c99 -o snac_v1.o snac_v1.c

clean:
	del *.o 
	del snac.exe
