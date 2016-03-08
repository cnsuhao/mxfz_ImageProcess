#pragma once
#include <math.h>
#define PI_2 6.2831853071796 
class Complex
{
public:
	Complex(void);
	Complex(double r,double i);
	~Complex(void);
	double real;
	double image;
	Complex operator+(Complex &add);
	void operator+=(Complex &add);
	Complex operator+(double add);
	Complex operator-(Complex &sub);
	void operator-=(Complex &sub);
	Complex operator-(double sub);
	Complex operator*(Complex &mul);
	void operator*=(Complex &mul);
	Complex operator*(double mul);
	Complex operator/(double div);
	void operator=(Complex &equ);
	void operator=(double equ);
	double GetAmplitude();
	double GetAngle();
};

