#include <iostream>
#include <omp.h>
#include <ctime>

const int N = 1100;
const int M = 990;
const int K = 1000;

void mult_martices(int** matrix1, int** matrix2,int** res_matrix, int M, int N, int K) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < K; j++) {
			for (int t = 0; t < M; t++)
			{
				res_matrix[i][j] += matrix1[i][t] * matrix2[t][j];
			}
		}

	}
}

void mult_martices(int** matrix1, int** matrix2, int** res_matrix, int M, int N, int K, bool parallel) {
	omp_set_num_threads(4);
  
	for (int i = 0; i < N; i++) { 
#pragma omp parallel for
		for (int j = 0; j < K; j++) {
			for (int t = 0; t < M; t++)
			{
				res_matrix[i][j] += matrix1[i][t] * matrix2[t][j];
			}
		}

	}
}

int main(){

	double start_gen = clock();
	int** matrix1 = new int*[N];
	for (int i = 0; i < N; i++)
	{
		matrix1[i] = new int[M];
		for (int j = 0; j < M; j++)
		{
			matrix1[i][j] = rand() % 100;
		}
	}

	int** matrix2 = new int*[M];
	for (int i = 0; i < M; i++)
	{
		matrix2[i] = new int[K];
		for (int j = 0; j < K; j++)
		{
			matrix2[i][j] = rand() % 100;
		}
	}

	int** matrix_res = new int*[N];
	for (int i = 0; i < N; i++)
	{
		matrix_res[i] = new int[K];
		for (int j = 0; j < K; j++)
		{
			matrix_res[i][j] = 0;
		}
	}
	double end_gen = clock();
	std::cout << "Generation time: " << (end_gen - start_gen) / CLK_TCK << std::endl;

	double start_common = clock();
	mult_martices(matrix1, matrix2, matrix_res, M, N, K);
	double end_common = clock();
	std::cout << "One thread mult time: " << (end_common - start_common) / CLK_TCK << std::endl;

	double start_paral = omp_get_wtime();
	mult_martices(matrix1, matrix2, matrix_res, M, N, K, true);
	double end_paral = omp_get_wtime();
	std::cout << "Four thread mult time: " << (end_paral - start_paral) << std::endl;

	system("pause");
	return 0;
}
