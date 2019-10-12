// Variables
int numCompartments;

// Structs
struct compartment
{
    // Description
    // v is the voltage across the capacitor
    float v, m, h, n;         // Actual array
    float fev, fem, feh, fen; // Forward Euler array
    float bev, bem, beh, ben; // Backward Euler array

    // References to whatever nodes are connected
    struct compartment *left;
    struct compartment *right;

    // Extracellular voltage
    float vext;
    // Intracellular voltage
    float vin;
};

struct simEnv
{
    // array comp
    struct compartment *compartments[51];

    // time
    float t;
};

void takeTimeStep(struct simEnv sim, float dt, float injected[], int rowNum, float data[10000][205]);

float derivV(float v, float m, float h, float n, float i, float vinCenter, float vinLeft, float vinRight);
float derivN(float v, float n);
float derivM(float v, float m);
float derivH(float v, float h);

void writeToFile(float data[10000][205], char* name);