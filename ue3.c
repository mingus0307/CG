#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
//#include <math.h>
#include <sys/select.h>

GLuint program;  // Shader-Programmpointer
GLuint vao;      // Vertex Array Object


// Initialisierung von OpenGL und Shadern
void innit(void){
    // Vertex Shader (Berechnet Geometrie und Position)
    const char* vertexText = 
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPosition;\n"  // Position der Vertices
        "layout (location = 1) in vec3 aColor;\n"     // Farbe der Vertices lol
        "layout (location = 2) in float vertValue;\n"
        "out vec3 vertexColor;\n"                     // Gibt die Farbe an den Fragment-Shader weiter
        "out float fragValue;\n"                      // gibt einen float value an den fragment shader weiter 
        "void main() {\n"
        "    vertexColor = aColor;\n"                   // Setzt die Farbe
        "    gl_Position = vec4(aPosition, 0.0f, 1.0f);\n"  // Setzt die Position im Clipspace, wobei x und y aus der postion kommen und z und w ignoriert werden 
        "    fragValue = vertValue;\n"                  // setzt den wert gleich dem vertvalue der an den f shader gehen soll
        "}\n"; 

    // Vertex Shader erstellen
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); 
    glShaderSource(vertexShader, 1, &vertexText, NULL);  // Shader-Quellcode an den Shader übergeben
    glCompileShader(vertexShader);  // Shader kompilieren
    GLint status; 
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);  // Fehlerüberprüfung der Kompilierung
    if (!status) {
        printf("ERROR COMPILING VERTEX SHADER");
        GLchar infoLog[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog); 
        printf("%s", infoLog);  // Gibt den Fehler-Log aus
    }

    // Fragment Shader (Berechnet die Pixel-Farbe)
    const char* fragmentText = 
        "#version 330 core\n"
        "in vec3 vertexColor;\n"  // Eingabe: Farbe aus dem Vertex-Shader
        "in float fragValue;\n"
        "uniform vec3 fragColor1;\n" // erste fragColor
        "uniform vec3 fragColor2;\n" // zweite fragColor
        "in vec2 xPos;\n"     // zeit 
        "void main() {\n"
        "    float wave = sin(time * 2.00);\n"
        //"    vec3 finalColor = mix(fragColor1, fragColor2, step( 0.5f, fragValue));\n" // erstellt einen übergnag in einem begrenzten bereich 
        //"    vec3 finalColor = mix(fragColor1, fragColor2, smoothstep( 0.7f, 0.3f, fragValue));\n" // erstellt einen harten übergang
        //"    vec3 finalColor = mix(fragColor1, fragColor2, fragValue);\n" // erstellt einen mix mit weitem übergang 
        "    vec3 finalColor = mix(fragColor1, fragColor2, (wave +1)/ 2);\n" // erstellt eine sinus kurve 
        "    gl_FragColor = vec4(finalColor, 1.0);\n"  // Setzt die Farbe des Pixels
        "}\n"; 
    // Fragment Shader erstellen
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); 
    glShaderSource(fragmentShader, 1, &fragmentText, NULL);  
    glCompileShader(fragmentShader);  // Shader kompilieren
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);  // Fehlerüberprüfung
    if (!status) {
        printf("ERROR COMPILING FRAGMENT SHADER");
        GLchar infoLog[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog); 
        printf("%s", infoLog);  // Gibt den Fehler-Log aus
    }

    // Shader-Programm erstellen und verbinden
    program = glCreateProgram(); 
    glAttachShader(program, vertexShader);  // Vertex Shader anhängen
    glAttachShader(program, fragmentShader);  // Fragment Shader anhängen
    glLinkProgram(program);  // Verlinkt die Shader zum Programm
    glGetProgramiv(program, GL_LINK_STATUS, &status);  // Fehlerüberprüfung der Verlinkung
    if (!status) {
        printf("ERROR LINKING SHADER");
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog); 
        printf("%s", infoLog);  // Gibt den Fehler-Log aus
    }

    // Shader-Programm validieren
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (!status) {
        printf("ERROR VALIDATING SHADER");
        GLchar infoLog[1024];
        glGetProgramInfoLog(program, 1024, NULL, infoLog); 
        printf("%s", infoLog);  // Gibt den Fehler-Log aus
    }
    // Dreieecke bauen 
    GLfloat triangles[] = 
    {
        //X         Y        R          G       B           vertValue
        -0.9f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,  
        -0.9f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 
        0.9f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
        // first (pls work)
        -0.9f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.9f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.9f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f
    };
    // Vertex Buffer Object (VBO) erstellen
    GLuint triangleVertexBufferObject; 
    glGenBuffers(1, &triangleVertexBufferObject);  // Puffer generieren
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferObject);  // Puffer binden
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangles), triangles, GL_STATIC_DRAW);  // Daten in den Puffer laden
    glBindBuffer(GL_ARRAY_BUFFER, 0);  // Puffer entbinden

    // Vertex Array Object (VAO) erstellen
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);  // VAO binden
    glBindBuffer(GL_ARRAY_BUFFER, triangleVertexBufferObject);  // VBO binden

   // Definieren der Attribut-Positionen im Shader
   glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);  // Position
   glEnableVertexAttribArray(0);  // Attribut aktivieren

   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));  // Farbe
   glEnableVertexAttribArray(1);  // Attribut aktivierenttrib

   glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat))); // fragValue
   glEnableVertexAttribArray(2); // aktivieren des Attributes 

   glBindBuffer(GL_ARRAY_BUFFER, 0);  // Puffer entbinden
   glBindVertexArray(0);  // VAO entbinden

   // Hintergrundfarbe setzen und Viewport festlegen
   glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
   glViewport(0, 0, 800, 600);
 
}

void draw(){
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(program); 

    GLint loc1 = glGetUniformLocation(program, "fragColor1");
    GLint loc2 = glGetUniformLocation(program, "fragColor2"); 
    glUniform3f(loc1, 0.0f, 1.0f, 0.0f);
    glUniform3f(loc2, 1.0f, 0.0f, 0.0f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6); 
}

// Fenstergröße anpassen
void framebuffer_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0 , width, height);  // Setze den Viewport basierend auf der Fenstergröße
}

// Hauptfunktion
int main(void){
    glfwInit();  // GLFW initialisieren
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Fenster erstellen
    GLFWwindow *window = glfwCreateWindow(800, 600, "Computergrafik 1", NULL, NULL);
    if(!window){
        printf("window cant be created");
        glfwTerminate(); 
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // Callback für Fenstergrößenänderung
    glfwMakeContextCurrent(window);  // Setze den Kontext des Fensters
    glewInit();  // GLEW initialisieren

    innit();  // Initialisiere alles (Shader, Buffers, etc.)

    // Rendering-Schleife
    while(!glfwWindowShouldClose(window)){
        draw();  // Zeichne die Szene

        glfwSwapBuffers(window);  // Tausche die Puffer (Front-Buffer und Back-Buffer)
        glfwPollEvents();  // Bearbeite Eingaben und Events
    }

    glfwTerminate();  // GLFW beenden
    return 0; 
}