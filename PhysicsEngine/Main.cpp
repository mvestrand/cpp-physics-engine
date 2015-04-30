#include "stdafx.h"

#include "Grid2d.h"

int main(int argc, char **argv)
{
	int x, y, val;

	Grid2d<int> *grid = new Grid2d < int >;
	bool run = true;
	while (run) {
		int opt = 0;
		printf("enter(1), print(2), exit(3): ");
		scanf_s("%d", &opt);

		switch (opt) {
		case 1:
			printf("x and y: ");
			scanf_s("%d %d", &x, &y);
			printf("value: ");
			scanf_s("%d", &val);
			grid->set(x, y, val);
			break;
		case 2:
			printf("x and y: ");
			scanf_s("%d %d", &x, &y);
			printf("(%d,%d) = %d\n", x, y, grid->get(x, y));
			break;
		default:
			run = false;
		}

	}

	delete grid;
	GridChunk<int>::deletePool();

	return 0;
}