#pragma once

#include <vector>
#include <list>

#define X_CHUNK_SIZE 100
#define Y_CHUNK_SIZE 100
#define X_MULTIPLIER (1.0 / X_CHUNK_SIZE)
#define Y_MULTIPLIER (1.0 / Y_CHUNK_SIZE)

template <class T>
class GridChunk {
	T contents[X_CHUNK_SIZE][Y_CHUNK_SIZE];


public:
	int count = 0;

	GridChunk() {
		for (int i = 0; i < X_CHUNK_SIZE; i++) {
			for (int j = 0; j < Y_CHUNK_SIZE; j++) {
				contents[i][j] = NULL;
			}
		}
	}

	inline void set(int x, int y, T val) {
		if (contents[x][y] == NULL) {
			if (val != NULL) {
				count++;
				contents[x][y] = val;
			}
		}
		else {
			if (val == NULL) {
				count--;
				contents[x][y] = NULL;
			}
			else {
				contents[x][y] = val;
			}
		}
	}
	inline T get(int x, int y) {
		return contents[x][y];
	}

	static GridChunk<T> *newChunk() {
		if (objPool.size() == 0) {
			return new GridChunk<T>;
		}
		else {
			GridChunk<T> *tmp = objPool.front();
			objPool.pop_front();
			return tmp;
		}
	}
	static void deleteChunk(GridChunk<T> *ptr) {
		objPool.push_back(ptr);
	}
	static void deletePool() {
		while (!objPool.empty()) {
			delete objPool.front();
			objPool.pop_front();
		}
	}
	static std::list<GridChunk*> objPool;
};

template <class T> std::list<GridChunk<T>*> GridChunk<T>::objPool;


template <class T>
class Grid2d
{

public:
	struct vector_pair {
		std::vector<GridChunk<T>*> positive; // 0...n
		std::vector<GridChunk<T>*> negative; // -1...-n
	};

	Grid2d();
	~Grid2d();

	std::vector<vector_pair> positive;
	std::vector<vector_pair> negative;

	T get(int x, int y);
	void set(int x, int y, T val);
};

template <class T>
Grid2d<T>::Grid2d() {}

template <class T>
Grid2d<T>::~Grid2d()
{
	for (vector_pair pair : positive) {
		for (GridChunk<T> *chunk : pair.positive) {
			if (chunk != NULL) {
				GridChunk<T>::deleteChunk(chunk);
			}
		}
		for (GridChunk<T> *chunk : pair.negative) {
			if (chunk != NULL) {
				GridChunk<T>::deleteChunk(chunk);
			}
		}
	}
}

