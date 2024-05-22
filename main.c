#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 20
#define HEIGHT 30

typedef struct {
    int x, y;  // 方块的左上角坐标
    int shape[4][4];  // 方块形状
} Block;

// 初始化方块形状
Block newBlock() {
    Block block;
    block.x = 8;
    block.y = 0;
    int shape[4][4] = {{1, 1, 1, 0},
                       {0, 1, 0, 0},
                       {0, 0, 0, 0},
                       {0, 0, 0, 0}};
    memcpy(block.shape, shape, sizeof(shape));
    return block;
}

void rotateBlock(Block *block) {
    int temp[4][4];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp[j][3 - i] = block->shape[i][j];
        }
    }
    memcpy(block->shape, temp, sizeof(temp));
}

int checkCollision(int screen[HEIGHT][WIDTH], Block *block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block->shape[i][j] == 1) {
                int newX = block->x + j;
                int newY = block->y + i;
                if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT || screen[newY][newX] == 1) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void placeBlock(int screen[HEIGHT][WIDTH], Block *block) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (block->shape[i][j] == 1) {
                screen[block->y + i][block->x + j] = 1;
            }
        }
    }
}

void clearLines(int screen[HEIGHT][WIDTH]) {
    for (int i = HEIGHT - 1; i >= 0; i--) {
        int fullLine = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (screen[i][j] == 0) {
                fullLine = 0;
                break;
            }
        }
        if (fullLine) {
            for (int k = i; k > 0; k--) {
                memcpy(screen[k], screen[k - 1], sizeof(screen[k]));
            }
            memset(screen[0], 0, sizeof(screen[0]));
            i++;
        }
    }
}

void printScreen(int screen[HEIGHT][WIDTH]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%d ", screen[i][j]);
        }
        printf("\n");
    }
    printf("============\n\n");
}
int main() {
    int screen[HEIGHT][WIDTH] = {0};
    Block block = newBlock();

    while (1) {
        // 模拟方块下降
        block.y++;
        if (checkCollision(screen, &block)) {
            block.y--;
            placeBlock(screen, &block);
            
            // 检查新生成的方块是否超出屏幕边界
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    // 注意这里是 屏幕上边界 y = 0 
                    if (block.shape[i][j] == 1 && block.y + i <= 0) {
                        printf("Overflow! Game over.\n");
                        return 0; // 终止程序
                    }
                }
            }
            clearLines(screen);
            block = newBlock(); // 生成新方块

        }

        // 打印屏幕矩阵
        printScreen(screen);

        // 延迟以模拟帧率
        usleep(500000);

        // 不能清屏，会有问题
        // printf("\033[H\033[J");
    }

    return 0;
}
