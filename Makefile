CXX=g++
CXXFLAGS=-std=c++20

TARGET_NAME=ds
MANPAGE_NAME=ds.1

SOURCE_DIR=src/
BUILD_DIR=build/

INSTALL_DIR=usr/local/bin/
MANPAGE_DIR=usr/share/man/man1/

all : main.o
	$(CXX) -o $(BUILD_DIR)$(TARGET_NAME) $(BUILD_DIR)$^

dirs :
	@if [ ! -d "build" ]; then mkdir build; fi

main.o : dirs
	$(CXX) $(CXXFLAGS) -c $(SOURCE_DIR)main.cpp -o $(BUILD_DIR)main.o

clean :
	rm $(BUILD_DIR)*

install :
	cp $(BUILD_DIR)$(TARGET_NAME) $(INSTALL_DIR)
	cp $(SOURCE_DIR)$(MANPAGE_NAME) $(MANPAGE_DIR)