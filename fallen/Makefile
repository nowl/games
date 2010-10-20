MAIN = fallen

.PHONY: clean $(MAIN)

$(MAIN):
	cd Black; $(MAKE)
	cp Black/libblack.so .
	cp Black/libblacklua.so .
	cd src; $(MAKE)
	mv src/$(MAIN) $(MAIN)

clean:
	cd Black; $(MAKE) clean
	cd src; $(MAKE) clean
	rm -f $(MAIN)
