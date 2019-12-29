
#include "stdafx.h"
#include <stdio.h>
#include "iostream"
#include "omp.h"
#include "ctime"

#include <dos.h>

const int N = 12000;
const int M = 12900;



int find_min_el_str(int* str,int const M) {
	int min = str[0];
	for (int i = 0; i < M; i++) {
		if (str[i] < min) {
			min = str[i];
		}
	}
	return min;
}

int find_max_of_min(int** mas, const int N, const int M, bool parallel) {
	int* minimums = new int [N];
	omp_set_num_threads(4);
	#pragma omp parallel for
		for (int i = 0; i < N; i++) {
			minimums[i] = find_min_el_str(mas[i], M);
		}

	int max = minimums[0];

	#pragma omp parallel for
	for (int i = 0; i < N; i++) {
		if (minimums[i] > max)
			max = minimums[i];
		}
	return max;
	
}

int find_max_of_min(int** mas, const int N, const int M) {
	int* minimums = new int[N];

	for (int i = 0; i < N; i++) {
		minimums[i] = find_min_el_str(mas[i], M);
	}
	int max = minimums[0];
	for (int i = 0; i < N; i++) {
		if (minimums[i] > max)
			max = minimums[i];
	}
	return max;

}

int main(){
	double start_gen = clock();
	int** mas = new int*[N];
	for (int i = 0; i < N; i++)
	{
		mas[i] = new int[M];
		for (int j = 0; j < M; j++) {
			mas[i][j] = std::rand() % 10000;
		}
	}

	double end_gen = clock();

	std::cout << "Generation time:" << (end_gen - start_gen) / CLK_TCK << std::endl;

	double start_common = clock();

	int max = find_max_of_min(mas, N, M);

	double end_common = clock();

	std::cout << "Common time:" << (end_common - start_common) / CLK_TCK << " result: " << max << std::endl;

	double start = omp_get_wtime();

	int maxx = find_max_of_min(mas, N, M, true);
	
	double end = omp_get_wtime();


	std::cout << "Paralell time:" << (end - start)  << " result: " << maxx << std::endl;

	
	system("pause");
	return 0;
}

