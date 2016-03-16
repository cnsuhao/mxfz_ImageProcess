#pragma once
#include "Complex.h"
#include <math.h>

class Bmp
{
public:
	Bmp(void);//构造函数，用于初始化一个新的Bmp类
	Bmp(char* FileName);//带有文件地址的构造函数，用于初始化一个新的Bmp类
	~Bmp(void);//析构函数
	bool Open(const char* FileName);//打开文件函数，参数为文件地址
	bool Save(const char* FileName);//保存文件函数，参数为文件地址
	bool SaveAsTxt(const char* FileName);//存为文本函数，参数为文件地址
	bool Close();//关闭函数，用于关闭一张图片时的内存释放
	bool Exist();//存在函数，如果该类已经打开一张图片，返回真，否则为假
	long GetWidth();//返回图片的宽
	long GetHeight();//返回图片的高
	long GetColor();//返回图片的颜色层数，灰度图为1，24位图为3，32位图为4
	unsigned char GetRGB(long color,long x,long y);//返回图片具体位置的像素值
	void SetRGB(long color,long x,long y,unsigned char RGB);//设定图片具体位置的像素值
	unsigned char* GetBmpBuf();//用于转换bmp格式存储方式为该类使用的存储方式前的缓存
	unsigned char* GetBmpBufTrans();//用于将该类像素值存储方式转换为显示在窗口中的存储方式
	void ReverseColor();//反色函数
	void ReverseLR();//左右翻转函数
	void ReverseUD();//上下翻转函数
	void Reverse180();//180度翻转函数
	void SmoothAverage(int x,int y);//平均模糊函数
	void BackUpRGB();//备份图片像素值，用于备份图片
	void RestoreRGB();//加载备份的图片像素值，用于还原图片
	void SmoothMedian(int x,int y);//中值模糊函数
	void FilterDefine(double filter[5][5],double divide,int move);//自定义滤波器函数
	void GaussSmooth(int r, double sigma = 1);//高斯模糊
	void BilateralFilter(int r, double sigma1 = 1, double sigma2 = 1);//双边滤波
	void CurveFunction(int color,int curve[256]);//曲线函数，用于实现图片函数变换
	int** GetHistogram();//直方图函数，用于获得图片的直方图
	COMPLEX** GetFFTComplex();//复数函数，用于获得FFT后的复数数据
	void HistogramEqualization();//直方图均衡化，实现色调均化算法
	void Threshold(int Threshold);//阈值函数
	int OtsuThreshold();//得到大津法的阈值
	int IterationThreshold();//得到迭代法阈值
	int HalfCutThreshold();//得到对半分法阈值
	void Histogram(int Color,int Min,int Max);//直方图变换函数，实现直方图的拉伸操作
	void RGB2Gray();//彩色转灰度函数
	void Zoom(int TargetHeight,int TargetWidth,int Method);//缩放函数，Method：0：near 1：linear 2：cube
	void DFT();//离散傅里叶变换函数
	void iDFT();//反离散傅里叶变换函数
	void FFT();//快速傅立叶变换函数
	void iFFT();//反快速傅立叶变换函数
	void Morphology(int Type,bool** filter,int threshold);//形态学运算，0:Dilation 1:Erosion
	void BkRGBMinusRGB(bool reverse=0);//备份像素值减去图片像素值，用于计算边缘
	bool Cut(CPoint pt_lu,CPoint pt_rd);//裁剪函数
	BITMAPFILEHEADER GetBmpFileHeader();//返回图片的文件头
	BITMAPINFOHEADER GetBmpInfoHeader();//返回图片的信息头
private:
	BITMAPFILEHEADER m_BmpFileHeader;//图片的文件头
	BITMAPINFOHEADER m_BmpInfoHeader;//图片的信息头
	RGBQUAD *m_ColorTable;//存储图片的调色板
	COMPLEX **m_FFTComplex;//存储图片的傅里叶变换后的复数
	unsigned char *m_BmpBuf;//以Bmp格式的顺序存储的图片
	unsigned char ***m_RGB;//以本程序能够处理的方式存储图片，以颜色层，高，宽三个层次来存储图片
	unsigned char ***m_RGB_Bk;//图片的备份
	int **m_Histogram;//存储图片的直方图
	long m_Color;//图片的颜色层数，灰度图为1，24位图为3，32位图为4
	long m_Height;//图片的高
	long m_Width;//图片的宽
	long m_LineBit;//图片一行中的字节数
};

