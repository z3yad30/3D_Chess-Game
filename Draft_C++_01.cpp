#include <GL/glut.h>
#include <cmath>
#include <iostream>

const int BOARD_SIZE = 8;
const float CELL_SIZE = 1.0f;
const float BOARD_HEIGHT = 0.2f;
float angle = 45.0f;
float camX = 10.0f, camY = 15.0f, camZ = 10.0f;

enum PieceType { NONE, PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING };
enum Color { WHITE, BLACK };

struct Piece {
    PieceType type = NONE;
    Color color;
};

Piece board[BOARD_SIZE][BOARD_SIZE];
bool whiteTurn = true;
int selectedX = -1, selectedY = -1;

void initBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = { NONE, WHITE };
        }
    }

    int row = 0;
    board[row][0] = { ROOK, WHITE };
    board[row][1] = { KNIGHT, WHITE };
    board[row][2] = { BISHOP, WHITE };
    board[row][3] = { QUEEN, WHITE };
    board[row][4] = { KING, WHITE };
    board[row][5] = { BISHOP, WHITE };
    board[row][6] = { KNIGHT, WHITE };
    board[row][7] = { ROOK, WHITE };
    for (int i = 0; i < BOARD_SIZE; i++) board[row + 1][i] = { PAWN, WHITE };

    row = 7;
    board[row][0] = { ROOK, BLACK };
    board[row][1] = { KNIGHT, BLACK };
    board[row][2] = { BISHOP, BLACK };
    board[row][3] = { QUEEN, BLACK };
    board[row][4] = { KING, BLACK };
    board[row][5] = { BISHOP, BLACK };
    board[row][6] = { KNIGHT, BLACK };
    board[row][7] = { ROOK, BLACK };
    for (int i = 0; i < BOARD_SIZE; i++) board[row - 1][i] = { PAWN, BLACK };
}

bool isPathClear(int sr, int sc, int dr, int dc) {
    int stepX = (dc > sc) ? 1 : (dc < sc) ? -1 : 0;
    int stepY = (dr > sr) ? 1 : (dr < sr) ? -1 : 0;
    int steps = std::max(abs(dc - sc), abs(dr - sr));

    for (int i = 1; i < steps; i++) {
        int x = sc + stepX * i;
        int y = sr + stepY * i;
        if (board[y][x].type != NONE) return false;
    }
    return true;
}

bool isValidMove(int sr, int sc, int dr, int dc) {
    if (dr < 0 || dr >= BOARD_SIZE || dc < 0 || dc >= BOARD_SIZE) return false;
    if (sr == dr && sc == dc) return false;

    Piece src = board[sr][sc];
    Piece dest = board[dr][dc];

    if (src.type == NONE || (dest.type != NONE && dest.color == src.color)) return false;

    int dx = abs(dc - sc);
    int dy = abs(dr - sr);

    switch (src.type) {
    case PAWN: {
        int dir = (src.color == WHITE) ? 1 : -1;
        int startRow = (src.color == WHITE) ? 1 : 6;

        if (sc == dc) {
            if (dest.type == NONE) {
                if (dr == sr + dir) return true;
                if (sr == startRow && dr == sr + 2 * dir && isPathClear(sr, sc, dr, dc)) return true;
            }
        }
        else if (dx == 1 && dy == 1) {
            return (dest.type != NONE || dr == (src.color == WHITE ? 5 : 2)) && dr == sr + dir;
        }
        return false;
    }
    case ROOK:
        if ((sr != dr) ^ (sc != dc)) return isPathClear(sr, sc, dr, dc);
        return false;
    case KNIGHT:
        return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
    case BISHOP:
        if (dx == dy) return isPathClear(sr, sc, dr, dc);
        return false;
    case QUEEN:
        if (dx == dy || sr == dr || sc == dc) return isPathClear(sr, sc, dr, dc);
        return false;
    case KING:
        return dx <= 1 && dy <= 1;
    default:
        return false;
    }
}

void setMaterial(Color color) {
    GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_diffuse[] = { color == WHITE ? 0.9f : 0.1f,
                             color == WHITE ? 0.9f : 0.1f,
                             color == WHITE ? 0.9f : 0.1f, 1.0f };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void drawCylinder(float base, float top, float height) {
    GLUquadricObj* quadric = gluNewQuadric();
    gluCylinder(quadric, base, top, height, 32, 32);
    gluDeleteQuadric(quadric);
}

void drawPawn(Color color) {
    setMaterial(color);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    drawCylinder(0.3, 0.2, 0.5);
    glTranslatef(0, 0, 0.5);
    glutSolidSphere(0.2, 32, 32);
    glPopMatrix();
}

void drawRook(Color color) {
    setMaterial(color);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    drawCylinder(0.4, 0.4, 0.6);
    glTranslatef(0, 0, 0.6);
    drawCylinder(0.5, 0.3, 0.2);
    glPopMatrix();
}

void drawKnight(Color color) {
    setMaterial(color);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glutSolidTeapot(0.3);
    glPopMatrix();
}

void drawBishop(Color color) {
    setMaterial(color);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    drawCylinder(0.3, 0.1, 0.8);
    glTranslatef(0, 0, 0.8);
    glutSolidSphere(0.1, 32, 32);
    glPopMatrix();
}

void drawQueen(Color color) {
    setMaterial(color);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    drawCylinder(0.4, 0.2, 0.6);
    glTranslatef(0, 0, 0.6);
    glutSolidSphere(0.3, 32, 32);
    for (int i = 0; i < 4; ++i) {
        glRotatef(90, 0, 0, 1);
        glTranslatef(0.2, 0, 0);
        glutSolidCone(0.1, 0.3, 32, 32);
        glTranslatef(-0.2, 0, 0);
    }
    glPopMatrix();
}

void drawKing(Color color) {
    setMaterial(color);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    drawCylinder(0.4, 0.3, 0.7);
    glTranslatef(0, 0, 0.7);
    drawCylinder(0.2, 0.1, 0.2);
    glTranslatef(0, 0, 0.2);
    glutSolidCube(0.3);
    glPopMatrix();
}

void drawBoardCells() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            glPushMatrix();
            glTranslatef(j * CELL_SIZE + CELL_SIZE / 2, 0, i * CELL_SIZE + CELL_SIZE / 2);

            int colorIndex = (i + j) % 3;
            switch (colorIndex) {
            case 0:
                glColor3f(0.75f, 0.75f, 0.75f);
                break;
            case 1:
                glColor3f(1.0f, 1.0f, 1.0f);
                break;
            case 2:
                glColor3f(0.3f, 0.3f, 0.3f);
                break;
            }

            glPushMatrix();
            glTranslatef(0, -BOARD_HEIGHT / 2, 0);
            glScalef(CELL_SIZE, BOARD_HEIGHT, CELL_SIZE);
            glutSolidCube(1);
            glPopMatrix();

            glPopMatrix();
        }
    }
}

