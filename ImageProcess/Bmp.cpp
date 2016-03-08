#include "stdafx.h"
#include "Bmp.h"
#include <fstream>
using namespace std;

Bmp::Bmp(void): //构造函数
m_ColorTable(NULL),
m_FFTComplex(NULL),
m_BmpBuf(NULL),
m_RGB(NULL),
m_RGB_Bk(NULL),
m_Histogram(NULL),
m_Color(0x00),
m_Height(0x00),
m_Width(0x00),
m_LineBit(0x00)
{

}

Bmp::Bmp(char* FileName)//带有路径的构造函数
{
	m_Color=0;
	m_Height=0;
	m_Width=0;
	m_LineBit=0;
	Open(FileName);
}

Bmp::~Bmp(void)//析构函数
{
}

bool Bmp::Open(char* FileName)//打开图片
{
	if(Exist())
	{
		Close();
	}

	FILE *fp;//建立文件指针
	fopen_s(&fp,FileName,"rb");//以二进制方式读取文件
	if(!fp)
	{
		return false;//如果读取不了返回
	}

	fread(&m_BmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);//获取文件头，数据的大小也可以直接换成数字14
	fread(&m_BmpInfoHeader,sizeof(BITMAPINFOHEADER),1,fp);//获取信息头，数据的大小也可以直接换成数字40

	if(m_BmpInfoHeader.biBitCount==8)
	{
		m_ColorTable=new RGBQUAD[256];   
		fread(m_ColorTable,sizeof(RGBQUAD),256,fp); //如果是8位图像则把调色板也获取下来
	}

	m_Color=m_BmpInfoHeader.biBitCount/8;//计算颜色层数
	m_Height=m_BmpInfoHeader.biHeight;//获取图片大小
	m_Width=m_BmpInfoHeader.biWidth;
	m_LineBit=(m_BmpInfoHeader.biWidth*m_BmpInfoHeader.biBitCount/8+3)/4*4;//计算出图像一行真正的字节数（因为图像每行需要4的倍数的字节）

	m_BmpBuf=new unsigned char[m_Height*m_LineBit];//建立一个与图片的数组
	fread(m_BmpBuf,1,m_Height*m_LineBit,fp);//把数据读入数组
	fclose(fp);//关闭文件

	m_RGB=new unsigned char**[m_Color];//在内存建立一个m_Color层数的数组用以把数据以颜色不同来分开存放
	for(int i=0;i<m_Color;i++)
	{
		m_RGB[i]=new unsigned char*[m_Height];
	}
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			m_RGB[i][j]=new unsigned char[m_Width];
		}
	}

	m_RGB_Bk=new unsigned char**[m_Color];//在内存建立一个备份数组，在图像处理中用于还原图像
	for(int i=0;i<m_Color;i++)
	{
		m_RGB_Bk[i]=new unsigned char*[m_Height];
	}
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			m_RGB_Bk[i][j]=new unsigned char[m_Width];
		}
	}

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=m_BmpBuf[(m_Height-j-1)*m_LineBit+m_Color*k+m_Color-1-i];
				//将数组赋值，因为图像是由左往右，下往上的方式存放，24位图像顺序为蓝绿红，32位图像顺序为透蓝绿红，所以需要把数组位置与图像数据位置进行转换
			}
		}
	}

	return true;//读取成功，返回
}

bool Bmp::Save(char* FileName)//保存图片
{

	FILE *fp;
	fopen_s(&fp,FileName,"wb+");//与读取一样的方式，不过打开方式换为二进制写入，文件不存在则创建
	if(!fp)
	{
		return false;
	}

	fwrite(&m_BmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);//写入文件头，文件头可以在处理中进行修改
	fwrite(&m_BmpInfoHeader,sizeof(BITMAPINFOHEADER),1,fp);//写入信息头，信息头可以在处理中进行修改

	if(m_BmpInfoHeader.biBitCount==8)
	{
		fwrite(m_ColorTable,sizeof(RGBQUAD),256,fp);//如果是8位图则写入调色板
	}

	m_BmpBuf=new unsigned char[m_Height*m_LineBit];//建立一个图像大小的数组

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				m_BmpBuf[(m_Height-j-1)*m_LineBit+m_Color*k+m_Color-1-i]=*(*(*(m_RGB+i)+j)+k);
				//将以颜色分层的数组转换为图片存储方式的数组，方法与读取相反
			}
		}
	}

	fwrite(m_BmpBuf,m_Height*m_LineBit,1,fp);//将数组写入文件
	fclose(fp);//关闭文件
	return true;//成功保存，返回
}

bool Bmp::Exist()//返回图片是否存在
{
	return 0x00==m_Width?false:true;
}

long Bmp::GetWidth()//返回图片宽度
{
	return m_Width;
}

long Bmp::GetHeight()//返回图片高度
{
	return m_Height;
}

long Bmp::GetColor()//返回图片颜色层数，灰度图为1，24位图为3，32位图为4
{
	return m_Color;
}

unsigned char Bmp::GetRGB(long color,long x,long y)//返回具体某一像素点的值
{
	return m_RGB[min(color,m_Color-1)][x][y];
}

void Bmp::SetRGB(long color,long x,long y,unsigned char RGB)//直接设定图像像素值
{
	m_RGB[min(color,m_Color-1)][x][y]=RGB;
}

unsigned char* Bmp::GetBmpBuf()//返回图片的原始数据存储排列方式的指针
{
	return m_BmpBuf;
}

