#include "stdafx.h"
#include "Bmp.h"
#include <fstream>
using namespace std;

Bmp::Bmp(void): //���캯��
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

Bmp::Bmp(char* FileName)//����·���Ĺ��캯��
{
	m_Color=0;
	m_Height=0;
	m_Width=0;
	m_LineBit=0;
	Open(FileName);
}

Bmp::~Bmp(void)//��������
{
}

bool Bmp::Open(char* FileName)//��ͼƬ
{
	if(Exist())
	{
		Close();
	}

	FILE *fp;//�����ļ�ָ��
	fopen_s(&fp,FileName,"rb");//�Զ����Ʒ�ʽ��ȡ�ļ�
	if(!fp)
	{
		return false;//�����ȡ���˷���
	}

	fread(&m_BmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);//��ȡ�ļ�ͷ�����ݵĴ�СҲ����ֱ�ӻ�������14
	fread(&m_BmpInfoHeader,sizeof(BITMAPINFOHEADER),1,fp);//��ȡ��Ϣͷ�����ݵĴ�СҲ����ֱ�ӻ�������40

	if(m_BmpInfoHeader.biBitCount==8)
	{
		m_ColorTable=new RGBQUAD[256];   
		fread(m_ColorTable,sizeof(RGBQUAD),256,fp); //�����8λͼ����ѵ�ɫ��Ҳ��ȡ����
	}

	m_Color=m_BmpInfoHeader.biBitCount/8;//������ɫ����
	m_Height=m_BmpInfoHeader.biHeight;//��ȡͼƬ��С
	m_Width=m_BmpInfoHeader.biWidth;
	m_LineBit=(m_BmpInfoHeader.biWidth*m_BmpInfoHeader.biBitCount/8+3)/4*4;//�����ͼ��һ���������ֽ�������Ϊͼ��ÿ����Ҫ4�ı������ֽڣ�

	m_BmpBuf=new unsigned char[m_Height*m_LineBit];//����һ����ͼƬ������
	fread(m_BmpBuf,1,m_Height*m_LineBit,fp);//�����ݶ�������
	fclose(fp);//�ر��ļ�

	m_RGB=new unsigned char**[m_Color];//���ڴ潨��һ��m_Color�������������԰���������ɫ��ͬ���ֿ����
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

	m_RGB_Bk=new unsigned char**[m_Color];//���ڴ潨��һ���������飬��ͼ���������ڻ�ԭͼ��
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
				//�����鸳ֵ����Ϊͼ�����������ң������ϵķ�ʽ��ţ�24λͼ��˳��Ϊ���̺죬32λͼ��˳��Ϊ͸���̺죬������Ҫ������λ����ͼ������λ�ý���ת��
			}
		}
	}

	return true;//��ȡ�ɹ�������
}

bool Bmp::Save(char* FileName)//����ͼƬ
{

	FILE *fp;
	fopen_s(&fp,FileName,"wb+");//���ȡһ���ķ�ʽ�������򿪷�ʽ��Ϊ������д�룬�ļ��������򴴽�
	if(!fp)
	{
		return false;
	}

	fwrite(&m_BmpFileHeader,sizeof(BITMAPFILEHEADER),1,fp);//д���ļ�ͷ���ļ�ͷ�����ڴ����н����޸�
	fwrite(&m_BmpInfoHeader,sizeof(BITMAPINFOHEADER),1,fp);//д����Ϣͷ����Ϣͷ�����ڴ����н����޸�

	if(m_BmpInfoHeader.biBitCount==8)
	{
		fwrite(m_ColorTable,sizeof(RGBQUAD),256,fp);//�����8λͼ��д���ɫ��
	}

	m_BmpBuf=new unsigned char[m_Height*m_LineBit];//����һ��ͼ���С������

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				m_BmpBuf[(m_Height-j-1)*m_LineBit+m_Color*k+m_Color-1-i]=*(*(*(m_RGB+i)+j)+k);
				//������ɫ�ֲ������ת��ΪͼƬ�洢��ʽ�����飬�������ȡ�෴
			}
		}
	}

	fwrite(m_BmpBuf,m_Height*m_LineBit,1,fp);//������д���ļ�
	fclose(fp);//�ر��ļ�
	return true;//�ɹ����棬����
}

bool Bmp::Exist()//����ͼƬ�Ƿ����
{
	return 0x00==m_Width?false:true;
}

long Bmp::GetWidth()//����ͼƬ���
{
	return m_Width;
}

