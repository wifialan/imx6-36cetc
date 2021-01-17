#include <iostream>
#include "hs.h"

HS::HS()
{
    this->freq = 150;
}

void HS::clear()
{
    if (over == 1)
    {
        Peakvalue = 0;
        signal_start = 0;
        signal_stop = 0;
        over = 0;
        lfm_sp = 0;
        status = 0;
    }
    else if (over == 2)//over表示一次整体判断结束，将全局变量复位；
    {
        Peakvalue = 0;
        lfm_sp = 0;
        signal_start = 0;
        signal_stop = 0;
        over = 0;
        status = 0;
    }
}

int HS::recon(float *sig_input,int data_n)
{
    int Peakloc1 = 0, Peakn = 0, otherPeak = 0, Peakloc2 = 0, Peakloc3 = 0, Peakloc = 0;
    float *sigx_TF,xcorr_mean = 0;

    int signal_exist,signal_type = 0;

        FFT(sig_input);
        sigx_TF = sig_input;
        signal_exist = SIG_EX(sigx_TF);

        if (signal_exist == 1 && status == 0)//第一次检测到信号
        {
            int Peakvalue1,Peakvalue2,Peakvalue3;
            signal_start = data_segment;//定位第一帧信号数据
            xCORR(sigx_TF);
            IFFT(scorr);
            IFFT(scorr_X);
            IFFT(scorr_Y);
            Peakloc1 = Peakfind(scorr);
            Peakloc2 = Peakfind(scorr_X);
            Peakloc3 = Peakfind(scorr_Y);
            Peakvalue1 = scorr[Peakloc1];
            Peakvalue2 = scorr_X[Peakloc2];
            Peakvalue3 = scorr_Y[Peakloc3];
            if (Peakvalue1 > Peakvalue2 && Peakvalue1 > Peakvalue3)
            {
                swith_lfm = 100;
                Peakvalue = scorr[Peakloc1];
                Peakloc = Peakloc1;
            }
            else if(Peakvalue2 > Peakvalue1 && Peakvalue2 > Peakvalue3)
            {
                swith_lfm = 50;
                Peakvalue = scorr_X[Peakloc2];
                Peakloc = Peakloc2;
            }
            else if (Peakvalue3 > Peakvalue1 && Peakvalue3 > Peakvalue2)
            {
                swith_lfm = 200;
                Peakvalue = scorr_Y[Peakloc3];
                Peakloc = Peakloc3;
            }
            Peak_start = Peakloc;
            //flag_start = data_n;
            skip_t = (Peakloc + swith_lfm * freq) / (NL / 2);
            status = 1;
            cw_sp = 1;
        }
        else if (status == 1)//第一帧之后根据相关设置跳帧
        {
            skip_t -= 1;
            if(skip_t  == 0)
            {
                status = 2;
            }
        }

        else if (signal_exist == 0 && status == 2)//跳帧结束，且未检测到信号；
        {
            if (swith_lfm == 50)//信号脉宽50ms
            {
                lfm_sp = 2;
                status = 0;
            }
            if (swith_lfm == 100 && lfm_sp == 0)//信号脉宽100ms
            {
                lfm_sp = 1;
                skip_t = 2;
                status = 1;
            }
            else if (swith_lfm == 100 && lfm_sp == 1)//信号脉宽100ms
            {
                lfm_sp = 2;
                status = 3;
            }
            else if (swith_lfm == 200)//信号脉宽100ms
            {
                lfm_sp = 2;
                status = 3;
            }
        }
        else if (signal_exist == 1 && status == 2)//跳帧结束，且判断到信号；
        {
            status = 3; lfm_sp = 0;
        }
        else if (signal_exist == 0 && status == 3 && lfm_sp == 2)
        {
            over = 2; status = 4;signal_stop = data_segment - 5;
        }
        else if(signal_exist == 0 && status == 3 && lfm_sp == 0)
        {
            signal_stop = data_segment - 1;over = 1; status = 5;
        }

    return status;
}

int HS::SIG_EX(float* s1)
{
    int signal_exist = 0;
    float amp[NL / 2] = { 0 };
    float sum = 0, num = 0;
    int epsilon, fft_type = 0;

    for (int index = 0; index < NL / 2; index++)
    {
        amp[index] = (float)(sqrt(s1[2 * index] * s1[2 * index] + s1[2 * index + 1] * s1[2 * index + 1]));
        //amp[index] = 2 * amp[index] / NL;
        //printf("%f\n",amp[index]);
        sum += amp[index];
    }
    sum = sum / (NL/2);//计算均值；

    for (int index = 0; index < NL / 2; index++)
    {
        epsilon = (amp[index] > 10 * sum) ? 1 : 0;
        num += epsilon;
    }

    signal_exist = (num > 15) ? 1 : 0;

    return signal_exist;
}

