CC = gcc
CFLAGS = -Wall -O0
TARGET = simulador_oficial.exe

all:
	$(CC) $(CFLAGS) main.c algoritmos.c utils.c -o $(TARGET)

clean:
	del /Q /F $(TARGET)