unsigned char* Bmp::GetBmpBufTrans()//返回转换为显示图片需要的排列方式的指针
{
	long n=m_Width*m_Height*4;
	unsigned char* Buf=new unsigned char[n];

	if(m_Color==1)
	{
		for(int i=0;i<n;i+=4)
		{
			*(Buf+i)=m_RGB[0][i/4/m_Width][i/4%m_Width];
			*(Buf+i+1)=*(Buf+i);
			*(Buf+i+2)=*(Buf+i+1);
		}
	}
	else if(m_Color==3)
	{
		for(long i=0;i<n;i+=4)
		{
			*(Buf+i)=m_RGB[2][i/4/m_Width][i/4%m_Width];
			*(Buf+i+1)=m_RGB[1][i/4/m_Width][i/4%m_Width];
			*(Buf+i+2)=m_RGB[0][i/4/m_Width][i/4%m_Width];
		}
	}
	else if(m_Color==4)
	{
		for(long i=0;i<n;i+=4)
		{
			*(Buf+i)=m_RGB[3][i/4/m_Width][i/4%m_Width];
			*(Buf+i+1)=m_RGB[2][i/4/m_Width][i/4%m_Width];
			*(Buf+i+2)=m_RGB[1][i/4/m_Width][i/4%m_Width];
			*(Buf+i+3)=m_RGB[0][i/4/m_Width][i/4%m_Width];
		}
	}

	return Buf;
}

void Bmp::ReverseColor()//反色
{
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=255-*(*(*(m_RGB+i)+j)+k);
			}
		}
	}
}

bool Bmp::SaveAsTxt(char* FileName)//存为文本文档
{
	fstream f;//建立文件类，为了方便这里使用C++函数，不懂的可以去学习C++
	StrCatA(FileName,"1.txt");//让文件加上后缀名txt，1表示第一层颜色

	for(int i=0;i<m_Color;i++)
	{
		f.open(FileName,ios::out);//打开用于存储第一层颜色数值的文本文档
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				f<<int(*(*(*(m_RGB+i)+j)+k))<<" ";//将数组写入文件，用空格分开
			}
			f<<endl;//每写完一行加上换行符
		}
		f.close();//关闭文件
		FileName[strlen(FileName)-5]++;//如果颜色层数不为一，便将文件名的颜色层数的值+1
	}
	return true;//保存成功，返回
}

bool Bmp::Close()//关闭图像，用于重新读取图像前释放内存
{
	for(int i=0;i<m_Color;i++)
	{
		if(m_FFTComplex)delete(*(m_FFTComplex+i));
	}
	if(m_FFTComplex)delete(m_FFTComplex);
	m_FFTComplex=0;

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			if(m_RGB)delete(*(*(m_RGB+i)+j));
		}
		if(m_RGB)delete(*(m_RGB+i));
	}
	if(m_RGB)delete(m_RGB);

	return true;
}

void Bmp::ReverseLR()//左右反转
{
	char tmp;
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width/2;k++)
			{
				tmp=*(*(*(m_RGB+i)+j)+k);
				*(*(*(m_RGB+i)+j)+k)=*(*(*(m_RGB+i)+j)+m_Width-1-k);
				*(*(*(m_RGB+i)+j)+m_Width-1-k)=tmp;
			}
		}
	}

}
void Bmp::ReverseUD()//上下反转
{
	char tmp;
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height/2;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				tmp=*(*(*(m_RGB+i)+j)+k);
				*(*(*(m_RGB+i)+j)+k)=*(*(*(m_RGB+i)+m_Height-1-j)+k);
				*(*(*(m_RGB+i)+m_Height-1-j)+k)=tmp;
			}
		}
	}
}

void Bmp::Reverse180()//180度反转
{
	ReverseLR();
	ReverseUD();
}

BITMAPFILEHEADER Bmp::GetBmpFileHeader()//返回图像文件头
{
	return m_BmpFileHeader;
}
BITMAPINFOHEADER Bmp::GetBmpInfoHeader()//返回图像信息头
{
	return m_BmpInfoHeader;
}

void Bmp::SmoothAverage(int x,int y)//平均滤波
{
	int area=x*y,i,j,k,l,m,Hmax=m_Height-1,Wmax=m_Width-1;
	
	unsigned int ***tmp=new unsigned int**[m_Color];//创建临时空间
	for(i=0;i<m_Color;i++)
	{
		tmp[i]=new unsigned int*[m_Height];
	}
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			tmp[i][j]=new unsigned int[m_Width];
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(tmp+i)+j)+k)=0;
				for(l=-y/2;l<(y+1)/2;l++)
				{
					for(m=-x/2;m<(x+1)/2;m++)
					{
						*(*(*(tmp+i)+j)+k)+=*(*(*(m_RGB+i)+min(max(j+l,0),Hmax))+min(max(k+m,0),Wmax));
					}//将像素值相加
				}
				*(*(*(tmp+i)+j)+k)/=area;//然后求平均
			}
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=*(*(*(tmp+i)+j)+k);//保存结果并清除内存
			}
			delete(tmp[i][j]);
		}
		delete(tmp[i]);
	}
	delete(tmp);
}

void Bmp::BackUpRGB()//备份图片像素值
{
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB_Bk+i)+j)+k)=*(*(*(m_RGB+i)+j)+k);
			}
		}
	}
}

void Bmp::RestoreRGB()//从备份的像素值中还原图像
{
	int i,j,k;
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=*(*(*(m_RGB_Bk+i)+j)+k);
			}
		}
	}
}

