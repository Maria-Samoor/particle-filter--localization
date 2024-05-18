//Maria Abu Samoor (237111) && Reem Ihdeib(201066)

#include <iostream>
#include <iomanip> // for setprecision
#include <cmath> // for M_PI
#include <fstream>
#include <sstream>
#include <cstdlib>  // for rand() and RAND_MAX

using namespace std;

const int ENV_SIZE = 100;//100*100 enviroment
const int PART_SIZE = ENV_SIZE /4; //25*25 square

struct PFStruct {
    float x, y, theta; // particle position
    float w;           // particle weight
};

struct Command {
    int steps;      // Forward steps
    float theta;    // Angle to rotate 
};
void update(PFStruct &particle, int Z) {
    //update logic based on observation Z and particle location
    int indexX = static_cast<int>(particle.x);
    int indexY = static_cast<int>(particle.y);
    bool isBright = ((indexX / PART_SIZE) % 2 == 0) == ((indexY / PART_SIZE) % 2 == 0);

    if (Z == 0) { // Bright observation
        if (isBright) {
            // particle in a bright quarter
            particle.w = 0.7;
        } else {
            // particle in a dark quarter
            particle.w = 0.2;
        }
    } else { // Dark observation
        if (isBright) {
            // particle in a bright quarter
            particle.w = 0.3;
        } else {
            // particle in a dark quarter
            particle.w = 0.8;
        }
    }
}

void normalize(PFStruct (&S)[], int size) {
    float sum = 0;

    // calculate the sum of weights
    for (int i = 0; i < size; ++i) {
        sum += S[i].w;
    }

    // normalize the weights
    for (int i = 0; i < size; ++i) {
        S[i].w = S[i].w / sum;
    }
}

void initi(PFStruct (&S)[], int size) {

    for (int i = 0; i < size; ++i) {
        // initialize particle position with random float values in the specified ranges
        S[i].x = static_cast<float>(rand()) / RAND_MAX * 100.0f;       // random float between 0.0 and 100.0, where RAND_MAX=1
        S[i].y = static_cast<float>(rand()) / RAND_MAX * 100.0f;       // random float between 0.0 and 100.0, where RAND_MAX=1
        S[i].theta = static_cast<float>(rand()) / RAND_MAX * 360.0f;   // random float between 0.0 and 360.0, where RAND_MAX=1
        S[i].theta = (S[i].theta * 3.14159265358979323846f) / 180.0;  // convert theta from degrees to radians, where M_PI=3.14159265358979323846

        // initialize weight to 0.01
        S[i].w = 1.0/size;
    }
}

int sample(PFStruct (&S)[], int size) {
    // create weightRange array for cdf and initialize weightRange[0]
    float weightRange[size];
    weightRange[0] = S[0].w;

    // populate weightRange based on the specified formula it has values [0,1]
    for (int i = 1; i < size; ++i) {
        weightRange[i] = weightRange[i - 1] + S[i].w;
    }

    // generate a random float between 0 and 1
    float randNum = static_cast<float>(rand()) / RAND_MAX;

    // find the index in weightRange where randNum exists in 
    int index = 0;
    while (index < size && randNum > weightRange[index]) {
        ++index;
    }

    // return the index of the sampled particle
    return index;
}
void ParticleFilter(PFStruct (&S)[] , int size, int Z, Command &U,PFStruct (&updatedParticles)[]) {
    for (int j = 0; j < 100; ++j) {
        int i = sample(S, size);
        updatedParticles[j] = S[i]; // copy the particle with index i(sampled) to the new array

        // update the selected particle based on the command U
        updatedParticles[j].theta = U.theta;
        
        // assign particles new x and y to proposedX and proposedY variables to validate there values
        float proposedX = updatedParticles[j].x + (cos(updatedParticles[j].theta) * U.steps);
        float proposedY = updatedParticles[j].y + (sin(updatedParticles[j].theta) * U.steps);

        // enforce bounds(100*100) using conditional assignment to find if it was >100 do not change it is location 
        updatedParticles[j].x = (proposedX >= 0 && proposedX <= 100) ? proposedX : updatedParticles[j].x;
        updatedParticles[j].y = (proposedY >= 0 && proposedY <= 100) ? proposedY : updatedParticles[j].y;
        

        // update the weight based on the observation Z
       update(updatedParticles[j], Z);
    }

    // normalize the weights in the updatedParticles array
    normalize(updatedParticles, 100);
}
// function to create the environment
void createEnvironment (int Env[][ENV_SIZE]) {
  for (int i = 0; i < ENV_SIZE; i++)
    {
      for (int j = 0; j < ENV_SIZE; j++)
	{

	  bool isBright =
	    ((i / PART_SIZE) % 2 == 0) == ((j / PART_SIZE) % 2 == 0);
	  if (isBright)
	    {
	      Env[i][j] = 0;
	    }
	  else
	    {
	      Env[i][j] = 1;
	    }

	}
    }
}

