# Compiler
CXX = g++
# Number of runs to execute
RUNS = 10
# Matrix size can be matrix_1000x1000 or matrix_4x4
matrix = matrix_1000x1000
# the timer definition
timer = timer
# The cpp flags
CXXFLAGS := -D $(matrix) -D $(timer)

# This will compile and run the serial version of the matrix multiplication
Matrix_Multiply_Serial_Fixed_Values:
	@echo "Compiling Matrix_Multiply_Serial_Fixed_Values"
	@$(CXX) $(CXXFLAGS) ./src/Matrix_Multiply_Serial_Fixed_Values.cpp ./includes/*.h -o ./bin/Matrix_Multiply_Serial_Fixed_Values.exe
	@echo "Done Compiling Matrix_Multiply_Serial_Fixed_Values"
	@echo "Running Matrix_Multiply_Serial_Fixed_Values for $(RUNS) times using the $(matrix) matrix"
	@for number in {1..$(RUNS)}; do ./bin/Matrix_Multiply_Serial_Fixed_Values.exe; done