void Bmp::SmoothMedian(int x,int y)//中值滤波
{
	int area=x*y,i,j,k,l,m,Hmax=m_Height-1,Wmax=m_Width-1;

	unsigned int ***tmp=new unsigned int**[m_Color];
	for(i=0;i<m_Color;i++)
	{
		tmp[i]=new unsigned int*[m_Height];
	}
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			tmp[i][j]=new unsigned int[m_Width];
		}
	}

	unsigned char *data=new unsigned char[area];

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				for(l=-y/2;l<(y+1)/2;l++)
				{
					for(m=-x/2;m<(x+1)/2;m++)
					{
						*(data+(l+y/2)*x+m+x/2)=*(*(*(m_RGB+i)+min(max(j+l,0),Hmax))+min(max(k+m,0),Wmax));
					}//获得需要中值滤波的数组
				}
				
				for(l=0;l<area/2+1;l++)
				{
					for(m=l+1;m<area;m++)
					{
						if(*(data+l)>*(data+m))
						{
							unsigned char n=*(data+l);
							*(data+l)=*(data+m);
							*(data+m)=n;
						}//进行大小排序
					}
				}

				*(*(*(tmp+i)+j)+k)=*(data+area/2);//得到中值
			}
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=*(*(*(tmp+i)+j)+k);//保存结果并清空内存
			}
			delete(tmp[i][j]);
		}
		delete(tmp[i]);
	}
	delete(tmp);
}

void Bmp::FilterDefine(double filter[5][5],double divide,int move)//自定义滤波器
{
	int i,j,k,l,m,Hmax=m_Height-1,Wmax=m_Width-1;

	double ***tmp=new double**[m_Color];
	for(i=0;i<m_Color;i++)
	{
		tmp[i]=new double*[m_Height];
	}
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			tmp[i][j]=new double[m_Width];
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(tmp+i)+j)+k)=0;
				for(l=-2;l<3;l++)
				{
					for(m=-2;m<3;m++)
					{
						if(filter[l+2][m+2])
						{
							*(*(*(tmp+i)+j)+k)+=*(*(*(m_RGB+i)+min(max(j+l,0),Hmax))+min(max(k+m,0),Wmax))*filter[l+2][m+2];
						}//按照定义的滤波器参数对像素值进行相应的运算
					}
				}
				if(divide)
				{
					*(*(*(tmp+i)+j)+k)/=divide;//按照定义的滤波器参数对像素值进行相应的均值
				}
				if(move)
				{
					*(*(*(tmp+i)+j)+k)+=move;//按照定义的滤波器参数对像素值进行相应的位移
				}
			}
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=static_cast<unsigned char>(min(abs(*(*(*(tmp+i)+j)+k)),255));
			}
		}
	}
}

void Bmp::GaussSmooth(int r, double sigma)//高斯模糊
{
	if (!sigma)
	{
		sigma = 1;
	}

	sigma *= sigma * 2;

	r = 2 * r + 1;
	int i, j, k, l, m;
	double **filter=new double*[r];
	double sum=0;
	for ( i = 0; i < r; i++)
	{
		filter[i] = new double[r];
		for ( j = 0; j < r; j++)
		{
			filter[i][j] = exp(-double((i - r / 2)*(i - r / 2) + (j - r / 2)*(j - r / 2)) / sigma);
			sum += filter[i][j];
		}
	}

	for (i = 0; i < r; i++)
	{
		for (j = 0; j < r; j++)
		{
			filter[i][j] /= sum;
		}
	}
	
	double ***tmp = new double**[m_Color];
	for (i = 0; i<m_Color; i++)
	{
		tmp[i] = new double*[m_Height];
	}
	for (i = 0; i<m_Color; i++)
	{
		for (j = 0; j<m_Height; j++)
		{
			tmp[i][j] = new double[m_Width];
			for (k = 0; k < m_Width; k++)
			{
				tmp[i][j][k] = 0;
				for (l = -r / 2; l <= r / 2; l++)
				{
					for (m = -r / 2; m <= r / 2; m++)
					{
						tmp[i][j][k] += m_RGB[i][max(0,min(m_Height-1,j + l))][max(0,min(m_Width-1,k + m))] * filter[l + r / 2][m + r / 2];
					}
				}
			}
		}
	}

	for (i = 0; i<m_Color; i++)
	{
		for (j = 0; j<m_Height; j++)
		{
			for (k = 0; k<m_Width; k++)
			{
				*(*(*(m_RGB + i) + j) + k) = static_cast<unsigned char>(*(*(*(tmp + i) + j) + k));//保存结果并清空内存
			}
			delete(tmp[i][j]);
		}
		delete(tmp[i]);
	}
	delete(tmp);
}

void Bmp::BilateralFilter(int r, double sigma1, double sigma2)
{
	if (!sigma1)
	{
		sigma1 = 1;
	}
	if (!sigma2)
	{
		sigma2 = 1;
	}

	sigma1 *= sigma1;
	sigma1 *= 2;
	sigma2 *= sigma2;
	sigma2 *= 2;

	r = 2 * r + 1;
	int i, j, k, l, m,x,y;
	double **filterG = new double*[r];
	double sum = 0;
	for (i = 0; i < r; i++)
	{
		filterG[i] = new double[r];
		for (j = 0; j < r; j++)
		{
			filterG[i][j] = exp(-double((i - r / 2)*(i - r / 2) + (j - r / 2)*(j - r / 2)) / sigma1);
		}
	}

	double **filter = new double*[r];
	for (i = 0; i < r; i++)
	{
		filter[i] = new double[r];
	}

	double ***tmp = new double**[m_Color];
	for (i = 0; i<m_Color; i++)
	{
		tmp[i] = new double*[m_Height];
	}
	for (i = 0; i<m_Color; i++)
	{
		for (j = 0; j<m_Height; j++)
		{
			tmp[i][j] = new double[m_Width];
			for (k = 0; k < m_Width; k++)
			{
				tmp[i][j][k] = 0;

				sum = 0;
				for (x = 0; x < r; x++)
				{
					for (y = 0; y < r; y++)
					{
						double f=double(m_RGB[i][max(0, min(m_Height - 1, j + x - r / 2))][max(0, min(m_Width - 1, k + y - r / 2))] - m_RGB[i][j][k]);
						filter[x][y] = exp(-f*f/sigma2);
						filter[x][y] *= filterG[x][y];
						sum += filter[x][y];
					}
				}

				for (x = 0; x < r; x++)
				{
					for (y = 0; y < r; y++)
					{
						filter[x][y] /= sum;
					}
				}

				for (l = -r / 2; l <= r / 2; l++)
				{
					for (m = -r / 2; m <= r / 2; m++)
					{
						tmp[i][j][k] += double(m_RGB[i][max(0, min(m_Height - 1, j + l))][max(0, min(m_Width - 1, k + m))]) * filter[l + r / 2][m + r / 2];
					}
				}
			}
		}
	}

	for (i = 0; i<m_Color; i++)
	{
		for (j = 0; j<m_Height; j++)
		{
			for (k = 0; k<m_Width; k++)
			{
				m_RGB[i][j][k] = static_cast<unsigned char>(*(*(*(tmp + i) + j) + k));//保存结果并清空内存
			}
			delete(tmp[i][j]);
		}
		delete(tmp[i]);
	}
	delete(tmp);
}


