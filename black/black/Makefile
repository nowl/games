SRC = src/libblack.so
DEST = ../libblack.so

.PHONY: clean $(MAIN)

all:
	cd src; $(MAKE)
	mv $(SRC) $(DEST)

clean:
	cd src; $(MAKE) clean
	rm -f $(DEST)