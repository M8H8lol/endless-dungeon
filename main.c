#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include "raylib.h"

/*
0: a side room that is not on the solution path
1: a room that is guaranteed to have a left exit and a right exit
2: a room that is guaranteed to have exits on the left, right, and bottom. If there's another "2"
room above it, then it also is guaranteed a top exit
3: a room that is guaranteed to have exists on the left, right, and top
*/

typedef unsigned char byte;
typedef int i32;
typedef uint32_t u32;
typedef float f32;
typedef double f64;

const int tile_size = 16;
const int room_width = 12;
const int room_height = 8;
const int map_height = 8;
const int map_width = 8;

int map[map_height][map_width];
int map_big[map_height * room_height][map_width * room_width];

byte Room0[room_height][room_width] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

byte Room1[room_height][room_width] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

byte Room2[room_height][room_width] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
};

byte Room3[room_height][room_width] = {
    {1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

// int16_t Room0[room_height] = {
//     0b0000111111111111,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000111111111111,
// };

// int16_t Room1[room_height] = {
//     0b0000111111111111,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000000000000000,
//     0b0000000000000000,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000111111111111,
// };

// int16_t Room2 = {
//     0b0000111111111111,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000000000000000,
//     0b0000000000000000,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000111100011111,
// };

// int16_t Room3 = {
//     0b0000111100011111,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000000000000000,
//     0b0000000000000000,
//     0b0000100000000001,
//     0b0000100000000001,
//     0b0000111111111111,
// };
 
void room_draw(byte room[8][12], int x_offset, int y_offset)
{

    for (int y = 0; y < room_height; y++)
    {
        for (int x = 0; x < room_width; x++)
        {
            if (room[y][x] == 1)
            {
                DrawRectangle((x * tile_size) + x_offset, (y * tile_size) + y_offset, tile_size, tile_size, RED);
            }
        }
    }
}

void map_draw()
{
    for (int y = 0; y < map_height; y++)
    {
        for (int x = 0; x < map_width; x++)
        {
            switch (map[y][x])
            {
            case 0:
            {
                room_draw(Room0, x * room_width * tile_size, y * room_height * tile_size);
            }
            break;
            case 1:
            {
                room_draw(Room1, x * room_width * tile_size, y * room_height * tile_size);
            }
            break;
            case 2:
            {
                room_draw(Room2, x * room_width * tile_size, y * room_height * tile_size);
            }
            break;
            case 3:
            {
                room_draw(Room3, x * room_width * tile_size, y * room_height * tile_size);
            }
            break;
            }
        }
    }
}

void map_generate()
{
    time_t t;
    srand(time(&t));

    
    

    for (int y = 0; y < map_height; y++)
    {
        for (int x = 0; x < map_width; x++)
        {
            map[y][x] = (rand() % 4);
        }
    }
}

int main(void)
{

    InitWindow(map_width * room_width * tile_size, map_height * room_height * tile_size, "Sporkly");

    map_generate();

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {

            map_generate();
        }

        BeginDrawing();
        ClearBackground(BLACK);

        map_draw();

        EndDrawing();
    }
}