long Bmp::GetHeight()//����ͼƬ�߶�
{
	return m_Height;
}

long Bmp::GetColor()//����ͼƬ��ɫ�������Ҷ�ͼΪ1��24λͼΪ3��32λͼΪ4
{
	return m_Color;
}

unsigned char Bmp::GetRGB(long color,long x,long y)//���ؾ���ĳһ���ص��ֵ
{
	return m_RGB[min(color,m_Color-1)][x][y];
}

void Bmp::SetRGB(long color,long x,long y,unsigned char RGB)//ֱ���趨ͼ������ֵ
{
	m_RGB[min(color,m_Color-1)][x][y]=RGB;
}

unsigned char* Bmp::GetBmpBuf()//����ͼƬ��ԭʼ���ݴ洢���з�ʽ��ָ��
{
	return m_BmpBuf;
}

unsigned char* Bmp::GetBmpBufTrans()//����ת��Ϊ��ʾͼƬ��Ҫ�����з�ʽ��ָ��
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

void Bmp::ReverseColor()//��ɫ
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

bool Bmp::SaveAsTxt(char* FileName)//��Ϊ�ı��ĵ�
{
	fstream f;//�����ļ��࣬Ϊ�˷�������ʹ��C++�����������Ŀ���ȥѧϰC++
	StrCatA(FileName,"1.txt");//���ļ����Ϻ�׺��txt��1��ʾ��һ����ɫ

	for(int i=0;i<m_Color;i++)
	{
		f.open(FileName,ios::out);//�����ڴ洢��һ����ɫ��ֵ���ı��ĵ�
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				f<<int(*(*(*(m_RGB+i)+j)+k))<<" ";//������д���ļ����ÿո�ֿ�
			}
			f<<endl;//ÿд��һ�м��ϻ��з�
		}
		f.close();//�ر��ļ�
		FileName[strlen(FileName)-5]++;//�����ɫ������Ϊһ���㽫�ļ�������ɫ������ֵ+1
	}
	return true;//����ɹ�������
}

bool Bmp::Close()//�ر�ͼ���������¶�ȡͼ��ǰ�ͷ��ڴ�
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

void Bmp::ReverseLR()//���ҷ�ת
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
void Bmp::ReverseUD()//���·�ת
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

void Bmp::Reverse180()//180�ȷ�ת
{
	ReverseLR();
	ReverseUD();
}

BITMAPFILEHEADER Bmp::GetBmpFileHeader()//����ͼ���ļ�ͷ
{
	return m_BmpFileHeader;
}
BITMAPINFOHEADER Bmp::GetBmpInfoHeader()//����ͼ����Ϣͷ
{
	return m_BmpInfoHeader;
}

void Bmp::SmoothAverage(int x,int y)//ƽ���˲�
{
	int area=x*y,i,j,k,l,m,Hmax=m_Height-1,Wmax=m_Width-1;
	
	unsigned int ***tmp=new unsigned int**[m_Color];//������ʱ�ռ�
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
					}//������ֵ���
				}
				*(*(*(tmp+i)+j)+k)/=area;//Ȼ����ƽ��
			}
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=*(*(*(tmp+i)+j)+k);//������������ڴ�
			}
			delete(tmp[i][j]);
		}
		delete(tmp[i]);
	}
	delete(tmp);
}

void Bmp::BackUpRGB()//����ͼƬ����ֵ
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

void Bmp::RestoreRGB()//�ӱ��ݵ�����ֵ�л�ԭͼ��
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

void Bmp::SmoothMedian(int x,int y)//��ֵ�˲�
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
					}//�����Ҫ��ֵ�˲�������
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
						}//���д�С����
					}
				}

				*(*(*(tmp+i)+j)+k)=*(data+area/2);//�õ���ֵ
			}
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)=*(*(*(tmp+i)+j)+k);//������������ڴ�
			}
			delete(tmp[i][j]);
		}
		delete(tmp[i]);
	}
	delete(tmp);
}

void Bmp::FilterDefine(double filter[5][5],double divide,int move)//�Զ����˲���
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
						}//���ն�����˲�������������ֵ������Ӧ������
					}
				}
				if(divide)
				{
					*(*(*(tmp+i)+j)+k)/=divide;//���ն�����˲�������������ֵ������Ӧ�ľ�ֵ
				}
				if(move)
				{
					*(*(*(tmp+i)+j)+k)+=move;//���ն�����˲�������������ֵ������Ӧ��λ��
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

void Bmp::GaussSmooth(int r, double sigma)//��˹ģ��
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
				*(*(*(m_RGB + i) + j) + k) = static_cast<unsigned char>(*(*(*(tmp + i) + j) + k));//������������ڴ�
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
				m_RGB[i][j][k] = static_cast<unsigned char>(*(*(*(tmp + i) + j) + k));//������������ڴ�
			}
			delete(tmp[i][j]);
		}
		delete(tmp[i]);
	}
	delete(tmp);
}


