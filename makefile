TARGET=runtest
C_FLAGS= -std=c++14

test: $(TARGET)
	@./$(TARGET)

$(TARGET) : example.cpp Mix/*.cpp
	@echo building
	@$(CXX) $(C_FLAGS)  example.cpp Mix/*.cpp -o $(TARGET)
	@echo "Done"

clean:
	@echo "cleaning"
	@rm $(TARGET)
	
.PHONY: clean test
