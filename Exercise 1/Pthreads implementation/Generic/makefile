# Compiler
CXX = g++
# The cpp flags
CXXFLAGS := -lpthread 
# The number of runs to do
runs ?= 1
# Size of A
a_size ?= 2000x2000
# Size of B
b_size ?= 2000x2000
# Number of threads
threads ?= 11

Matrix_Multiply_Pthreads:
    # Compiling the application
	@echo "Compiling Matrix_Multiply_Pthreads"
	@$(CXX) $(CXXFLAGS) ./src/Matrix_Multiply_Pthreads.cpp -o ./bin/Matrix_Multiply_Pthreads.exe
	@echo "Done Compiling Matrix_Multiply_Pthreads"
    # Running the application for the number of runs
	@echo "Running Matrix_Multiply_Pthreads.exe for $(runs) times using A of size $(a_size) and B of size $(b_size) using $(threads) threads"
	@for i in {1..$(runs)}; do ./bin/Matrix_Multiply_Pthreads.exe -A $(a_size) -B $(b_size) -T $(threads); done