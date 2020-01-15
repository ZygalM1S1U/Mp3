mp3.exe:	mp3decode.o	main.o
	gcc -L -o mp3decode.o main.o

mp3decode.o: mp3decode.c
	gcc -c mp3decode.c

main.o:	main.c
	gcc -c main.c

clean:
	rm main.o mp3decode.o
