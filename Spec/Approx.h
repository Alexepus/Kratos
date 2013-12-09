#ifndef _APPROX_H_
#define _APPROX_H_
void ApproxLine(double x[], double y[], int N, double &a, double &b);
void ApproxLineMean(double x[], double y[], int N, double &a, double &b);
void ApproxParabola(double x[], double y[], int N, double &a, double &b, double &c);
void ApproxParabolaMean(double x[], double y[], int N, double &a, double &b, double &c);
double FindXApproxLine(double x[], double y[], int N, double Y0);
double FindExtremumApproxParabola(double x[], double y[], int N);
double YApproxLine(double x[], double y[], int N, double X0);
double YApproxParabola(double x[], double y[], int N, double X0);

#endif
