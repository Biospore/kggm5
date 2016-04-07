#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <vector>

#include <iostream>


using namespace std;

const int WIDTH = 1000, HEIGHT = 1000;
const char* vertexfp = "/home/biospore/Documents/kggm/go/vertex.glsl";
const char* fragmentfp = "/home/biospore/Documents/kggm/go/fragment.glsl";

const char* texvertexfp = "/home/biospore/Documents/kggm/go/vertex_texture.glsl";
const char* texfragmentfp = "/home/biospore/Documents/kggm/go/fragment_texture.glsl";

//GLfloat light0_direction[] = {0, 10, 5, 0};
/************************************************************************/



GLuint program0;
GLuint program1;


char* readShaderSource(const char *name)
{
    char* content;
    FILE *ifs = fopen(name, "r");

    fseek(ifs, 0, SEEK_END);
    long size = ftell(ifs);
    fseek(ifs, 0, SEEK_SET);

    content = new char[size+1];
    fread(content, 1, size, ifs);
    fclose(ifs);
    content[size] = '\0';
    return content;
}

GLuint initShaders(const char* vertex, const char* fragment)
{

    GLuint vShader;
    GLuint fShader;

    GLuint program = glCreateProgram();

    vShader = glCreateShader(GL_VERTEX_SHADER);
    fShader = glCreateShader(GL_FRAGMENT_SHADER);

    char const* vertexSource = readShaderSource(vertex);
    char const* fragmentSource = readShaderSource(fragment);

    glShaderSource(vShader, 1, &vertexSource , NULL);
    glCompileShader(vShader);




    glShaderSource(fShader, 1, &fragmentSource , NULL);
    glCompileShader(fShader);



    GLint maxLength = 0;
//    glGetShaderiv(vShader, GL_INFO_LOG_LENGTH​, &isCompiled);
    glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &maxLength);
    cout << "fragment" << endl;
    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(fShader, maxLength, &maxLength, &errorLog[0]);
    for (std::vector<GLchar>::iterator it = errorLog.begin(); it != errorLog.end(); it++)
    {
        cout << *it;
    }

    glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &maxLength);
    cout << "vertex" << endl;
    std::vector<GLchar> errorLog2(maxLength);
    glGetShaderInfoLog(vShader, maxLength, &maxLength, &errorLog2[0]);
    for (std::vector<GLchar>::iterator it = errorLog2.begin(); it != errorLog2.end(); it++)
    {
        cout << *it;
    }



    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    return program;
};

/************************************************************************/


struct bufferContainer
{
    GLuint vbo_pos = 0;
    GLuint vbo_color = 0;
    GLuint shaderArg_pos = 0;
    GLuint shaderArg_color = 1;
};

struct colorRGB
{
    float r, g, b;
};

struct vertex3D
{
    float x, y, z;
//    float r, g, b, a;
};

/************************************************************************/


void initQuadBuffer(GLuint vbo, GLuint shaderAttribute, vertex3D (&data)[4])
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) *3, data, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//    glEnableVertexAttribArray(shaderAttribute);
}

void initColorBuffer(GLuint vbo, GLuint shaderAttribute, colorRGB (&colors)[4])
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) *3, colors, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, NULL);
//    glEnableVertexAttribArray(shaderAttribute);
}


/************************************************************************/


/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////


//float ambient[4] = {2.5, 2.5, 2.5, 1};
GLfloat eye_pos[] = {0, 1, 0};
GLfloat up_pos[] = {0, 0, 1};
GLfloat center_pos[] = {0, 0, 0};
//GLfloat material_diffuse[] = {1.0, 1.0, 1.0, 1.0};



double rotate_y = 5;
double rotate_x = 5;


void lighting()
{
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glLoadIdentity();
//    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, eye_pos);
    glPopMatrix();
}

vector<bufferContainer> buffers;

glm::mat4 biasMatrix = {
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
};


GLuint depthtex;
//GLuint FramebufferName = 0;
GLuint FramebufferName = 0;

void initDepthTexture()
{
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    glGenTextures(1, &depthtex);
    glBindTexture(GL_TEXTURE_2D, depthtex);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, WIDTH, HEIGHT, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthtex, 0);

    glDrawBuffer(GL_NONE);
