#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#define GRID_SIZE 100
#define WORLD_SIZE 10000 // 100 * 100

typedef bool world_t[10000];

typedef bool neighbors_t[8];

void clear_screen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void world_print(world_t w)
{
  for (size_t i = 0; i < GRID_SIZE; i++) {
    for (size_t j = 0; j < GRID_SIZE; j++) {
      printf("%s", w[i * GRID_SIZE + j] ? "O " : "  ");
    }
    printf("\n");
  }
}

void world_init(world_t w, float percentage)
{
  for (size_t i = 0; i < WORLD_SIZE; i++) {
    if (rand() % 100 < percentage * 100) w[i] = rand() % 2 == 0;
  }
}

size_t cell_check_neighbors(world_t w, size_t i, size_t j)
{
  neighbors_t neighbors;
  // From 0th index its: left, right, top, bottom, left top, left bottom,
  // right top and right bottom. Zero if index is out of bounds.
  neighbors[0] = i == 0 ? 0 : w[(i - 1) * GRID_SIZE + j];
  neighbors[1] = i == GRID_SIZE ? 0 : w[(i + 1) * GRID_SIZE + j];
  neighbors[2] = j == 0 ? 0 : w[i * GRID_SIZE + (j - 1)];
  neighbors[3] = j == GRID_SIZE ? 0 : w[i * GRID_SIZE + (j + 1)];
  neighbors[4] = i == 0 || j == 0 ? 0 : w[(i - 1) * GRID_SIZE + (j - 1)];
  neighbors[5] = i == 0 || j == GRID_SIZE ? 0 : w[(i - 1) * GRID_SIZE + (j + 1)];
  neighbors[6] = i == GRID_SIZE || j == 0 ? 0 : w[(i + 1) * GRID_SIZE + (j - 1)];
  neighbors[7] = i == GRID_SIZE || j == GRID_SIZE ? 0 : w[(i + 1) * GRID_SIZE + (j + 1)];

  size_t living_neighbors = 0;
  for (size_t k = 0; k < 8; k++) if (neighbors[k]) living_neighbors++;
  return living_neighbors;
}

/*  Any live cell with fewer than two live neighbors dies, as if by underpopulation.
    Any live cell with two or three live neighbors lives on to the next generation.
    Any live cell with more than three live neighbors dies, as if by overpopulation.
    Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction. */
void cell_determine_livelihood(world_t w)
{
  for (size_t i = 0; i < WORLD_SIZE; i++) {
    size_t living_neighbors = cell_check_neighbors(w, i / GRID_SIZE, i % GRID_SIZE);
    bool underpopulation = w[i] == 1 && living_neighbors < 2;
    bool lives_on = w[i] && (living_neighbors == 2 || living_neighbors == 3);
    bool overpopulation = w[i] == 1 && living_neighbors > 3;
    bool reproduction = w[i] == 0 && living_neighbors == 3;

    if (underpopulation || overpopulation) w[i] = 0;
    else if (lives_on) w[i] = 1;
    else if (reproduction) w[i] = 1;
    else w[i] = w[i];
  }
}

int main()
{
  world_t w = {};

  world_init(w, 0.2);

  size_t generations = 100;

  for (size_t i = 0; i < generations; i++) {
    clear_screen();
    world_print(w);
    cell_determine_livelihood(w);
    usleep(100000);
  }
}
