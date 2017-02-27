# Makefile for Lab 2
# CSE 2431
# Dylan Brown

lab2: lab2.o
	gcc -o lab2 lab2.o

lab2.o: lab2.c buffer.h
	gcc -c -Wall lab2.c buffer.h -lpthread -lrt