void Bmp::CurveFunction(int color,int curve[256])//实现图片的函数变换
{
	for(int i=0;i<m_Height;i++)
	{	
		for(int j=0;j<m_Width;j++)
		{
			m_RGB[color][i][j]=curve[m_RGB[color][i][j]];//简单来说就是：a=f(a)
		}
	}
}

int** Bmp::GetHistogram()//计算图片的直方图，非归一化直方图
{
	m_Histogram=new int*[m_Color];
	for(int i=0;i<m_Color;i++)
	{
		*(m_Histogram+i)=new int[256];
	}
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<256;j++)
		{
			*(*(m_Histogram+i)+j)=0;//申请直方图内存空间
		}
	}
	

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				m_Histogram[i][m_RGB[i][j][k]]++;//进行直方图统计
			}
		}
	}
	return m_Histogram;
}

void Bmp::HistogramEqualization()//色调均化，算法采用PS的算法
{
	GetHistogram();

	if(m_Color!=1)
	{
		for(int i=0;i<256;i++)
		{
			*(*(m_Histogram+m_Color-3)+i)=*(*(m_Histogram+m_Color-2)+i)=*(*(m_Histogram+m_Color-1)+i)=(*(*(m_Histogram+m_Color-3)+i)+*(*(m_Histogram+m_Color-2)+i)+*(*(m_Histogram+m_Color-1)+i))/3;
		}//如果是彩色图像，将其三个直方图相加，这是PS算法的关键
	}

	double** t=new double*[m_Color];
	for(int i=0;i<m_Color;i++)
	{
		*(t+i)=new double[256];
	}

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<256;j++)
		{
			*(*(t+i)+j)=double(*(*(m_Histogram+i)+j))/(m_Width*m_Height);//归一化直方图
		}
	}

	for(int i=0;i<m_Color;i++)
	{
		for(int j=1;j<256;j++)
		{
			*(*(t+i)+j)+=*(*(t+i)+j-1);//累计直方图
		}
	}

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<256;j++)
		{
			*(*(t+i)+j)=255*(*(*(t+i)+j))+0.5;//将归一化累计直方图转化为像素值
		}
	}

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=static_cast<unsigned char>(*(*(t+i)+*(*(*(m_RGB+i)+j)+k)));
			}
		}
	}
}

void Bmp::Threshold(int Threshold)//阈值处理
{
	if(m_Color!=1||Threshold<0)
	{
		RGB2Gray();
	}//先将图片转换为灰度图

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)>=Threshold?*(*(*(m_RGB+i)+j)+k)=255:*(*(*(m_RGB+i)+j)+k)=0;
			}//大于等于阈值就是255，否则就是0
		}
	}
}

int Bmp::OtsuThreshold()//大津法求阈值
{
	if(m_Color!=1)
	{
		RGB2Gray();
	}//先将图片转换为灰度图

	double p[256],u=0,w0=0,w1=0,u0=0,u1=0,g=0,gtmp,Threshold;
	int i,j;
	long HistogramMax=m_Height*m_Width;

	if(!m_Histogram)
	{
		GetHistogram();
	}//获得直方图

	for(i=0;i<256;i++)//接下来就是按照大津法的算法计算了
	{
		p[i]=double(m_Histogram[0][i])/HistogramMax;
	}
	for(i=0;i<256;i++)
	{
		u+=i*p[i];
	}

	for(i=0;i<256;i++)
	{
		w0=0;
		for(j=0;j<=i;j++)
		{
			w0+=p[j];
		}
		w1=0;
		for(j=i+1;j<256;j++)
		{
			w1+=p[j];
		}

		u0=0;
		if(w0!=0)
		{
			for(j=0;j<=i;j++)
			{
				u0+=j*p[j]/w0;
			}
		}
		u1=0;
		if(w1!=0)
		{
			for(j=i+1;j<256;j++)
			{
				u1+=j*p[j]/w1;
			}
		}

		gtmp=w0*(u0-u)*(u0-u)+w1*(u1-u)*(u1-u);
		if(g<gtmp)
		{
			g=gtmp;
			Threshold=i;
		}
	}
	return static_cast<int>(Threshold);//最后返回得到的阈值
}

int Bmp::IterationThreshold()//迭代法求阈值
{
	if(m_Color!=1)
	{
		RGB2Gray();
	}//先将图片转换为灰度图

	double T=128,T_o=0,u0,u1,n0,n1;;//从128开始迭代
	int i;

	if(!m_Histogram)
	{
		GetHistogram();
	}//获得直方图
	while(T!=T_o)//接下来就是迭代法的算法计算了，当前后两次阈值相等就跳出while
	{
		T_o=T;
		n0=u0=0;
		for(i=0;i<=T;i++)
		{
			n0+=m_Histogram[0][i];
			u0+=i*m_Histogram[0][i];
		}
		u0/=n0;
		n1=u1=0;
		for(i=static_cast<int>(T)+1;i<256;i++)
		{
			n1+=m_Histogram[0][i];
			u1+=i*m_Histogram[0][i];
		}
		u1/=n1;
	
		T=(u0+u1)/2;
	}
	return static_cast<int>(T);//最后返回迭代法的阈值
}

