# Host-side tests: builds the library against a small Arduino stand-in and runs the checks.
#   make -C test          build and run with the default standard (C++17)
#   make -C test STD=c++11
#   make -C test examples   also build and run every example

STD      ?= c++17
CXX      ?= g++
CXXFLAGS  = -std=$(STD) -Wall -Wextra -Wdouble-promotion -Wsign-compare -Wshadow -Werror \
            -Istub -I../src

run: test_main
	./test_main

test_main: test_main.cpp ../src/VectorUtilities.cpp ../src/VectorUtilities.h stub/Arduino.h
	$(CXX) $(CXXFLAGS) test_main.cpp ../src/VectorUtilities.cpp -lm -o $@

# Build every example against the stub and run it for a few simulated seconds
examples: ../src/VectorUtilities.cpp stub/host_main.cpp
	@for dir in ../examples/*/; do \
	  name=$$(basename $$dir); \
	  $(CXX) $(CXXFLAGS) -include Arduino.h -x c++ $$dir$$name.ino -x none \
	    ../src/VectorUtilities.cpp stub/host_main.cpp -lm -o example_$$name || exit 1; \
	  ./example_$$name > example_$$name.log || exit 1; \
	  echo "$$name ok"; \
	done

clean:
	rm -f test_main example_*

.PHONY: run examples clean
