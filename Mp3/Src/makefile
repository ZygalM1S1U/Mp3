mp3.exe:	main.o	mp3decode.o
	gcc -L -o main.o mp3decode.o

main.o:	main.c
	gcc -c main.c

mp3decode.o: mp3decode.c
	gcc -c mp3decode.c

clean:
	rm main.o