int Bmp::HalfCutThreshold()//自己发明的对半分阈值算法！
{
	if(m_Color!=1)
	{
		RGB2Gray();
	}
	if(!m_Histogram)
	{
		GetHistogram();
	}

	int i=0,j=255;
	long n0=0,n1=0;

	while (i<=j)//简单来说，就是如果前边累计的像素值多的话，后面的像素值就会向前累计，反之一样
	{
		if(n0>=n1)
		{
			n1+=m_Histogram[0][j--];
		}
		else
		{
			n0+=m_Histogram[0][i++];
		}
	}

	return j;
}

void Bmp::Histogram(int Color,int Min,int Max)//直方图函数处理
{
	if(Min>=Max)
	{
		return;
	}

	double curve[256];
	for(int i=0;i<256;i++)
	{
		curve[i]=min(255,double(255*max(0,i-Min))/(Max-Min));
	}//通过函数的参数设定好曲线，应该说就是一个线性变化的直线

	for(int i=0;i<m_Height;i++)
	{
		for(int j=0;j<m_Width;j++)
		{
			*(*(*(m_RGB+Color)+i)+j)=static_cast<unsigned char>(*(curve+*(*(*(m_RGB+Color)+i)+j)));
		}//然后就是最简单的曲线变换了
	}
}

void Bmp::RGB2Gray()//使彩色图像转换为灰度图
{
	if(m_Color==1)
	{
		return;
	}

	unsigned char ***Gray=new unsigned char**[1];
	Gray[0]=new unsigned char*[m_Height];
	for(int i=0;i<m_Height;i++)
	{
		*(*Gray+i)=new unsigned char[m_Width];
	}
	for(int i=0;i<m_Height;i++)
	{
		for(int j=0;j<m_Width;j++)
		{
			*(*(*Gray+i)+j)=static_cast<unsigned char>(min(255,max(0,0.299*m_RGB[m_Color-3][i][j]+0.587*m_RGB[m_Color-2][i][j]+0.114*m_RGB[m_Color-1][i][j])));
		}//通过转换公式将RGB像素值按照一定比例转换为灰度
	}

	for(int i=0;i<m_Color;i++)//然后就是一堆的内存操作了
	{
		for(int j=0;j<m_Height;j++)
		{
			free(m_RGB[i][j]);
			free(m_RGB_Bk[i][j]);
		}
	}
	for(int i=0;i<m_Color;i++)
	{
		free(m_RGB[i]);
		free(m_RGB_Bk[i]);
	}
	free(m_RGB);
	free(m_RGB_Bk);

	m_RGB=Gray;//将新的灰度图的地址给图片

	m_ColorTable=new RGBQUAD[256];//设定新的调色板，计算新的图片参数保存回图片的文件头和信息头
	for(int i=0;i<256;i++)
	{
		m_ColorTable[i].rgbBlue=m_ColorTable[i].rgbGreen=m_ColorTable[i].rgbRed=i;
		m_ColorTable[i].rgbReserved=0;
	}

	m_LineBit=(m_Width+3)/4*4;
	m_Color=1;

	m_BmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+1024+m_LineBit*m_Height;
	m_BmpFileHeader.bfOffBits=54+1024;

	m_BmpInfoHeader.biBitCount=8;
	m_BmpInfoHeader.biSizeImage=m_LineBit;

	m_RGB_Bk=new unsigned char**[m_Color];
	for(int i=0;i<m_Color;i++)
	{
		m_RGB_Bk[i]=new unsigned char*[m_Height];
	}
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			m_RGB_Bk[i][j]=new unsigned char[m_Width];
		}
	}
}