template <class T>
void Grid2d<T>::set(int x, int y, T val)
{

	int x_chunk = (int)floor(x * X_MULTIPLIER);
	int y_chunk = (int)floor(y * Y_MULTIPLIER);

	if (x_chunk >= 0) { // Positive x
		if (x_chunk >= positive.size()) { // Resize x if necessary
			positive.resize(x_chunk + 1);
		}
		vector_pair &y_vectors = positive[x_chunk];

		if (y_chunk >= 0) { // Positive y
			if (y_chunk >= y_vectors.positive.size()) { // Resize y if necessary
				y_vectors.positive.resize(y_chunk + 1, NULL);
			}

			if (y_vectors.positive[y_chunk] == NULL) { // Allocate if needed
				y_vectors.positive[y_chunk] = GridChunk<T>::newChunk();
			}

			y_vectors.positive[y_chunk]->set(x % X_CHUNK_SIZE, y % Y_CHUNK_SIZE, val);

			if (y_vectors.positive[y_chunk]->count == 0) { // Release if needed
				GridChunk<T>::deleteChunk(y_vectors.positive[y_chunk]);
				y_vectors.positive[y_chunk] = NULL;
			}
		}
		else { // Negative y
			y_chunk = -y_chunk - 1;

			if (y_chunk >= y_vectors.negative.size()) { // Resize y if necessary
				y_vectors.negative.resize(y_chunk + 1, NULL);
			}

			if (y_vectors.negative[y_chunk] == NULL) {
				y_vectors.negative[y_chunk] = GridChunk<T>::newChunk();
			}
			y_vectors.negative[y_chunk]->set(x % X_CHUNK_SIZE, (-y) % Y_CHUNK_SIZE, val);

			if (y_vectors.negative[y_chunk]->count == 0) { // Release if needed
				GridChunk<T>::deleteChunk(y_vectors.negative[y_chunk]);
				y_vectors.negative[y_chunk] = NULL;
			}
		}
	}
	else { // Negative x
		x_chunk = -x_chunk - 1;

		if (x_chunk >= negative.size()) { // Resize x if necessary
			negative.resize(x_chunk + 1);
		}
		vector_pair &y_vectors = negative[x_chunk];

		if (y_chunk >= 0) { // Positive y
			if (y_chunk >= y_vectors.positive.size()) { // Resize y if necessary
				y_vectors.positive.resize(y_chunk + 1, NULL);
			}

			if (y_vectors.positive[y_chunk] == NULL) {
				y_vectors.positive[y_chunk] = GridChunk<T>::newChunk();
			}
			y_vectors.positive[y_chunk]->set((-x) % X_CHUNK_SIZE, y % Y_CHUNK_SIZE, val);

			if (y_vectors.positive[y_chunk]->count == 0) { // Release if needed
				GridChunk<T>::deleteChunk(y_vectors.positive[y_chunk]);
				y_vectors.positive[y_chunk] = NULL;
			}
		}
		else { // Negative y
			y_chunk = -y_chunk - 1;

			if (y_chunk >= y_vectors.negative.size()) { // Resize y if necessary
				y_vectors.negative.resize(y_chunk + 1, NULL);
			}

			if (y_vectors.negative[y_chunk] == NULL) {
				y_vectors.negative[y_chunk] = GridChunk<T>::newChunk();
			}
			y_vectors.negative[y_chunk]->set((-x) % X_CHUNK_SIZE, (-y) % Y_CHUNK_SIZE, val);

			if (y_vectors.negative[y_chunk]->count == 0) { // Release if needed
				GridChunk<T>::deleteChunk(y_vectors.negative[y_chunk]);
				y_vectors.negative[y_chunk] = NULL;
			}
		}

	}
}

template <class T>
T Grid2d<T>::get(int x, int y)
{

	int x_chunk = (int)floor(x * X_MULTIPLIER);
	int y_chunk = (int)floor(y * Y_MULTIPLIER);


	if (x_chunk >= 0) { // Positive x
		if (x_chunk >= positive.size()) { // x out of range
			return NULL;
		}
		vector_pair &y_vectors = positive[x_chunk];

		if (y_chunk >= 0) { // Positive y
			if (y_chunk >= y_vectors.positive.size()) { // y out of range
				return NULL;
			}

			GridChunk<T> *chunk = y_vectors.positive[y_chunk];
			if (chunk == NULL)
				return NULL;

			return chunk->get(x % X_CHUNK_SIZE, y % Y_CHUNK_SIZE);
		}
		else { // Negative y
			y_chunk = -y_chunk - 1;

			if (y_chunk >= y_vectors.negative.size()) { // y out of range
				return NULL;
			}

			GridChunk<T> *chunk = y_vectors.negative[y_chunk];
			if (chunk == NULL)
				return NULL;

			return chunk->get(x % X_CHUNK_SIZE, (-y) % Y_CHUNK_SIZE);
		}
	}
	else { // Negative x
		x_chunk = -x_chunk - 1;

		if (x_chunk >= negative.size()) { // Resize x if necessary
			negative.resize(x_chunk + 1);
		}
		vector_pair &y_vectors = negative[x_chunk];

		if (y_chunk >= 0) { // Positive y
			if (y_chunk >= y_vectors.positive.size()) { // y out of range
				return NULL;
			}

			GridChunk<T> *chunk = y_vectors.positive[y_chunk];
			if (chunk == NULL)
				return NULL;

			return chunk->get((-x) % X_CHUNK_SIZE, y % Y_CHUNK_SIZE);
		}
		else { // Negative y
			y_chunk = -y_chunk - 1;

			if (y_chunk >= y_vectors.negative.size()) { // y out of range
				return NULL;
			}

			GridChunk<T> *chunk = y_vectors.negative[y_chunk];
			if (chunk == NULL)
				return NULL;

			return chunk->get((-x) % X_CHUNK_SIZE, (-y) % Y_CHUNK_SIZE);
		}
	}
}

