#include <iostream>
#include <omp.h>

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
		std::cout << "Thread used: " << p << ",  time on common:" << (end - start) << std::endl;
	}

	if (parall == 1) {
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
		std::cout << "Thread used: " << p << ",  time on lenta1:" << (end - start) << std::endl;
	}

	if (parall == 2) {
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

	if (parall == 3) {
		//by blocks 
		for (int p = 1; p < 20; p++) {
			omp_set_num_threads(p);
			start = omp_get_wtime();

#pragma omp parallel num_threads(threads_num)
			{
				int p = omp_get_num_threads();
				int q = p;
				int r = p;
				int a_block_height = M / p;
				int a_block_width = N / q;
				int b_block_heigth = N / q;
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


	system("pause");
	return 0;
}