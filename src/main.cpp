#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include "Shaders.h"
#include "Model.h"

#define I glm::mat4(1.0)

void funInit();
void funReshape(int w, int h);
void funDisplay();

void drawObject(Model model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPlane(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCone(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelix(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSphere(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawArm(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTop(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawFrustum(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawModel(glm::mat4 P, glm::mat4 V, glm::mat4 M);

// Shaders
   Shaders shaders;

// Modelos
   Model plane; //2*2*2
   Model cone;  //5.37*4.85*5.37
   Model cylinder;  //2*2*2
   Model sphere;    //2*2*2

// Viewport
   int w = 600;
   int h = 600;

int main(int argc, char** argv) {

 // Inicializamos GLUT
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Sesion 5");

 // Inicializamos GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Inicializaciones específicas
    funInit();

 // Configuración CallBacks
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);

 // Bucle principal
    glutMainLoop();

    return 0;
}

void funInit() {

 // Test de profundidad
    glEnable(GL_DEPTH_TEST);
    glPolygonOffset(1.0,1.0);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Modelos
    plane.initModel("resources/models/plane.obj");
    cone.initModel("resources/models/cone.obj");
    cylinder.initModel("resources/models/cylinder.obj");
    sphere.initModel("resources/models/sphere.obj");

}

void funReshape(int wnew, int hnew) {

 // Configuración del Viewport
    glViewport(0, 0, wnew, hnew);

 // Actualización de w y h
    w = wnew;
    h = hnew;

}

void funDisplay() {

 // Borramos el buffer de color
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Indicamos los shaders a utilizar
    shaders.useShaders();

 // Matriz P
    float fovy   = 60.0;
    float nplane = 0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matriz V
    glm::vec3 pos(4.0, 4.0, 4.0);
    glm::vec3 lookat(0.0, 0.0, 0.0);
    glm::vec3 up(0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(pos, lookat, up);

 // Dibujamos la escena
    drawPlane(P,V,I);
    drawModel(P,V,I);

 // Intercambiamos los buffers
    glutSwapBuffers();

}

void drawObject(Model model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    shaders.setMat4("uPVM",P*V*M);

    glEnable(GL_POLYGON_OFFSET_FILL);
        shaders.setVec3("uColor",color);
        model.renderModel(GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    shaders.setVec3("uColor",glm::vec3(0.75*color[0], 0.75*color[1], 0.75*color[2]));
    model.renderModel(GL_LINE);

}

void drawPlane(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    drawObject(plane,glm::vec3(0.5, 0.5, 0.5),P,V,M*S);

}

void drawCone(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -2.8, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.012, 0.068, 0.038));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObject(cone,glm::vec3(1.0, 0.0, 0.0),P,V,M*R90*S*T);

}

void drawFrustum(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 2.1, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.094, 0.027, 0.094));
    drawObject(cone,glm::vec3(0.9, 0.4, 0.0),P,V,M*S*T);

}

void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 1, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.5, 0.05, 0.5));
    drawObject(cylinder,glm::vec3(1.0, 1.0, 0.0),P,V,M*S*T);

}

void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -1.0, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.05, 0.5, 0.05));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObject(cylinder,glm::vec3(0.0, 0.0, 1.0),P,V,M*R90*S*T);

}

void drawSphere(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.3, 0.3, 0.3));
    drawObject(sphere,glm::vec3(0.0, 1.0, 0.0),P,V,M*S);

}

void drawArm(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 R45 = glm::rotate(I, glm::radians(45.0f), glm::vec3(0, 1, 0));
    drawCylinder(P,V,M);
    drawHelix(P,V,M*T*R45);

}

void drawHelix(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
    drawCone(P,V,M);
    drawCone(P,V,M*R90);
    drawCone(P,V,M*R90*R90);
    drawCone(P,V,M*R90*R90*R90);

}


void drawTop(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R72 = glm::rotate(I, glm::radians(72.0f), glm::vec3(0, 1, 0));
    drawSphere(P,V,M);
    drawArm(P,V,M);
    drawArm(P,V,M*R72);
    drawArm(P,V,M*R72*R72);
    drawArm(P,V,M*R72*R72*R72);
    drawArm(P,V,M*R72*R72*R72*R72);

}

void drawModel(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 1.25, 0.0));
    drawBase(P,V,M);
    drawFrustum(P,V,M);
    drawTop(P,V,M*T);

}