//
//    glActiveTexture(GL_TEXTURE0);
//    glGenTextures(1, &depthtex);
//    glBindTexture(GL_TEXTURE_2D, depthtex);
//    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB, WIDTH, HEIGHT);
////    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, imageFormat, type, data);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

GLint shader_matrix;
glm::mat4 shaderMatrix;

void initDepth()
{
     shader_matrix = glGetUniformLocation(program1, "lightMatrix");
}

void drawScene()
{
    for (vector<bufferContainer>::iterator it = buffers.begin(); it != buffers.end(); it++)
    {
        GLuint vbo1 = it->vbo_pos;
        GLuint shaderAttribute1 = it->shaderArg_pos;
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glVertexAttribPointer(shaderAttribute1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderAttribute1);
//
        GLuint vbo = it->vbo_color;
        GLuint shaderAttribute = it->shaderArg_color;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderAttribute);
        glDrawArrays(GL_QUADS, 0, 4);

        glDisableVertexAttribArray(shaderAttribute1);
        glDisableVertexAttribArray(shaderAttribute);
    }

}

GLuint depthTexture;

//double anglex = 35;
double angley = 10;

void display()
{
    glPushMatrix();

        gluLookAt(eye_pos[0], eye_pos[1], eye_pos[2], 0, 0, 0, up_pos[0], up_pos[1], up_pos[2]);
//        glRotated(anglex, 1.0, 0.0, 0.0);
        glRotated(angley, 0.0, 1.0, 0.0);
        GLint shadowMapID = glGetUniformLocation(program0, "shadowMap");
        GLint finalMatrixID = glGetUniformLocation(program0, "finalMatrix");
        GLint depthBiasMatrix = glGetUniformLocation(program0, "depthBiasMatrix");
//        GLint lightMatrix = glGetUniformLocation(shaderProgram2, "lightMatrix");

//        projectionMatrix = perspective(radians(45.0f), d.w/d.h, 0.1f, 100.0f);
//        projectionMatrixLight = ortho(-10.0f,10.0f,-10.0f,10.0f, -5.0f, 30.0f);
//
//
//        viewMatrix = lookAt(vec3(-17,25,-17), vec3(0,0,0), vec3(0,1,0));
//        viewMatrixLight = lookAt(vec3(10,10,10), vec3(0,0,0), vec3(0,1,0));
//    gluLookAt(0,0,0, 0,0,0 ,0,0,0);
//    glUniformMatrix4fv(shader_matrix, 1, GL_FALSE, &shaderMatrix[0][0]);
        GLfloat mv[16];
        glGetFloatv(GL_MODELVIEW_MATRIX, mv);
        glm::mat4 matmv;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                matmv[i][j] = mv[i * 4 + j];
//            cout << matmv[i][j] << ' ';
            }
//        cout << endl;
        }
//    cout << endl;

        GLfloat pj[16];
        glGetFloatv(GL_PROJECTION_MATRIX, pj);
        glm::mat4 matpj;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                matpj[i][j] = pj[i * 4 + j];
//            cout << matpj[i][j] << ' ';
            }
//        cout << endl;
        }
//    cout << endl;




        glm::mat4 finalMatrix = matpj * matmv;
//    for (int i = 0; i < 4; i++)
//        for (int l = 0; l < 4; l++)
//            cout << depthMVP[i][l] << ' ';


        glUniformMatrix4fv(finalMatrixID, 1, GL_FALSE, &finalMatrix[0][0]);

