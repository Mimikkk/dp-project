rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
SOURCES=$(call rwildcard, src/, *.cpp)
HEADERS=$(call rwildcard, src/, *.hpp)
FLAGS=-g -std=gnu++2a -fconcepts

all: build run-1-1

build: $(SOURCES) $(HEADERS)
	@echo "Building..."
	@mkdir -p bin/
	@mpicxx $(SOURCES) $(FLAGS) -o bin/application
	@echo "Build success!"

clear:
	@echo "Clearing..."
	@rm -rf ./bin
	@echo "Cleared!"

start: build run

run:
	@echo "Running application with $(PC) poet/s $(VC) volunteer/s"
	@echo "Running with $$(( $(PC) + $(VC) )) process/es"
	mpirun -np $$(( $(PC) + $(VC) )) -oversubscribe bin/application $(PC) $(VC)

run-1-1:
	@make run PC=1 VC=1

run-4-1:
	@make run PC=4 VC=1

run-12-4:
	@make run PC=12 VC=4
