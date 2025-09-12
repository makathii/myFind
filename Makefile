#
# \file Makefile
# \author Kathi
# \date Created at: 2025-09-12
# \date Last modified at: 2025-09-12
# ---
#



all: main.cpp
	
main.cpp: finder 
	g++ -Wall -Wextra --std=c++20 -o ./build/myFind ./src/main.cpp ./build/finder

finder: 
	g++ -Wall -Wextra --std=c++20 -c -o ./build/finder ./src/finder/finder.cpp

run: 
	./build/myFind

clean:
	rm ./build/myFind