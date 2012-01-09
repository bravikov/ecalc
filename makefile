TARGET = ecalc
SRC = ecalc.cpp

TARGET:
	g++ $(SRC) -o $(TARGET)
	
run: $(TARGET)
	./$(TARGET)
