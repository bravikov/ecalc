TARGET = ecalc
SRC = ecalc.cpp

TARGET:
	g++ -Wall $(SRC) -o $(TARGET)
	
run: $(TARGET)
	./$(TARGET)