void drawBoardPieces() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j].type != NONE) {
                glPushMatrix();
                glTranslatef(j * CELL_SIZE + CELL_SIZE / 2, 0, i * CELL_SIZE + CELL_SIZE / 2);

                glTranslatef(0, BOARD_HEIGHT / 2, 0);
                if (selectedX == j && selectedY == i) {
                    glDisable(GL_LIGHTING);
                    glColor3f(1, 1, 0);
                    glutWireSphere(0.5, 8, 8);
                    glEnable(GL_LIGHTING);
                }
                switch (board[i][j].type) {
                case PAWN: drawPawn(board[i][j].color); break;
                case ROOK: drawRook(board[i][j].color); break;
                case KNIGHT: drawKnight(board[i][j].color); break;
                case BISHOP: drawBishop(board[i][j].color); break;
                case QUEEN: drawQueen(board[i][j].color); break;
                case KING: drawKing(board[i][j].color); break;
                }

                glPopMatrix();
            }
        }
    }
}

void draw2DOverlayBoard() {
    float overlayHeight = BOARD_HEIGHT - 0.1f; // slightly above the 3D board's surface

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            glPushMatrix();
            glTranslatef(j * CELL_SIZE + CELL_SIZE / 2.0f, overlayHeight, i * CELL_SIZE + CELL_SIZE / 2.0f);

            if ((i + j) % 2 == 0)
                glColor4f(0.75f, 0.75f, 0.75f, 0.7f);  // silver-ish
            else
                glColor4f(0.2f, 0.2f, 0.2f, 0.7f);      // light black-ish

            glDisable(GL_LIGHTING);
            glDisable(GL_DEPTH_TEST); // Ensure the overlay is drawn on top
            glBegin(GL_QUADS);
            glVertex3f(-CELL_SIZE / 2, -0.09, -CELL_SIZE / 2);
            glVertex3f(CELL_SIZE / 2, -0.09, -CELL_SIZE / 2);
            glVertex3f(CELL_SIZE / 2, -0.09, CELL_SIZE / 2);
            glVertex3f(-CELL_SIZE / 2, -0.09, CELL_SIZE / 2);
            glEnd();
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LIGHTING);

            glPopMatrix();
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camX, camY, camZ,
        BOARD_SIZE / 2, 0, BOARD_SIZE / 2,
        0, 1, 0);

    GLfloat light_position[] = { 10.0, 15.0, 10.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    drawBoardCells();
    draw2DOverlayBoard();
    drawBoardPieces();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    const float RADIUS = 15.0f;  // Increased orbit radius for better visibility
    const float SPEED = 3.0f;    // Smoother rotation

    switch (key) {
    case 'a':
        angle -= SPEED;
        break;
    case 'd':
        angle += SPEED;
        break;
    case 'w':
        camY += 1.5f;  // More noticeable vertical movement
        break;
    case 's':
        if (camY > 3.0f) camY -= 1.5f;  // Prevent going below board
        break;
    case 27:  // ESC key
        exit(0);
    }

    // Convert angle to radians for trigonometric functions
    float theta = angle * 3.14159265f / 180.0f;

    // Update camera position (orbit around board center)
    camX = BOARD_SIZE / 2 + RADIUS * sin(theta);
    camZ = BOARD_SIZE / 2 + RADIUS * cos(theta);

    glutPostRedisplay();  // Trigger redraw
}

void mouse(int button, int state, int x, int y) {
    if (state != GLUT_DOWN) return;

    GLint viewport[4];
    GLdouble modelview[16], projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetIntegerv(GL_VIEWPORT, viewport);
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    int cellX = (int)(posX / CELL_SIZE);
    int cellY = (int)(posZ / CELL_SIZE);

    if (cellX < 0 || cellX >= BOARD_SIZE || cellY < 0 || cellY >= BOARD_SIZE) return;

    if (selectedX == -1) {
        if (board[cellY][cellX].type != NONE &&
            board[cellY][cellX].color == (whiteTurn ? WHITE : BLACK)) {
            selectedX = cellX;
            selectedY = cellY;
        }
    }
    else {
        if (isValidMove(selectedY, selectedX, cellY, cellX)) {
            board[cellY][cellX] = board[selectedY][selectedX];
            board[selectedY][selectedX].type = NONE;
            whiteTurn = !whiteTurn;
        }
        selectedX = selectedY = -1;
    }
    glutPostRedisplay();
}

void initGL() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Chess Game");

    initGL();
    initBoard();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glutMainLoop();
    return 0;
}