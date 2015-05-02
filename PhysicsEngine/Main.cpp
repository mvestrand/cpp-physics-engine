#include "stdafx.h"

#include "PointerGrid3d.h"

int main(int argc, char **argv)
{
	int x, y, z, val;

	PointerGrid3d<int> *grid = new PointerGrid3d < int >;
	bool run = true;
	while (run) {
		int opt = 0;
		printf("enter(1), print(2), exit(3): ");
		scanf_s("%d", &opt);

		switch (opt) {
		case 1:
			printf("x y z: ");
			scanf_s("%d %d %d", &x, &y, &z);
			printf("value: ");
			scanf_s("%d", &val);
			grid->set(x, y, z, val);
			break;
		case 2:
			printf("x y z: ");
			scanf_s("%d %d %d", &x, &y, &z);
			printf("(%d,%d,%d) = %d\n", x, y, z, grid->get(x, y, z));
			break;
		default:
			run = false;
		}

	}

	delete grid;
	PointerGridChunk3d<int>::deletePool();

	return 0;
}