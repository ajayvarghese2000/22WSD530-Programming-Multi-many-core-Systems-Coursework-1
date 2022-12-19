# Compiler
CXX = g++
# The cpp flags
CXXFLAGS := -lpthread
# Number of Producer threads
producer_threads ?= 3
# Number of Consumer threads
consumer_threads ?= 1
# Number of items for each producer to produce
items ?= 10
# Size of the buffer
buffer_size ?= 10


producer_consumer:
    # Compiling the application
	@echo "Compiling producer_consumer"
	@$(CXX) $(CXXFLAGS) ./src/producer_consumer.cpp -o ./bin/producer_consumer.exe
	@echo "Done Compiling producer_consumer"
    # Running the application for the number of runs
	@echo "Running producer_consumer.exe"
	@./bin/producer_consumer.exe -P $(producer_threads) -C $(consumer_threads) -I $(items) -B $(buffer_size)