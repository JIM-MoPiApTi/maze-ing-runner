#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include "windows.h"

typedef struct {
    bool Left;
    bool Right;
    bool Up;
    bool Down;
    bool Coin;
    int unvisited;
} Cell;

typedef struct{
    int x;
    int y;
    int score;
}Player;

typedef struct {
    int x;
    int y;
}GenCell;

#define winW 1280
#define winH 720

#define mapW 10
#define mapH 10

int width = winW;
int height = winH;

int mapS=10;

bool torch = 0;
bool easy = 0;

Cell map[mapW][mapH];
Player hero = {0,0,0};
GenCell Exit;

int next_cell(int x, int y) {
    int ways[4] = { 0,0,0,0 };
    int i = 0;
    if (x > 0 && map[x - 1][y].unvisited) {
        ways[i] = 1;
        i++;
    }
    if (x < mapW - 1 && map[x + 1][y].unvisited) {
        ways[i] = 3;
        i++;
    }
    if (y > 0 && map[x][y - 1].unvisited) {
        ways[i] = 2;
        i++;
    }
    if (y < mapH - 1 && map[x][y + 1].unvisited) {
        ways[i] = 4;
        i++;
    }
    printf("    %d%d%d%d %d", ways[0], ways[1], ways[2], ways[3], i);
    if (i) return(ways[rand() % i]);
    else return 0;
}

int help_next(int x, int y) {
    if (x > 0 && map[x - 1][y].unvisited && !(map[x][y].Left)) {
        return 1;
    }
    if (x < mapW - 1 && map[x + 1][y].unvisited && !(map[x][y].Right)) {
        return 3;
    }
    if (y > 0 && map[x][y - 1].unvisited && !(map[x][y].Up)) {
        return 2;
    }
    if (y < mapH - 1 && map[x][y + 1].unvisited && !(map[x][y].Down)) {
        return 4;
    }
    return 0;
}

void Generate() {
    memset(map, 1, sizeof(map));
    srand(time(NULL));
    GenCell current_cell = { 0,0 };
    map[0][0].unvisited = 0;
    GenCell stack[mapW * mapH];
    stack[0] = current_cell;
    int unvisited = mapW * mapH - 1;
    int i = 1;
    int max_i = 0;
    int choice = next_cell(current_cell.x, current_cell.y);
    bool gencel = 1;

    for (int j = 0; j < mapH; j++) for (int i = 0; i < mapW; i++) map[i][j].Coin = 0;

    while (unvisited > 0) {
        switch (choice)
        {
        case 0:
            if (i > max_i) {
                Exit = current_cell;
                max_i = i;
            }
            if(gencel && !(rand()%2)) map[current_cell.x][current_cell.y].Coin = 1;
            i--;
            gencel = 0;
            current_cell = stack[i];
            break;
        case 1:
            stack[i] = current_cell;
            i++;
            map[current_cell.x][current_cell.y].Left = 0;
            current_cell.x--;
            map[current_cell.x][current_cell.y].Right = 0;
            map[current_cell.x][current_cell.y].unvisited = 0;
            unvisited--;
            gencel = 1;
            break;
        case 2:
            stack[i] = current_cell;
            i++;
            map[current_cell.x][current_cell.y].Up = 0;
            current_cell.y--;
            map[current_cell.x][current_cell.y].Down = 0;
            map[current_cell.x][current_cell.y].unvisited = 0;
            unvisited--;
            gencel = 1;
            break;
        case 3:
            stack[i] = current_cell;
            i++;
            map[current_cell.x][current_cell.y].Right = 0;
            current_cell.x++;
            map[current_cell.x][current_cell.y].Left = 0;
            map[current_cell.x][current_cell.y].unvisited = 0;
            unvisited--;
            gencel = 1;
            break;
        case 4:
            stack[i] = current_cell;
            i++;
            map[current_cell.x][current_cell.y].Down = 0;
            current_cell.y++;
            map[current_cell.x][current_cell.y].Up = 0;
            map[current_cell.x][current_cell.y].unvisited = 0;
            unvisited--;
            gencel = 1;
            break;
        }
        printf("%d %d", current_cell.x, current_cell.y);
        choice = next_cell(current_cell.x, current_cell.y);
        printf("   %d\n", choice);
    }
    
    /*for (int j = 0; j < mapH - 1; j++) {
         for (int i = 0; i < mapW-1; i++)
         {
             if (rand() % 2) {
                 map[i][j].Right = 0;
                 map[i+1][j].Left = 0;
             }
             else {
                 map[i][j].Down = 0;
                 map[i][j+1].Up = 0;
             }
         }
     }
     for (int i = 0; i < mapW-1; i++) {
         map[i][mapH - 1].Right = 0;
         map[i+1][mapH - 1].Left = 0;
     }
     for (int i = 0; i < mapH - 1; i++) {
         map[mapW-1][i].Down = 0;
         map[mapW-1][i+1].Up = 0;
     }
     for (int j = 0; j < mapH; j++)
         for (int i = 0; i < mapW; i++)
             if (rand() % 10) map[i][j].Coin = 0;
     /*for (int j = 0; j < mapH; j++) {
         for (int i = 0; i < mapW; i++) {
             printf("%d ", map[i][j].Left);
             printf("%d ", map[i][j].Up);
             printf("%d ", map[i][j].Right);
             printf("%d   ", map[i][j].Down);
         }
         printf("\n");
     }*/
}

