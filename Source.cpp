#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>

struct City {
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


/* Prototypes */

void displayGrid(City**, int, int);
void assignCities(City**, int, int);


int main() {
	
	/* variables */
	unsigned int numOfCities; // number of cities
	unsigned int gridSize;  //size of sqaure grid

	std::cout << "Enter number of cities: ";
	std::cin >> numOfCities;

	std::cout << "Enter size of square grid: ";  // must be more than the sqrt(numOfcities)
	std::cin >> gridSize;

	City ** listOfCities = new City*[numOfCities];  // dynamic array of Cities

	assignCities(listOfCities, gridSize, numOfCities);

	displayGrid(listOfCities,gridSize,numOfCities); // displays the grid with cities
	
	delete[] listOfCities;

	return 0;
}

void assignCities(City** listOfCities, int gridSize, int numOfCities) {
	/* random seed */
	srand(time(NULL));
	
	//assigns city spot on grid
	for (int i = 0; i < numOfCities; i++) {
		unsigned int tempX = rand() % gridSize; // assigns random x coordinate
		unsigned int tempY = rand() % gridSize; // assigns random y coordinate
		if (i == 0)
			listOfCities[i] = new City(tempX, tempY, i);
		else {
			// checks for duplicate cities
			for (int j = 0; j < i; j++) {
				if ((listOfCities[j]->getXCord() == tempX) && (listOfCities[j]->getYCord() == tempY)) {
					tempX = rand() % gridSize;
					tempY = rand() % gridSize;
					j = -1;  // reset loop
				}
				else
					listOfCities[i] = new City(tempX, tempY, i);
			}
		}
		std::cout << "City " << (i + 1) << ": (" << listOfCities[i]->getXCord() << ", " << listOfCities[i]->getYCord() << ")\n";
	}
}

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
				if ((list[k]->getXCord() == j) && (list[k]->getYCord() == i)) {
					cityFound = true; 
					cityNum = list[k]->getCityNum();
					break;
				}
			}
			if (cityFound)
				std::cout << "|" << cityNum;
			else
				std::cout << "| ";
		}
		std::cout << "|" << std::endl;
	}
}
