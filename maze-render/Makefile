MAIN = dm

.PHONY: $(MAIN) 

$(MAIN):
	cd src; $(MAKE)
	mv src/$(MAIN) $(MAIN)

clean:
	cd src; $(MAKE) clean
	rm -f $(MAIN)