void Help() {
    GenCell current_cell = {hero.x,hero.y};
    for (int j = 0; j < mapH; j++) for (int i = 0; i < mapW; i++) map[i][j].unvisited = 1;
    map[current_cell.x][current_cell.y].unvisited = 0;
    GenCell stack[mapW * mapH];
    stack[0] = current_cell;
    
    int i = 1;
    int choice = help_next(current_cell.x, current_cell.y);

    while (current_cell.x!=Exit.x || current_cell.y != Exit.y) {
        switch (choice)
        {
        case 0:
            i--;
            current_cell = stack[i];
            break;
        case 1:
            stack[i] = current_cell;
            i++;
            current_cell.x--;
            map[current_cell.x][current_cell.y].unvisited = 0;
            break;
        case 2:
            stack[i] = current_cell;
            i++;
            current_cell.y--;
            map[current_cell.x][current_cell.y].unvisited = 0;
            break;
        case 3:
            stack[i] = current_cell;
            i++;
            current_cell.x++;
            map[current_cell.x][current_cell.y].unvisited = 0;
            break;
        case 4:
            stack[i] = current_cell;
            i++;
            current_cell.y++;
            map[current_cell.x][current_cell.y].unvisited = 0;
            break;
        }
        choice = help_next(current_cell.x, current_cell.y);
    }
    stack[i] = Exit;
    for (int j = 0; j < mapH; j++) for (int i = 0; i < mapW; i++) map[i][j].unvisited = 0;
    for (int j = 0; j <= i; j++) {
        map[stack[j].x][stack[j].y].unvisited = (1000*j/i)+1;
    }
}


void Score() {
    if (map[hero.x][hero.y].Coin) {
        hero.score += 1;
        map[hero.x][hero.y].Coin = 0;
    }
    if (Exit.x==hero.x && Exit.y==hero.y) {
        hero.score += 100;
        Generate();
        hero.x = 0;
        hero.y = 0;
    }
}

void PrintStats() {
    system("cls");
    printf("%d\n",next_cell(hero.x,hero.y));
    printf("X:%d Y:%d SCORE:%d\n winW:%d winH:%d\n", hero.x, hero.y, hero.score,width,height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
    if ((key == GLFW_KEY_UP || key == GLFW_KEY_W) && !(action == GLFW_RELEASE))
        if (!(map[hero.x][hero.y].Up)) {
            hero.y -= 1;
            Score();
            PrintStats();
            Help();
        }
    if ((key == GLFW_KEY_DOWN || key == GLFW_KEY_S) && !(action == GLFW_RELEASE))
        if (!(map[hero.x][hero.y].Down)){
            hero.y += 1;
            Score();
            PrintStats();
            Help();
        }
            
    if ((key == GLFW_KEY_LEFT || key == GLFW_KEY_A) && !(action == GLFW_RELEASE))
        if (!(map[hero.x][hero.y].Left)){
            hero.x -= 1;
            Score();
            PrintStats();
            Help();
        }
            
    if ((key == GLFW_KEY_RIGHT || key == GLFW_KEY_D) && !(action == GLFW_RELEASE))
        if (!(map[hero.x][hero.y].Right)){
            hero.x += 1;
            Score();
            PrintStats();
            Help();
        }
    if (key == GLFW_KEY_TAB && (action == GLFW_PRESS))
        torch = !(torch);
    if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS)) 
        easy = !(easy);
}

