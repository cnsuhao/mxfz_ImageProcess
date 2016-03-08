#include "stdafx.h"
#include "Complex.h"


Complex::Complex(void)
{
	real=0;
	image=0;
}

Complex::Complex(double r,double i)
{
	real=r;
	image=i;
}

Complex::~Complex(void)
{
}

Complex Complex::operator+(Complex &add)
{
	return Complex(real+add.real,image+add.image);
}

Complex Complex::operator+(double add)
{
	return Complex(real+add,image);
}

Complex Complex::operator-(Complex &sub)
{
	return Complex(real-sub.real,image-sub.image);
}

Complex Complex::operator-(double sub)
{
	return Complex(real-sub,image);
}

Complex Complex::operator*(Complex &mul)
{
	return Complex(real*mul.real-image*mul.image,real*mul.image+image*mul.real);
}

Complex Complex::operator*(double mul)
{
	return Complex(real*mul,image*mul);
}

Complex Complex::operator/(double div)
{
	return Complex(real/div,image/div);
}

void Complex::operator=(Complex &equ)
{
	real=equ.real;
	image=equ.image;
}

void Complex::operator=(double equ)
{
	real=equ;
	image=0;
}

void Complex::operator+=(Complex &add)
{
	real+=add.real;
	image+=add.image;
}

void Complex::operator-=(Complex &sub)
{
	real-=sub.real;
	image-=sub.image;
}

void Complex::operator*=(Complex &mul)
{
	double r=real*mul.real-image*mul.image;
	double i=real*mul.image+image*mul.real;
	real=r;
	image=i;
}

double Complex::GetAmplitude()
{
	return sqrt(real*real+image*image);
}

double Complex::GetAngle()
{
	return atan(image/real);
}