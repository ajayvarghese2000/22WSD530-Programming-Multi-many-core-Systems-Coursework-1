# Compiler
CXX = g++
# The number of runs to do
runs ?= 1
# Number of threads
threads ?= 8
# The matrix to use
matrix ?= matrix_4x4
# Printmode
printmode ?= PRINT_MODE
# The cpp flags
CXXFLAGS := -D $(matrix) -D $(printmode) -lpthread

Matrix_Multiply_Pthreads_Fixed_Values:
    # Checking if inputs are correct
	@if [ "$(matrix)" != "matrix_1000x1000" ] && [ "$(matrix)" != "matrix_4x4" ]; then echo "The matrix size is not correct. Please use matrix_1000x1000 or matrix_4x4"; exit 1; fi
	@if [ "$(printmode)" != "PRINT_MODE" ] && [ "$(printmode)" != "NO_PRINT_MODE" ]; then echo "The printmode is not correct. Please use PRINT_MODE or NO_PRINT_MODE"; exit 1; fi
    # Compiling the application
	@echo "Compiling Matrix_Multiply_Pthreads_Fixed_Values"
	$(CXX) $(CXXFLAGS) ./src/Matrix_Multiply_Pthreads_Fixed_Values.cpp ./includes/*.h -o ./bin/Matrix_Multiply_Pthreads_Fixed_Values.exe
	@echo "Done Compiling Matrix_Multiply_Pthreads_Fixed_Values"
    # Running the application for the number of runs
	@echo "Running Matrix_Multiply_Pthreads_Fixed_Values.exe for $(runs) times using  using $(threads) threads"
	@for i in {1..$(runs)}; do ./bin/Matrix_Multiply_Pthreads_Fixed_Values.exe -T $(threads); done