

/*
 * @Description: 识别子函数
 * @Date: 2020-08-24 13:09:03
 * @LastEditors: JiangYe
 * @LastEditTime: 2020-09-16 12:23:42
 */
#include "RECON.h"

RECON::RECON()
{
    recon_counter = 0;
}

int RECON::recon(float s1[5000], int N)
{
	float* in;
	fftwf_complex* out;
	fftwf_plan fft;
	int signal_exist;
	float amp[2500];
	float sum = 0, num = 0;
	int epsilon[2500];

	in = (float*)fftwf_malloc(sizeof(float) * N);
	out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * N);


	for (int index4 = 0; index4 < N; index4++)
	{
		in[index4] = s1[index4];
	}

	fft = fftwf_plan_dft_r2c_1d(N, in, out, FFTW_ESTIMATE);//设置FFT变换方案；
	fftwf_execute(fft); /*变换*/


	for (int index5 = 0; index5 < N / 2 - 1; index5++)
	{
		amp[index5] = (float)(sqrt(out[index5][0] * out[index5][0] + out[index5][1] * out[index5][1]));
		sum += amp[index5];
//        qDebug() << out[index5][0];
	}

	sum = sum / 2500;//计算均值；

	for (int index6 = 0; index6 < N / 2 - 1; index6++)
	{
		epsilon[index6] = (amp[index6] > 3.5 * sum) ? 1 : 0;
	}


	for (int index7 = 0; index7 < N / 2 - 1; index7++)
	{
		num += epsilon[index7];
	}

    if ((signal_exist = (num > 15) ? 1 : 0) == 1)
    {
        this->recon_counter ++;
    }
    if(this->recon_counter >= 5)
    {
        this->recon_counter = 0;
        signal_exist = 1;
    } else {
        signal_exist = 0;
    }

	num = 0;

	fftwf_destroy_plan(fft);
	fftwf_free(in);
	fftwf_free(out);

	return signal_exist;
}