void Draw_Cell(int x, int y)
{
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.5f+0.0002*x*mapS, 0.5f + 0.0002 * y * mapS, 0.5f);
    glVertex2f(0, mapS);
    glColor3f(0.6f + 0.0002 * x * mapS, 0.6f + 0.0002 * y * mapS, 0.6f);
    glVertex2f(mapS, mapS);
    glColor3f(0.6f + 0.0002 * x * mapS, 0.6f + 0.0002 * y * mapS, 0.6f);
    glVertex2f(0, 0);
    glColor3f(0.7f + 0.0002 * x * mapS, 0.7f + 0.0002 * y * mapS, 0.7f);
    glVertex2f(mapS, 0);
    glEnd();
}

void Draw_Walls(int x, int y)
{
    if (map[x][y].Left) {
        glLineWidth(mapS/8);
        glBegin(GL_LINES);
        glColor3f(0,0,0);
        glVertex2f(0,0);
        glVertex2f(0,mapS);
        glEnd();
    }
    if (map[x][y].Up) {
        glLineWidth(mapS / 8);
        glBegin(GL_LINES);
        glColor3f(0, 0, 0);
        glVertex2f(0, 0);
        glVertex2f(mapS, 0);
        glEnd();
    }
    if (map[x][y].Right) {
        glLineWidth(mapS / 8);
        glBegin(GL_LINES);
        glColor3f(0, 0, 0);
        glVertex2f(mapS, mapS);
        glVertex2f(mapS, 0);
        glEnd();
    }
    if (map[x][y].Down) {
        glLineWidth(mapS / 8);
        glBegin(GL_LINES);
        glColor3f(0, 0, 0);
        glVertex2f(0, mapS);
        glVertex2f(mapS, mapS);
        glEnd();
    }
}

void Draw_Coin() 
{
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.97f, 0.95f, 0.17f);
    glVertex2f(mapS/3, 2* mapS/3);
    glColor3f(1.0f, 0.96f, 0.31f);
    glVertex2f(2 * mapS / 3, 2 * mapS / 3);
    glColor3f(0.97f, 0.95f, 0.17f);
    glVertex2f(mapS / 3, mapS / 3);
    glColor3f(1.0f, 0.96f, 0.31f);
    glVertex2f(2 * mapS / 3, mapS / 3);
    glEnd();
}

void Draw_Player()
{
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(mapS / 4, 3 * mapS / 4);
    glVertex2f(3 * mapS / 4, 3 * mapS / 4);
    glVertex2f(mapS / 4, mapS / 4);
    glVertex2f(3 * mapS / 4, mapS / 4);
    glEnd();
}

void Draw_Exit()
{
    glBegin(GL_TRIANGLE_STRIP);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(mapS / 4, 3 * mapS / 4);
    glVertex2f(3 * mapS / 4, 3 * mapS / 4);
    glVertex2f(mapS / 4, mapS / 4);
    glVertex2f(3 * mapS / 4, mapS / 4);
    glEnd();
}

void Draw_Help(int x, int y)
{
    glBegin(GL_TRIANGLE_STRIP);
    glColor3ub(238-(151*map[x][y].unvisited/1000), 110+ (75 * map[x][y].unvisited / 1000), 245+ (map[x][y].unvisited / 100));
    glVertex2f(0, mapS);
    glVertex2f(mapS, mapS);
    glVertex2f(0, 0);
    glVertex2f(mapS, 0);
    glEnd();
}

