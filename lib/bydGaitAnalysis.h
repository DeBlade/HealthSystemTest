#ifndef _BYD_GAIT_ANALYSIS_H_
#define _BYD_GAIT_ANALYSIS_H_

#include <vector>
using namespace std;

#define BYD_EXPORTS __declspec(dllexport)

/* ͼ������ */
enum ImageType
{
	IMG_GRAY,
	IMG_RGB,
	IMG_YUV,
};

/* ��̬��Ƶ�����Ƕ� */
enum
{
	PLANE_SAGGITAL,		//ʧ״��
	PLANE_FRONTAL,		//��״��
};

typedef struct MyPoint
{
	float x;
	float y;
}MyPoint;

/* ͼ��ṹ */
typedef struct Image
{
	int cols;	//��������
	int rows;	//��������
	int type;	//���ͣ���ImageType
	char* data;	//����ָ��
}Image;

/* ��̬����֮ʱ������ */
typedef struct TimeSpaceDsp
{
	float srideLength;			//�粽�����ף�
	float stepLength;			//�������ף�
	float cadence;				//��Ƶ������/����
	float walkingSpeed;			//���٣���/�룩
	float stepWidth;			//����
	float footAngle;			//��ƫ��
}TimeSpaceDsp;

/* ��̬����֮�ؽڽǶ� */
typedef struct JointAngle
{
	vector<float> hipAngle;		//�ŹؽڽǶ�
	vector<float> kneeAngle;	//ϥ�ؽڽǶ�
	vector<float> ankleAngle;	//�׹ؽڽǶ�
}JointAngle;

/* ��̬���� */
typedef struct GaitParams
{
	TimeSpaceDsp timeSpaceDsp;				//ʱ�ղ���
	JointAngle jointAngle;					//ʧ״��Ƕ�
	vector<vector<MyPoint> > markPoints;	//���б�ǵ�
}GaitParams;

class BYD_EXPORTS GaitAnalysis
{
public:
	GaitAnalysis();
	~GaitAnalysis();

	/***********************************************************************
		��̬����API����ʾ�����̣�
		init();
		analysis();
	************************************************************************/

	/*
		[funcion]:init -- ��ʼ��.
		[in]:configFilePath -- ��config.ini���ļ�·����Ĭ�ϵ�ǰִ���ļ�Ŀ¼
		[return]: 0 -- ok, else -- error
	*/
	int init(string configFilePath = ".\\config.ini");

	/*
		[function]:getReferenceParam -- ��ȡ�ο���̬����������ָ���ǹؽڽǶ�
		[out]:angelRef -- �ο��Ƕ�
		[in]:planeType -- ��̬��Ƶ�����Ƕ�
	*/
	void getReferenceParam(JointAngle& angelRef, int planeType);

	/* 
		[function]:analysis -- ��ȡ��Ƶ�����߷���
		[in]:videoPath -- ��Ƶ·��
		[out]:params -- ��̬����
		[in]:planeType -- ��̬��Ƶ�����Ƕ� "PLANE_SAGGITAL or PLANE_FRONTAL"
		[in]:bShowDetails -- ��ʾ�м䴦��ϸ��
		[return]: 0 -- ok, else -- error
	*/
	int analysis(string videoPath, GaitParams& params, int planeType, bool bShowDetails = false);

private:
	//�ڲ�����ָ�룬�������
	void* m_pHandler;
};

#endif