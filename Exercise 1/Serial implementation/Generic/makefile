# Compiler
CXX = g++
# The cpp flags
CXXFLAGS :=
# The number of runs to do
runs ?= 10
# Size of A
a_size ?= 100x100
# Size of B
b_size ?= 100x100

Matrix_Multiply_Serial:
    # Compiling the application
	@echo "Compiling Matrix_Multiply_Serial"
	@$(CXX) $(CXXFLAGS) ./src/Matrix_Multiply_Serial.cpp -o ./bin/Matrix_Multiply_Serial.exe
	@echo "Done Compiling Matrix_Multiply_Serial"
    # Running the application for the number of runs
	@echo "Running Matrix_Multiply_Serial.exe for $(runs) times using A of size $(a_size) and B of size $(b_size)"
	@for i in {1..$(runs)}; do ./bin/Matrix_Multiply_Serial.exe -A $(a_size) -B $(b_size); done