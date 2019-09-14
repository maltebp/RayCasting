#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "fpscounter.hpp"

using namespace std;

double mouseX=0;
double mouseY=0;

static unsigned int compileShader(unsigned int type, const string &source){
    unsigned int id = glCreateShader(type);
    const char* cSource = source.c_str();
    glShaderSource(id, 1, &cSource, nullptr ); 
    glCompileShader(id);
    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if( status == GL_TRUE ){
        cout<<"Shader compile successful!"<<endl;
    }else{
        int logLength = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        char* message = (char*) malloc(sizeof(char)*logLength);
        glGetShaderInfoLog(id, logLength, NULL, message); 

        cout<<"Compile error with "<<((type == GL_VERTEX_SHADER) ? "vertex" : "fragment") << " shader:"<<endl;
        cout<<message<<endl;
        free(message);
        return 0;
    } 
    return id;
}


static int createShader( const string &vsSource, const string &fsSource ){
    unsigned int program = glCreateProgram();
    
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    glAttachShader(program, vertexShader);
    
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource );
    glAttachShader(program, fragmentShader);

    if(fragmentShader == 0 || vertexShader == 0){
        glDeleteProgram(program);
    }else{
        glLinkProgram(program);
        glValidateProgram(program);
        return program;
    }
    return 0;    
}  


void getShaderSource( string filename, string &source ){
    ifstream inFile(filename);
    string input(
        (istreambuf_iterator<char>(inFile)),
        istreambuf_iterator<char>()    
    );
    source += input;
}


void glfwErrorCallback(int errCode, const char* errStr){
    cout<<"GLFW Error: "<<"("<<errCode<<") "<<errStr<<endl;
}



static void cursorPosCallback(GLFWwindow *window, double x, double y){
    mouseX = x;
    mouseY = y;
}


int main(void)
{
    cout<<"Starting..."<<endl<<endl;


    string vsSource, fsSource;
    getShaderSource("res/circle_fragShader.glsl", fsSource);
    getShaderSource("res/vertexshader.shader", vsSource);

    cout<<"Vertex shader: "<<vsSource<<endl;
    cout<<"Fragment shader: "<<fsSource<<endl;

    /* Initialize the library */
    if (!glfwInit()){
        cout<<"GLFW init  error"<<endl;
        return -1; 
    }

    GLFWwindow* window;

    glfwSetErrorCallback(glfwErrorCallback);

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_SAMPLES, 16);
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        cout<<"Window creation error"<<endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    if(glewInit() != GLEW_OK){
        std::cout<<"Glew not ok"<<std::endl;
    }

    glfwSetCursorPosCallback(window, cursorPosCallback);


    // float positions[] = {
    //     0.5f, -0.5f,

    //     0, -0.5f,

    //     0.5f, 0.5f
    // };

    unsigned int shader = createShader(vsSource, fsSource);
    glUseProgram(shader);


    //glEnable(GL_POLYGON_SMOOTH);
    //glEnable(GL_BLEND);
    
    //glDisable(GL_DEPTH_TEST);
    //glEnable(GL_DEPTH_TEST);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    FPSCounter fpsCounter;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    { 
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(10);

        glEnable(GL_MULTISAMPLE);

        float centerX = mouseX;
        float centerY = mouseY;
        float radius = 200;

        float positions[] = {
            centerX-radius, centerY-radius,
            1.0, 0., 0., 1.,

            centerX+radius, centerY-radius,
            1.0, 0., 0., 1.,

            centerX+radius, centerY+radius,
            1.0, 0., 0., 1.,

            centerX-radius, centerY+radius,
            1.0, 0., 0., 1.,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };


        unsigned int buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, 4*6* sizeof(float), positions, GL_DYNAMIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) 0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) (2*sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) (0*sizeof(float))); //(sizeof(float)*2) );
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        

        unsigned ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);
        

        unsigned int loc = glGetUniformLocation(shader, "center");
        if( loc != -1){
            glUniform2f(loc, centerX, centerY);
        }else{
            cout<<"Couldn't find uniform 'center' variable!"<<endl;
        }
        

        loc = glGetUniformLocation(shader, "radius");
        if( loc != -1){
            glUniform1f(loc, radius);
        }else{
            cout<<"Couldn't find uniform 'radius' variable!"<<endl;
        }

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        //glDrawElements(GL_TRIANGLES, 0, 3);

        fpsCounter.tick();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    cout<<"Exitted loop"<<endl;

    glfwTerminate();

    cout<<endl<<"Finished"<<endl;
    return 0;
}