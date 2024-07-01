build:
	gcc -o mterm main.c `pkg-config --cflags --libs gtk+-3.0 vte-2.91`
install:
	sudo cp ./mterm /usr/bin
clean:
	rm ./mterm
