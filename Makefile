CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11
PREFIX = /usr/local
COMBIGEN_DIR = ./src

all: combigen

combigen: combigen.o
	$(CXX) $(CXXFLAGS) build/combigen.o -o combigen

combigen.o: $(COMBIGEN_DIR)/combigen.cpp $(COMBIGEN_DIR)/combigen.h
	$(CXX) $(CXXFLAGS) $(COMBIGEN_DIR)/combigen.cpp -c -o build/combigen.o

.PHONY: clean
clean:
	@rm -f build/*.o combigen


.PHONY: install
install: combigen
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp $< $(DESTDIR)$(PREFIX)/bin/combigen

.PHONY: uninstall
uninstall:
	@rm -f $(DESTDIR)$(PREFIX)/bin/combigen