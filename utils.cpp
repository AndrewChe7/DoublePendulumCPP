//
// Created by godknows on 11.05.2020.
//

#include "utils.h"

mdref addMData (mdref a, mdref b, mdref res) {
    res->alpha1 = a->alpha1 + b->alpha1;
    res->alpha2 = a->alpha2 + b->alpha2;
    res->p1 = a->p1 + b->p1;
    res->p2 = a->p2 + b->p2;
    return res;
}

mdref mulMData (mdref a, double b, mdref res) {
    res->alpha1 = a->alpha1 * b;
    res->alpha2 = a->alpha2 * b;
    res->p1 = a->p1 * b;
    res->p2 = a->p2 * b;
    return res;
}

mdref setMData (mdref from, mdref to) {
    to->alpha1 = from->alpha1;
    to->alpha2 = from->alpha2;
    to->p1 = from->p1;
    to->p2 = from->p2;
    return to;
}

Mayatnik::Mayatnik (double l, double m1, double m2, double tau) {
    this->l = l;
    this->m1 = m1;
    this->m2 = m2;
    this->tau = tau;
    this->myu = m2 / m1;
}

double Mayatnik::A1 (mdref Z) {
    double alpha1 = Z->alpha1;
    double alpha2 = Z->alpha2;
    double p1 = Z->p1;
    double p2 = Z->p2;
    return  (p1 * p2 * sin(alpha1 - alpha2)) /
            (this->m1 * this->l * this->l * (1 + this->myu * pow(sin(alpha1 - alpha2), 2)));
}

double Mayatnik::A2 (mdref Z) {
    double alpha1 = Z->alpha1;
    double alpha2 = Z->alpha2;
    double p1 = Z->p1;
    double p2 = Z->p2;
    return ((p1 * p1 * this->myu - 2 * p1 * p2 * this->myu * cos(alpha1 - alpha2) +
             p2 * p2 * (1 + this->myu)) * sin(2 * (alpha1-alpha2))) /
           (2 * this->m1 * this->l * this->l * pow(1 + this->myu * pow(sin(alpha1 - alpha2), 2), 2));
}

double Mayatnik::f1 (mdref Z) {
    double alpha1 = Z->alpha1;
    double alpha2 = Z->alpha2;
    double p1 = Z->p1;
    double p2 = Z->p2;
    return  (p1 - p2 * cos(alpha1 - alpha2)) /
            (this->m1 * this->l * this->l * (1 + this->myu * pow(sin (alpha1 - alpha2), 2)));
}

double Mayatnik::f2 (mdref Z) {
    double alpha1 = Z->alpha1;
    double alpha2 = Z->alpha2;
    double p1 = Z->p1;
    double p2 = Z->p2;
    return  (p2 * (1+this->myu) - p1 * this->myu * cos(alpha1 - alpha2)) /
            (this->m1* this->l*this->l * (1 + this->myu * pow(sin (alpha1 - alpha2), 2)));
}

double Mayatnik::f3 (mdref Z) {
    double alpha1 = Z->alpha1;
    double alpha2 = Z->alpha2;
    double p1 = Z->p1;
    double p2 = Z->p2;
    return -this->m1 * (1 + this->myu) * G * this->l * sin(alpha1) - this->A1(Z) + this->A2(Z);
}

double Mayatnik::f4 (mdref Z) {
    double alpha1 = Z->alpha1;
    double alpha2 = Z->alpha2;
    double p1 = Z->p1;
    double p2 = Z->p2;
    return  -this->m1 * this->myu * G * this->l * sin(alpha2) + this->A1(Z) - this->A2(Z);
}

mdref Mayatnik::f (mdref Z, mdref res) {
    res->alpha1 = this->f1(Z);
    res->alpha2 = this->f2(Z);
    res->p1 = this->f3(Z);
    res->p2 = this->f4(Z);
    return res;
}

mdref Mayatnik::next_Z (mdref Z, mdref next_z) {
    auto Y1 = new MData;
    auto Y2 = new MData;
    auto Y3 = new MData;
    auto Y4 = new MData;
    auto t  = new MData;
    auto t1 = new MData;
    auto t2 = new MData;
    auto t3 = new MData;

    this->f(Z, t);
    mulMData(t, this->tau, Y1);

    mulMData(Y1, 0.5, t);
    addMData(t, Z, t1);
    this->f(t1, t2);
    mulMData(t2, this->tau, Y2);

    mulMData(Y2, 0.5, t);
    addMData(t, Z, t1);
    this->f(t1, t2);
    mulMData(t2, this->tau, Y3);

    addMData(Y3, Z, t1);
    this->f(t1, t2);
    mulMData(t2, this->tau, Y4);

    addMData(Y1, Y4, t);
    mulMData(Y2, 2, t1);
    mulMData(Y3, 2, t2);
    addMData(t, t1, t3);
    addMData(t3, t2, t);
    mulMData(t, 0.166666666, t1);
    addMData(Z, t1, next_z);

    delete t;
    delete t1;
    delete t2;
    delete t3;
    delete Y1;
    delete Y2;
    delete Y3;
    delete Y4;

    return next_z;
}

void Mayatnik::setTau(double t) {
    this->tau = t;
}

void Mayatnik::setM1(double m) {
    this->m1 = m;
}

void Mayatnik::setM2(double m) {
    this->m2 = m;
}
