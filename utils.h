//
// Created by godknows on 11.05.2020.
//

#ifndef ILYUHACPPVERSION_UTILS_H
#define ILYUHACPPVERSION_UTILS_H
#include <cmath>

const double G = 9.80665;

struct MData
{
    double alpha1;
    double alpha2;
    double p1;
    double p2;
};

typedef struct MData* mdref;

mdref addMData (mdref a, mdref b, mdref res);
mdref mulMData (mdref a, double b, mdref res);
mdref setMData (mdref from, mdref to);

class Mayatnik {
private:
    double l;
    double m1, m2, myu;
    double tau;
    double A1 (mdref Z);
    double A2 (mdref Z);

    double f1 (mdref Z);
    double f2 (mdref Z);
    double f3 (mdref Z);
    double f4 (mdref Z);

    mdref f (mdref Z, mdref res);
public:
    Mayatnik (double l, double m1, double m2, double tau=0.001);
    void setM1(double m);
    void setM2(double m);
    void setTau(double t);
    mdref next_Z (mdref Z, mdref next_z);
};


#endif //ILYUHACPPVERSION_UTILS_H
