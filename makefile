rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SOURCES=$(call rwildcard, src/, *.cpp)
HEADERS=$(call rwildcard, src/, *.hpp)
FLAGS=-g -std=gnu++2a -fconcepts

all: main run-1-1

main: $(SOURCES) $(HEADERS)
	mpicxx $(SOURCES) $(FLAGS) -o bin/application

clear: clean

clean:
	rm main a.out

run-1-1:
	@echo "Running with 1 poet and 1 volunteer."
	@mpirun -np 2 -oversubscribe ./bin/application 1 1

run-4-1:
	@echo "Running with 4 poet and 1 volunteer."
	@mpirun -np 5 -oversubscribe ./bin/application 4 1

run-12-4:
	@echo "Running with 12 poets and 4 volunteers."
	@mpirun -np 16 -oversubscribe ./bin/application 12 4
