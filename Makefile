BIN_PATH=/usr/bin
DESKTOP_DIR=/usr/share/applications

all: build install clean

build:
	gcc -o mterm main.c `pkg-config --cflags --libs gtk+-3.0 vte-2.91`

install:
	# Install the compiled mterm binary
	install -Dm755 mterm $(BIN_PATH)/mterm

	# Install the .desktop file
	install -Dm644 mterm.desktop $(DESKTOP_DIR)/mterm.desktop

clean:
	rm -f mterm
