#pragma once
#include "Complex.h"
#include <math.h>

class Bmp
{
public:
	Bmp(void);//���캯�������ڳ�ʼ��һ���µ�Bmp��
	Bmp(char* FileName);//�����ļ���ַ�Ĺ��캯�������ڳ�ʼ��һ���µ�Bmp��
	~Bmp(void);//��������
	bool Open(const char* FileName);//���ļ�����������Ϊ�ļ���ַ
	bool Save(const char* FileName);//�����ļ�����������Ϊ�ļ���ַ
	bool SaveAsTxt(const char* FileName);//��Ϊ�ı�����������Ϊ�ļ���ַ
	bool Close();//�رպ��������ڹر�һ��ͼƬʱ���ڴ��ͷ�
	bool Exist();//���ں�������������Ѿ���һ��ͼƬ�������棬����Ϊ��
	long GetWidth();//����ͼƬ�Ŀ�
	long GetHeight();//����ͼƬ�ĸ�
	long GetColor();//����ͼƬ����ɫ�������Ҷ�ͼΪ1��24λͼΪ3��32λͼΪ4
	unsigned char GetRGB(long color,long x,long y);//����ͼƬ����λ�õ�����ֵ
	void SetRGB(long color,long x,long y,unsigned char RGB);//�趨ͼƬ����λ�õ�����ֵ
	unsigned char* GetBmpBuf();//����ת��bmp��ʽ�洢��ʽΪ����ʹ�õĴ洢��ʽǰ�Ļ���
	unsigned char* GetBmpBufTrans();//���ڽ���������ֵ�洢��ʽת��Ϊ��ʾ�ڴ����еĴ洢��ʽ
	void ReverseColor();//��ɫ����
	void ReverseLR();//���ҷ�ת����
	void ReverseUD();//���·�ת����
	void Reverse180();//180�ȷ�ת����
	void SmoothAverage(int x,int y);//ƽ��ģ������
	void BackUpRGB();//����ͼƬ����ֵ�����ڱ���ͼƬ
	void RestoreRGB();//���ر��ݵ�ͼƬ����ֵ�����ڻ�ԭͼƬ
	void SmoothMedian(int x,int y);//��ֵģ������
	void FilterDefine(double filter[5][5],double divide,int move);//�Զ����˲�������
	void GaussSmooth(int r, double sigma = 1);//��˹ģ��
	void BilateralFilter(int r, double sigma1 = 1, double sigma2 = 1);//˫���˲�
	void CurveFunction(int color,int curve[256]);//���ߺ���������ʵ��ͼƬ�����任
	int** GetHistogram();//ֱ��ͼ���������ڻ��ͼƬ��ֱ��ͼ
	COMPLEX** GetFFTComplex();//�������������ڻ��FFT��ĸ�������
	void HistogramEqualization();//ֱ��ͼ���⻯��ʵ��ɫ�������㷨
	void Threshold(int Threshold);//��ֵ����
	int OtsuThreshold();//�õ���򷨵���ֵ
	int IterationThreshold();//�õ���������ֵ
	int HalfCutThreshold();//�õ��԰�ַ���ֵ
	void Histogram(int Color,int Min,int Max);//ֱ��ͼ�任������ʵ��ֱ��ͼ���������
	void RGB2Gray();//��ɫת�ҶȺ���
	void Zoom(int TargetHeight,int TargetWidth,int Method);//���ź�����Method��0��near 1��linear 2��cube
	void DFT();//��ɢ����Ҷ�任����
	void iDFT();//����ɢ����Ҷ�任����
	void FFT();//���ٸ���Ҷ�任����
	void iFFT();//�����ٸ���Ҷ�任����
	void Morphology(int Type,bool** filter,int threshold);//��̬ѧ���㣬0:Dilation 1:Erosion
	void BkRGBMinusRGB(bool reverse=0);//��������ֵ��ȥͼƬ����ֵ�����ڼ����Ե
	bool Cut(CPoint pt_lu,CPoint pt_rd);//�ü�����
	BITMAPFILEHEADER GetBmpFileHeader();//����ͼƬ���ļ�ͷ
	BITMAPINFOHEADER GetBmpInfoHeader();//����ͼƬ����Ϣͷ
private:
	BITMAPFILEHEADER m_BmpFileHeader;//ͼƬ���ļ�ͷ
	BITMAPINFOHEADER m_BmpInfoHeader;//ͼƬ����Ϣͷ
	RGBQUAD *m_ColorTable;//�洢ͼƬ�ĵ�ɫ��
	COMPLEX **m_FFTComplex;//�洢ͼƬ�ĸ���Ҷ�任��ĸ���
	unsigned char *m_BmpBuf;//��Bmp��ʽ��˳��洢��ͼƬ
	unsigned char ***m_RGB;//�Ա������ܹ�����ķ�ʽ�洢ͼƬ������ɫ�㣬�ߣ�������������洢ͼƬ
	unsigned char ***m_RGB_Bk;//ͼƬ�ı���
	int **m_Histogram;//�洢ͼƬ��ֱ��ͼ
	long m_Color;//ͼƬ����ɫ�������Ҷ�ͼΪ1��24λͼΪ3��32λͼΪ4
	long m_Height;//ͼƬ�ĸ�
	long m_Width;//ͼƬ�Ŀ�
	long m_LineBit;//ͼƬһ���е��ֽ���
};

