#include <iostream>
#include <ctime>
#include <omp.h>



void mult(int** matrix, int* vector, int* res, int N, int M, int parall) {

	double start_paral;
	double end_paral;
	
	if (parall == 1) {
		//by rows
		for (int p = 1; p < 20; p++) {

			omp_set_num_threads(p);
			start_paral = omp_get_wtime();
				
			#pragma omp parallel for 
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					res[i] += matrix[i][j] * vector[j];
				}
					
			}
			end_paral = omp_get_wtime();
			std::cout /*<< "Thread used: " << p << ", time on rows spent:"*/ << (end_paral - start_paral) << std::endl;
		}
		for (int i = 0; i < N; i++)
			res[i] = 0;
	}


	if (parall == 2) {
		//by columns
		for (int p = 1; p < 20; p++) {
			omp_set_num_threads(p);
			start_paral = omp_get_wtime();

			#pragma omp parallel for
			for (int j = 0; j < M; j++) {
				for (int i = 0; i < N; i++) {
					res[i] += matrix[i][j] * vector[j];
				}
			}
			end_paral = omp_get_wtime();
			std::cout /*<< "Thread used: " << p << ", time on columns spent:"*/ << (end_paral - start_paral) << std::endl;
		}
		for (int i = 0; i < N; i++)
			res[i] = 0;
	}


	if (parall == 3) {
		//by blocks 
		for (int p = 1; p < 20; p++) {
			omp_set_num_threads(p);
			start_paral = omp_get_wtime();

			int s = omp_get_num_threads();
			int q = s;
			int block_height = N / s;
			int block_width = M / q;

			#pragma omp parallel for 
			for (int block_count = 0; block_count < s*q; block_count++)
			{
				int i = block_count / q;
				int j = block_count % q;
				for (int k = i*block_height; k < (i + 1) * block_height; k++)
					for (int l = j*block_width; l < (j + 1) * block_width; l++)
						res[k] += matrix[k][l] * vector[l];
			}

			end_paral = omp_get_wtime();
			std::cout /*<< "Thread used: " << p << ", time on block spent:"*/ << (end_paral - start_paral) << std::endl;
		}
		
}
	

}

int main() {
	int N ;
	int M ;

	
	for (int i = 1; i <= 5; i++) {
		std::cout << "Input N, M:  " << std::endl;
		//std::cin >> N >> M;
		std::cout << std::endl;
		N = 1000 * i;
		M = 1500 * i;
		double start_gen = clock();

		int** matrix = new int*[N];
		for (int i = 0; i < N; i++) {
			matrix[i] = new int[M];
			for (int j = 0; j < M; j++) {
				matrix[i][j] = rand() % 1000;
			}
		}

		int * vect = new int[M];
		for (int i = 0; i < M; i++) {
			vect[i] = rand() % 1000;
		}

		int* res = new int[N];
		for (int i = 0; i < N; i++) {
			res[i] = 0;
		}

		double end_gen = clock();
		std::cout << "Generation time: " << (end_gen - start_gen) / CLK_TCK << std::endl;
		
		std::cout << std:: endl << "N= " << N << " M= " << M << std::endl;
		mult(matrix, vect, res, N, M, 1);//By rows 
		std::cout << std::endl;
		mult(matrix, vect, res, N, M, 2);//By columns
		std::cout << std::endl;
		mult(matrix, vect, res, N, M, 3);//By blocks
	}

	system("pause");
	return 0;
}