//    GLint depthMVPI = glGetUniformLocation(program1, "depthMVP");
//    glUniformMatrix4fv(depthMVPI, 1, GL_FALSE, &depthMVP[0][0]);




        glUseProgram(program1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    lighting();
//    glGenTextures(1, &depthtex);
//    glBindTexture(GL_TEXTURE_BUFFER, depthtex);
//    glActiveTexture(GL_TEXTURE0);





//    glViewport(0, 0, WIDTH, HEIGHT);


    drawScene();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glPopMatrix();
//    gluLookAt(light0_direction[0], light0_direction[1], light0_direction[2], 0, 0, 0, up_pos[0], up_pos[1], up_pos[2]);
    glUseProgram(program0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,1);
    glClearDepth(1);


    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(shadowMapID, 1);
//    gluLookAt(light0_direction[0], light0_direction[1], light0_direction[2], 0, 0, 0, 0, 0, 0.1);
//    gluLookAt(eye_pos[0], eye_pos[1], eye_pos[2], 0, 0, 0, up_pos[0], up_pos[1], up_pos[2]);
//    glRotated(anglex, 1.0, 0.0, 0.0);
//    glRotated(angley, 0.0, 1.0, 0.0);

//    cout << "bias" << endl;
    glm::mat4 depthBiasMVP = biasMatrix * finalMatrix;


//    for (int i = 0; i < 4; i++)
//        for (int j = 0; j < 4; j++)
//            cout << depthBiasMVP[i][j] << ' ';

    glUniformMatrix4fv(depthBiasMatrix, 1, GL_FALSE, &depthBiasMVP[0][0]);
    drawScene();
    glUseProgram(0);

    glutSwapBuffers();
}


void specialKeys( int key, int x, int y )
{
    if (key == GLUT_KEY_RIGHT) {
//        rotate_y += 0.1;
        glRotated(rotate_y, 0.0, 1.0, 0.0);
    }
    else if (key == GLUT_KEY_LEFT) {
//        rotate_y -= 0.1;
        glRotated(-rotate_y, 0.0, 1.0, 0.0);
    }
    else if (key == GLUT_KEY_UP) {
//        rotate_x += 0.1;
        glRotated(rotate_x, -1.0, 0.0, 0.0);
    }
    else if (key == GLUT_KEY_DOWN)
    {
//        rotate_x -= 0.1;
        glRotated(-rotate_x, -1.0, 0.0, 0.0);
    }
    glutPostRedisplay();
}




//
//GLuint vbo0_pos = 0;
//GLuint vbo0_color = 0;
//GLuint shaderArg0_pos = 0;
//GLuint shaderArg0_color = 1;

void createQuad(vertex3D (&quad)[4], colorRGB (&color)[4])
{
    GLuint vbo_pos = 0;
    GLuint vbo_color = 0;
    GLuint shaderArg_pos = 0;
    GLuint shaderArg_color = 1;

    glGenBuffers(1, &vbo_pos);
    initQuadBuffer(vbo_pos, shaderArg_pos, quad);
    glGenBuffers(1, &vbo_color);
    initColorBuffer(vbo_color, shaderArg_color, color);

    bufferContainer container;
    container.vbo_pos =  vbo_pos;
    container.vbo_color = vbo_color;
    container.shaderArg_pos = shaderArg_pos;
    container.shaderArg_color =  shaderArg_color;

    buffers.push_back(container);
}


