#include <iostream>
#include <ogl/glew.h>
#include <ogl/freeglut.h>

#include "Shaders.h"
#include "Model.h"

#define I glm::mat4(1.0)

void funInit();
void funReshape(int w, int h);
void funDisplay();
void helixAutoRotation(int value);
void cameraZoom(int key, int status, int x, int y);
void cameraMovement(int x, int y);
void moveModel(unsigned char key, int x, int y);
void traslation(int key, int x, int y);

void drawObject(Model model, glm::vec3 color, glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawPlane(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCone(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawHelix(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawLittleCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawSphere(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawArticulation(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawArm(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawTop(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBody(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawModel(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawBodyAndTop(glm::mat4 P, glm::mat4 V, glm::mat4 M);
void drawExtreme(glm::mat4 P, glm::mat4 V, glm::mat4 M);


 // Shaders variable
    Shaders shaders;

 // Model variables
    Model plane;     // 2*2*2
    Model cone;      // 5.37*4.85*5.37
    Model cylinder;  // 2*2*2
    Model sphere;    // 2*2*2

 // Viewport variables
    int w = 600;
    int h = 600;

 // Animation variables
    GLint speed = 20;  // 20 ms
    float rotY = 0.0;
    float zoom = 60.0;
    float alphaX = 0.0;
    float alphaY = 0.0;
    float rotY_b_t = 0.0;
    float rotArms = 0.0;
    float movTop = 1.3;
    float movX= 0.0;
    float movZ= 0.0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char** argv) {

 // GLUT init
    glutInit(&argc, argv);
    glutInitContextVersion(3,3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(w,h);
    glutInitWindowPosition(50,50);
    glutCreateWindow("Practica 2");

 // GLEW init
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if(GLEW_OK != err) {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        return 0;
    }
    std::cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;
    const GLubyte *oglVersion = glGetString(GL_VERSION);
    std::cout <<"This system supports OpenGL Version: " << oglVersion << std::endl;

 // Specific initializations
    funInit();

 // CallBacks configuration
    glutReshapeFunc(funReshape);
    glutDisplayFunc(funDisplay);
    glutTimerFunc(speed, helixAutoRotation, 0);
    glutMouseFunc(cameraZoom);
    glutMotionFunc(cameraMovement);
    glutKeyboardFunc(moveModel);
    glutSpecialFunc(traslation);

 // Main loop
    glutMainLoop();

    return 0;
}

void funInit() {

 // Depth test
    glEnable(GL_DEPTH_TEST);
    glPolygonOffset(1.0,1.0);

 // Shaders
    shaders.initShaders("resources/shaders/vshader.glsl","resources/shaders/fshader.glsl");

 // Models
    plane.initModel("resources/models/plane.obj");
    cone.initModel("resources/models/cone.obj");
    cylinder.initModel("resources/models/cylinder.obj");
    sphere.initModel("resources/models/sphere.obj");

}

void funReshape(int wnew, int hnew) {

 // Viewport configuration
    glViewport(0, 0, wnew, hnew);

 // w & h update
    w = wnew;
    h = hnew;

}

void funDisplay() {

 // Erase the color buffer
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

 // Declare the shaders
    shaders.useShaders();

// ----------------------------------------------      MATRIX     ----------------------------------------------

 // Matrix P
    float fovy   = zoom;
    float nplane = 0.1;
    float fplane = 25.0;
    float aspect = (float)w/(float)h;
    glm::mat4 P = glm::perspective(glm::radians(fovy), aspect, nplane, fplane);

 // Matrix V
    float x = 5.0f*glm::cos(glm::radians(alphaY))*glm::sin(glm::radians(alphaX));
    float y = 5.0f*glm::sin(glm::radians(alphaY));
    float z = 5.0f*glm::cos(glm::radians(alphaY))*glm::cos(glm::radians(alphaX));
    glm::vec3 pos(x,y,z);
    glm::vec3 lookat(0.0, 0.0, 0.0);
    glm::vec3 up(0.0, 1.0,  0.0);
    glm::mat4 V = glm::lookAt(pos, lookat, up);

 // Matrix M1
    glm::mat4 T1 = glm::translate(I, glm::vec3(2.0, 0.0, 2.0));
    glm::mat4 R18y_1 = glm::rotate(I, glm::radians(18.0f), glm::vec3(0, 1, 0));
    glm::mat4 M1 = T1*R18y_1;

 // Matrix M2
    glm::mat4 T2 = glm::translate(I, glm::vec3(1.0, 0.0, 1.0));
    glm::mat4 R45xz_2 = glm::rotate(I, glm::radians(45.0f), glm::vec3(-1, 0, 1));
    glm::mat4 R45y_2 = glm::rotate(I, glm::radians(45.0f), glm::vec3(0, 1, 0));
    glm::mat4 M2 = T2*R45xz_2*R45y_2;

// ------------------------------------------------------------------------------------------------------------------ \\

 // Draw the scene
    drawPlane(P,V,I);
    // drawModel(P,V,I*M1);  // a
    // drawModel(P,V,I*M2);  // b

    glm::mat4 T = glm::translate(I, glm::vec3(movX, 0.0, movZ));
    drawModel(P,V,I*T);
    // drawArm(P,V,I);

 // Swap buffers
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

// ----------------------------------------------      Drawing Models Functions     ----------------------------------------------

void drawPlane(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(2.0, 1.0, 2.0));
    drawObject(plane,glm::vec3(0.5, 0.5, 0.5),P,V,M*S);

}

void drawBase(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 1, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.6, 0.05, 0.6));
    drawObject(cylinder,glm::vec3(1.0, 1.0, 0.0),P,V,M*S*T);

}

void drawBody(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, 2.1, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.094, 0.268, 0.094));
    drawObject(cone,glm::vec3(0.9, 0.4, 0.0),P,V,M*S*T);

}

void drawCone(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -2.8, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.012, 0.062, 0.038));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObject(cone,glm::vec3(1.0, 0.0, 0.0),P,V,M*R90*S*T);

}

void drawCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -1.0, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.05, 0.5, 0.05));
    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 0, 1));
    drawObject(cylinder,glm::vec3(0.0, 0.0, 1.0),P,V,M*R90*S*T);

}

