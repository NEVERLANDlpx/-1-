#include<iostream>
#include<cstdlib>
#include<ctime>
#include<chrono>
#include <arm_neon.h>
#define N 200
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

void NEON_LU(int n, float a[][N]) {
	float32x4_t temp;
	float32x4_t div, t1, t2, sub;
	for (int i = 0; i < n - 1; ++i) {
		for (int j = i + 1; j < n; ++j) {
			// ��temp�ݴ����ı���
			float b[1];
			b[0] = a[j][i] / a[i][i];
			temp = vld1q_f32(b);
			//temp=vdivq_f32(a[j]+i,a[i]+i);

			//ÿ�ĸ�һ����м��㣬˼��ʹ�������
			int k = n - 3;
			for (; k >= i + 1; k -= 4) {
				t1 = vld1q_f32(a[i] + k);
				t2 = vld1q_f32(a[j] + k);
				sub = vsubq_f32(t2, vmulq_f32(t1, temp));
				vst1q_f32(a[j] + k, sub);
			}
			//����ʣ�ಿ��
			for (k += 3; k >= i + 1; --k) {
				a[j][k] -= a[i][k] * b[0];
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
		auto t1 = Clock::now();
		NEON_LU(N, a);
		auto t2 = Clock::now();
		sum2 += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count() / 1e+6;
	}
	cout << sum2 / 100.0 << endl;
}

