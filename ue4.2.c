//#include <math.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLuint program;
GLuint vao; 
GLuint ebo; 
const int segments = 100; 

void innit(void){
    //copile vertex shader -> geometrie berechnung  
    const char* vertexText = 
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPosition;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "out vec3 vertexColor;\n"
        "void main() {;\n"
        " vertexColor = aColor;\n"
        " gl_Position = vec4(aPosition, 0.0, 1.0);\n"
        "}\n"; 

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); 
    glShaderSource(vertexShader, 1, &vertexText, NULL); 
    glCompileShader(vertexShader);
    GLint status; 
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status); 

    if(!status){
        printf("ERROR COMPILING VERTEX SHADER");
        GLchar infoLog[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog); 
        printf("%s", infoLog);  
         
    }

    //compile fragment shader    
    const char* fragmentText = 
        "#version 330 core\n"
        "in vec3 vertexColor;\n"
        "void main() {;\n"
        " gl_FragColor = vec4(vertexColor, 1.0);\n"
        "}\n"; 

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(fragmentShader, 1, &fragmentText, NULL); 
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status); 

    if(!status){
        printf("ERROR COMPILING FRAGMENT SHADER");
        GLchar infoLog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog); 
        printf("%s", infoLog);  
         
    }
    
    //create and link shader program
    program = glCreateProgram(); 
    glAttachShader(program, vertexShader); 
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if(!status){
        printf("ERROR COMPILING LINKING SHADER");
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog); 
        printf("%s", infoLog);  
         
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);

    if(!status){
        printf("ERROR COMPILING VALIDATION SHADER");
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog); 
        printf("%s", infoLog);  
         
    }
   
   
    // create triangle buffer
    GLfloat triangleVertices[] = 
    {   // X         Y
        // linker Balken
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 
        -0.3f, 0.5f, 1.0f, 0.0f, 0.0f,
        -0.3f, -0.5f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        // querbalken 
        -0.3f, 0.1f, 1.0f, 0.0f, 0.0f,
        0.3f, 0.1f, 1.0f, 0.0f, 0.0f,
        0.3f, -0.1f, 1.0f, 0.0f, 0.0f,
        -0.3f, -0.1f,1.0f, 0.0f, 0.0f,
        // rechter Balken
        0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.3f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.3f, -0.5f,1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,1.0f, 0.0f, 0.0f,

        // unterer Balken 
        -0.5f, -0.7, 1.0f, 0.0f, 0.0f,
        0.5f, -0.7, 1.0f, 0.0f, 0.0f,
        0.5f, -0.8, 1.0f, 0.0f, 0.0f,
        -0.5, -0.8f, 1.0f, 0.0f, 0.0f
    }; 
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(9999); 

    GLuint indexes[] = 
    { 0, 1, 3, 2, 9999, 
      4, 5, 6, 7, 9999, 
      8, 9, 11, 10, 9999, 
      12, 13, 15, 14
    };


    // create, bind and upload VBO 
    GLuint triangleVertexBufferObject; 

    glGenBuffers(1, &triangleVertexBufferObject); 
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferObject); 
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 


    
    //create vertrex array object 
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferObject);
    glVertexAttribPointer(
            0, 
            2, 
            GL_FLOAT, 
            GL_FALSE, 
            5 * sizeof(GLfloat), 
            0
            );
      

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(
            1, 
            3, 
            GL_FLOAT, 
            GL_FALSE, 
            5 * sizeof(GLfloat), 
            (GLvoid*)(2 * sizeof(GLfloat))
        
        );
      

    glEnableVertexAttribArray(1);  
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // create Element Object Buffer -> ebo 

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
    glBindVertexArray(0);
     

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glViewport(0, 0, 800, 600);
}

void draw(void){
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program);
    
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_TRIANGLE_STRIP, 20, GL_UNSIGNED_INT, 0);  
     

}

void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0 , width, height); 
}

int main(void){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    GLFWwindow *window = glfwCreateWindow(800, 600, "Computergrafik 1", NULL, NULL);

    if(!window){
        printf("window cant be creatted"); 
        glfwTerminate(); 
        return -1; 
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
    
    glfwMakeContextCurrent(window);
    glewInit(); 

    innit(); 
    
    while(!glfwWindowShouldClose(window)){
        draw(); 

        glfwSwapBuffers(window); 
        glfwPollEvents(); 
    }


    glfwTerminate();

    return 0; 

}