void Bmp::CurveFunction(int color,int curve[256])//ʵ��ͼƬ�ĺ����任
{
	for(int i=0;i<m_Height;i++)
	{	
		for(int j=0;j<m_Width;j++)
		{
			m_RGB[color][i][j]=curve[m_RGB[color][i][j]];//����˵���ǣ�a=f(a)
		}
	}
}

int** Bmp::GetHistogram()//����ͼƬ��ֱ��ͼ���ǹ�һ��ֱ��ͼ
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
			*(*(m_Histogram+i)+j)=0;//����ֱ��ͼ�ڴ�ռ�
		}
	}
	

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				m_Histogram[i][m_RGB[i][j][k]]++;//����ֱ��ͼͳ��
			}
		}
	}
	return m_Histogram;
}

void Bmp::HistogramEqualization()//ɫ���������㷨����PS���㷨
{
	GetHistogram();

	if(m_Color!=1)
	{
		for(int i=0;i<256;i++)
		{
			*(*(m_Histogram+m_Color-3)+i)=*(*(m_Histogram+m_Color-2)+i)=*(*(m_Histogram+m_Color-1)+i)=(*(*(m_Histogram+m_Color-3)+i)+*(*(m_Histogram+m_Color-2)+i)+*(*(m_Histogram+m_Color-1)+i))/3;
		}//����ǲ�ɫͼ�񣬽�������ֱ��ͼ��ӣ�����PS�㷨�Ĺؼ�
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
			*(*(t+i)+j)=double(*(*(m_Histogram+i)+j))/(m_Width*m_Height);//��һ��ֱ��ͼ
		}
	}

	for(int i=0;i<m_Color;i++)
	{
		for(int j=1;j<256;j++)
		{
			*(*(t+i)+j)+=*(*(t+i)+j-1);//�ۼ�ֱ��ͼ
		}
	}

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<256;j++)
		{
			*(*(t+i)+j)=255*(*(*(t+i)+j))+0.5;//����һ���ۼ�ֱ��ͼת��Ϊ����ֵ
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

void Bmp::Threshold(int Threshold)//��ֵ����
{
	if(m_Color!=1||Threshold<0)
	{
		RGB2Gray();
	}//�Ƚ�ͼƬת��Ϊ�Ҷ�ͼ

	for(int i=0;i<m_Color;i++)
	{
		for(int j=0;j<m_Height;j++)
		{
			for(int k=0;k<m_Width;k++)
			{
				*(*(*(m_RGB+i)+j)+k)>=Threshold?*(*(*(m_RGB+i)+j)+k)=255:*(*(*(m_RGB+i)+j)+k)=0;
			}//���ڵ�����ֵ����255���������0
		}
	}
}

int Bmp::OtsuThreshold()//�������ֵ
{
	if(m_Color!=1)
	{
		RGB2Gray();
	}//�Ƚ�ͼƬת��Ϊ�Ҷ�ͼ

	double p[256],u=0,w0=0,w1=0,u0=0,u1=0,g=0,gtmp,Threshold;
	int i,j;
	long HistogramMax=m_Height*m_Width;

	if(!m_Histogram)
	{
		GetHistogram();
	}//���ֱ��ͼ

	for(i=0;i<256;i++)//���������ǰ��մ�򷨵��㷨������
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
	return static_cast<int>(Threshold);//��󷵻صõ�����ֵ
}

int Bmp::IterationThreshold()//����������ֵ
{
	if(m_Color!=1)
	{
		RGB2Gray();
	}//�Ƚ�ͼƬת��Ϊ�Ҷ�ͼ

	double T=128,T_o=0,u0,u1,n0,n1;;//��128��ʼ����
	int i;

	if(!m_Histogram)
	{
		GetHistogram();
	}//���ֱ��ͼ
	while(T!=T_o)//���������ǵ��������㷨�����ˣ���ǰ��������ֵ��Ⱦ�����while
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
	return static_cast<int>(T);//��󷵻ص���������ֵ
}

int Bmp::HalfCutThreshold()//�Լ������Ķ԰����ֵ�㷨��
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

	while (i<=j)//����˵���������ǰ���ۼƵ�����ֵ��Ļ������������ֵ�ͻ���ǰ�ۼƣ���֮һ��
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

void Bmp::Histogram(int Color,int Min,int Max)//ֱ��ͼ��������
{
	if(Min>=Max)
	{
		return;
	}

	double curve[256];
	for(int i=0;i<256;i++)
	{
		curve[i]=min(255,double(255*max(0,i-Min))/(Max-Min));
	}//ͨ�������Ĳ����趨�����ߣ�Ӧ��˵����һ�����Ա仯��ֱ��

	for(int i=0;i<m_Height;i++)
	{
		for(int j=0;j<m_Width;j++)
		{
			*(*(*(m_RGB+Color)+i)+j)=static_cast<unsigned char>(*(curve+*(*(*(m_RGB+Color)+i)+j)));
		}//Ȼ�������򵥵����߱任��
	}
}

void Bmp::RGB2Gray()//ʹ��ɫͼ��ת��Ϊ�Ҷ�ͼ
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
		}//ͨ��ת����ʽ��RGB����ֵ����һ������ת��Ϊ�Ҷ�
	}

	for(int i=0;i<m_Color;i++)//Ȼ�����һ�ѵ��ڴ������
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

	m_RGB=Gray;//���µĻҶ�ͼ�ĵ�ַ��ͼƬ

	m_ColorTable=new RGBQUAD[256];//�趨�µĵ�ɫ�壬�����µ�ͼƬ���������ͼƬ���ļ�ͷ����Ϣͷ
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

