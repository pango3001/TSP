#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <string>
#include <math.h>	/* sqrt */

/* code adapted from https://sahebg.github.io/computersceince/travelling-salesman-problem-c-program/ */

int visited_cities[10], cost = 0;
 
int tsp (int c, float matrix[30][30], int numcities)
{
	int count, nearest_city = 999;
	int minimum = 999, temp;
	for(count = 0; count < numcities; count++) {
		if((matrix[c][count] != 0) && (visited_cities[count] == 0)) {
			if(matrix[c][count] < minimum) {
				minimum = matrix[count][0] + matrix[c][count];
			}
			temp = matrix[c][count];
			nearest_city = count;
		}
	}
	if(minimum != 999) {
		cost = cost + temp;
	}
	return nearest_city;
}
 
void minimum_cost(int city, float matrix[30][30], int numcities)
{
	int nearest_city;
	visited_cities[city] = 1;
	printf("%d ", city + 1);
	nearest_city = tsp(city, matrix, numcities);
	
	if (nearest_city == 999) {
		nearest_city = 0;
		printf("%d", nearest_city + 1);
		cost = cost + matrix[city][nearest_city];
		return;
	}
	minimum_cost(nearest_city, matrix, numcities);
}
 


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


/* Prototypes */
int getValidNumber(int, int);
void displayGrid(City**, int, int);
void assignCities(City**, int, int);

float city_distance (City *a, City *b)
{
	float dx = a->getXCord() - b->getXCord();
	float dy = a->getYCord() - b->getYCord();
	
	return sqrt((dx * dx) + (dy * dy));
}

int main() {
	
	/* variables */
	unsigned int numOfCities; // number of cities
	unsigned int gridSize;  //size of sqaure grid

	std::cout << "Enter number of cities (min:4, max:9): ";
	numOfCities = getValidNumber(4,9);

	std::cout << "Enter size of square grid (min:10, max:30): ";  // must be more than the sqrt(numOfcities)
	gridSize = getValidNumber(10, 30);

	City ** listOfCities = new City*[numOfCities];  // dynamic array of Cities

	assignCities(listOfCities, gridSize, numOfCities);

	displayGrid(listOfCities,gridSize,numOfCities); // displays the grid with cities
	
	printf("cost matrix: \n");
	float cm[30][30];
	for (int i = 0; i < numOfCities; i++) {
		for (int j = 0; j < numOfCities; j++) {
			cm[i][j] = city_distance(listOfCities[i], listOfCities[j]);
			printf("%5.1f ", cm[i][j]);
		}
		printf("\n");
	}
	
	printf("minimum path: ");
	
	minimum_cost(0, cm, numOfCities);
	
	printf("\ncost %d\n", cost);
	
	delete[] listOfCities;

	return 0;
}

int getValidNumber(int min, int max) {
		std::string num;
		int number;
		bool isNumber = true;
		bool end_loop = false;
		while (!end_loop) {
			getline(std::cin, num);
			if (num.empty()) {
				isNumber = false;
				std::cout << "No input was entered." << std::endl;
			}
			else {
				for (unsigned int i = 0; i < num.length(); i++) {
					if (!isdigit(num[i])) {
						isNumber = false;
						std::cout << "Only numbers between " << min << " and " << max << " can be given as input." << std::endl;
						break;
					}
					else
						isNumber = true;
				}
			}
			if (isNumber) {
				number = stoi(num);
				if (number >= min && number <= max)
					end_loop = true;
				else
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
				if ((listOfCities[j]->getXCord() == tempX) && (listOfCities[j]->getYCord() == tempY)) {
					tempX = rand() % gridSize;
					tempY = rand() % gridSize;
					j = -1;  // reset loop
				}
				else
					listOfCities[i] = new City(tempX, tempY, (i + 1));
			}
		}
		std::cout << "City " << (i+1) << ": (" << listOfCities[i]->getXCord() << ", " << listOfCities[i]->getYCord() << ")\n";
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
