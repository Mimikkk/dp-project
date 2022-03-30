all: build run-12-4

build:
	@echo "Building..."
	$(MAKE) -C ./cmake-build-debug/
	@echo "Built!."

run-12-4:
	@echo "Running with 12 poets and 4 volunteers."
	@mpirun -np 16 -oversubscribe ./bin/task 12 4
