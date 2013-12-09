#include "stdafx.h"
#include "math.h"

void ApproxLine(double x[], double y[], int N, double &a, double &b)
{	//y=a+b*x
	//Для получения точности при малых (x[i]-x[j])/x_mean или (y[i]-y[j])/y_mean
	//a+b*(X1-X_mean)=Y1-Y_mean
	//a1+b*X1=Y1, где a=a1-Y_mean+b*X_mean

	double X=0, X2=0, Y=0, XY=0; 
	for(int i=0; i<N; i++)
	{
		double t=x[i];
		X+=t;
		X2+=t*t;
		Y+=y[i];
		XY+=t*y[i];
	}
	double Det0;
	Det0=N*X2-X*X;
	if(Det0==0)
	{
		a=0; b=0;
		return;
	}
	a=(Y*X2-XY*X)/Det0;
	b=(N*XY-X*Y)/Det0;
}

void ApproxLineMean(double x[], double y[], int N, double &a, double &b)
{
	double x_mean=0, y_mean=0, a1;
	double *X1, *Y1;
	for(int i=0; i<N; i++)
	{
		x_mean+=x[i];
		y_mean+=y[i];
	}
	x_mean=x_mean/N;
	y_mean=y_mean/N;
	X1=(double*)malloc(N*sizeof(double));
	Y1=(double*)malloc(N*sizeof(double));
	for(i=0; i<N; i++)
	{
		X1[i]=x[i]-x_mean;
		Y1[i]=y[i]-y_mean;
	}
	ApproxLine(X1, Y1, N, a1, b);
	a=a1+y_mean-b*x_mean;
	free(X1);
	free(Y1);
}

void ApproxParabola(double x[], double y[], int N, double &a, double &b, double &c)
{	//y=a+b*x+c*x^2
	double X=0, X2=0, X3=0, X4=0, Y=0, XY=0, X2Y=0; 
	for(int i=0; i<N; i++)
	{
		double t=x[i];
		double t2=t*t;
		double t3=t2*t;
		X+=t;
		X2+=t2;
		X3+=t3;
		X4+=t3*t;
		Y+=y[i];
		XY+=t*y[i];
		X2Y+=t2*y[i];
	}
	double Det0, Det11, Det12, Det13;
	Det11=X2*X4-X3*X3;
	Det12=X*X4-X2*X3;
	Det13=X*X3-X2*X2;
	Det0=N*Det11-X*Det12+X2*Det13;
	if(Det0==0)
	{
		a=0; b=0; c=0;
		return;
	}
	a=(Y*Det11-X*(XY*X4-X2Y*X3)+X2*(XY*X3-X2Y*X2))/Det0;
	b=(N*(XY*X4-X2Y*X3)-Y*Det12+X2*(X*X2Y-X2*XY))/Det0;
	c=(N*(X2*X2Y-X3*XY)-X*(X*X2Y-X2*XY)+Y*Det13)/Det0;
}

void ApproxParabolaMean(double x[], double y[], int N, double &a, double &b, double &c)
{
	double x_mean=0, y_mean=0, a1, b1;
	double *X1, *Y1;
	for(int i=0; i<N; i++)
	{
		x_mean+=x[i];
		y_mean+=y[i];
	}
	x_mean=x_mean/N;
	y_mean=y_mean/N;
	X1=(double*)malloc(N*sizeof(double));
	Y1=(double*)malloc(N*sizeof(double));
	for(i=0; i<N; i++)
	{
		X1[i]=x[i]-x_mean;
		Y1[i]=y[i]-y_mean;
	}
	ApproxParabola(X1, Y1, N, a1, b1, c);
	a=a1-b1*x_mean+c*x_mean*x_mean+y_mean;
	b=b1-2*c*x_mean;
	free(X1);
	free(Y1);
}

double FindXApproxLine(double x[], double y[], int N, double Y0)
{//y=a+b*x, find X0: y(X0)==Y0
	double a, b;
	ApproxLineMean(x, y, N, a, b);
	if(b==0)
		return HUGE_VAL;
	return (Y0-a)/b;
}

double FindExtremumApproxParabola(double x[], double y[], int N)
{//y=a+b*x+c*x^2, find extremum (X0: y'(X0)==0)
	double a, b, c;
	ApproxParabola(x, y, N, a, b, c);
	if(c==0) //Line equation
		return HUGE_VAL;
	return -b/(2.*c);
}

double YApproxLine(double x[], double y[], int N, double X0)
{//approximates by line and return Y=a+b*X0 
	double a, b;
	ApproxLine(x, y, N, a, b);
	return a+b*X0;
}
	
double YApproxParabola(double x[], double y[], int N, double X0)
{//approximates by parabola and return Y=a+b*X0+c*x^2 
	double a, b, c;
	ApproxParabola(x, y, N, a, b, c);
	return a+b*X0+c*X0*X0;
}
