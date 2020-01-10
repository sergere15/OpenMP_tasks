#include <iostream>
#include <omp.h>
#include <ctime>

void mult_martices(int** matrix1, int** matrix2, int** res_matrix, int M, int N, int K, int parall) {

	double start;
	double end;

	if (parall == 0) {
		start = omp_get_wtime();
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < K; j++) {
				for (int t = 0; t < M; t++)
				{
					res_matrix[i][j] += matrix1[i][t] * matrix2[t][j];
				}
			}
		}
		end = omp_get_wtime();
		std::cout << "Without threads,  time on common:" << (end - start) << std::endl;
	}

	if (parall == 1) {
		for (int p = 1; p < 20; p++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < K; j++)
					res_matrix[i][j] = 0;
			}

			omp_set_num_threads(p);
			start = omp_get_wtime();
			for (int i = 0; i < N; i++) {
#pragma omp parallel for
				for (int j = 0; j < K; j++) {
					for (int t = 0; t < M; t++)
					{
						res_matrix[i][j] += matrix1[i][t] * matrix2[t][j];
					}
				}
			}
			end = omp_get_wtime();
			std::cout << "Thread used: " << p << ",  time on tape:" << (end - start) << std::endl;
		}
	}

	

	if (parall == 2) {
		//by blocks 
		for (int p = 1; p < 20; p++) {

			omp_set_num_threads(p);
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < K; j++)
					res_matrix[i][j] = 0;
			}

			start = omp_get_wtime();

#pragma omp parallel
			{
				int p = omp_get_num_threads();
				int q = p;
				int r = p;
				int a_block_height = N / p;
				int a_block_width = M / q;
				int b_block_heigth = M / q;
				int b_block_width = K / r;
#pragma omp for
				for (int a_block_count = 0; a_block_count < p * q; a_block_count++)
				{
					int i_block_index = a_block_count / q;
					int j_block_index = a_block_count % q;
					for (int t_block_index = 0; t_block_index < r; t_block_index++)
					{
						for (int i = i_block_index * a_block_height; i < (i_block_index + 1) * a_block_height; i++)
							for (int j = j_block_index * a_block_width; j < (j_block_index + 1) * a_block_width; j++)
								for (int t = t_block_index * b_block_width; t < (t_block_index + 1) * b_block_width; t++)
									res_matrix[i][t] += matrix1[i][j] * matrix2[j][t];
					}
				}
			}

			end = omp_get_wtime();
			std::cout << "Thread used: " << p << ",  time on block spent:" << (end - start) << std::endl;
		}
	}

}

int main() {
	int N;
	int M;
	int K;


	for (int p = 1; p <= 5; p++) {
		//std::cout << "Input N, M:  " << std::endl;
		//std::cin >> N >> M;
		std::cout << std::endl;
		N = 100 * p;
		M = 150 * p;
		K = 120 * p;
		double start_gen = clock();

		int** matrix1 = new int*[N];
		for (int i = 0; i < N; i++) {
			matrix1[i] = new int[M];
			for (int j = 0; j < M; j++) {
				matrix1[i][j] = rand() % 1000;
			}
		}

		int** matrix2 = new int*[M];
		for (int i = 0; i < M; i++) {
			matrix2[i] = new int[K];
			for (int j = 0; j < K; j++) {
				matrix2[i][j] = rand() % 1000;
			}
		}

		int** res = new int*[N];
		for (int i = 0; i < N; i++) {
			res[i] = new int[K];
			for (int j = 0; j < K; j++)

			res[i][j] = 0;
		}

		double end_gen = clock();

		std::cout << std::endl << "N= " << N << " M= " << M << " K= " << K << std::endl;
		std::cout << "Generation time: " << (end_gen - start_gen) / CLK_TCK << std::endl;

		mult_martices(matrix1, matrix2, res, M, N, K, 0);//Common
		std::cout << std::endl;
		mult_martices(matrix1, matrix2, res, M, N, K, 1);//By Tape 
		std::cout << std::endl;
		mult_martices(matrix1, matrix2, res, M, N, K, 2);//By blocks 
		
	}

	system("pause");
	return 0;
}