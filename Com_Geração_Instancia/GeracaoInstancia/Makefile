CC = gcc
CFLAGS = -Wall -Wextra -O3 -Wl,--large-address-aware
TARGET = gerador_mochila.exe

all:
	$(CC) $(CFLAGS) main.c gerador.c dp.c -o $(TARGET)

clean:
	del /Q /F $(TARGET)