void createCube(vertex3D point000, vertex3D pointxyz)
{
    colorRGB color0[4] =
            {
                    {0.5, 0.5, 1},
                    {0.5, 0.5, 1},
                    {0.5, 0.5, 1},
                    {0.5, 0.5, 1}
            };

    colorRGB color1[4] =
            {
                    {0.5, 1, 0.5},
                    {0.5, 1, 0.5},
                    {0.5, 1, 0.5},
                    {0.5, 1, 0.5}
            };

    colorRGB color2[4] =
            {
                    {1, 0.5, 0.5},
                    {1, 0.5, 0.5},
                    {1, 0.5, 0.5},
                    {1, 0.5, 0.5}

            };

    colorRGB color3[4] =
            {
                    {0.5, 1, 1},
                    {0.5, 1, 1},
                    {0.5, 1, 1},
                    {0.5, 1, 1}
            };

    colorRGB color4[4] =
            {
                    {1, 1, 0.5},
                    {1, 1, 0.5},
                    {1, 1, 0.5},
                    {1, 1, 0.5}
            };

    colorRGB color5[4] =
            {
                    {1, 0.5, 1},
                    {1, 0.5, 1},
                    {1, 0.5, 1},
                    {1, 0.5, 1}
            };

    vertex3D quad0[4] =
            {
                    {point000.x, point000.y, point000.z},
                    {pointxyz.x, point000.y, point000.z},
                    {pointxyz.x, pointxyz.y, point000.z},
                    {point000.x, pointxyz.y, point000.z}
            };

    vertex3D quad1[4] =
            {
                    {point000.x, point000.y, point000.z},
                    {pointxyz.x, point000.y, point000.z},
                    {pointxyz.x, point000.y, pointxyz.z},
                    {point000.x, point000.y, pointxyz.z}
            };
    vertex3D quad2[4] =
            {
                    {point000.x, point000.y, point000.z},
                    {point000.x, pointxyz.y, point000.z},
                    {point000.x, pointxyz.y, pointxyz.z},
                    {point000.x, point000.y, pointxyz.z}

            };
    vertex3D quad3[4] =
            {
                    {pointxyz.x, pointxyz.y, pointxyz.z},
                    {point000.x, pointxyz.y, pointxyz.z},
                    {point000.x, point000.y, pointxyz.z},
                    {pointxyz.x, point000.y, pointxyz.z}
            };
    vertex3D quad4[4] =
            {
                    {pointxyz.x, pointxyz.y, pointxyz.z},
                    {pointxyz.x, point000.y, pointxyz.z},
                    {pointxyz.x, point000.y, point000.z},
                    {pointxyz.x, pointxyz.y, point000.z}
            };
    vertex3D quad5[4] =
            {
                    {pointxyz.x, pointxyz.y, pointxyz.z},
                    {pointxyz.x, pointxyz.y, point000.z},
                    {point000.x, pointxyz.y, point000.z},
                    {point000.x, pointxyz.y, pointxyz.z}
            };
    createQuad(quad0, color0);
    createQuad(quad1, color1);
    createQuad(quad2, color2);

    createQuad(quad3, color3);
    createQuad(quad4, color4);
    createQuad(quad5, color5);
}

void initObjects()
{
    vertex3D point0 = {-1, -1, -1};
    vertex3D point1 = {+1, +1, +1};
    createCube(point0, point1);
    vertex3D point2 = {-1, -1, 2};
    vertex3D point3 = {+1, 1, 3};
    createCube(point2, point3);

    vertex3D quad0[4] =
            {
                    {-10, -1.01f, -10},
                    {+10, -1.01f, -10},
                    {+10, -1.01f, +10},
                    {-10, -1.01f, +10}
            };
    colorRGB color0[4] =
            {
                    {0.3, 0.7, 0.4},
                    {0.3, 0.7, 0.4},
                    {0.3, 0.7, 0.4},
                    {0.3, 0.7, 0.4}
            };

    createQuad(quad0, color0);


}


void initTexture()
{
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glDrawBuffer(GL_NONE);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
}

void cleanUp()
{
    for (vector<bufferContainer>::iterator it = buffers.begin(); it != buffers.end(); it++)
    {
        GLuint vbo1 = it->vbo_pos;
        glDeleteBuffers(1, &vbo1);
        vbo1 = it->vbo_color;
        glDeleteBuffers(1, &vbo1);
    }
    glDeleteProgram(program0);
    glDeleteProgram(program1);
    glDeleteFramebuffers(1, &FramebufferName);
    glDeleteTextures(1, &depthTexture);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("KGGM - 5 - GO");
    glClearColor(0, 0, 0, 1.0);
    glClearDepth(1.0);


    // shader init
    glewInit();
    program0 = initShaders(vertexfp, fragmentfp);
    program1 = initShaders(texvertexfp, texfragmentfp);
    initObjects();
    initTexture();
//    initDepthTexture();




//    prism1 = initPrism(prism1);
//    prism2 = initPrism(prism2);
//    program = glCreateProgram();

//    prism2.addZ(-4);

//    initShadow();


//    gluLookAt(light0_direction[0], light0_direction[1], light0_direction[2], 0, 0, 0, 0, 0, 0.1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20, 20, -20, 20, -20, 20);
////
    glutDisplayFunc(display);
//
    glRotated(35, 1.0, 0.0, 0.0);
    glRotated(45, 0.0, 1.0, 0.0);
//
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_ALPHA_TEST);
//    glEnable(GL_PROGRAM_POINT_SIZE);
//    glEnable(GL_CULL_FACE);
//    glEnable(GL_LIGHTING);
//
//    glutSpecialFunc(specialKeys);
    glutMainLoop();
    cleanUp();
    return 0;
}