#include <GL/glew.h>
#include <GL/glut.h>
//#include <glm/glm.hpp>
#include <vector>

#include <iostream>


using namespace std;

const int WIDTH = 1000, HEIGHT = 1000;
const char* vertexfp = "/home/biospore/Documents/kggm/go/vertex.glsl";
const char* fragmentfp = "/home/biospore/Documents/kggm/go/fragment.glsl";

GLfloat light0_direction[] = {0, 10, 5, 0};
/************************************************************************/

GLuint vShader;
GLuint fShader;

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
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vertex3D), data, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderAttribute);
}

void initColorBuffer(GLuint vbo, GLuint shaderAttribute, colorRGB (&colors)[4])
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(colorRGB), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderAttribute);
}


/************************************************************************/


/////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////


//float ambient[4] = {2.5, 2.5, 2.5, 1};
GLfloat light0_diffuse[] = {1, 1, 1, 1};
//GLfloat material_diffuse[] = {1.0, 1.0, 1.0, 1.0};



double rotate_y = 5;
double rotate_x = 5;


void lighting()
{
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glPushMatrix();
    glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glPopMatrix();
}

vector<bufferContainer> buffers;


void display()
{

    glUseProgram(program1);
//    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1);
//    glClearColor(0.0, 0.0, 0.0, 1.0);
    for (vector<bufferContainer>::iterator it = buffers.begin(); it != buffers.end(); it++)
    {
        GLuint vbo = it->vbo_pos;
        GLuint shaderAttribute = it->shaderArg_pos;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderAttribute);

        vbo = it->vbo_color;
        shaderAttribute = it->shaderArg_color;
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(shaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderAttribute);
        glDrawArrays(GL_QUADS, 0, 4);
    }

//    glDrawArrays(GL_QUADS, 4, 8);

//    lighting();

//    drawPrism(prism1);
//    drawPrism(prism2);
//    glBegin(GL_QUADS);
//    glColor3f(1, 1, 1);
//    glVertex3f(10.0f, -0.01f, 10.0f);
//    glVertex3f(-10.0f, -0.01f, 10.0f);
//    glVertex3f(-10.0f, -0.01f, -10.0f);
//    glVertex3f(10.0f, -0.01f, -10.0f);
//    glEnd();

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
    vertex3D point2 = {2, -1, -1};
    vertex3D point3 = {4, +1, +1};
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
    program1 = initShaders(vertexfp, fragmentfp);
    initObjects();




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
    glEnable(GL_PROGRAM_POINT_SIZE);
////    glEnable(GL_LIGHTING);
//
    glutSpecialFunc(specialKeys);
    glutMainLoop();
    return 0;
}