void Bmp::Zoom(int TargetHeight,int TargetWidth,int Method)//ͼ�����ţ��������ŷ�ʽ
	//Method��0��near 1��linear 2��cube
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

	if(Method==0)//�ڽ�
	{
		for(int i=0;i<m_Color;i++)
		{
			for(int j=0;j<TargetHeight;j++)
			{
				for(int k=0;k<TargetWidth;k++)
				{
					*(*(*(RGB+i)+j)+k)=*(*(*(m_RGB+i)+j*m_Height/TargetHeight)+k*m_Width/TargetWidth);
				}//�ڽ�Ӧ�ò��ý�����
			}
		}
	}
	else if(Method==1)//��������
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
				}//�������Ծ�������һ�κ���ĵı���������һ������ı���
			}
		}
	}
	else if(Method==2)//��������
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
				}//��������ֱ��������������ʽ��ԭ�����������Բ�࣬�����õ��Ĳ�������
			}
		}
	}

	for(int i=0;i<m_Color;i++)//���Ǿ�����ڴ��ͷŵȲ���
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

	m_LineBit=(TargetWidth*m_Color+3)/4*4;//�����ļ�ͷ��Ϣͷ�������趨
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

void Bmp::DFT()//��ɢ����Ҷ�任
{
	int i,j,k,m,n,u,v,RGB;//������ֲ���
	double real1,image1,big=0;
	Complex w_h0(cos(PI_2/m_Height),-sin(PI_2/m_Height)),w_w0(cos(PI_2/m_Width),-sin(PI_2/m_Width));
	//���еĸ���Ҷ�任ϵ������������������w_h0��w_w0������ڤ�����������ȶ�ϵ�����м��㣺
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
	//����Ϊȷ������Ҷ�任���������濪��һ����ʱ�洢�ռ�
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
	//���ٸ����ռ�洢�������
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
	//������ʱ��������ڼ���
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
					{//���ݻ���Ĺ�ʽ���н������
						v=n*k;
						tmpComplex1.real+=(*(tmpW_W+v)).real*(RGB=(*(*(*(m_RGB+i)+j)+k)));
						tmpComplex1.image+=(*(tmpW_W+v)).image*RGB;
					}//�ٶ��н������
					u=m*j;
					real1=(*(tmpW_H+u)).real;
					image1=(*(tmpW_H+u)).image;
					tmpComplex.real+=real1*tmpComplex1.real-image1*tmpComplex1.image;
					tmpComplex.image+=real1*tmpComplex1.image+image1*tmpComplex1.real;
				}
				tmp[i][m][n]=tmpComplex.GetAmplitude();//�����ֵ������ʾ
				tmpFFTComplex=&(*(*(m_FFTComplex+i)+m*m_Width+n));//�洢���������Ժ���
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
				*(*(*(tmp+i)+j)+k)=log10(*(*(*(tmp+i)+j)+k));//ȡ����������ʾ
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
				*(*(*(m_RGB+i)+j)+k)=static_cast<unsigned char>(*(*(*(tmp+i)+j)+k)*255/big);//�����ݷ�Χ������0-255����
			}
		}
	}

	int tmpRGB;//���Ļ���ֵͼ
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

