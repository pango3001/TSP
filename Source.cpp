/*
 Title: Travelling Saleman Brute Force
 Filename: Source.cpp
 External Files Used: n/a
 External Files Created: n/a
 Author(s): Jesse McCarville-Schueths, Cooper Epstein, Ben Morrison
 Author Email(s):jtmzv8@mail.umsl.edu, bjmz67@mail.umsl.edu
 Course: 4500-002
 Date: 2/28/2020
 Summary: Simulates the travelling saleman problem, using
 a brute force method.
 Resources: https://sahebg.github.io/computersceince/travelling-salesman-problem-c-program/
 */

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>
#include <math.h>	/* sqrt */

/* Class for City Objects*/
class City {
private:
	unsigned int cityNumber; // numerical identifier for city
	unsigned int xCord;
	unsigned int yCord;
	bool visited;

public:
	/* Constructor */
	City(int x, int y, int cityNumber) {
		this->xCord = x;
		this->yCord = y;
		this->cityNumber = cityNumber;
		visited = false;
	}

	/* Accessors */
	int getXCord() { return xCord; }
	int getYCord() { return yCord; }
	int getCityNum() { return cityNumber; }


	/* Mutators */
	void setCords(int x, int y) {
		this->xCord = x;
		this->yCord = y;
	}

	void setVisited(bool visit) {
		this->visited = visit;
	}
};

/* Global variables*/
int visited_cities[10], cost = 0;


/* Prototypes */
int getValidNumber(int, int);
void displayGrid(City**, int, int);
void assignCities(City**, int, int);
int tsp(int c, float matrix[30][30], int numcities);
void minimum_cost(int city, float matrix[30][30], int numcities);

float city_distance(City* a, City* b){
	float dx = a->getXCord() - b->getXCord();
	float dy = a->getYCord() - b->getYCord();

	return sqrt((dx * dx) + (dy * dy));
}

int main() {

	/* variables */
	unsigned int numOfCities; // number of cities
	unsigned int gridSize;  //size of sqaure grid

	std::cout << "Enter number of cities (min:4, max:9): ";
	numOfCities = getValidNumber(4, 9);

	std::cout << "Enter size of square grid (min:10, max:30): ";  // must be more than the sqrt(numOfcities)
	gridSize = getValidNumber(10, 30);

	City** listOfCities = new City * [numOfCities];  // dynamic array of Cities

	assignCities(listOfCities, gridSize, numOfCities); // assigns cities to coordinates

	displayGrid(listOfCities, gridSize, numOfCities); // displays the grid with cities

	printf("cost matrix: \n");
	float cm[30][30];
	// iterate through citys and display cost matrix
	for (int i = 0; i < numOfCities; i++) {
		for (int j = 0; j < numOfCities; j++) {
			cm[i][j] = city_distance(listOfCities[i], listOfCities[j]);
			printf("%5.1f ", cm[i][j]);
		}
		printf("\n");
	}

	// display shortest city path
	printf("minimum path: ");
	minimum_cost(0, cm, numOfCities);

	printf("\ncost %d\n", cost); //display min cost

	delete[] listOfCities;

	return 0;
}

//Checks that recieved input is valid
int getValidNumber(int min, int max) {
	std::string num;
	int number;
	bool isNumber = true;
	bool end_loop = false;
	while (!end_loop) {
		getline(std::cin, num);
		if (num.empty()) { //branch if no input is given
			isNumber = false;
			std::cout << "No input was entered." << std::endl;
		}
		else { //validates if recieved input is a number, and only a  number
			for (unsigned int i = 0; i < num.length(); i++) {
				if (!isdigit(num[i])) { //branch if any point in input is not a number
					isNumber = false;
					std::cout << "Only numbers between " << min << " and " << max << " can be given as input." << std::endl;
					break;
				}
				else //branch if input is only digits
					isNumber = true;
			}
		}
		if (isNumber) {
			number = stoi(num);
			if (number >= min && number <= max) //validates number is between acceptable min and max
				end_loop = true;
			else //branch if number is outside min and max
				std::cout << "Only numbers between " << min << " and " << max << " can be given as input." << std::endl;
		}
	}
	return number;
}


// assigns city to spaces on grid
void assignCities(City** listOfCities, int gridSize, int numOfCities) {
	/* random seed */
	srand(time(NULL));

	//assigns city spot on grid
	for (int i = 0; i < numOfCities; i++) {
		unsigned int tempX = rand() % gridSize; // assigns random x coordinate
		unsigned int tempY = rand() % gridSize; // assigns random y coordinate
		if (i == 0)
			listOfCities[i] = new City(tempX, tempY, (i + 1));
		else {
			// checks for duplicate cities
			for (int j = 0; j < i; j++) {
				//if duplicate set new coordinates
				if ((listOfCities[j]->getXCord() == tempX) && (listOfCities[j]->getYCord() == tempY)) {
					tempX = rand() % gridSize;
					tempY = rand() % gridSize;
					j = -1;  // reset loop
				}
				else
					listOfCities[i] = new City(tempX, tempY, (i + 1)); //place new city
			}
		}
		std::cout << "City " << (i + 1) << ": (" << listOfCities[i]->getXCord() << ", " << listOfCities[i]->getYCord() << ")\n"; //display city and location
	}
}


// displays grid of cities and spaces
void displayGrid(City** list, int grid, int numOfCities) {

	unsigned int cityNum;
	bool cityFound = false;

	// display y axis on board
	for (int i = 0; i < grid; i++) {
		// display x axis on board
		for (int j = 0; j < grid; j++) {
			cityFound = false;
			//checks if city is at this coordinate
			for (int k = 0; k < numOfCities; k++) {
				if ((list[k]->getXCord() == j) && (list[k]->getYCord() == i)) {  //check if city is at that coordinate
					cityFound = true;
					cityNum = list[k]->getCityNum();
					break;
				}
			}
			if (cityFound)  //display city number
				std::cout << "|" << cityNum;
			else  // display empty space
				std::cout << "| ";
		}
		std::cout << "|" << std::endl;
	}
}

//finds the nearest city to the current one that has not yet been visited
int tsp(int c, float matrix[30][30], int numcities){
	int count, nearest_city = 999;
	int minimum = 999, temp;
	for (count = 0; count < numcities; count++) { //loops through matrix of cities
		//checks that current city is not starting city and that the city has not yet been visited
		if ((matrix[c][count] != 0) && (visited_cities[count] == 0)) {
			if (matrix[c][count] < minimum) { //sets minimum to the lowest city distance
				minimum = matrix[count][0] + matrix[c][count];
			}
			temp = matrix[c][count];
			nearest_city = count;
		}
	}
	if (minimum != 999) { //activates if a closer city does exist
		cost = cost + temp;
	}
	return nearest_city;
}

//calculates the shortest path
void minimum_cost(int city, float matrix[30][30], int numcities){
	int nearest_city;
	visited_cities[city] = 1;
	printf("%d ", city + 1);
	nearest_city = tsp(city, matrix, numcities);

	if (nearest_city == 999) { //activates if all available cities have been visted
		nearest_city = 0;
		printf("%d", nearest_city + 1);
		cost = cost + matrix[city][nearest_city];
		return;
	}
	minimum_cost(nearest_city, matrix, numcities); //recursively calculates the smallest path until all cities are reached
}