void Draw_Text(char a) {
    glLineWidth(mapS/6.5);
    glColor3f(1, 1, 0);
    glBegin(GL_LINES);

    // Верхний горизонтальный сегмент (A)
    if (a != '1' && a != '4') {
        glVertex2f(0.3*mapS, 0.15 * mapS);
        glVertex2f(0.7 * mapS, 0.15 * mapS);
    }

    // Средний горизонтальный сегмент (G)
    if (a != '0' && a != '1' && a != '7' && a != 'c' && a != 'r') {
        glVertex2f(0.3 * mapS, 0.5 * mapS);
        glVertex2f(0.7 * mapS, 0.5 * mapS);
    }

    // Нижний горизонтальный сегмент (D)
    if (a != '1' && a != '4' && a != '7' && a != 'r') {
        glVertex2f(0.3 * mapS, 0.85 * mapS);
        glVertex2f(0.7 * mapS, 0.85 * mapS);
    }

    // Правый верхний вертикальный сегмент (B)
    if (a != '5' && a != '6' && a != 's' && a != 'c' && a != 'r' && a != 'e') {
        glVertex2f(0.7 * mapS, 0.15 * mapS);
        glVertex2f(0.7 * mapS, 0.5 * mapS);
    }

    // Правый нижний вертикальный сегмент (C)
    if (a != '2' && a != 'c' && a != 'r' && a != 'e') {
        glVertex2f(0.7 * mapS, 0.5 * mapS);
        glVertex2f(0.7 * mapS, 0.85 * mapS);
    }

    // Левый верхний вертикальный сегмент (F)
    if (a != '1' && a != '2' && a != '3' && a != '7') {
        glVertex2f(0.3 * mapS, 0.15 * mapS);
        glVertex2f(0.3 * mapS, 0.5 * mapS);
    }

    // Левый нижний вертикальный сегмент (E)
    if (a == '0' || a == '2' || a == '6' || a == '8' || a == 'c' || a == 'r' || a == 'e'    ) {
        glVertex2f(0.3 * mapS, 0.5 * mapS);
        glVertex2f(0.3 * mapS, 0.85 * mapS);
    }

    glEnd();
}

void Game_Show()
{
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    for (int j = 0; j < mapH; j++) {
        for (int i = 0; i < mapW; i++) {
            glPushMatrix();
            glTranslatef(i*mapS,j*mapS,0);
            if (torch) {
                if (abs(hero.x - i) < 3 && abs(hero.y - j) < 3 && abs(hero.x - i) + abs(hero.y - j) < 4)
                {
                    Draw_Cell(i, j);
                    Draw_Walls(i, j);
                    if (map[i][j].Coin) Draw_Coin();
                }
            }
            else {
                Draw_Cell(i, j);
                Draw_Walls(i, j);
                if (map[i][j].Coin) Draw_Coin();
            }
            if (easy && map[i][j].unvisited) {
                Draw_Help(i, j);
                Draw_Walls(i, j);
            }
            if (i==hero.x && j==hero.y) Draw_Player();
            if (i == Exit.x && j == Exit.y) Draw_Exit();
            glPopMatrix();
        }
    }
    glTranslatef(mapW * mapS, 0, 0);
    mapS = (width - height) / 5;
    Draw_Text('s');
    glTranslatef(mapS, 0, 0);
    Draw_Text('c');
    glTranslatef(mapS, 0, 0);
    Draw_Text('0');
    glTranslatef(mapS, 0, 0);
    Draw_Text('r');
    glTranslatef(mapS, 0, 0);
    Draw_Text('e');

    glTranslatef(-4*mapS, mapS, 0);
    Draw_Text((hero.score/10000)%10+48);
    glTranslatef(mapS, 0, 0);
    Draw_Text((hero.score / 1000) % 10 + 48);
    glTranslatef(mapS, 0, 0);
    Draw_Text((hero.score / 100) % 10 + 48);
    glTranslatef(mapS, 0, 0);
    Draw_Text((hero.score / 10) % 10 + 48);
    glTranslatef(mapS, 0, 0);
    Draw_Text(hero.score%10 + 48);
}

int main(void)
{
    Generate();
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(winW, winH, "Chiken Jokey!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glLoadIdentity();
    glOrtho(0, winW, winH, 0, -1, 1);
    //PrintStats();

    /* Loop until the user closes the window */
    Help();
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, width, height);
        glfwGetWindowSize(window, &width, &height);
        mapS = height/max(mapW, mapH);

        Game_Show();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}