void Bmp::FFT()//���ٸ���Ҷ�任
{
	int i,j,k;
	double big=0;
	COMPLEX **complexOrg=new COMPLEX*[m_Color];//���ڴ�����ԭʼ������Ŀ�����ݵĸ����ռ�
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
		FFT2(*(complexOrg+i),m_Width,m_Height,*(complexDes+i));//����FFT����
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				tmp[i][j][k]=log10(sqrt((*(*(complexDes+i)+j*m_Width+k)).re*(*(*(complexDes+i)+j*m_Width+k)).re+(*(*(complexDes+i)+j*m_Width+k)).im*(*(*(complexDes+i)+j*m_Width+k)).im));
				big<tmp[i][j][k]?big=tmp[i][j][k]:NULL;
			}//�����Ƶ�򣬲�ȡ����
		}
	}

	for(i=0;i<m_Color;i++)
	{
		for(j=0;j<m_Height;j++)
		{
			for(k=0;k<m_Width;k++)
			{
				
				m_RGB[i][j][k]=static_cast<unsigned char>(tmp[i][j][k]*255/big);//Ȼ����һ��ֱ��ͼ����Ŵ�Աȶ�
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
			}//���Ļ�
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

void Bmp::iFFT()//���ٸ���Ҷ���任����FFT����
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

COMPLEX** Bmp::GetFFTComplex()//���ظ���Ҷ�任��ĸ�������
{
	return m_FFTComplex;
}

void Bmp::Morphology(int Type,bool** filter,int threshold)//��̬ѧ���㣬0:Dilation 1:Erosion
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
						{//���͵Ļ����㷨��Ҫ�ѽṹ���ӷ�����
							if(*(*(filter+8-l)+8-m))//����ṹ����Ϊ1���Ž��������Խ�ʡʱ��
								if(*(*(*(tmp+i)+j)+k)<(a=(*(*(*(m_RGB+i)+max(0,min(m_Height-1,j+l-4)))+max(0,min(m_Width-1,k+m-4))))))
									*(*(*(tmp+i)+j)+k)=a;//����ýṹ�����ڵ����ֵ
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
							if(*(*(filter+l)+m))//����ṹ����Ϊ1���Ž��������Խ�ʡʱ��
								if(*(*(*(tmp+i)+j)+k)>(a=(*(*(*(m_RGB+i)+max(0,min(m_Height-1,j+l-4)))+max(0,min(m_Width-1,k+m-4))))))
									*(*(*(tmp+i)+j)+k)=a;//����ýṹ�����ڵ���Сֵ
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
			}//����趨����ֵ�����ͼ����ж�ֵ��
			delete(*(*(tmp+i)+j));
		}
		delete(*(tmp+i));
	}
	free(tmp);
}

void Bmp::BkRGBMinusRGB(bool reverse)//����������ֵ��ͼ������ֵ���
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

bool Bmp::Cut(CPoint pt_lu,CPoint pt_rd)//ͼ��ü�
{
	long h1=pt_lu.y,h2=pt_rd.y,w1=pt_lu.x,w2=pt_rd.x,tmp;//��òü�����
	
	if(h1>=m_Height||h2>=m_Height||w1>=m_Width||w2>=m_Width||h1==h2||w1==w2)
	{
		return false;//����ü����򲻷���Ҫ����ȡ���ü�
	}

	if(h1>h2)//�����ü����򣬾���С����ǰ����ں�
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

	long width=w2-w1,height=h2-h1;//����ü�����Ŀ����߶�

	unsigned char ***RGB=new unsigned char **[m_Color];
	for(int i=0;i<m_Color;i++)
	{
		RGB[i]=new unsigned char *[height];
		for(int j=0;j<height;j++)
		{
			RGB[i][j]=new unsigned char [width];
			for(int k=0;k<width;k++)
			{
				RGB[i][j][k]=m_RGB[i][h1+j][w1+k];//����ü������С���ڴ�ռ䣬���ҽ�����ֵ����ת��
			}
		}
	}

	for(int i=0;i<m_Color;i++)//������ڴ����
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

	m_RGB=RGB;//���ü��������ֵ��ַ�����ͼ��

	m_LineBit=(width*m_Color+3)/4*4;//���о�����ļ�ͷ��Ϣͷ�趨
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