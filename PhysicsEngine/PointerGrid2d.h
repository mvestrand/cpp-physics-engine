#pragma once

#include <vector>
#include <list>

#define X_CHUNK_SIZE 100
#define Y_CHUNK_SIZE 100
#define X_MULTIPLIER (1.0 / X_CHUNK_SIZE)
#define Y_MULTIPLIER (1.0 / Y_CHUNK_SIZE)

template <class T>
class PointerGridChunk {
	T contents[X_CHUNK_SIZE][Y_CHUNK_SIZE];


public:
	int count = 0;

	PointerGridChunk() {
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

	static PointerGridChunk<T> *newChunk() {
		if (objPool.size() == 0) {
			return new PointerGridChunk<T>;
		}
		else {
			PointerGridChunk<T> *tmp = objPool.front();
			objPool.pop_front();
			return tmp;
		}
	}
	static void deleteChunk(PointerGridChunk<T> *ptr) {
		objPool.push_back(ptr);
	}
	static void deletePool() {
		while (!objPool.empty()) {
			delete objPool.front();
			objPool.pop_front();
		}
	}
	static std::list<PointerGridChunk*> objPool;
};

template <class T> std::list<PointerGridChunk<T>*> PointerGridChunk<T>::objPool;


template <class T>
class PointerGrid2d
{

public:
	struct vector_pair {
		std::vector<PointerGridChunk<T>*> positive; // 0...n
		std::vector<PointerGridChunk<T>*> negative; // -1...-n
	};

	PointerGrid2d();
	~PointerGrid2d();

	std::vector<vector_pair> positive;
	std::vector<vector_pair> negative;

	T get(int x, int y);
	void set(int x, int y, T val);
};

template <class T>
PointerGrid2d<T>::PointerGrid2d() {}

template <class T>
PointerGrid2d<T>::~PointerGrid2d()
{
	for (vector_pair pair : positive) {
		for (PointerGridChunk<T> *chunk : pair.positive) {
			if (chunk != NULL) {
				PointerGridChunk<T>::deleteChunk(chunk);
			}
		}
		for (PointerGridChunk<T> *chunk : pair.negative) {
			if (chunk != NULL) {
				PointerGridChunk<T>::deleteChunk(chunk);
			}
		}
	}
}

template <class T>
void PointerGrid2d<T>::set(int x, int y, T val)
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
				y_vectors.positive[y_chunk] = PointerGridChunk<T>::newChunk();
			}

			y_vectors.positive[y_chunk]->set(x % X_CHUNK_SIZE, y % Y_CHUNK_SIZE, val);

			if (y_vectors.positive[y_chunk]->count == 0) { // Release if needed
				PointerGridChunk<T>::deleteChunk(y_vectors.positive[y_chunk]);
				y_vectors.positive[y_chunk] = NULL;
			}
		}
		else { // Negative y
			y_chunk = -y_chunk - 1;

			if (y_chunk >= y_vectors.negative.size()) { // Resize y if necessary
				y_vectors.negative.resize(y_chunk + 1, NULL);
			}

			if (y_vectors.negative[y_chunk] == NULL) {
				y_vectors.negative[y_chunk] = PointerGridChunk<T>::newChunk();
			}
			y_vectors.negative[y_chunk]->set(x % X_CHUNK_SIZE, (-y) % Y_CHUNK_SIZE, val);

			if (y_vectors.negative[y_chunk]->count == 0) { // Release if needed
				PointerGridChunk<T>::deleteChunk(y_vectors.negative[y_chunk]);
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
				y_vectors.positive[y_chunk] = PointerGridChunk<T>::newChunk();
			}
			y_vectors.positive[y_chunk]->set((-x) % X_CHUNK_SIZE, y % Y_CHUNK_SIZE, val);

			if (y_vectors.positive[y_chunk]->count == 0) { // Release if needed
				PointerGridChunk<T>::deleteChunk(y_vectors.positive[y_chunk]);
				y_vectors.positive[y_chunk] = NULL;
			}
		}
		else { // Negative y
			y_chunk = -y_chunk - 1;

			if (y_chunk >= y_vectors.negative.size()) { // Resize y if necessary
				y_vectors.negative.resize(y_chunk + 1, NULL);
			}

			if (y_vectors.negative[y_chunk] == NULL) {
				y_vectors.negative[y_chunk] = PointerGridChunk<T>::newChunk();
			}
			y_vectors.negative[y_chunk]->set((-x) % X_CHUNK_SIZE, (-y) % Y_CHUNK_SIZE, val);

			if (y_vectors.negative[y_chunk]->count == 0) { // Release if needed
				PointerGridChunk<T>::deleteChunk(y_vectors.negative[y_chunk]);
				y_vectors.negative[y_chunk] = NULL;
			}
		}

	}
}

template <class T>
T PointerGrid2d<T>::get(int x, int y)
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

			PointerGridChunk<T> *chunk = y_vectors.positive[y_chunk];
			if (chunk == NULL)
				return NULL;

			return chunk->get(x % X_CHUNK_SIZE, y % Y_CHUNK_SIZE);
		}
		else { // Negative y
			y_chunk = -y_chunk - 1;

			if (y_chunk >= y_vectors.negative.size()) { // y out of range
				return NULL;
			}

			PointerGridChunk<T> *chunk = y_vectors.negative[y_chunk];
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

			PointerGridChunk<T> *chunk = y_vectors.positive[y_chunk];
			if (chunk == NULL)
				return NULL;

			return chunk->get((-x) % X_CHUNK_SIZE, y % Y_CHUNK_SIZE);
		}
		else { // Negative y
			y_chunk = -y_chunk - 1;

			if (y_chunk >= y_vectors.negative.size()) { // y out of range
				return NULL;
			}

			PointerGridChunk<T> *chunk = y_vectors.negative[y_chunk];
			if (chunk == NULL)
				return NULL;

			return chunk->get((-x) % X_CHUNK_SIZE, (-y) % Y_CHUNK_SIZE);
		}
	}
}