void Bmp::Zoom(int TargetHeight,int TargetWidth,int Method)//图像缩放，三种缩放方式
	//Method：0：near 1：linear 2：cube
{
	unsigned char ***RGB=new unsigned char**[m_Color];
	for(int i=0;i<m_Color;i++)
	{
		RGB[i]=new unsigned char*[TargetHeight];
	}
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<TargetHeight;j++)
		{
			RGB[i][j]=new unsigned char[TargetWidth];
		}
	}

	if(Method==0)//邻近
	{
		for(int i=0;i<m_Color;i++)
		{
			for(int j=0;j<TargetHeight;j++)
			{
				for(int k=0;k<TargetWidth;k++)
				{
					*(*(*(RGB+i)+j)+k)=*(*(*(m_RGB+i)+j*m_Height/TargetHeight)+k*m_Width/TargetWidth);
				}//邻近应该不用解释了
			}
		}
	}
	else if(Method==1)//两次线性
	{
		double x,y,fu,fd;
		int x1,y1,x2,y2;
		for(int i=0;i<m_Color;i++)
		{
			for(int j=0;j<TargetHeight;j++)
			{
				for(int k=0;k<TargetWidth;k++)
				{
					x=k*double(m_Width)/TargetWidth;
					y=j*double(m_Height)/TargetHeight;
					x1=static_cast<int>(x);
					y1=static_cast<int>(y);
					x2=min(m_Width-1,x1+1);
					y2=min(m_Height-1,y1+1);

					fu=m_RGB[i][y1][x1]+(x-x1)*(m_RGB[i][y1][x2]-m_RGB[i][y1][x1]);
					fd=m_RGB[i][y2][x1]+(x-x1)*(m_RGB[i][y2][x2]-m_RGB[i][y2][x1]);
					*(*(*(RGB+i)+j)+k)=static_cast<unsigned char>(fu+(y-y1)*(fd-fu));
				}//两次线性就是先算一次横向的的比例，再算一次纵向的比例
			}
		}
	}
	else if(Method==2)//两次立方
	{
		int a[5],b[5];
		double A,B,C,D,F[4],deltaA,deltaB;
		for(int i=0;i<m_Color;i++)
		{
			for(int j=0;j<TargetHeight;j++)
			{
				for(int k=0;k<TargetWidth;k++)
				{
					a[4]=j*m_Height/TargetHeight;
					b[4]=k*m_Width/TargetWidth;
					a[1]=max(min(a[4],m_Height-1),0);
					b[1]=max(min(b[4],m_Width-1),0);
					a[2]=min(a[1]+1,m_Height-1);
					b[2]=min(b[1]+1,m_Width-1);
					a[3]=min(a[2]+1,m_Height-1);
					b[3]=min(b[2]+1,m_Width-1);
					a[0]=max(a[1]-1,0);
					b[0]=max(b[1]-1,0);
					deltaA=j*double(m_Height)/TargetHeight-a[4];
					deltaB=k*double(m_Width)/TargetWidth-b[4];

					for(int l=0;l<4;l++)
					{
						D=m_RGB[i][a[l]][b[1]];
						B=double(m_RGB[i][a[l]][b[2]]+m_RGB[i][a[l]][b[0]]-2*D)/2;
						A=double(m_RGB[i][a[l]][b[3]]-2*m_RGB[i][a[l]][b[2]]+D-2*B)/6;
						C=m_RGB[i][a[l]][b[2]]-D-A-B;
						F[l]=A*deltaB*deltaB*deltaB+B*deltaB*deltaB+C*deltaB+D;
					}

					D=F[1];
					B=double(F[2]+F[0]-2*D)/2;
					A=double(F[3]-2*F[2]+D-2*B)/6;
					C=F[2]-A-B-D;
					*(*(*(RGB+i)+j)+k)=static_cast<unsigned char>(min(max(A*deltaA*deltaA*deltaA+B*deltaA*deltaA+C*deltaA+D,0),255));
				}//两次立方直接求两次立方公式，原理与两次线性差不多，不过用到的参数多了
			}
		}
	}

	for(int i=0;i<m_Color;i++)//还是经典的内存释放等操作
	{
		for(int j=0;j<m_Height;j++)
		{
			free(m_RGB[i][j]);
			free(m_RGB_Bk[i][j]);
		}
	}
	for(int i=0;i<m_Color;i++)
	{
		free(m_RGB[i]);
		free(m_RGB_Bk[i]);
	}
	free(m_RGB);
	free(m_RGB_Bk);

	m_RGB=RGB;

	m_LineBit=(TargetWidth*m_Color+3)/4*4;//还有文件头信息头的重新设定
	m_Height=TargetHeight;
	m_Width=TargetWidth;
	int ColorTableSize=(m_Color==1?1024:0);
	m_BmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+ColorTableSize+m_LineBit*m_Height;
	m_BmpInfoHeader.biHeight=TargetHeight;
	m_BmpInfoHeader.biWidth=TargetWidth;
	m_BmpInfoHeader.biSizeImage=m_LineBit*TargetHeight;

	m_RGB_Bk=new unsigned char**[m_Color];
	for(int i=0;i<m_Color;i++)
	{
		m_RGB_Bk[i]=new unsigned char*[m_Height];
	}
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			m_RGB_Bk[i][j]=new unsigned char[m_Width];
		}
	}
}

void Bmp::DFT()//离散傅里叶变换
{
	int i,j,k,m,n,u,v,RGB;//定义各种参数
	double real1,image1,big=0;
	Complex w_h0(cos(PI_2/m_Height),-sin(PI_2/m_Height)),w_w0(cos(PI_2/m_Width),-sin(PI_2/m_Width));
	//所有的傅里叶变换系数都是以上两个参数w_h0与w_w0的整数冥，所以下面先对系数进行计算：
	Complex *tmpW_H=new Complex[m_Height*m_Height];
	Complex *tmpW_W=new Complex[m_Width*m_Width];
	*tmpW_H=1;
	for(i=1;i<m_Height*m_Height;i++)
	{
		*(tmpW_H+i)=*(tmpW_H+i-1)*w_h0;
	}
	*tmpW_W=1;
	for(i=1;i<m_Width*m_Width;i++)
	{
		*(tmpW_W+i)=*(tmpW_W+i-1)*w_w0;
	}
	//以上为确定傅里叶变换参数，下面开辟一个临时存储空间
	double ***tmp=new double**[m_Color];
	for(i=0;i<m_Color;i++)
	{
		tmp[i]=new double*[m_Height];
	}
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			tmp[i][j]=new double[m_Width];
		}
	}
	//开辟复数空间存储复数结果
	for(i=0;i<m_Color;i++)
	{
		if(m_FFTComplex)delete(*(m_FFTComplex+i));
	}
	if(m_FFTComplex)delete(m_FFTComplex);
	m_FFTComplex=new COMPLEX*[m_Color];
	for(i=0;i<m_Color;i++)
	{
		*(m_FFTComplex+i)=new COMPLEX[m_Height*m_Width];
	}
	COMPLEX *tmpFFTComplex;
	//定义临时复数类便于计算
	Complex tmpComplex,tmpComplex1;

	for(m=0;m<m_Height;m++)
	{
		for(n=0;n<m_Width;n++)
		{
			for(i=0;i<m_Color;i++)
			{
				tmpComplex=0;
				for(j=0;j<m_Height;j++)
				{
					tmpComplex1=0;
					for(k=0;k<m_Width;k++)
					{//根据化简的公式对行进行相加
						v=n*k;
						tmpComplex1.real+=(*(tmpW_W+v)).real*(RGB=(*(*(*(m_RGB+i)+j)+k)));
						tmpComplex1.image+=(*(tmpW_W+v)).image*RGB;
					}//再对列进行相加
					u=m*j;
					real1=(*(tmpW_H+u)).real;
					image1=(*(tmpW_H+u)).image;
					tmpComplex.real+=real1*tmpComplex1.real-image1*tmpComplex1.image;
					tmpComplex.image+=real1*tmpComplex1.image+image1*tmpComplex1.real;
				}
				tmp[i][m][n]=tmpComplex.GetAmplitude();//计算幅值用于显示
				tmpFFTComplex=&(*(*(m_FFTComplex+i)+m*m_Width+n));//存储复数用于以后处理
				tmpFFTComplex->re=tmpComplex.real;
				tmpFFTComplex->im=tmpComplex.image;
			}
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(tmp+i)+j)+k)=log10(*(*(*(tmp+i)+j)+k));//取对数便于显示
				if(big<tmp[i][j][k])
					big=tmp[i][j][k];
			}
		}
	}
	
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=static_cast<unsigned char>(*(*(*(tmp+i)+j)+k)*255/big);//将数据范围拉伸至0-255区间
			}
		}
	}

	int tmpRGB;//中心化幅值图
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height/2;j++)
		{
			for(k=0;k<m_Width/2;k++)
			{
				tmpRGB=m_RGB[i][j][k];
				m_RGB[i][j][k]=m_RGB[i][j+m_Height/2][k+m_Width/2];
				m_RGB[i][j+m_Height/2][k+m_Width/2]=tmpRGB;

				tmpRGB=m_RGB[i][j+m_Height/2][k];
				m_RGB[i][j+m_Height/2][k]=m_RGB[i][j][k+m_Width/2];
				m_RGB[i][j][k+m_Width/2]=tmpRGB;
			}
		}
	}
}

