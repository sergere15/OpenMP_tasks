#include <iostream>
#include <ctime>
#include <omp.h>
#include <string>


void gen_random(char* s, const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = 0;
}

int find_substr(std::string& str, std::string& sub_str, int* vec, bool parall) {
	if (!parall) {
		for (int i = 0; i < str.length() - sub_str.length() + 1; i++) {
			vec[i] = 0;
			for (int j = 0; j < sub_str.length(); j++) {
				if (str[i + j] == sub_str[j]) {
					vec[i]++;
				}
				else {
					break;
				}
			}
		}

		for (int i = 0; i < str.length() - sub_str.length() + 1; i++) {
			if (vec[i] == sub_str.length()) return i;
		}
		return -1;
	}
	else {
		omp_set_num_threads(4);
		#pragma omp parallel for
		for (int i = 0; i < str.length() - sub_str.length() + 1; i++) {
			vec[i] = 0;
			for (int j = 0; j < sub_str.length(); j++) {
				if (str[i + j] == sub_str[j]) {
					vec[i]++;
				}
				else {
					break;
				}
			}
		}
		
		#pragma omp for
		for (int i = 0; i < str.length() - sub_str.length() + 1; i++) {
			if (vec[i] == sub_str.length()) return i;
		}
		return -1;
	}
	return -1;
}

int main() {
	std::string str;
	std::string sub_str;
	int len1 = 10000000;
	int len2 = 2000;
	//std::cin >> str >> sub_str;
	double start_gen = clock();
	char* str1 = new char[len1];
	char* str2 = new char[len2];
	gen_random(str1, len1);
	gen_random(str2, len2);
	str = str1;
	sub_str = str2;
	int* vec = new int[str.length()-sub_str.length() + 1];
	double end_gen = clock();
	std::cout << "Generation time: " << (end_gen - start_gen) / CLK_TCK << std::endl;

	int pos;

	double start_com = omp_get_wtime();
	pos = find_substr(str, sub_str, vec, false);
	double end_com = omp_get_wtime();

	if (pos != -1) std::cout << "it is exist, starting on " << pos + 1 << " position" << std::endl;
	std::cout << "Used no threads, time spent: " << (end_com - start_com) << std::endl;


	double start_par = omp_get_wtime();
	pos = find_substr(str, sub_str, vec, true);
	double end_par = omp_get_wtime();

	if (pos != -1) std::cout << "it is exist, starting on " << pos + 1 << " position" << std::endl;
	std::cout << "Used four threads, time spent: " << (end_par - start_par) << std::endl;
	system("pause");
	return 0;
}