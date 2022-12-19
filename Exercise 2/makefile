# Compiler
CXX = g++
# The cpp flags
CXXFLAGS := -lpthread
# Number of Producer threads
producer_threads ?= 4
# Number of Consumer threads
consumer_threads ?= 1
# Number of items for each producer to produce
items ?= 4
# Size of the buffer
buffer_size ?= 10


producer_consumer:
    # Compiling the application
	@echo "Compiling producer_consumer"
	@$(CXX) $(CXXFLAGS) ./src/producer_consumer.cpp -o ./bin/producer_consumer.exe
	@echo "Done Compiling producer_consumer"

test:
    # Running the application for the number of runs
	@echo "Running producer_consumer.exe with $(producer_threads) producer threads, $(consumer_threads) consumer threads, $(items) items per therad, and a buffer size of $(buffer_size)"
	@./bin/producer_consumer.exe -P $(producer_threads) -C $(consumer_threads) -I $(items) -B $(buffer_size)
	@echo "Done Running producer_consumer.exe"