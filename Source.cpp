#include <iostream>
#include <chrono>
#include <ctime>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VBO.h"
#include "VAO.h"

#include "pluck.h"
#include "irrKlang.h"
//#include "SFML/Audio.hpp"

using namespace std; 
using namespace irrklang;

#define ELEMENTS_AMOUNT 30
#define STRING_LENGTH 0.63
#define STRING_STEP STRING_LENGTH/ELEMENTS_AMOUNT
#define STRING_START_X -0.8
#define STRING_START_Y 0.0
#define ELEMENT_MASS 0.9
#define ELASTIC_MODULUS 0.3e6
#define PULL_COEFFICIENT 0.08
#define TIME_MODIFIER 1.

ISoundEngine* SoundEngine = createIrrKlangDevice();

GLfloat frets[3 * 7 * 2]{
        STRING_START_X + 0.1, STRING_START_Y + 0.1, 0,
        STRING_START_X + 0.1, STRING_START_Y - 0.1, 0,
        STRING_START_X + 0.2, STRING_START_Y + 0.1, 0,
        STRING_START_X + 0.2, STRING_START_Y - 0.1, 0,
        STRING_START_X + 0.3, STRING_START_Y + 0.1, 0,
        STRING_START_X + 0.3, STRING_START_Y - 0.1, 0,
        STRING_START_X + 0.4, STRING_START_Y + 0.1, 0,
        STRING_START_X + 0.4, STRING_START_Y - 0.1, 0,
        STRING_START_X + 0.5, STRING_START_Y + 0.1, 0,
        STRING_START_X + 0.5, STRING_START_Y - 0.1, 0,
        STRING_START_X, STRING_START_Y + 0.1, 0,
        STRING_START_X, STRING_START_Y - 0.1, 0,
        STRING_START_X - 5e-3, STRING_START_Y + 0.1, 0,
        STRING_START_X - 5e-3, STRING_START_Y - 0.1, 0,
};

int windowsize[2]{ 800, 800 };

double pluck_distribution(double x, double fretted_node) {
    if (x >= fretted_node) {
        return PULL_COEFFICIENT*STRING_LENGTH - PULL_COEFFICIENT/(STRING_LENGTH-fretted_node)*abs(x-(STRING_LENGTH+fretted_node)/2);
    }
    else {
        return 0;
    };
};

double starting_distribution(double x) {
    return 0;
};

double fretted_node{ 0. };
bool crutch{ 0 };

void fret_pluck(double x, double y) {
    if (abs(windowsize[1] / 2 - y) <= 0.1 * windowsize[1] / 2) {
        if (x < (1 + STRING_START_X) * windowsize[0] / 2) {
            cout << "open string\n";
            fretted_node = 0;
            crutch = 1;
            SoundEngine->stopAllSounds();
            SoundEngine->play2D("audio/2-0fin.mp3", false);
        }
        if ((x >= (1 + STRING_START_X) * windowsize[0] / 2) && (x < (1 + frets[0]) * windowsize[0] / 2)) {
            cout << "fret 1\n";
            fretted_node = (frets[0]-STRING_START_X)*0.63/1.6;
            crutch = 1;
            SoundEngine->stopAllSounds();
            SoundEngine->play2D("audio/2-1fin.mp3", false);
        }
        else if ((x >= (1 + frets[0]) * windowsize[0] / 2) && (x < (1 + frets[6])* windowsize[0] / 2)) {
            cout << "fret 2\n";
            fretted_node = (frets[6] - STRING_START_X) * STRING_LENGTH / (2 * abs(STRING_START_X));
            crutch = 1;
            SoundEngine->stopAllSounds();
            SoundEngine->play2D("audio/2-2fin.mp3", false);
        }
        else if ((x >= (1 + frets[6]) * windowsize[0] / 2) && (x < (1 + frets[12]) * windowsize[0] / 2)) {
            cout << "fret 3\n";
            fretted_node = (frets[12] - STRING_START_X) * STRING_LENGTH / (2 * abs(STRING_START_X));
            crutch = 1;
            SoundEngine->stopAllSounds();
            SoundEngine->play2D("audio/2-3fin.mp3", false);
        }
        else if ((x >= (1 + frets[12]) * windowsize[0] / 2) && (x < (1 + frets[18]) * windowsize[0] / 2)) {
            cout << "fret 4\n";
            fretted_node = (frets[18] - STRING_START_X) * STRING_LENGTH / (2 * abs(STRING_START_X));
            crutch = 1;
            SoundEngine->stopAllSounds();
            SoundEngine->play2D("audio/2-4fin.mp3", false);
        }
        else if ((x >= (1 + frets[18]) * windowsize[0] / 2) && (x < (1 + frets[24]) * windowsize[0] / 2)) {
            cout << "fret 5\n";
            fretted_node = (frets[24] - STRING_START_X) * STRING_LENGTH / (2 * abs(STRING_START_X));
            crutch = 1;
            SoundEngine->stopAllSounds();
            SoundEngine->play2D("audio/2-5fin.mp3", false);
        }
        else if (x >= (1 + frets[24]) * windowsize[0] / 2) {
            cout << "mute\n";
            fretted_node = STRING_LENGTH;
            crutch = 1;
            SoundEngine->stopAllSounds();
            SoundEngine->play2D("audio/mute.mp3", false);
        }
        cout << "fretted: " << fretted_node << endl;
    }
};

