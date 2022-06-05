#pragma once

#include <glad/glad.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

vector<GLfloat> generateStartingVertices(double, int, int);

void updateCoordsChain(float* vertices, float* speeds, int size, GLfloat BASE_DISTANCE, GLfloat ELASTIC_MODULUS, GLfloat ELEMENT_MASS, GLfloat TIME_PASSSED, double fretted_node);

void fret_pluck(double x, double y);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

void fret_coords(float* vertices, float* speeds);

//double startingDistribution(double x);