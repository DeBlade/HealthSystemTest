#ifndef _BYD_GAIT_ANALYSIS_H_
#define _BYD_GAIT_ANALYSIS_H_

#include <vector>
using namespace std;

#define BYD_EXPORTS __declspec(dllexport)

/* 图像类型 */
enum ImageType
{
	IMG_GRAY,
	IMG_RGB,
	IMG_YUV,
};

/* 步态视频分析角度 */
enum
{
	PLANE_SAGGITAL,		//失状面
	PLANE_FRONTAL,		//额状面
};

typedef struct MyPoint
{
	float x;
	float y;
}MyPoint;

/* 图像结构 */
typedef struct Image
{
	int cols;	//列数，宽
	int rows;	//行数，高
	int type;	//类型，见ImageType
	char* data;	//数据指针
}Image;

/* 步态参数之时空描述 */
typedef struct TimeSpaceDsp
{
	float srideLength;			//跨步长（米）
	float stepLength;			//步长（米）
	float cadence;				//步频，步数/分钟
	float walkingSpeed;			//步速（米/秒）
	float stepWidth;			//步宽
	float footAngle;			//足偏角
}TimeSpaceDsp;

/* 步态参数之关节角度 */
typedef struct JointAngle
{
	vector<float> hipAngle;		//髋关节角度
	vector<float> kneeAngle;	//膝关节角度
	vector<float> ankleAngle;	//踝关节角度
}JointAngle;

/* 步态参数 */
typedef struct GaitParams
{
	TimeSpaceDsp timeSpaceDsp;				//时空参数
	JointAngle jointAngle;					//失状面角度
	vector<vector<MyPoint> > markPoints;	//所有标记点
}GaitParams;

class BYD_EXPORTS GaitAnalysis
{
public:
	GaitAnalysis();
	~GaitAnalysis();

	/***********************************************************************
		步态分析API调用示例流程：
		init();
		analysis();
	************************************************************************/

	/*
		[funcion]:init -- 初始化.
		[in]:configFilePath -- “config.ini”文件路径，默认当前执行文件目录
		[return]: 0 -- ok, else -- error
	*/
	int init(string configFilePath = ".\\config.ini");

	/*
		[function]:getReferenceParam -- 获取参考步态参数，这里指的是关节角度
		[out]:angelRef -- 参考角度
		[in]:planeType -- 步态视频分析角度
	*/
	void getReferenceParam(JointAngle& angelRef, int planeType);

	/* 
		[function]:analysis -- 读取视频，离线分析
		[in]:videoPath -- 视频路径
		[out]:params -- 步态参数
		[in]:planeType -- 步态视频分析角度 "PLANE_SAGGITAL or PLANE_FRONTAL"
		[in]:bShowDetails -- 显示中间处理细节
		[return]: 0 -- ok, else -- error
	*/
	int analysis(string videoPath, GaitParams& params, int planeType, bool bShowDetails = false);

private:
	//内部管理指针，无需关心
	void* m_pHandler;
};

#endif