int checkObservation(float robotX, float robotY, const int Env[][ENV_SIZE]) {
    // convert floating-point coordinates to integer to find there value at the env 
    int indexX = static_cast<int>(robotX);
    int indexY = static_cast<int>(robotY);

    //  if the point is brightit will return 0 and if at black it will return 1
    return Env[indexX][indexY];
}

void readCSVLine(const string& filename, int lineNumber, float &x, float &y, float &displacement, float &angle) {
    ifstream file(filename);
    string line;

    for (int i = 1; i <= lineNumber; ++i) {
        if (!getline(file, line)) {
            cout << "out of scope\n";
            return; 
        }
        if (i == lineNumber) {
            istringstream iss(line);
            string value;
            int count = 0;
            while (getline(iss, value, ',')) {
                count++;
                switch(count) {
                    case 1: x = stof(value); break;
                    case 2: y = stof(value); break;
                    case 3: displacement = stof(value); break;
                    case 4: angle = stof(value); break;
                }
            }
            break;
        }
    }
    file.close();
}
void report(PFStruct (&S)[], int arraySize, float robotX, float robotY) {
    float meanParticleX = 0, meanParticleY = 0, varParticleX = 0, varParticleY = 0;

    // Calculate the mean for x and y
    for (int i = 0; i < arraySize; i++) {
        meanParticleX += S[i].x;
        meanParticleY += S[i].y;
    }
    meanParticleX /= arraySize;
    meanParticleY /= arraySize;

    // Calculate the variance for x and y
    for (int i = 0; i < arraySize; i++) {
        varParticleX += (S[i].x - meanParticleX) * (S[i].x - meanParticleX);
        varParticleY += (S[i].y - meanParticleY) * (S[i].y - meanParticleY);
    }
    varParticleX /= arraySize;
    varParticleY /= arraySize;

    cout << "RobotX: " << robotX << ", RobotY: " << robotY
        << ", MeanParticleX: " << meanParticleX
        << ", MeanParticleY: " << meanParticleY
        << ", VarParticleX: " << varParticleX
        << ", VarParticleY: " << varParticleY
        << "\n";
}
int main() {
    const int arraySize = 100;
    PFStruct S[arraySize]; // declare an array of type PFStruct
    PFStruct updatedParticles[arraySize];  // declare an array of type PFStruct to store updated particles
    int Z;                                // observation
    int Env[ENV_SIZE][ENV_SIZE ] ; // initialize Env
    // create the environment
    createEnvironment(Env);
    // initialize particles
    initi(S, arraySize);
    struct Command U; // command to update paticles 
    float robotX, robotY, displacement, angle;
    for (int i = 0; i <= 500; i++) {
        readCSVLine("robot.txt",i + 2, robotX, robotY, displacement, angle);
        Z=checkObservation(robotX, robotY, Env);
        U.steps=displacement;
        U.theta=(angle* 3.14159265358979323846f) / 180.0; // Convert theta from degrees to radians, where M_PI=3.14159265358979323846
        ParticleFilter(S, arraySize, Z, U, updatedParticles);
        for (int i = 0; i < arraySize; ++i) {
            S[i] = updatedParticles[i];
        }
        report(S, arraySize, robotX, robotY);
    }

    return 0;
}
