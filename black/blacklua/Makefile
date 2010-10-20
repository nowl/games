SRC = src/libblacklua.so
DEST = ../libblacklua.so

.PHONY: clean $(MAIN)

all:
	cd src; $(MAKE)
	mv $(SRC) $(DEST)

clean:
	cd src; $(MAKE) clean
	rm -f $(DEST)