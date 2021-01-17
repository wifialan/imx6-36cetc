#ifndef RECON_H
#define RECON_H

#include "fftw3.h"
#include "math.h"
#include <QDebug>

class RECON
{
public:
    RECON();

    int recon(float s1[5000], int N);

    quint32 recon_counter;
};


#endif // RECON_H
