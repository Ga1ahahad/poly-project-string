#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

void updateCoordsChain(float* vertices, float* speeds, int size, float BASE_DISTANCE, float ELASTIC_MODULUS, float ELEMENT_MASS, float TIME_PASSSED, double fretted_node) {
    for (int i = 4; i<size - 2; i += 3) {
        if (vertices[i - 1] <= fretted_node) {
            vertices[i] = 0;
            speeds[i] = 0;
        };
        float G_1 = pow((pow((vertices[i] - vertices[i - 3]), 2) + pow(BASE_DISTANCE + vertices[i - 1] - vertices[i - 4], 2)), 0.5); //G_(n-1)
        float G_2 = pow((pow((vertices[i + 3] - vertices[i]), 2) + pow(BASE_DISTANCE + vertices[i + 2] - vertices[i - 4], 2)), 0.5); //G_(n+1)
        float sin_1 = (vertices[i + 3] - vertices[i]) / G_2;
        float sin_2 = (vertices[i] - vertices[i - 3]) / G_1;
        float cos_1 = pow(1 - sin_1 * sin_1, 0.5);
        float cos_2 = pow(1 - sin_2 * sin_2, 0.5);
        //speeds[i - 1] += (ELASTIC_MODULUS / ELEMENT_MASS * (vertices[i + 2] - 2 * vertices[i - 1] + vertices[i - 4]) + ELASTIC_MODULUS * BASE_DISTANCE / ELEMENT_MASS * (cos_2 - cos_1)) * TIME_PASSSED;
        speeds[i] += (ELASTIC_MODULUS / ELEMENT_MASS * (vertices[i + 3] - 2 * vertices[i] + vertices[i - 3]) + ELASTIC_MODULUS * BASE_DISTANCE / ELEMENT_MASS * (sin_2 - sin_1)) * TIME_PASSSED;
    };
    //speeds[4] *= 0.5;
    //speeds[size - 2] *= 0.5;
    for (int i = 4; i< size-4; i += 3) {
        if (vertices[i - 1] <= fretted_node) {
            //cout << vertices[i - 1] << " " << fretted_node << endl;
            vertices[i] = 0;
            speeds[i] = 0;
        };
        //vertices[i - 1] += speeds[i - 1] * TIME_PASSSED;
        vertices[i] += speeds[i] * TIME_PASSSED * (1-0.01*(1 - fretted_node));
        //cout << "vertex " << (i - 1) / 3 << " modified to " << vertices[i] << endl;
    };
    //vertices[0] = 0;
    //vertices[1] = 0;
    //vertices[size-3] = 0;
    //vertices[size-2] = 0;
};

vector<float> generateStartingVertices(double STRING_LENGTH, int ELEMENTS_AMOUNT, int STRING_POSITION) {
    std::vector<float> vertices;
    double step{ STRING_LENGTH / ELEMENTS_AMOUNT };
    for (int i = 0; i < ELEMENTS_AMOUNT+1; i++) {
        vertices.push_back(step * i);
        vertices.push_back(0.);
        vertices.push_back(0.);
    };
    return vertices;
};