void Bmp::FFT()//快速傅立叶变换
{
	int i,j,k;
	double big=0;
	COMPLEX **complexOrg=new COMPLEX*[m_Color];//在内存申请原始数据与目的数据的复数空间
	COMPLEX **complexDes=new COMPLEX*[m_Color];

	for(i=0;i<m_Color;i++)
	{
		*(complexOrg+i)=new COMPLEX[m_Height*m_Width];
		*(complexDes+i)=new COMPLEX[m_Height*m_Width];
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				(*(*(complexOrg+i)+j*m_Width+k)).re=m_RGB[i][j][k];
				(*(*(complexOrg+i)+j*m_Width+k)).im=0;
			}
		}
	}

	double ***tmp=new double**[m_Color];
	for(i=0;i<m_Color;i++)
	{
		tmp[i]=new double*[m_Height];
	}
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			tmp[i][j]=new double[m_Width];
		}
	}

	for(i=0;i<m_Color;i++)
	{
		FFT2(*(complexOrg+i),m_Width,m_Height,*(complexDes+i));//调用FFT函数
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				tmp[i][j][k]=log10(sqrt((*(*(complexDes+i)+j*m_Width+k)).re*(*(*(complexDes+i)+j*m_Width+k)).re+(*(*(complexDes+i)+j*m_Width+k)).im*(*(*(complexDes+i)+j*m_Width+k)).im));
				big<tmp[i][j][k]?big=tmp[i][j][k]:NULL;
			}//计算出频域，并取对数
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				
				m_RGB[i][j][k]=static_cast<unsigned char>(tmp[i][j][k]*255/big);//然后做一次直方图拉伸放大对比度
			}
		}
	}

	int tmpRGB;
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height/2;j++)
		{
			for(k=0;k<m_Width/2;k++)
			{
				tmpRGB=m_RGB[i][j][k];
				m_RGB[i][j][k]=m_RGB[i][j+m_Height/2][k+m_Width/2];
				m_RGB[i][j+m_Height/2][k+m_Width/2]=tmpRGB;

				tmpRGB=m_RGB[i][j+m_Height/2][k];
				m_RGB[i][j+m_Height/2][k]=m_RGB[i][j][k+m_Width/2];
				m_RGB[i][j][k+m_Width/2]=tmpRGB;
			}//中心化
		}
	}

	for(i=0;i<m_Color;i++)
	{
		if(m_FFTComplex)delete(*(m_FFTComplex+i));
		delete(*(complexOrg+i));
	}
	delete(m_FFTComplex);
	delete(complexOrg);
	m_FFTComplex=complexDes;
}

void Bmp::iFFT()//快速傅立叶反变换，与FFT相似
{
	int i,j,k;
	COMPLEX **complexOrg=new COMPLEX*[m_Color];
	for(i=0;i<m_Color;i++)
	{
		*(complexOrg+i)=new COMPLEX[m_Height*m_Width];
	}
	COMPLEX **complexDes=new COMPLEX*[m_Color];
	for(i=0;i<m_Color;i++)
	{
		*(complexDes+i)=new COMPLEX[m_Height*m_Width];
		for(j=0;j<m_Height*m_Width;j++)
		{
			*(*(complexDes+i)+j)=*(*(m_FFTComplex+i)+j);
		}
	}


	for(i=0;i<m_Color;i++)
	{
		IFFT2(*(complexOrg+i),m_Width,m_Height,*(complexDes+i));
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				m_RGB[i][j][k]=static_cast<unsigned char>(sqrt((*(*(complexOrg+i)+j*m_Width+k)).re*(*(*(complexOrg+i)+j*m_Width+k)).re+(*(*(complexOrg+i)+j*m_Width+k)).im*(*(*(complexOrg+i)+j*m_Width+k)).im));
			}
		}
	}
	for(i=0;i<m_Color;i++)
	{
		delete(*(complexDes+i));
		delete(*(complexOrg+i));
	}
	delete(complexDes);
	delete(complexOrg);
}