void drawLittleCylinder(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, -0.15, 0.0));
    glm::mat4 S = glm::scale(I, glm::vec3(0.025, 0.15, 0.025));
    drawObject(cylinder,glm::vec3(0.0, 1.0, 1.0),P,V,M*T*S);

}

void drawSphere(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.3, 0.3, 0.3));
    drawObject(sphere,glm::vec3(0.0, 1.0, 0.0),P,V,M*S);

}


void drawHelix(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R90 = glm::rotate(I, glm::radians(90.0f), glm::vec3(0, 1, 0));
    glm::mat4 R_auto = glm::rotate(I, glm::radians(rotY), glm::vec3(0, 1, 0));
    drawCone(P,V,M*R_auto);
    drawCone(P,V,M*R_auto*R90);
    drawCone(P,V,M*R_auto*R90*R90);
    drawCone(P,V,M*R_auto*R90*R90*R90);

}

void drawArticulation(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 S = glm::scale(I, glm::vec3(0.075, 0.075, 0.075));
    drawObject(sphere,glm::vec3(1.0, 0.0, 1.0),P,V,M*S);

}

void drawExtreme(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 Ty_helix = glm::translate(I, glm::vec3(0.0, -0.3, 0.0));
    glm::mat4 R45 = glm::rotate(I, glm::radians(45.0f), glm::vec3(0, 1, 0));
    drawLittleCylinder(P,V,M);
    drawHelix(P,V,M*Ty_helix*R45);

}

void drawArm(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(1.0, 0.0, 0.0));
    glm::mat4 R_antiR = glm::rotate(I, glm::radians(rotArms), glm::vec3(0, 0, 1));
    drawCylinder(P,V,M);
    drawArticulation(P,V,M*T);
    drawExtreme(P,V,M*T*R_antiR);

}

void drawTop(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R72 = glm::rotate(I, glm::radians(72.0f), glm::vec3(0, 1, 0));
    glm::mat4 R = glm::rotate(I, glm::radians(-rotArms), glm::vec3(0, 0, 1));
    drawSphere(P,V,M);
    drawArm(P,V,M*R);
    drawArm(P,V,M*R72*R);
    drawArm(P,V,M*R72*R72*R);
    drawArm(P,V,M*R72*R72*R72*R);
    drawArm(P,V,M*R72*R72*R72*R72*R);

}

void drawBodyAndTop(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 T = glm::translate(I, glm::vec3(0.0, movTop, 0.0));
    drawBody(P,V,M);
    drawTop(P,V,M*T);

}

void drawModel(glm::mat4 P, glm::mat4 V, glm::mat4 M) {

    glm::mat4 R = glm::rotate(I, glm::radians(rotY_b_t), glm::vec3(0, 1, 0));
    drawBase(P,V,M);
    drawBodyAndTop(P,V,M*R);

}

// ----------------------------------------------      Animation Functions     ----------------------------------------------

void helixAutoRotation(int value) {

    rotY -= 2.5;
    glutPostRedisplay();
    glutTimerFunc(speed, helixAutoRotation ,0);

}

void cameraZoom(int key, int status, int x, int y) {

    switch (key) {
        case 4:
            if (zoom < 60) zoom += 1;
            break;
        case 3:
            if (zoom > 10) zoom -= 1;
            break;
        default:
            break;
    }
    glutPostRedisplay();

}

void cameraMovement(int x, int y) {

    int ax = x - w / 2;
    int ay = y - h / 2;

    int px2degX = ax * 180 / w;
    int px2degY = ay * 90 / (h/2);

    if(px2degX < 180 && px2degX > -180) alphaX = px2degX;
    if(px2degY < 90 && px2degY > -90) alphaY = -px2degY;
    glutPostRedisplay();

}

void moveModel(unsigned char key, int x, int y) {

    switch (key) {
        case 'r':
            rotY_b_t += 5;
            break;
        case 'R':
            rotY_b_t -= 5;
            break;
        case 'a':
            if (rotArms < 45 && rotArms >= 0) rotArms += 3;
            break;
        case 'A':
            if (rotArms <= 45 && rotArms > 0) rotArms -= 3;
            break;
        case 'y':
            if (movTop > 0.35) movTop -= 0.01;
            break;
        case 'Y':
            if (movTop < 1.3) movTop += 0.01;
            break;
        default:
            break;
    }
    glutPostRedisplay();

}

void traslation(int key, int x, int y){

    switch (key) {
        case GLUT_KEY_LEFT:  if(movX > -1.4) movX -= 0.1;
            break;
        case GLUT_KEY_RIGHT: if(movX < 1.4) movX += 0.1;
            break;
        case GLUT_KEY_UP: if(movZ > -1.4) movZ -= 0.1;
            break;
        case GLUT_KEY_DOWN: if(movZ < 1.4) movZ += 0.1;
            break;
        default:
            break;
    }
    glutPostRedisplay();

}
