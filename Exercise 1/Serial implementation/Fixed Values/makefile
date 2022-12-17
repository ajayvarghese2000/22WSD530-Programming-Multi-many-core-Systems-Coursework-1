# Compiler
CXX = g++
# Number of runs to execute
runs = 10
# Matrix size can be matrix_1000x1000 or matrix_4x4
matrix = matrix_1000x1000
# the timer definition
timer = timer
# The cpp flags
CXXFLAGS := -D $(matrix) -D $(timer)

# This will compile and run the serial version of the matrix multiplication
Matrix_Multiply_Serial_Fixed_Values:
    # Checking if the inputs are correct
	@if [ "$(matrix)" != "matrix_1000x1000" ] && [ "$(matrix)" != "matrix_4x4" ]; then echo "The matrix size is not correct. Please use matrix_1000x1000 or matrix_4x4"; exit 1; fi
	@if [ "$(timer)" != "timer" ] && [ "$(timer)" != "no_timer" ]; then echo "The timer is not correct. Please use timer or no_timer"; exit 1; fi
    # Compiling the application
	@echo "Compiling Matrix_Multiply_Serial_Fixed_Values"
	@$(CXX) $(CXXFLAGS) ./src/Matrix_Multiply_Serial_Fixed_Values.cpp ./includes/*.h -o ./bin/Matrix_Multiply_Serial_Fixed_Values.exe
	@echo "Done Compiling Matrix_Multiply_Serial_Fixed_Values"
    # Running the application
	@echo "Running Matrix_Multiply_Serial_Fixed_Values for $(runs) times using the $(matrix) matrix"
	@for number in {1..$(runs)}; do ./bin/Matrix_Multiply_Serial_Fixed_Values.exe; done
