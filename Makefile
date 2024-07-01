BIN_PATH=/usr/bin
all: build install clean

build:
	gcc -o mterm main.c `pkg-config --cflags --libs gtk+-3.0 vte-2.91`
install:
	install -Dm755 mterm $(BIN_PATH)/mterm
clean:
	rm ./mterm
