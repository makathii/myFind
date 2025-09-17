#
# \file Makefile
# \author Kathi
# \date Created at: 2025-09-12
# \date Last modified at: 2025-09-12
# ---
#



all: main.cpp
	
main.cpp: finder options
	g++ -Wall -Wextra --std=c++20 -o ./build/myFind ./src/main.cpp ./build/finder

finder: 
	g++ -Wall -Wextra --std=c++20 -c -o ./build/finder ./src/finder/finder.cpp

finder_debug:
	g++ -Wall -Wextra --std=c++20 -g -c -o ./build/finder ./src/finder/finder.cpp

options:
	g++ -Wall -Wextra --std=c++20 -c -o ./build/options ./src/finder/options.cpp

options_debug:
	g++ -Wall -Wextra --std=c++20 -g -c -o ./build/options ./src/finder/options.cpp

run: 
	./build/myFind

debug: finder_debug options_debug
	g++ -Wall -Wextra --std=c++20 -o ./build/myfind ./src/main.cpp ./build/finder ./build/options

clean:
	rm ./build/myFind