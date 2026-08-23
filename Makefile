CC = gcc
STD = gnu99
CFLAGS = -Wall -Wextra -Werror -pedantic -pedantic-errors -std=$(STD) -g
MAKEFLAGS += --no-print-directory

%.o: %.c
	@$(CC) -c -o $@ $<

main: main.o logger/logger.o
	@$(CC) $(CFLAGS) -o $@.exe $^
	@./$@.exe

ifeq ($(OS),Windows_NT)
SHELL := cmd.exe
clean:
	@del /s /q *.o 2>nul || exit 0
	@del /s /q *.exe 2>nul || exit 0
else
clean:
	@find . -type f -name "*.o" -delete
	@find . -type f -name "*.exe" -exec rm -f {} +
endif