COMPLEX** Bmp::GetFFTComplex()//返回傅里叶变换后的复数数据
{
	return m_FFTComplex;
}

void Bmp::Morphology(int Type,bool** filter,int threshold)//形态学运算，0:Dilation 1:Erosion
{
	int i,j,k,l,m,a;

	unsigned char ***tmp=new unsigned char**[m_Color];
	for(i=0;i<m_Color;i++)
	{
		*(tmp+i)=new unsigned char*[m_Height];
	}
	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			*(*(tmp+i)+j)=new unsigned char[m_Width];
		}
	}

	if(!Type)//Dilation
	{
		for(i=0;i<m_Color;i++)
		{
			for(j=0;j<m_Height;j++)
			{
				for(k=0;k<m_Width;k++)
				{
					*(*(*(tmp+i)+j)+k)=0;
					for(l=0;l<9;l++)
					{
						for(m=0;m<9;m++)
						{//膨胀的话，算法需要把结构因子反过来
							if(*(*(filter+8-l)+8-m))//如果结构因子为1，才进行运算以节省时间
								if(*(*(*(tmp+i)+j)+k)<(a=(*(*(*(m_RGB+i)+max(0,min(m_Height-1,j+l-4)))+max(0,min(m_Width-1,k+m-4))))))
									*(*(*(tmp+i)+j)+k)=a;//求出该结构因子内的最大值
						}
					}
				}
			}
		}
	}
	else//Erosion
	{
		for(i=0;i<m_Color;i++)
		{
			for(j=0;j<m_Height;j++)
			{
				for(k=0;k<m_Width;k++)
				{
					*(*(*(tmp+i)+j)+k)=255;
					for(l=0;l<9;l++)
					{
						for(m=0;m<9;m++)
						{
							if(*(*(filter+l)+m))//如果结构因子为1，才进行运算以节省时间
								if(*(*(*(tmp+i)+j)+k)>(a=(*(*(*(m_RGB+i)+max(0,min(m_Height-1,j+l-4)))+max(0,min(m_Width-1,k+m-4))))))
									*(*(*(tmp+i)+j)+k)=a;//求出该结构因子内的最小值
						}
					}
				}
			}
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=threshold==-1?*(*(*(tmp+i)+j)+k):*(*(*(tmp+i)+j)+k)>=threshold?255:0;
			}//如果设定了阈值，则对图像进行二值化
			delete(*(*(tmp+i)+j));
		}
		delete(*(tmp+i));
	}
	free(tmp);
}

void Bmp::BkRGBMinusRGB(bool reverse)//将备份像素值与图像像素值相减
{
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				if (reverse)
				{
					*(*(*(m_RGB + i) + j) + k) = *(*(*(m_RGB + i) + j) + k) - *(*(*(m_RGB_Bk + i) + j) + k);
				}
				else
				{
					*(*(*(m_RGB + i) + j) + k) = *(*(*(m_RGB_Bk + i) + j) + k) - *(*(*(m_RGB + i) + j) + k);
				}
			}
		}
	}
}

bool Bmp::Cut(CPoint pt_lu,CPoint pt_rd)//图像裁剪
{
	long h1=pt_lu.y,h2=pt_rd.y,w1=pt_lu.x,w2=pt_rd.x,tmp;//获得裁剪区域
	
	if(h1>=m_Height||h2>=m_Height||w1>=m_Width||w2>=m_Width||h1==h2||w1==w2)
	{
		return false;//如果裁剪区域不符合要求则取消裁剪
	}

	if(h1>h2)//调整裁剪区域，就是小的在前大的在后
	{
		tmp=h1;
		h1=h2;
		h2=tmp;
	}
	if(w1>w2)
	{
		tmp=w1;
		w1=w2;
		w2=tmp;
	}

	long width=w2-w1,height=h2-h1;//计算裁剪区域的宽度与高度

	unsigned char ***RGB=new unsigned char **[m_Color];
	for(int i=0;i<m_Color;i++)
	{
		RGB[i]=new unsigned char *[height];
		for(int j=0;j<height;j++)
		{
			RGB[i][j]=new unsigned char [width];
			for(int k=0;k<width;k++)
			{
				RGB[i][j][k]=m_RGB[i][h1+j][w1+k];//申请裁剪区域大小的内存空间，并且将像素值进行转移
			}
		}
	}

	for(int i=0;i<m_Color;i++)//经典的内存操作
	{
		for(int j=0;j<m_Height;j++)
		{
			free(m_RGB[i][j]);
			free(m_RGB_Bk[i][j]);
		}
	}
	for(int i=0;i<m_Color;i++)
	{
		free(m_RGB[i]);
		free(m_RGB_Bk[i]);
	}
	free(m_RGB);
	free(m_RGB_Bk);

	m_RGB=RGB;//将裁剪后的像素值地址赋予给图像

	m_LineBit=(width*m_Color+3)/4*4;//还有经典的文件头信息头设定
	m_Height=height;
	m_Width=width;
	int ColorTableSize=(m_Color==1?1024:0);
	m_BmpFileHeader.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+ColorTableSize+m_LineBit*m_Height;
	m_BmpInfoHeader.biHeight=height;
	m_BmpInfoHeader.biWidth=width;
	m_BmpInfoHeader.biSizeImage=m_LineBit*height;

	m_RGB_Bk=new unsigned char**[m_Color];
	for(int i=0;i<m_Color;i++)
	{
		m_RGB_Bk[i]=new unsigned char*[m_Height];
	}
	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			m_RGB_Bk[i][j]=new unsigned char[m_Width];
		}
	}
	return true;
}