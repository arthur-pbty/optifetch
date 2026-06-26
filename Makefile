CC = gcc
CFLAGS = -Wall -Wextra -O3 -s
TARGET = optifetch
SRC = main.c sysinfo.c logos.c config.c renderer.c

all: $(TARGET)

 $(TARGET): $(SRC)
    $(CC) $(CFLAGS) -o $(TARGET) $(SRC)

install: $(TARGET)
    mkdir -p $(DESTDIR)/usr/bin
    cp $(TARGET) $(DESTDIR)/usr/bin/
    mkdir -p $(DESTDIR)/usr/share/optifetch/logos
    cp -r logos/*.txt $(DESTDIR)/usr/share/optifetch/logos/ 2>/dev/null || true
    mkdir -p $(DESTDIR)/etc
    cp optifetch.conf $(DESTDIR)/etc/optifetch.conf 2>/dev/null || touch $(DESTDIR)/etc/optifetch.conf

uninstall:
    rm -f $(DESTDIR)/usr/bin/$(TARGET)
    rm -rf $(DESTDIR)/usr/share/optifetch
    rm -f $(DESTDIR)/etc/optifetch.conf

clean:
    rm -f $(TARGET)