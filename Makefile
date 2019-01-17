CXX = g++
CPPFLAGS = -g -Wall -std=c++11
BIN_DIR = bin

all: search

crawler: config.h crawler.cpp
	$(CXX) $(CPPFLAGS) config.h crawler.cpp -o crawler

search: $(BIN_DIR)/webpage.o $(BIN_DIR)/database.o
	$(CXX) $(CPPFLAGS) $(BIN_DIR)/webpage.o $(BIN_DIR)/database.o search.cpp -o search

$(BIN_DIR)/database.o: database.h database.cpp $(BIN_DIR)/.dirstamp
	$(CXX) $(CPPFLAGS) -c database.cpp -o $(BIN_DIR)/database.o

$(BIN_DIR)/webpage.o: webpage.h webpage.cpp $(BIN_DIR)/.dirstamp
	$(CXX) $(CPPFLAGS) -c webpage.cpp -o $(BIN_DIR)/webpage.o

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)

$(BIN_DIR)/.dirstamp:
	mkdir -p $(BIN_DIR)
	touch $(BIN_DIR)/.dirstamp