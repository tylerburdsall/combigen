CXX = g++
CXXFLAGS = -Wall -O2 -std=c++14
LIBFLAGS =
BOOSTFLAGS = -DUSE_BOOST
PREFIX = /usr/local
COMBIGENDIR = ./src
COMBIGENFILE = combigen.cpp
BUILDDIR = release

all: main

main:	cli_functions.o combigen.o main.o
	$(CXX) $(CXXFLAGS) build/$(BUILDDIR)/main.o build/$(BUILDIR)/combigen.o build/$(BUILDDIR)/cli_functions.o -o combigen $(LIBFLAGS)

main.o: $(COMBIGENDIR)/main.cpp
	$(CXX) $(CXXFLAGS) $(COMBIGENDIR)/main.cpp -c -o build/$(BUILDDIR)/main.o

combigen.o: $(COMBIGENDIR)/combigen.cpp $(COMBIGENDIR)/combigen.h
	$(CXX) $(CXXFLAGS) $(COMBIGENDIR)/$(COMBIGENFILE) -c -o build/$(BUILDDIR)/combigen.o

cli_functions.o: $(COMBIGENDIR)/cli_functions.cpp $(COMBIGENDIR)/combigen.h
	$(CXX) $(CXXFLAGS) $(COMBIGENDIR)/cli_functions.cpp -c -o build/$(BUILDDIR)/cli_functions.o

.PHONY: perf
perf: CXXFLAGS += $(BOOSTFLAGS)
perf: LIBFLAGS += -lboost_random
perf: COMBIGENFILE = boost_functions.cpp
perf: BUILDDIR = perf
perf: main

.PHONY: clean
clean:
	@rm -f build/*/*.o combigen


.PHONY: install
install: combigen
	@mkdir -p $(DESTDIR)$(PREFIX)/bin
	@cp $< $(DESTDIR)$(PREFIX)/bin/combigen
	@cp doc/combigen.1 /usr/local/man/man1/
	@gzip /usr/local/man/man1/combigen.1

.PHONY: uninstall
uninstall:
	@rm -f $(DESTDIR)$(PREFIX)/bin/combigen
	@rm -f /usr/local/man1/combigen.1 /usr/local/man1/combigen.1.gz
