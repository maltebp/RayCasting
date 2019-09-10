#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;
 

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






int main(void)
{
    cout<<"Starting..."<<endl<<endl;


    string vsSource, fsSource;
    getShaderSource("res/fragmentshader.shader", fsSource);
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


    // float positions[] = {
    //     0.5f, -0.5f,

    //     0, -0.5f,

    //     0.5f, 0.5f
    // };

    float positions[] = {
        0.5f, -0.5f,
        1.0, 0., 0., 1.,

        0, -0.5f,
        1.0, 0., 0., 1.,

        0.5f, 0.5f,
        1.0, 0., 0., 1.
    };



    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 3*6 * sizeof(float), positions, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*) (2*sizeof(float))); //(sizeof(float)*2) );
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    unsigned int shader = createShader(vsSource, fsSource);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

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