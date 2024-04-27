#include<iostream>
#include<cstdlib>
#include<ctime>
#include<chrono>
#include <xmmintrin.h> 
#include <emmintrin.h> 
#include <pmmintrin.h> 
#include <tmmintrin.h> 
#include <smmintrin.h> 
#include <nmmintrin.h> 
#include <immintrin.h> 
#define N 500
using namespace std;
using namespace chrono;
float x[N], a[N][N];
int i, j, k;

void initialize(int n, float x[], float a[][N])
{
	for (i = 0; i < n; i++)
	{
		x[i] = rand() % 1000 / 100.0;
		for (j = 0; j < n; j++)
			a[j][i] = rand() % 10000 / 100.0;
	}
}

void LU(int n, float a[][N]) {
	//������һ�е���ֵ������Ԫ
	for (i = 0; i < n - 1; ++i) {
		//����һ�������У���ǰi������Ϊ0
		for (j = i + 1; j < n; ++j) {
			//�������
			float temp = a[j][i] / a[i][i];

			//������һ�е�����ֵ����i�������ֵ���μ�ȥ���Ӧ��ֵ���Ա���
			for (k = i + 1; k < n; ++k) {
				a[j][k] -= a[i][k] * temp;
			}
			//��i��Ϊ0
			a[j][i] = 0.00;
		}
	}
}


void SSE_LU(int n, float a[][N]) { //����
	float temp;
	__m128 div, t1, t2, sub;
	for (int i = 0; i < n - 1; ++i) {
		for (int j = i + 1; j < n; ++j) {
			// ��temp�ݴ����ı���
			temp = a[j][i] / a[i][i];
			// divȫ�����ڴ洢temp������������
			div = _mm_set1_ps(temp);

			k = i + 1;
			if ((i + 1) % 4 != 0)
			{
				for (k = i + 1; k < (i + 5) / 4 * 4; k++)
				{
					a[j][k] -= a[i][k] * temp;
				}
			}
			for (; k + 4 <= n; k += 4) {
				t1 = _mm_load_ps(a[i] + k);
				t2 = _mm_load_ps(a[j] + k);
				sub = _mm_sub_ps(t2, _mm_mul_ps(t1, div));
				_mm_store_ss(a[j] + k, sub);
			}
			//����ʣ�ಿ��
			for (; k < n; k++) {
				a[j][k] -= a[i][k] * temp;
			}
			a[j][i] = 0.00;
		}
	}
}



int main()
{
	initialize(N, x, a);
	typedef std::chrono::high_resolution_clock Clock;
	float sum1 = 0, sum2 = 0, sum3 = 0;
	for (int c = 0; c < 100; c++)
	{
		initialize(N, x, a);
		auto t1 = Clock::now();
		LU(N, a);
		auto t2 = Clock::now();
		sum1 += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1e+6;
	}
	cout << sum1 / 100.0 << endl;

	for (int c = 0; c < 100; c++)
	{
		initialize(N, x, a);
		auto t5 = Clock::now();
		SSE_LU(N, a);
		auto t6 = Clock::now();
		sum3 += std::chrono::duration_cast<std::chrono::nanoseconds>(t6 - t5).count() / 1e+6;
	}
	cout << sum3 / 100.0 << endl;
}
