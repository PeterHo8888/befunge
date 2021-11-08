#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#define HEIGHT 25
#define WIDTH 80
#define STACK_SIZE 1024*1024*256

char grid[HEIGHT][WIDTH];

int stack[STACK_SIZE] = {0};
int sp = 0;

struct {
    int x;
    int y;
} pc;

enum {
    RIGHT, LEFT, UP, DOWN
} dir = RIGHT;

int pop()
{
    if (sp == 0)
        return 0;
    return stack[--sp];
}

void push(int val)
{
    if (sp == STACK_SIZE) {
        printf("Stack overflow!\n");
        exit(EXIT_FAILURE);
    }
    stack[sp++] = val;
}

void move()
{
    if (dir == RIGHT)
        ++pc.x;
    else if (dir == LEFT)
        --pc.x;
    else if (dir == UP)
        --pc.y;
    else if (dir == DOWN)
        ++pc.y;

    if (pc.x >= WIDTH)
        pc.x = 0;
    else if (pc.x < 0)
        pc.x = WIDTH - 1;
    else if (pc.y >= HEIGHT)
        pc.y = 0;
    else if (pc.y < 0)
        pc.y = HEIGHT - 1;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if (argc != 2) {
        printf("Usage: %s <prog.bg>\n", *argv);
        exit(EXIT_FAILURE);
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        printf("Failed to open %s. %s.\n", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < HEIGHT; ++i)
        memset(grid[i], ' ', WIDTH);

    for (int y = 0; y < HEIGHT; ++y) {
        char c;
        for (int x = 0; x < WIDTH; ++x) {
            c = fgetc(f);
            if (c == '\n')
                break;
            if (c == EOF)
                break;
            grid[y][x] = c;
        }
        if (c == EOF)
            break;
    }

    pc.x = 0;
    pc.y = 0;

    while (1) {
        char c = grid[pc.y][pc.x];
        int a, b, x, y, v, val, ch;
        switch (c) {
        case '+':
            push(pop() + pop());
            break;
        case '-':
            a = pop();
            push(pop() - a);
            break;
        case '*':
            push(pop() * pop());
            break;
        case '/':
            a = pop();
            push(pop() / a);
            break;
        case '%':
            a = pop();
            push(pop() % a);
            break;
        case '!':
            push(!pop());
            break;
        case '`':
            a = pop();
            push(pop() > a);
            break;
        case '>':
            dir = RIGHT;
            break;
        case '<':
            dir = LEFT;
            break;
        case '^':
            dir = UP;
            break;
        case 'v':
            dir = DOWN;
            break;
        case '?':
            dir = rand() % 4;
            break;
        case '_':
            dir = (!pop() ? RIGHT : LEFT);
            break;
        case '|':
            dir += (!pop() ? DOWN : UP);
            break;
        case '"':
            while (1) {
                move();
                if ('"' == (ch = grid[pc.y][pc.x]))
                    break;
                push(ch);
            }
            break;
        case ':':
            val = pop();
            push(val);
            push(val);
            break;
        case '\\':
            a = pop();
            b = pop();
            push(a);
            push(b);
            break;
        case '$':
            pop();
            break;
        case '.':
            printf("%d ", pop());
            break;
        case ',':
            printf("%c", pop());
            break;
        case '#':
            move();
            break;
        case 'p':
            y = pop();
            x = pop();
            v = pop();
            grid[y][x] = v;
            break;
        case 'g':
            y = pop();
            x = pop();
            push(grid[y][x]);
            break;
        case '&':
            scanf("%d", &val);
            push(val);
            break;
        case '~':
            scanf("%c", &ch);
            push(ch);
            break;
        case '@':
            exit(EXIT_SUCCESS);
            break;
        default:
            if ('0' <= c && c <= '9') {
                push(c - '0');
            } else if (c != ' ') {
                printf("Illegal character: %c\n", c);
                exit(EXIT_FAILURE);
            }
        }
        move();
    }
}