int HS::Peakfind(float* ptrData)
{
    int Peakloc = 0;
    float Peakvalue1 = 0;
    for (int index = 0; index < NL;index++)
    {
        if (ptrData[index]> Peakvalue1)
        {
            Peakvalue1 = ptrData[index];
            Peakloc = index;
        }
    }
    Peakvalue = Peakvalue1;
    int bigger_index = 0;
    Peakvalue1 = Peakvalue1 / 3;
    for (int index = 0; index < NL; index++)
    {
        if (Peakvalue1 < ptrData[index])
        {
            bigger_index++;
        }
    }
    if (bigger_index <= NL/2 && bigger_index > 10) //大于3的设定是为了防止数组中均为0或某一个数持续出现的情况
    {
        return Peakloc;
    }
    else
    {
        return 0;
    }
}

void HS::xCORR(float *fft_sigin1)
{

    for (int index = 0; index < NL; index++)
    {
        float a = 0, b = 0, c1, d1, c2, d2,c3,d3;
        if (index < NL / 2)
        {
            sfft[2 * index] = fft_sigin1[2 * index];
            sfft[2 * index + 1] = fft_sigin1[2 * index + 1];
            //printf("%f %f \n", a, b);
        }
        else
        {
            sfft[2 * index] = fft_sigin1[2 * NL - 2*index - 2];
            sfft[2 * index + 1] = fft_sigin1[2 * NL - 2*index - 1];
            //printf("%f %f \n", a, b);
        }
        a = sfft[2 * index];
        b = sfft[2 * index + 1];


        c1 = real_LFM1_FFT[index];
        d1 = imag_LFM1_FFT[index];
        c2 = real_LFM2_FFT[index];
        d2 = imag_LFM2_FFT[index];
        c3 = real_LFM3_FFT[index];
        d3 = imag_LFM3_FFT[index];

        scorr_X[2 * index] = a * c1 + b * d1;
        scorr_X[2 * index + 1] = b * c1 - a * d1;
        scorr[2 * index] = a * c2 + b * d2;
        scorr[2 * index + 1] = b * c2 - a * d2;
        scorr_Y[2 * index] = a * c3 + b * d3;
        scorr_Y[2 * index + 1] = b * c3 - a * d3;
        //printf("%f %f \n", scorr[2 * index], scorr[2 * index + 1]);
    }


}

void HS::IFFT(float *ifft_s1)
{
    float* out;
    fftwf_complex* in;
    fftwf_plan ifft;

    out = (float*)fftwf_malloc(sizeof(float)* 2 * NL);
    in = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * NL);


    for (int index = 0; index < NL; index++)
    {
        in[index][0] = ifft_s1[index*2];
        in[index][1] = ifft_s1[index*2 + 1];
    }


    ifft = fftwf_plan_dft_c2r_1d(NL, in, out, FFTW_ESTIMATE);//设置FFT变换方案；
    fftwf_execute(ifft); /*变换*/


    for (int index = 0; index < NL; index++)
    {
        if (out[index] < 0)
        {
            ifft_s1[index] = -1 * out[index] / NL;
        }
        else
        {
            ifft_s1[index] = out[index] / NL;
        }
        //printf("%f\n", ifft_s1[index]);
    }

    //printf("\n");
    fftwf_destroy_plan(ifft);
    fftwf_free(in);
    fftwf_free(out);
}

void HS::FFT(float * fft_s1)
{
    float* in;
    fftwf_complex* out;
    fftwf_plan fft;
    in = (float*)fftwf_malloc(sizeof(float) * NL);
    out = (fftwf_complex*)fftwf_malloc(sizeof(fftwf_complex) * NL);


    for (int index = 0; index < NL; index++)
    {
        in[index] = fft_s1[index];
    }

    fft = fftwf_plan_dft_r2c_1d(NL, in, out, FFTW_ESTIMATE);//设置FFT变换方案；
    fftwf_execute(fft); /*变换*/

    out[0][0] = out[512][0];//频谱修正；

    for (int index = 0; index < NL/2; index++)
    {
        //printf("%f  %f\n", out[index][0], out[index][1]);
        fft_s1[2*index] = out[index][0];
        fft_s1[2*index + 1] = out[index][1];
    }

    /*for (int index = 0; index < NL; index++)
    {
        printf("%f\n", s1[index]);
    }*/

    fftwf_destroy_plan(fft);
    fftwf_free(in);
    fftwf_free(out);

}
