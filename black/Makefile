MAIN = fallen

.PHONY: clean $(MAIN)

$(MAIN):
	cd black; $(MAKE)
	cd blacklua; $(MAKE)

clean:
	cd black; $(MAKE) clean
	cd blacklua; $(MAKE) clean