void fret_coords(float* vertices, float* speeds, double fretted_node) {
    for (int i = 0; i < 3 * (ELEMENTS_AMOUNT + 1) - 1; i += 3) {
        speeds[i] = 0;
    };
    for (int i = 0; i < 3 * (ELEMENTS_AMOUNT + 1) - 1; i += 3) {
        vertices[i + 1] = pluck_distribution(vertices[i], fretted_node);
    };
};

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    if ((button == GLFW_MOUSE_BUTTON_LEFT)&&(action == GLFW_PRESS)) {
        fret_pluck(xpos, ypos);
    };
    if ((button == GLFW_MOUSE_BUTTON_RIGHT) && (action == GLFW_PRESS)) {
        cout << (xpos-80)*0.63/640 << " " << ypos << endl;
    };
    /*
    cout << xpos << " " << ypos << endl;
    cout << xpos << " " << (1 + frets[0]) * windowsize[0] / 2 << endl;
    cout << endl;
    */
};

int main(void) {
    /*
    {
        int x;
        cout << "String number 1 to 6: ";
        cin >> x;
    };
    */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /*
    GLfloat vertices[]{
        -0.9f, 0.9f, 0.0f,
        -0.8f, 0.8f, 0.0f,
        -0.8f, 0.9f, 0.0f,
        -0.7f, 0.8f, 0.0f,
    };
    */

    GLfloat vertices[3 * (ELEMENTS_AMOUNT + 1)];
    GLfloat speeds[3 * (ELEMENTS_AMOUNT + 1)];
    float calc_vertices[3 * (ELEMENTS_AMOUNT + 1)];
    float calc_speeds[3 * (ELEMENTS_AMOUNT + 1)];

    double stretch = 1.6 / STRING_LENGTH;
    //cout << stretch;
    for (int i = 0; i < 3 * (ELEMENTS_AMOUNT + 1) - 1; i += 3) {
        calc_vertices[i] = STRING_STEP * int(i / 3);
        vertices[i] = STRING_START_X + STRING_STEP * int(i / 3) * stretch;
        //vertices[i + 1] = startingDistribution(STRING_STEP * int(i / 3)) + STRING_START_Y;
        vertices[i + 1] = STRING_START_Y + starting_distribution(vertices[i]);
        calc_vertices[i + 1] = starting_distribution(calc_vertices[i]);
        //cout << i / 3 << " | " << vertices[i] << endl;
        vertices[i + 2] = 0;
        calc_vertices[i + 2] = 0;
        speeds[i] = 0;
        speeds[i+1] = 0;
        speeds[i+2] = 0;
        calc_speeds[i] = 0;
        calc_speeds[i + 1] = 0;
        calc_speeds[i + 2] = 0;
    };
    
    
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    //window = glfwCreateWindow(1920, 1080, "sup", glfwGetPrimaryMonitor(), NULL);
    window = glfwCreateWindow(windowsize[0], windowsize[1], "sup", NULL, NULL);

    

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    

    glfwMakeContextCurrent(window);
    gladLoadGL();
    //glViewport(0, 0, 1920, 1920);
    glViewport(0, 0, windowsize[0], windowsize[1]);

    

    //Shader shaderProgram("default.vert", "default.frag");

    cout << "1\n";

    VAO VAO1;
    VAO1.Bind();
    VBO VBO1(vertices, sizeof(vertices));
    VAO1.LinkVBO(VBO1, 0);
    VAO1.Unbind();
    VBO1.Unbind();

    

    VAO VAO2;
    VAO2.Bind();
    VBO VBO2(frets, sizeof(frets));
    VAO2.LinkVBO(VBO2, 0);
    VAO2.Unbind();
    VBO2.Unbind();

    glClearColor(0.17f, 0.13f, 0.17f, 1.0f);
    //shaderProgram.shaderActivate();

    chrono::steady_clock::time_point start = chrono::steady_clock::now();
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    chrono::duration<double> TIME_PASSSED{ end - start };

    double DIAG_CURRENT_TIME{};

    glLineWidth(1.5);
    glPointSize(5);

    

    while (!glfwWindowShouldClose(window))
    {
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        if (crutch) {
            fret_coords(calc_vertices, calc_speeds, fretted_node);
            calc_vertices[1] = 0;
            calc_vertices[3 * (ELEMENTS_AMOUNT + 1)-2] = 0;
            calc_speeds[1] = 0;
            calc_speeds[(ELEMENTS_AMOUNT - 1) * 3 + 1] = 0;
            crutch = 0;
        };

        VAO1.Unbind();
        glClear(GL_COLOR_BUFFER_BIT);
        VAO2.Bind();
        glDrawArrays(GL_LINES, 0, 8*3);
        VAO2.Unbind();
        VAO1.Bind();
        glDrawArrays(GL_LINE_STRIP, 0, (ELEMENTS_AMOUNT+1) );
        glDrawArrays(GL_POINTS, 0, (ELEMENTS_AMOUNT + 1));

        VBO1.Delete();

        end = chrono::steady_clock::now();
        chrono::duration<double> TIME_PASSED = (end - start);
        DIAG_CURRENT_TIME += TIME_PASSED.count();
        updateCoordsChain(calc_vertices, calc_speeds, 3 * (ELEMENTS_AMOUNT + 1), STRING_STEP/7, ELASTIC_MODULUS, ELEMENT_MASS, TIME_PASSED.count()*TIME_MODIFIER, fretted_node);
        for (int i = 0; i < 3 * (ELEMENTS_AMOUNT + 1) - 1; i += 3) {
            vertices[i] = STRING_START_X + stretch * calc_vertices[i];
            vertices[i + 1] = STRING_START_Y + stretch * calc_vertices[i+1];
        };
        //cout << DIAG_CURRENT_TIME * TIME_MODIFIER << " " << TIME_PASSED.count() << "\n";
        

        VBO VBO1(vertices, sizeof(vertices));
        VAO1.LinkVBO(VBO1, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        start = chrono::steady_clock::now();
    }

    VAO1.Delete();
    VBO1.Delete();
    //shaderProgram.shaderDelete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}