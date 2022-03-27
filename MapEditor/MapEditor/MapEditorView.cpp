
// MapEditorView.cpp: CMapEditorView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapEditor.h"
#endif

#include "MapEditorDoc.h"
#include "MapEditorView.h"
#include"CreateFileDig.h"
#include"Paint.h"
#include"WriteOrRead.h"
#include"Calculate.h"
#include"CPointParameterDlg.h"
#include"CLinParameterDlg.h"
#include"CRegParameterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapEditorView
///------------------点数据相关的全局控制变量----------------------///
bool GPntFCreated = false; //临时点文件是否创建
CString GPntFName; //永久文件名（含路径）
CString GPntTmpFName = CString("tempPntF.dat"); //临时文件名（含路径）
bool GPntchanged = false;//是否更改
int GPntNum  = 0; //物理数
int GPntLNum = 0; //逻辑数
CFile* GPntTmpF = new CFile(); //读取临时文件的指针对象

///------------------线数据相关的全局控制变量----------------------///
bool GLinFCreated = false; //临时线文件是否创建
CString GLinFName; //永久文件名（含路径）
CString GLinTmpNdxFName = CString("tempLinF.ndx");//临时索引文件名（含路径）
CString GLinTmpDatFName = CString("tempLinF.dat");//临时数据文件名（含路径）
bool GLinChanged = false; //是否更改
int GLinNum = 0; //物理数
int GLinLNum = 0; //逻辑数
CFile* GLinTmpDatF = new CFile(); //读取临时数据文件的指针对象
CFile* GLinTmpNdxF = new CFile(); //读取临时索引文件的指针对象

///------------------区数据相关的全局控制变量----------------------///
bool GRegFCreated = false; //临时区文件是否创建
CString GRegFName; //永久文件名（含路径）
CString GRegTmpNdxFName = CString("tempRegF.ndx");//临时索引文件名（含路径）
CString GRegTmpDatFName = CString("tempRegF.dat");//临时数据文件名（含路径）
bool GRegChanged = false; //是否更改
int GRegNum = 0; //物理数
int GRegLNum = 0; //逻辑数
CFile* GRegTmpDatF = new CFile(); //读取临时数据文件的指针对象
CFile* GRegTmpNdxF = new CFile(); //读取临时索引文件的指针对象

///------------------默认线索引结构、临时线索引结构及其相关----------------------///
LIN_NDX_STRU GLin = { GLin.isDel = 0, GLin.color = RGB(0, 0, 0), GLin.pattern = 0,GLin.dotNum = 0, GLin.datOff = 0 }; //默认线参数
LIN_NDX_STRU GTLin; //线
POINT GLPnt = { GLPnt.x = -1 , GLPnt.y = -1 }; //记录线段的起点
CPoint GMPnt(-1 , -1); //记录鼠标上一状态的点

///------------------与连接线相关-----------------/// 
LIN_NDX_STRU GStartLin = GLin;//选中的第一条线
int GnStart = -1;
LIN_NDX_STRU GEndLin = GLin;//选中的第二条线
int GnEnd = -1;
int GnLine = 0;//连接的线的数量


///------------------与操作相关-----------------/// 
enum Action {
	Noaction,
	OPERSTATE_INPUT_PNT,
	OPERSTATE_DELETE_PNT,
	OPERSTATE_MOVE_PNT,
	OPERSTATE_INPUT_LIN,
	OPERSTATE_DELETE_LIN,
	OPERSTATE_MOVE_LIN,
	OPERSTATE_INPUT_REG,
	OPERSTATE_DELETE_REG,
	OPERSTATE_MOVE_REG,
	OPERSTATE_ZOOM_IN,
	OPERSTATE_ZOOM_OUT,
	OPERSTATE_WINDOW_MOVE,
	OPERSTATE_LIN_DELETE_PNT,
	OPERSTATE_LIN_ADD_PNT,
	OPERSTATE_LINK_LIN,
	OPERSTATE_MODIFY_POINT_PARAMETER,
	OPERSTATE_MODIFY_LINE_PARAMETER,
	OPERSTATE_MODIFY_REGION_PARAMETER,
	OPERSTATE_UNDELETE_PNT,
	OPERSTATE_UNDELETE_LIN,
	OPERSTATE_UNDELETE_REG
}; //枚举操作状态
Action GCurOperState; //操作参数

//----------------默认点结构与临时结构--------------------///
PNT_STRU GPnt = { GPnt.isDel = 0 , GPnt.color = RGB(0 ,0, 0) , GPnt.pattern = 0 };//默认点参数

/// -----------------造区过程相关的点数据----------------///
CPoint GRegCreateMMPnt(-1, -1); //鼠标移动前一状态点
CPoint GRegCreateStartPnt(-1, -1); //造区的起点

//--------------默认区索引结构、临时索引结构及其相关------------//
REG_NDX_STRU GReg = { GReg.isDel = 0, GReg.color = RGB(0,0, 0) , GReg.pattern = 0,
GReg.dotNum = 0, GReg.datOff = 0 };
REG_NDX_STRU GTReg;

//----------------点文件版本信息---------------///
VERSION GPntVer = {
    GPntVer.flag[0] = 'P',
    GPntVer.flag[1] = 'N',
    GPntVer.flag[2] = 'T',
    GPntVer.version = 10 //默认版本号
};

//----------------线文件版本信息---------------///
VERSION GLinVer = {
    GLinVer.flag[0] = 'L',
    GLinVer.flag[1] = 'I',
    GLinVer.flag[2] = 'N',
    GLinVer.version = 10 //默认版本号
};

//----------------区文件版本信息---------------///
VERSION GRegVer = {
	GRegVer.flag[0] = 'R',
	GRegVer.flag[1] = 'E',
	GRegVer.flag[2] = 'G',
	GRegVer.version = 10 //默认版本号
};

///-----------------移动窗口------------------//
CPoint GWinMoveLBDPnt(-1, -1);//移动窗口时左键按下点
CPoint GWinMoveMMPnt(-1, -1);//移动窗口时鼠标移动前状态点位置

///--------------与显示状态有关的全局变量--------------///
enum State{ SHOWSTATE_UNDEL, SHOWSTATE_DEL }; //枚举显示类型
State GCurShowState = SHOWSTATE_UNDEL; //显示状态,默认为显示非删除状态
bool GShowPnt = true; //当前显示的结构是否为点
bool GShowLin = true;//当前显示的结构是否为线
bool GShowReg = true; //当前显示的结构是否为区


int GPntNdx = -1;//找到的点位于文件中的位置
int GLinNdx = -1;//找到的线位于文件中的位置
int GLineNdx = -1;
int GRegNdx = -1;//找到的区位于文件中的位置
int tempPntNum = 0;//线上找到的点位于线上的位置
PNT_STRU GTPnt;//临时点，存储找到的点数据

CPoint GLinLBDPnt(-1, -1);//记录鼠标左键按下的位置，用于计算偏移量
CPoint GLinMMPnt(-1, -1);//记录鼠标移动前的上一状态，用来擦除移动时的上一条线
long GLinMMOffsetX = 0;//记录鼠标移动时候x轴的偏移量
long GLinMMOffsetY = 0;//记录鼠标移动时候y轴的偏移量
LIN_NDX_STRU GLinMMTmpNdx; //记录鼠标选中的线的索引

REG_NDX_STRU GRegMMTmpNdx; //记录鼠标选中的区的索引
CPoint GRegLBDPnt(-1, -1); //记录鼠标左键按下的位置,用来计算偏移量
CPoint GRegMMPnt(-1, -1); //记录鼠标移动时上一状态,擦除移动时前一-个区
long GRegMMOffsetX = 0; //记录鼠标移动时候的X轴的偏移量
long GRegMMOffsetY = 0; //记录鼠标移动时候的Y轴的偏移量

CPoint GZoomLBDPnt(-1, -1); //放大时鼠标左键抬起的点
CPoint GZoomMMPnt(-1, -1); //放大时鼠标移动前状态
double GZoomOffset_x = 0; //偏移向量
double GZoomOffset_y = 0;
double GZoom = 1.0; //缩放系数
int GZoomStyle = 0; //放大方式

//线上加点时要加点线段的两个端点
D_DOT dot5{ dot5.x = -1,dot5.y = -1 };
D_DOT dot6{ dot6.x = -1,dot6.y = -1 };
//线上加点时要加点线段的两个端点的位置
int num1 = -1, num2 = -1;
//外包矩形的顶点坐标
double GMaxX = 0;
double GMaxY = 0;
double GMinX = 0;
double GMinY = 0;

IMPLEMENT_DYNCREATE(CMapEditorView, CView)

BEGIN_MESSAGE_MAP(CMapEditorView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_WINDOW_ZOOM_IN, &CMapEditorView::OnWindowZoomIn)
	ON_COMMAND(ID_FILE_NEW, &CMapEditorView::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN_POINT, &CMapEditorView::OnFileOpenPoint)
	ON_COMMAND(ID_FILE_OPEN_LINE, &CMapEditorView::OnFileOpenLine)
	ON_COMMAND(ID_FILE_OPEN_REGION, &CMapEditorView::OnFileOpenRegion)
	ON_COMMAND(ID_FILE_SAVE_POINT, &CMapEditorView::OnFileSavePoint)
	ON_COMMAND(ID_FILE_SAVE_LINE, &CMapEditorView::OnFileSaveLine)
	ON_COMMAND(ID_FILE_SAVE_REGION, &CMapEditorView::OnFileSaveRegion)
	ON_COMMAND(ID_FILE_SAVE_AS_POINT, &CMapEditorView::OnFileSaveAsPoint)
	ON_COMMAND(ID_FILE_SAVE_AS_LINE, &CMapEditorView::OnFileSaveAsLine)
	ON_COMMAND(ID_FILE_SAVE_AS_REGION, &CMapEditorView::OnFileSaveAsRegion)
	ON_COMMAND(ID_WINDOW_ZOOM_OUT, &CMapEditorView::OnWindowZoomOut)
	ON_COMMAND(ID_WINDOW_MOVE, &CMapEditorView::OnWindowMove)
	ON_COMMAND(ID_WINDOW_RESET, &CMapEditorView::OnWindowReset)
	ON_COMMAND(ID_WINDOW_SHOW_POINT, &CMapEditorView::OnWindowShowPoint)
	ON_COMMAND(ID_WINDOW_SHOW_LINE, &CMapEditorView::OnWindowShowLine)
	ON_COMMAND(ID_WINDOW_SHOW_REGION, &CMapEditorView::OnWindowShowRegion)
	ON_COMMAND(ID_POINT_CREATE, &CMapEditorView::OnPointCreate)
	ON_COMMAND(ID_POINT_MOVE, &CMapEditorView::OnPointMove)
	ON_COMMAND(ID_POINT_DELETE, &CMapEditorView::OnPointDelete)
	ON_COMMAND(ID_POINT_SHOW_DELETED, &CMapEditorView::OnPointShowDeleted)
	ON_COMMAND(ID_POINT_UNDELETE, &CMapEditorView::OnPointUndelete)
	ON_COMMAND(ID_POINT_MODIFY_PARAMETER, &CMapEditorView::OnPointModifyParameter)
	ON_COMMAND(ID_POINT_SET_DEFPARAMETER, &CMapEditorView::OnPointSetDefparameter)
	ON_COMMAND(ID_LINE_CREATE, &CMapEditorView::OnLineCreate)
	ON_COMMAND(ID_LINE_MOVE, &CMapEditorView::OnLineMove)
	ON_COMMAND(ID_LINE_DELETE, &CMapEditorView::OnLineDelete)
	ON_COMMAND(ID_LINE_SHOW_DELETED, &CMapEditorView::OnLineShowDeleted)
	ON_COMMAND(ID_LINE_UNDELETED, &CMapEditorView::OnLineUndeleted)
	ON_COMMAND(ID_LINE_DELETE_DOT, &CMapEditorView::OnLineDeleteDot)
	ON_COMMAND(ID_LINE_ADD_DOT, &CMapEditorView::OnLineAddDot)
	ON_COMMAND(ID_LINE_LINK, &CMapEditorView::OnLineLink)
	ON_COMMAND(ID_LINE_MODIFY_PARAMETER, &CMapEditorView::OnLineModifyParameter)
	ON_COMMAND(ID_LINE_SET_DEFPARAMETER, &CMapEditorView::OnLineSetDefparameter)
	ON_COMMAND(ID_REGION_CREATE, &CMapEditorView::OnRegionCreate)
	ON_COMMAND(ID_REGION_MOVE, &CMapEditorView::OnRegionMove)
	ON_COMMAND(ID_REGION_DELETE, &CMapEditorView::OnRegionDelete)
	ON_COMMAND(ID_REGION_SHOW_DELETED, &CMapEditorView::OnRegionShowDeleted)
	ON_COMMAND(ID_REGION_UNDELETED, &CMapEditorView::OnRegionUndeleted)
	ON_COMMAND(ID_REGION_MODIFY_PAREMETER, &CMapEditorView::OnRegionModifyParemeter)
	ON_COMMAND(ID_REGION_SET_DEFPARAMETER, &CMapEditorView::OnRegionSetDefparameter)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_APP_EXIT, &CMapEditorView::OnAppExit)
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_POINT, &CMapEditorView::OnUpdateWindowShowPoint)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_LINE, &CMapEditorView::OnUpdateWindowShowLine)
	ON_UPDATE_COMMAND_UI(ID_WINDOW_SHOW_REGION, &CMapEditorView::OnUpdateWindowShowRegion)
	ON_UPDATE_COMMAND_UI(ID_POINT_SHOW_DELETED, &CMapEditorView::OnUpdatePointShowDeleted)
	ON_UPDATE_COMMAND_UI(ID_LINE_SHOW_DELETED, &CMapEditorView::OnUpdateLineShowDeleted)
	ON_UPDATE_COMMAND_UI(ID_REGION_SHOW_DELETED, &CMapEditorView::OnUpdateRegionShowDeleted)
END_MESSAGE_MAP()

// CMapEditorView 构造/析构

CMapEditorView::CMapEditorView() noexcept
{
	// TODO: 在此处添加构造代码

}

CMapEditorView::~CMapEditorView()
{
}

BOOL CMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMapEditorView 绘图

void CMapEditorView::OnDraw(CDC* /*pDC*/)
{
	CMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect mrect;
	GetClientRect(&mrect); //获取窗口客户区的坐标
	CClientDC	dc(this);
	dc.FillSolidRect(0, 0, mrect.Width(), mrect.Height(), dc.GetBkColor ());//用一单色填充个矩形
	dc.SetROP2(R2_NOTXORPEN); //设置绘图模式，像素是 R2_XORPEN 颜色(最终像素的反=非(钢笔"异或"屏幕像素))
	//ShowAllPnt(&dc, GPntTmpF, GPntNum); //绘制显示所有点
	//ShowAllLin(&dc,GLinTmpNdxF, GLinTmpDatF, GLinNum); //绘制显示所有线
	//绘制显示所有点
	if (GShowPnt)
	{
		ShowAllPnt(&dc, GPntTmpF, GPntNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	}
	//绘制显示所有线
	if (GShowLin)
	{
		ShowAllLin(&dc, GLinTmpNdxF, GLinTmpDatF, GLinNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	}
	//绘制显示所有区
	if (GShowReg)
	{
		ShowAllReg(&dc, GRegTmpNdxF, GRegTmpDatF, GRegNum, GZoomOffset_x, GZoomOffset_y, GZoom, GCurShowState);
	}
	ReleaseDC(&dc); //释放dc

	// TODO: 在此处为本机数据添加绘制代码
}

void CMapEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	CClientDC dc(this);
	dc.SetROP2(R2_NOTXORPEN);//异或模式
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN://画线状态
			if (GTLin.dotNum > 1)//按右键时节点数大于1，则绘制出线
			{
				WriteLinNdxToFile(GLinTmpNdxF, GLinNum, GTLin); //将线索引写入线临时索引文件中
				GLinNum++;//线物理数加一
				GLinLNum++;//线逻辑数加一
				DrawSeg(&dc, GTLin, GLPnt, point);//默认模式画线
				GLin.datOff += (GTLin.dotNum * sizeof(D_DOT));//线节点坐标数据储存位置更改
				memset(&GTLin, 0, sizeof(LIN_NDX_STRU));//将缓冲区设置为指定的字符:指向目标的指针,要设置的字符,字符数
				GMPnt.SetPoint(-1, -1);
				GLPnt.x = -1;
				GLPnt.y = -1;
			}
			else if (GTLin.dotNum == 1)//按右键时节点数为1，无法绘制出线
			{
				DrawSeg(&dc, GTLin, GLPnt, point);
				memset(&GTLin, 0, sizeof(LIN_NDX_STRU));
				GMPnt.SetPoint(-1, -1);
				GLPnt.x = -1;
				GLPnt.y = -1;
			}
			break;
		case OPERSTATE_LIN_ADD_PNT://线上加点
		{
			
			POINT pnt5, pnt6;
			D_DOT dot,tdot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（窗口转数据）
			DotToPnt(point, dot);
			//线段的两个端点由Dot转Pnt
			DotToPnt(pnt5, dot5);
			DotToPnt(pnt6, dot6);
			if (GLineNdx != -1)
			{
				ReadTempFileToLinNdx(GLinTmpNdxF, GLineNdx, GLinMMTmpNdx);//读取线索引
				if (GLineNdx == GLinLNum - 1)//要加点的线是最后一条线
				{
					//讲要加点的数据加入节点文件中
					for (int i = GLinMMTmpNdx.dotNum - 2; i >= num2; i--)
					{
						//读取要加节点位置后面的点并往后移一位
						ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, tdot);
						WriteLinDatToFile(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, tdot);
					}
					//讲要加的点数据写在num2的位置
					WriteLinDatToFile(GLinTmpDatF, GLinMMTmpNdx.datOff, num2, dot);
					//更新线索引数据
					UpdateLin(GLinTmpNdxF, GLineNdx, GLinMMTmpNdx);
					//更新线节点数据
					UpdateLin(GLinTmpNdxF, GLinTmpDatF, GLineNdx, 0, 0);
				}
				else//要加点的线不是最后一条
				{
					LIN_NDX_STRU LinTmpNdx,TmpLinNdx,TmpLinNdx1;
					ReadTempFileToLinNdx(GLinTmpNdxF, GLinNum - 1, LinTmpNdx);//读取线索引文件中最后一条线的线索引
					for (int i = 0; i < GLinMMTmpNdx.dotNum; i++)
					{
						if (i <= num1)
						{
							//num1及num1之前的点，存放到线的点临时文件的末尾
							ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, tdot);//读取num1及之前的节点
							WriteLinDatToFile(GLinTmpDatF, LinTmpNdx.datOff + ((LinTmpNdx.dotNum) * sizeof(D_DOT)), i, tdot);//重新写入到文件末尾
						}
						else if (i == num2)
						{
							//写鼠标当前位置（已转换坐标系）到num2的位置
							WriteLinDatToFile(GLinTmpDatF, LinTmpNdx.datOff + ((LinTmpNdx.dotNum) * sizeof(D_DOT)), i, dot);
						}
						else
						{
							//原线num2及num2之后的点都往后移一位
							ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i - 1, tdot);//读取原线num2及num2之后的点
							WriteLinDatToFile(GLinTmpDatF, LinTmpNdx.datOff + ((LinTmpNdx.dotNum) * sizeof(D_DOT)), i, tdot);//重新写入到点临时文件末尾
						}
					}
					ReadTempFileToLinNdx(GLinTmpNdxF, GLineNdx, TmpLinNdx);//读取原线索引
					TmpLinNdx.isDel = 1;//设置该线为已删除
					TmpLinNdx.dotNum--;//线节点数减一（LBUTTONUP中将节点数加一，这里恢复到原样）
					UpdateLin(GLinTmpNdxF, GLineNdx, TmpLinNdx);//更新原线索引数据
					UpdateLin(GLinTmpNdxF, GLinTmpDatF, GLineNdx, 0, 0);//更新原线节点数据
					GLinNum++;//线物理数加一
					memcpy_s(&TmpLinNdx1, sizeof(LIN_NDX_STRU), &GLinMMTmpNdx, sizeof(LIN_NDX_STRU));//复制GLinMMTmpNdx的内容给TmpLinNdx1
					TmpLinNdx1.datOff = LinTmpNdx.datOff + ((LinTmpNdx.dotNum) * sizeof(D_DOT));//数据储存位置更改
					TmpLinNdx1.isDel = 0;//该线不删除
					WriteLinNdxToFile(GLinTmpNdxF, GLinNum, TmpLinNdx1);//将这条线索引写到线索引文件末尾
					UpdateLin(GLinTmpNdxF, GLinNum - 1, TmpLinNdx1);//更新该线的数据
					UpdateLin(GLinTmpNdxF, GLinTmpDatF, GLinNum-1, 0, 0);//更新该线节点数据
				}
				//数据重置
				dot5.x = -1;
				dot5.y = -1;
				dot6.x = -1;
				dot6.y = -1;
				GLineNdx = -1;
				GMPnt.x = -1;
				GMPnt.y = -1;
				this->InvalidateRect(NULL);//重绘窗口
			}
			break; 
		}
		default:
			break;
		}
	}
	if (GRegFCreated)//区临时文件已创建
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_REG://当前为造区操作
			if (GTReg.dotNum > 2)
			{
				//边界节点数大于2，可画区
				WriteRegNdxToFile(GRegTmpNdxF, GRegNum, GTReg);//区索引数据写入区临时文件
				++GRegNum;//区物理数加一
				++GRegLNum;//区逻辑数加一
				POINT* pt = new POINT[3];
				D_DOT dot;
				//从区临时文件读取第一个节点数据并进行坐标转换（数据转坐标）
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, 0, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[0], dot);
				//从区临时文件读取最后一个节点数据并进行坐标转换（数据转坐标）
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff,GTReg.dotNum - 1, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[1], dot);
				pt[2] = point;//第三个为鼠标点
				//画区
				DrawReg(&dc, GTReg, pt, 3);
				delete[] pt;
				GReg.datOff += (GTReg.dotNum * sizeof(D_DOT));//储存位置更新
				memset(&GTReg, 0, sizeof(REG_NDX_STRU));//将GTReg所有元素设为0
				GRegCreateMMPnt = CPoint(-1, -1);
				GRegCreateStartPnt = CPoint(-1, -1);
			}
			else if (GTReg.dotNum == 2)
			{
				//边界节点数为2，无法画区
				POINT* pt = new POINT[3];
				D_DOT dot;
				//从区临时文件读取第一个节点数据并进行坐标转换（数据转坐标）
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, 0, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[0], dot);
				//从区临时文件读取最后一个节点数据并进行坐标转换（数据转坐标）
				ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff,GTReg.dotNum - 1, dot);
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);
				DotToPnt(pt[1], dot);
				//pt数组第三个点为鼠标
				pt[2] = point;
				//画区
				DrawReg(&dc, GTReg, pt, 3);
				delete[] pt;
				memset(&GTReg, 0, sizeof(REG_NDX_STRU));//将GTReg所有元素设为0
				GRegCreateMMPnt = CPoint(-1, -1);
				GRegCreateStartPnt = CPoint(1, 1);
			}
			else if (GTReg.dotNum == 1)
			{
				LIN_NDX_STRU tln = { tln.pattern = GTReg.pattern, tln.color =GTReg.color };
				DrawSeg(&dc, tln, GRegCreateStartPnt, GRegCreateMMPnt);
				memset(&GTReg, 0, sizeof(REG_NDX_STRU));
				GRegCreateMMPnt = CPoint(-1, -1);
				GRegCreateStartPnt = CPoint(-1, -1);
			}
			break;
		default:
			break;
		}
	}
}

void CMapEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMapEditorView 诊断

#ifdef _DEBUG
void CMapEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditorDoc* CMapEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditorDoc)));
	return (CMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMapEditorView 消息处理程序





void CMapEditorView::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated && GLinFCreated && GRegFCreated)
	{
		MessageBox(L"文件已经被创建！", L" Message", MB_OK);
		return;
	}
	CCreateFileDig dig; //创建“新建临时文件”对话框对象
		if (dig.DoModal() != IDOK)//判断当前操作是否为“确定”按钮．不是则返回
			return;
		CString str; //创建输出信息对象
		if (!GPntFCreated)//判断点临时文件是否存在，不存在则新建
		{
			GPntTmpFName = dig.m_add + CString("\\") + GPntTmpFName; //临时点数据文件名
			if (GPntTmpF->Open(GPntTmpFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))//创建新临时文件，以读写形式打开，以二进制形式打开
			{
				GPntFCreated = true;// 设置点临时文件新建成功标志值
				str += "tempPntF.dat \n";
			}
			else
			{
				GPntTmpFName = CString("tempPntF.dat");
				TRACE(_T("文件无法打开！\n"));
			}
		}

		if (!GLinFCreated)//判断线临时文件是否存在，不存在则新建
		{
			GLinTmpNdxFName = dig.m_add + CString("\\") + GLinTmpNdxFName;//临时线索引文件名
			GLinTmpDatFName = dig.m_add + CString("\\") + GLinTmpDatFName;//临时线数据文件名
			if (GLinTmpNdxF->Open(GLinTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary) && GLinTmpDatF->Open(GLinTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GLinFCreated = true; //设置线临时文件新建成功标志值
					str +="tempLinF.dat tempLinF.ndx\n";
			}
			else
			{
				GLinTmpDatFName = CString("tempLinF.dat");
				GLinTmpNdxFName = CString("tempLinF.ndx");
				TRACE(_T("文件无法打开！\n"));
			}
		}

		if (!GRegFCreated)//判断区临时文件是否存在,不存在则新建
		{
			GRegTmpNdxFName = dig.m_add + CString("\\") + GRegTmpNdxFName;//临时区索引文件名
			GRegTmpDatFName = dig.m_add + CString("\\") + GRegTmpDatFName;//临时区数据文件名
			if (GRegTmpNdxF->Open(GRegTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary) && GRegTmpDatF->Open(GRegTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GRegFCreated = true; //设置区临时文件新建成功标志值
				str += "tempRegF.ndx tempRegF.dat \n";
			}
			else
			{
				GRegTmpNdxFName = CString("tempRegF.ndx");
				GRegTmpDatFName = CString("tempRegF.dat");
				TRACE(_T("文件无法打开！\ n"));
			}
		}
		if (GPntFCreated && GLinFCreated && GRegFCreated)
		{
			str += "创建成功！";
			MessageBox(str, L"message", MB_OK); //新建临时文件成功弹出提示框
		}
}


void CMapEditorView::OnFileOpenPoint()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(true);
	dlg.m_ofn.lpstrFilter = L"pnt\0 * .pnt";
	//如果临时文件中的数据己改变,提示是否保存,保存则调用OnFileSavePoint
	if (GPntchanged == true)
	{
		if (IDYES == AfxMessageBox(L"当前文件已发生更改，希望保存这些更改吗?", MB_YESNO, MB_ICONQUESTION))
		{
			OnFileSavePoint();
		}
	}
	if (dlg.DoModal() == IDOK)//弹出打开文件对话框让用户指定要打开的文件
	{
		GPntFName = dlg.m_ofn.lpstrFile; //永久文件（含路径）
		CFile* pntF = new CFile();
		if (!pntF->Open(GPntFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("抱歉，文件无法打开\n"));
			return;
		}
		int nPos = GPntFName.ReverseFind(_T('\\'));
		CString floderAdd = GPntFName.Left(nPos);
		if (GPntTmpFName != "tempPntF.dat") //如果临时点文件已经创建则定位到文件开头
		{
			GPntTmpF->SeekToBegin();
		}
		else
		{
			//创建临时文件
			GPntTmpFName = floderAdd + CString("\\") + GPntTmpFName;
			if (!GPntTmpF->Open(GPntTmpFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GPntTmpFName = CString("tempPntF.dat");
				TRACE(_T("抱歉，临时文件无法打开\n"));
			}
			else
			{
				GPntFCreated = true;
			}
		}
		ReadPntPermanentFileToTemp(pntF, GPntTmpF, GPntNum, GPntLNum);//读点永久文件到临时文件
		pntF->Close();
		delete pntF;
		CString windowText = dlg.GetFileName() + "-MapEditor ";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL); //让视窗口无效，触发MFC 调用OnDraw 函数重绘窗口
	}
	GCurOperState = Noaction;
}


void CMapEditorView::OnFileOpenLine()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(true);// TRUE for FileOpen, FALSE for FileSaveAs
	dlg.m_ofn.lpstrFilter = L"lin\0 *.lin"; 
	if (GLinChanged == true)
	{
		if (IDYES == AfxMessageBox(L"当前文件已发生更改，希望保存这些更改吗?", MB_YESNO, MB_ICONQUESTION))
		{
			OnFileSaveLine(); //保存线（从临时文件写入永久文件中）
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		GLinFCreated = false;
		int IsCreate = 0;
		GLinFName = dlg.m_ofn.lpstrFile; //永久文件（含路径）
		CFile * LinF = new CFile();
		if (!LinF -> Open(GLinFName, CFile::modeRead | CFile::typeBinary))
		{
			TRACE(_T("抱歉，文件无法打开 \ n"));
			return;
		}
		int nPos = GLinFName.ReverseFind(_T('\\'));
		CString floderAdd = GLinFName.Left(nPos);
		if (GLinTmpDatFName != "tempLinF.dat")
		{
			GLinTmpDatF -> SeekToBegin();
			++IsCreate;
		}
		else
		{
			GLinTmpDatFName = floderAdd + CString("\\") + GLinTmpDatFName;
			if (!GLinTmpDatF->Open(GLinTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GLinTmpDatFName = CString("tempLinF.dat");
				TRACE(_T("抱歉，临时数据文件无法打开 \n"));
			}
			else 
			{
				++IsCreate;
			}
		}
		if (GLinTmpNdxFName != "tempLinF.ndx")
		{
			GLinTmpNdxF -> SeekToBegin();
			++IsCreate;
		}
		else
		{
			GLinTmpNdxFName = floderAdd + CString("\\") + GLinTmpNdxFName;
			if (!GLinTmpNdxF->Open(GLinTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GLinTmpNdxFName = CString("tempLinF.ndx");
				TRACE(_T("抱歉，临时索引文件无法打开 \n"));
			}
			else
			{
				++IsCreate;
			}
		}
		if (2 == IsCreate)//临时数据文件和临时索引文件都成功以正确的方式打开
			GLinFCreated = true;//临时线文件成功创建
		ReadLinPermanentFileToTemp(LinF, GLinTmpDatF, GLinTmpNdxF, GLinVer,GLinNum, GLinLNum, GLin.datOff); //从永久文件读取线数据到临时文件
		LinF -> Close(); //关闭线永久文件
		delete LinF;
		CString windowText = dlg.GetFileName() + "-MapEditor";
		GetParent() -> SetWindowTextW(windowText);
		this->InvalidateRect(NULL); //重绘窗口显示
		GCurOperState = Noaction;
	}
}


void CMapEditorView::OnFileOpenRegion()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog  dlg(true);//true为打开文件，false为另存为文件
	dlg.m_ofn.lpstrFilter = L"reg\0*.reg";//过滤字符串的一个缓冲，“\0”前为显示部分，后为过滤部分
	if (GRegChanged == true)//如果临时文件中的数据已改变，则提示是否保存
	{
		if (IDYES == AfxMessageBox(L"当前文件已发生更改，希望保存这些更改吗?", MB_YESNO, MB_ICONQUESTION)) {
			OnFileSaveRegion();//保存区(从临时文件写入永久文件中)
		}
	}
	if (dlg.DoModal() == IDOK)
	{
		GRegFCreated = false;
		int IsCreate = 0;//标注是否创建
		GRegFName = dlg.m_ofn.lpstrFile; // 永久文件(含路径)
		CFile* RegF = new CFile();
		if (!RegF->Open(GRegFName, CFile::modeRead | CFile::typeBinary)) {
			TRACE(_T("文件无法打开！\n"));
			return;
		}
		int nPos = GRegFName.ReverseFind(_T('\\'));//获取字符长度
		CString floderAdd = GRegFName.Left(nPos);//获得地址名称
		if (GRegTmpDatFName != "tempRegF.dat") { //如果临时数据文件已经创建则定位到文件开头
			GRegTmpDatF->SeekToBegin();
			++IsCreate;
		}
		else {
			GRegTmpDatFName = floderAdd + CString("\\") + GRegTmpDatFName;//地址名称
			if (!GRegTmpDatF->Open(GRegTmpDatFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary)) {//未打开
				GRegTmpDatFName = CString("tempRegF.dat");
				TRACE(_T("文件无法打开！\n"));
			}
			else {
				++IsCreate;
			}
		}
		if (GRegTmpNdxFName != "tempRegF.ndx") {//如果临时索引文件已经创建则定位到文件开头
			GRegTmpNdxF->SeekToBegin();
			++IsCreate;

		}
		else {
			GRegTmpNdxFName = floderAdd + CString("\\") + GRegTmpNdxFName;
			if (!GRegTmpNdxF->Open(GRegTmpNdxFName, CFile::modeCreate | CFile::modeReadWrite | CFile::typeBinary))
			{
				GRegTmpNdxFName = CString("tempRegF.ndx");
				TRACE(_T("文件无法打开！ \n"));
			}
			else {
				++IsCreate;
			}
		}
		if (2 == IsCreate) {//两个临时文件均已经创建
			GRegFCreated = true;
		}
		ReadRegPermanentFileToTemp(RegF, GRegTmpDatF, GRegTmpNdxF, GRegVer, GRegNum, GRegLNum, GReg.datOff); //从永久文件读取区数据到临时文件
		RegF->Close(); //关闭区永久文件
		delete RegF;
		CString windowText = dlg.GetFileName() + "- MapEditor";
		GetParent()->SetWindowTextW(windowText);
		this->InvalidateRect(NULL);//重绘窗口显示
		GCurOperState = Noaction;
	}
}


void CMapEditorView::OnFileSavePoint()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState = Noaction;
	//1.如果还没有新建或打开点文件，则提示文件还没有打开，然后返回。
	if (GPntFCreated == false)
	{
		MessageBox(L"未创建文件！", L"Message", MB_OK);
		return;
	}
	CFile* pntF = new CFile();
	//2.如果点文件名不为空，则删除原来的文件，否则调用CFileDialog 类让用户输入文件名
	if (GPntFName.IsEmpty() == false)
	{
		CFile::Remove(GPntFName);
	}
	else
	{
		LPCTSTR lpszFilters;
		lpszFilters = _T("点(* .pnt)|* .pnt||");//文件类型说明和扩展名间用 | 分隔，同种类型文件的扩展名间可以用; 分割，每种文件类型间用 | 分隔，末尾用 || 指明
		CFileDialog dlg(false, _T("pnt"), NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,lpszFilters);
		int nPos = GPntTmpFName.ReverseFind(_T('\\'));//临时文件路径名最后一个‘\\’的索引
		CString folderAdd = GPntTmpFName.Left(nPos);//‘\\’前的字符串
		dlg.m_ofn.lpstrInitialDir = folderAdd;
		if (dlg.DoModal() == IDOK)//点击的是“确定”
		{
			GPntFName = dlg.GetPathName();
		}
		else//点击的是“取消”
		{
			return;
		}
	}
	PNT_STRU tempPnt;
	//3. 重新创建点永久文件，并写入版本信息，点物理数和逻辑数
	if (pntF->Open(GPntFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))//创建并打开，可写，以二进制形式打开
	{
		pntF->Write(&GPntVer, sizeof(VERSION));//写入版本信息
		pntF->Write(&GPntNum, sizeof(int));//写入点物理数
		pntF->Write(&GPntLNum, sizeof(int));//写入点逻辑数
	}
	else
	{
		TRACE(_T("抱歉，文件无法打开！\n"));
	}
	//4. 将点临时文件中的信息、逐条写到永久文件中
	for (int i = 0; i < GPntNum; i++)
	{
		ReadTempFileToPnt(GPntTmpF, i, tempPnt);
		pntF->Write(&tempPnt, sizeof(PNT_STRU));
	}
	pntF -> Close();//关闭文件
	delete pntF;//释放动态内存
	//5. 修改数据变化标志，改变主窗口标题名称
	GPntchanged = false;
	int nPos = GPntFName.ReverseFind(_T('\\'));
	CString windowText = GPntFName.Right(GPntFName.GetLength()-nPos - 1) +"-MapEditor";
	GetParent() -> SetWindowTextW(windowText);//窗口左上角显示“文件名-MapEditor”
}

//保存线
void CMapEditorView::OnFileSaveLine()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)//已存在临时文件
	{
		CFile* LinF = new CFile();
		if (GLinFName.IsEmpty())//如果线文件名为空，则删除原来的文件，否则调用CFileDialog 类让用户输入文件名
		{
			LPCTSTR lpszFilters;
			lpszFilters = _T("线(*.lin)|* .lin||");
			CFileDialog dlg(false, _T("lin"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilters); //保存线的对话框
			int nPos = GLinTmpDatFName.ReverseFind(_T('\\'));//临时文件路径找到‘\\'
			CString folderAdd = GLinTmpDatFName.Left(nPos);//临时文件路径左移到‘\\’
			dlg.m_ofn.lpstrInitialDir = folderAdd;
			if (dlg.DoModal() == IDOK)
			{
				GLinFName = dlg.GetPathName(); //线文件的名称
			}
			else
			{
				return;
			}
		}
		else
		{
			LinF -> Remove(GLinFName);
		}
		if (!LinF->Open(GLinFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			TRACE(_T( "抱歉，线永久文件打开错误！\n"));
			return;
		}
		WriteTempToLinPermanentFile(LinF, GLinTmpDatF, GLinTmpNdxF, GLinVer,GLinNum, GLinLNum); //将线的索引和点数据写入永久文件
		LinF -> Close();
		delete LinF;
		GLinChanged = false; //线数据无变更
		int nPos = GLinFName.ReverseFind(_T('\\'));
		CString windowText = GLinFName.Right(GLinFName.GetLength() - nPos - 1)+"-MapEditor";
		GetParent()->SetWindowTextW(windowText);
	}
	else
	{
		MessageBox(L"未创建临时文件！”, L”Message", MB_OK);
	}
	GCurOperState = Noaction;
}

//保存区文件
void CMapEditorView::OnFileSaveRegion()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)//已存在临时文件
	{
		CFile* RegF = new CFile();
		if (GRegFName.IsEmpty())//如果区文件名不为空，则删除原来的文件，否则调用CFileDialog 类让用户输入文件名
		{
			LPCTSTR lpszFilters;
			lpszFilters = _T("区(*.reg)|* .reg||");
			CFileDialog dlg(false, _T("reg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilters); //保存区的对话框
			int nPos = GRegTmpDatFName.ReverseFind(_T('\\'));
			CString folderAdd = GRegTmpDatFName.Left(nPos);
			dlg.m_ofn.lpstrInitialDir = folderAdd;
			if (dlg.DoModal() == IDOK)
			{
				GRegFName = dlg.GetPathName(); //区文件的名称
			}
			else
			{
				return;
			}
		}
		else
		{
			RegF->Remove(GRegFName);
		}
		if (!RegF->Open(GRegFName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			TRACE(_T("抱歉，区永久文件打开错误！\n"));
			return;
		}
		WriteTempToRegPermanentFile(RegF, GRegTmpDatF, GRegTmpNdxF, GRegVer, GRegNum, GRegLNum); //将区的索引和点数据写入永久文件
		RegF->Close();
		delete RegF;
		GRegChanged = false; //区数据无变更
		int nPos = GRegFName.ReverseFind(_T('\\'));
		CString windowText = GRegFName.Right(GRegFName.GetLength() - nPos - 1) + "-MapEditor";
		GetParent()->SetWindowTextW(windowText);
	}
	else
	{
		MessageBox(L"未创建临时文件！”, L”Message", MB_OK);
	}
	GCurOperState = Noaction;
}


void CMapEditorView::OnFileSaveAsPoint()
{
	// TODO: 在此添加命令处理程序代码
	CString tempFName = GPntFName; //保留原点永久文件名
	GPntFName = CString(""); //将点永久文件名设为空
	OnFileSavePoint(); //调用OnFileSavePoint 函数，永久文件名为空时自动弹出文件名对话框
	if (GPntFName == "") //若另存失败，则还原原永久文件名
	{
		GPntFName = tempFName;
	}
}


void CMapEditorView::OnFileSaveAsLine()
{
	// TODO: 在此添加命令处理程序代码
	CString tempFName = GLinFName; //保留原点永久文件名
	GLinFName = CString(""); //将线永久文件名设为空
	OnFileSaveLine(); //调用OnFileSaveLine 函数，永久文件名为空时自动弹出文件名对话框
	if (GLinFName == "") //若另存失败，则还原原永久文件名
	{
		GLinFName = tempFName;
	}
}


void CMapEditorView::OnFileSaveAsRegion()
{
	// TODO: 在此添加命令处理程序代码
	CString tempFName = GRegFName; //保留原点永久文件名
	GRegFName = CString(""); //将区永久文件名设为空
	OnFileSaveRegion(); //调用OnFileSaveRegion 函数，永久文件名为空时自动弹出文件名对话框
	if (GRegFName == "") //若另存失败，则还原原永久文件名
	{
		GRegFName = tempFName;
	}
}



void CMapEditorView::OnAppExit()
{
	// TODO: 在此添加命令处理程序代码
	//1.若点数据以改变，则保存
	if (GPntchanged == true)
	{
		if (IDYES == AfxMessageBox(L"文件已发生更改.希望保留这些更改吗?", MB_YESNO, MB_ICONQUESTION))
		{
			OnFileSavePoint();
		}
	}
	//2.若线数据以改变，则保存
	if (GLinChanged == true)
	{
		if (IDYES == AfxMessageBox(L"文件已发生更改.希望保留这些更改吗?", MB_YESNO, MB_ICONQUESTION))
		{
			OnFileSaveLine();
		}
	}
	//3.若区数据以改变，则保存
	if (GRegChanged == true)
	{
		if (IDYES == AfxMessageBox(L"文件已发生更改.希望保留这些更改吗?", MB_YESNO, MB_ICONQUESTION))
		{
			OnFileSaveRegion();
		}
	}
	//4.调用父窗口CMainFrame的销毁窗口函数DestoryWindow()
	GetParent()->DestroyWindow();
}


void CMapEditorView::OnWindowZoomIn()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_ZOOM_IN;//当前为放大操作状态
	}
	else
	{
		MessageBox(L"未创建临时文件！",L"Message", MB_OK);
	}
}



void CMapEditorView::OnWindowZoomOut()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated ||GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_ZOOM_OUT; //设置为缩小操作状态
    }
    else
    {
        MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
    }
}


void CMapEditorView::OnWindowMove()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		GCurOperState = OPERSTATE_WINDOW_MOVE;//设置窗口移动操作状态
	}
	else
	{
		MessageBox(L"未创建临时文件！", L"Message", MB_OK);
	}

}


void CMapEditorView::OnWindowReset()
{
	// TODO: 在此添加命令处理程序代码
	GCurShowState = SHOWSTATE_UNDEL; //设置 当前为显示未删除状态
	GShowPnt = true;
	GShowLin = true;
	GShowReg = true;
	//重量偏移量和放大倍数
	GZoomOffset_x = 0; //偏移向量x
	GZoomOffset_y = 0; //偏移向量y
	GZoom = 1.0; //放大系数
	//遍历点、线、区的点数据,计算外包络矩形
	D_DOT tempPt;
	PNT_STRU tempPnt;
	LIN_NDX_STRU tempLin;
	REG_NDX_STRU tempReg;
	bool isInit = false;
	//没有图形
	if (GPntLNum == 0 &&GLinLNum == 0 &&GRegLNum == 0)
	{
		return;
	}
	//初始化外包矩形
	if (isInit == false && GPntLNum > 0)
	{
		//初始化点的外包矩形
		for (int i = 0; i < GPntNum; ++i)
		{
			ReadTempFileToPnt(GPntTmpF, i, tempPnt);//从点临时文件中读取点数据
			if (tempPnt.isDel)
			{
				continue;
			}
			else
			{
				GMaxX = tempPnt.x;
				GMinX = tempPnt.x;
				GMaxY = tempPnt.y;
				GMinY = tempPnt.y;
				isInit = true;
				break;
			}
		}
	}
	if (isInit == false && GLinLNum > 0)
	{
		//初始化线的外包矩形
		for (int i = 0; i < GLinNum; ++i)
		{
			ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLin);//从临时文件读取线索引
			if (tempLin.isDel)
			{
				continue;
			}
			else
			{
				for (int j = 0; j < tempLin.dotNum; ++j)
				{
					ReadTempFileToLinDat(GLinTmpDatF, tempLin.datOff, j, tempPt);//从临时文件读取节点
					GMaxX = tempPt.x;
					GMinX = tempPt.x;
					GMaxY = tempPt.y;
					GMinY = tempPt.y;
					isInit = true;
					break;
				}
			}
		}
	}
	if (isInit == false && GRegLNum > 0)
	{
		//初始化区的外包矩形
		for (int i = 0; i < GRegNum; ++i)
		{
			ReadTempFileToRegNdx(GRegTmpNdxF, i, tempReg);//从临时文件读取区索引
			if (tempReg.isDel)
			{
				continue;
			}
			else
			{
				for (int j = 0; i < tempReg.dotNum; ++j)
				{
					ReadTempFileToRegDat(GRegTmpDatF, tempReg.datOff, j, tempPt);//从临时文件读取区节点
					GMaxX = tempPt.x;
					GMinX = tempPt.x;
					GMaxY = tempPt.y;
					GMinY = tempPt.y;
					isInit = true;
					break;
				}
			}
		}
	}
	//未能初始化
	if (isInit == false)
	{
		this->Invalidate();
		return;
	}
	//遍历所有点
	if (GPntFCreated)
	{
		for (int i = 0; i < GPntNum; i++)
		{
			ReadTempFileToPnt(GPntTmpF, i, tempPnt);//从临时文件读取点数据
			if (tempPnt.isDel)
			{
				continue;
			}
			else
			{
				if (tempPnt.x > GMaxX)
					GMaxX = tempPnt.x;
				if (tempPnt.y > GMaxY)
					GMaxY = tempPnt.y;
				if (tempPnt.x < GMinX)
					GMinX = tempPnt.x;
				if (tempPnt.y < GMinY)
					GMinY = tempPnt.y;
			}
		}
	}
	//遍历所有线
	if (GLinFCreated)
	{
		for (int i = 0; i < GLinNum; i++)
		{
			ReadTempFileToLinNdx(GLinTmpNdxF, i, tempLin);//从临时文件读取线索引
			if (tempLin.isDel)
			{
				continue;
			}
			else
			{
				for (int j = 0; j < tempLin.dotNum; j++)
				{
					ReadTempFileToLinDat(GLinTmpDatF, tempLin.datOff, j, tempPt);//从临时文件读取节点数据
					if (tempPt.x > GMaxX)
						GMaxX = tempPt.x;
					if (tempPt.y > GMaxY)
						GMaxY = tempPt.y;
					if (tempPt.x < GMinX)
						GMinX = tempPt.x;
					if (tempPt.y < GMinY)
						GMinY = tempPt.y;
				}
			}
		}
	}
	//遍历所有区
	if (GRegFCreated)
	{
		for (int i = 0; i < GRegNum; ++i)
		{
			ReadTempFileToRegNdx(GRegTmpNdxF, i, tempReg);//从临时文件读取区索引
			if (tempReg.isDel)
			{
				continue;
			}
			else
			{
				for (int j = 0; i < tempReg.dotNum; ++j)
				{
					ReadTempFileToRegDat(GRegTmpDatF, tempReg.datOff, j, tempPt);//从临时文件读取区节点
					if (tempPt.x > GMaxX)
						GMaxX = tempPt.x;
					if (tempPt.y > GMaxY)
						GMaxY = tempPt.y;
					if (tempPt.x < GMinX)
						GMinX = tempPt.x;
					if (tempPt.y < GMinY)
						GMinY = tempPt.y;
				}
			}
		}
	}
	GMaxX += 20;
	GMinX -= 20;
	GMaxY += 20;
	GMinY -= 20;
	RECT rect, client;
	double zoom;
	GetClientRect(&client);
	rect.right = (long)GMaxX;
	rect.left = (long)GMinX;
	rect.bottom = (long) GMaxY;
	rect.top = (long) GMinY;
	modulusZoom(client, rect, zoom);
	GMaxX += 20 / zoom;
	GMinX -= 20 / zoom;
	GMaxY += 20 / zoom;
	GMinY -= 20 / zoom;
	rect.right = (long) GMaxX;
	rect.left = (long)GMinX;
	rect.bottom = (long)GMaxY;
	rect.top = (long)GMinY;
	//根据外包络矩形计算偏移量和放大倍数,并重绘客户区
	modulusZoom(client, rect,zoom);
	double x0 = GetCenter(rect).x - (client.right / 2.0) + (client.right * (zoom - 1) /(2.0 * zoom));
	double y0 = GetCenter(rect).y - (client.bottom / 2.0) + (client.bottom * (zoom -1) / (2.0 * zoom));
	GZoomOffset_x += (x0 / GZoom);
	GZoomOffset_y += (y0 / GZoom);
	GZoom*=zoom;
	GCurOperState = Noaction;
	this->Invalidate();
}


void CMapEditorView::OnWindowShowPoint()
{
	// TODO: 在此添加命令处理程序代码
	//若当前显示状态是显示删除状态,则先把所有显示开关关闭
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL; //将显示状态更改为显示未删除状态
	//若当前已“显示点”,则将关闭开关,不再“显示点”
	if (GShowPnt == true)
	{
		GShowPnt = false;
	}
	else
	{
		GShowPnt = true;
	}
	this->InvalidateRect(NULL); //刷新窗口
}


void CMapEditorView::OnWindowShowLine()
{
	// TODO: 在此添加命令处理程序代码
	//若当前显示状态是显示删除状态,则先把所有显示开关关闭
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL; //将显示状态更改为显示未删除状态
	//若当前已“显示线”,则将关闭开关,不再“显示线”
	if (GShowLin == true)
	{
		GShowLin = false;
	}
	else
	{
		GShowLin = true;
	}
	this->InvalidateRect(NULL); //刷新窗口
}


void CMapEditorView::OnWindowShowRegion()
{
	// TODO: 在此添加命令处理程序代码
	//若当前显示状态是显示删除状态,则先把所有显示开关关闭
	if (GCurShowState == SHOWSTATE_DEL)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = false;
	}
	GCurShowState = SHOWSTATE_UNDEL; //将显示状态更改为显示未删除状态
	//若当前已“显示区”,则将关闭开关,不再“显示区”
	if (GShowReg == true)
	{
		GShowReg = false;
	}
	else
	{
		GShowReg = true;
	}
	this->InvalidateRect(NULL); //刷新窗口
}


void CMapEditorView::OnPointCreate()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_PNT; //设置为“造点”状态
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else
	{
		MessageBox(L"未创建临时文件！", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointMove()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_PNT;//设置当前状态为移动点
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else
	{
		MessageBox(L"未创建临时文件！", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_DELETE_PNT;//设置操作状态为删除点
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else
	{
		MessageBox(L"未创建临时文件！",L"Message",MB_OK);
	}
}


void CMapEditorView::OnPointShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
	//若当前显示状态不是显示删除状态，则切换为显示删除状态并显示点
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	//若当前状态为显示删除状态，但当前显示的不是点，则将显示点的开关打开
	else if (GCurShowState == SHOWSTATE_DEL && GShowPnt != true)
	{
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	//其他情况下则将显示状态设为显示未删除的状态，并打开所有现实的开关
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->InvalidateRect(NULL);//刷新窗口
}


void CMapEditorView::OnPointUndelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_PNT;//设置当前操作状态为恢复点
		GCurShowState = SHOWSTATE_DEL;//设置当前显示状态为显示删除点
		this->Invalidate();//刷新窗口，重绘
		//只显示点
		GShowPnt = true;
		GShowLin = false;
		GShowReg = false;
	}
	else
	{
		MessageBox(L"未创建点临时文件", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
	if (GPntFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_POINT_PARAMETER; //设置当前操作状态为修改点参数
		GCurShowState = SHOWSTATE_UNDEL;//显示状态(不删除点)
		GShowPnt = true; //打开显示点
		GShowLin = true; //打开显示线
		GShowReg = true;//打开显示区
		this->Invalidate();
	}
	else
	{
		MessageBox(L"未创建临时文件！", L"Message", MB_OK);
	}
}


void CMapEditorView::OnPointSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
	CPointParameterDlg dlg;//点参数设置的对话框
	dlg.m_Pattern = GPnt.pattern;//点型
	dlg.m_ColorButton.SetColor(GPnt.color);//颜色
	if (IDOK == dlg.DoModal())
	{
		GPnt.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GPnt.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}


void CMapEditorView::OnLineCreate()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_LIN; //当前为造线状态
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else
	{
		MessageBox(L"TempFile have not been created.", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineMove()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_LIN;//设置当前状态为移动线
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else
	{
		MessageBox(L"未创建线临时文件", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)//已创建线临时文件
	{
		GCurOperState = OPERSTATE_DELETE_LIN;
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else//为创建线临时文件
	{
		MessageBox(L"未创建线临时文件！", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
	//若当前显示状态不是显示删除状态，则切换为显示删除状态并显示线
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	//若当前状态为显示删除状态，但当前显示的不是线，则将显示线的开关打开
	else if (GCurShowState == SHOWSTATE_DEL && GShowLin != true)
	{
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	//其他情况下则将显示状态设为显示未删除的状态，并打开所有显示的开关
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->InvalidateRect(NULL);//刷新窗口
}


void CMapEditorView::OnLineUndeleted()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_LIN;//设置当前操作状态为恢复线
		GCurShowState = SHOWSTATE_DEL;//设置当前显示状态为显示删除线
		this->Invalidate();//刷新窗口，重绘
		//只显示线
		GShowPnt = false;
		GShowLin = true;
		GShowReg = false;
	}
	else
	{
		MessageBox(L"未创建线临时文件", L"Message", MB_OK);
	}

}


void CMapEditorView::OnLineDeleteDot()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_LIN_DELETE_PNT;//设置当前操作状态为线上删点
		GCurShowState = SHOWSTATE_UNDEL;//设置当前显示状态为显示未删除的部分
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	else
	{
		MessageBox(L"未创建临时文件", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineAddDot()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_LIN_ADD_PNT;//设置当前操作状态为线上加点
		GCurShowState = SHOWSTATE_UNDEL;//设置当前显示状态为显示未删除的部分
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	else
	{
		MessageBox(L"未创建临时文件", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineLink()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_LINK_LIN;//设置当前操作状态为连线操作状态
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else
	{
		MessageBox(L"未创建线临时文件！", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineModifyParameter()
{
	// TODO: 在此添加命令处理程序代码
	if (GLinFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_LINE_PARAMETER; //设置当前操作状态为修改点参数
		GCurShowState = SHOWSTATE_UNDEL;//显示状态(不删除线)
		GShowPnt = true; //打开显示点
		GShowLin = true; //打开显示线
		GShowReg = true;//打开显示区
		this->Invalidate();
	}
	else
	{
		MessageBox(L"未创建临时文件！", L"Message", MB_OK);
	}
}


void CMapEditorView::OnLineSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
	CLinParameterDlg dlg;//线参数设置的对话框
	dlg.m_Pattern = GLin.pattern;//线型
	dlg.m_ColorButton.SetColor(GLin.color);//颜色
	if (IDOK == dlg.DoModal())
	{
		GLin.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GLin.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}


void CMapEditorView::OnRegionCreate()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_INPUT_REG;//当前操作状态为造区
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else
	{
		MessageBox(L"未创建区临时文件", L"Message", MB_OK);
	}
}


void CMapEditorView::OnRegionMove()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_MOVE_REG;
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else
	{
		MessageBox(L"未创建区临时文件！",L"Message",MB_OK);
	}
}


void CMapEditorView::OnRegionDelete()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_DELETE_REG;
		GCurShowState = SHOWSTATE_UNDEL;//设置 当前为显示未删除状态
		this->Invalidate();
		GShowPnt = true;
		GShowReg = true;
		GShowLin = true;
	}
	else
	{
		MessageBox(L"未创建临时文件！", L"Message", MB_OK);
	}
}


void CMapEditorView::OnRegionShowDeleted()
{
	// TODO: 在此添加命令处理程序代码
	//若当前显示状态不是显示删除状态，则切换为显示删除状态并显示区
	if (GCurShowState != SHOWSTATE_DEL)
	{
		GCurShowState = SHOWSTATE_DEL;
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	//若当前状态为显示删除状态，但当前显示的不是区，则将显示点的开关打开
	else if (GCurShowState == SHOWSTATE_DEL && GShowReg != true)
	{
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	//其他情况下则将显示状态设为显示未删除的状态，并打开所有显示的开关
	else
	{
		GCurShowState = SHOWSTATE_UNDEL;
		GShowPnt = true;
		GShowLin = true;
		GShowReg = true;
	}
	this->InvalidateRect(NULL);//刷新窗口
}


void CMapEditorView::OnRegionUndeleted()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_UNDELETE_REG;//设置当前操作状态为恢复区
		GCurShowState = SHOWSTATE_DEL;//设置当前显示状态为显示删除区
		this->Invalidate();//刷新窗口，重绘
		//只显示区
		GShowPnt = false;
		GShowLin = false;
		GShowReg = true;
	}
	else
	{
		MessageBox(L"未创建区临时文件", L"Message", MB_OK);
	}
}


void CMapEditorView::OnRegionModifyParemeter()
{
	// TODO: 在此添加命令处理程序代码
	if (GRegFCreated)
	{
		GCurOperState = OPERSTATE_MODIFY_REGION_PARAMETER; //设置当前操作状态为修改区参数
		GCurShowState = SHOWSTATE_UNDEL;//显示状态(不删除线)
		GShowPnt = true; //打开显示点
		GShowLin = true; //打开显示线
		GShowReg = true;//打开显示区
		this->Invalidate();
	}
	else
	{
		MessageBox(L"未创建临时文件！", L"Message", MB_OK);
	}
}


void CMapEditorView::OnRegionSetDefparameter()
{
	// TODO: 在此添加命令处理程序代码
	CRegParameterDlg dlg;//区参数设置的对话框
	dlg.m_Pattern = GReg.pattern;//区型
	dlg.m_ColorButton.SetColor(GReg.color);//颜色
	if (IDOK == dlg.DoModal())
	{
		GReg.pattern = dlg.m_Pattern;
		COLORREF tempColor = dlg.m_ColorButton.GetColor();
		memcpy_s(&GReg.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
	}
}


void CMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnLButtonDown(nFlags, point);
	if (GPntFCreated)
	{
		switch(GCurOperState)
		{
		case OPERSTATE_MOVE_PNT://移动点操作
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换（窗口转数据）
			DotToPnt(point, dot);
			GTPnt = Findpoint(point, GPntNum, GPntTmpF, GPntNdx);//查最近点
			dot.x = GTPnt.x;
			dot.y = GTPnt.y;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（数据转窗口）
			GTPnt.x = dot.x;
			GTPnt.y = dot.y;
			break;
		default:
			break;
		}
	}
	if (GLinFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_LIN://移动线操作
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换（窗口转数据）
			DotToPnt(point, dot);
			GLinMMTmpNdx = Findline(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx); // 查找单击点最近的一条线
			GLinMMOffsetX = 0;
			GLinMMOffsetY = 0;
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换（数据转窗口）
			DotToPnt(point, dot);
			GLinLBDPnt = point;
			GLinMMPnt = point;
			break;
		default:
			break;
		}
	}
	if (GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_REG://移动区操作
			GRegLBDPnt = point;
			GRegMMPnt = point;
			D_DOT dot;
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//窗口转数据
			DotToPnt(point, dot);
			GRegMMTmpNdx = FindReg(GRegTmpNdxF, GRegTmpDatF, point,GRegNum, GRegNdx); //查找最近区,即点选中的区
			GRegMMOffsetX = 0;
			GRegMMOffsetY = 0;
			break;
		default:
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN://当前为放大操作状态
			GZoomLBDPnt = point;
			GZoomMMPnt = point;
			break;
		case OPERSTATE_WINDOW_MOVE ://当前为移动操作状态
			GWinMoveLBDPnt = point;
			GWinMoveMMPnt = point;
			break;
		default:
			break;
		}
	}
}


void CMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CClientDC dc(this); //画笔， 定义dc 时调用构造函数
	dc.SetROP2(R2_NOTXORPEN); //绘图的模式设置
	if (GPntFCreated)//已创建点临时文件
	{
		D_DOT dot;
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_PNT:// 当前为绘制点状态
			PNT_STRU pnt;//点对象
			memcpy_s(&pnt, sizeof(PNT_STRU), &GPnt, sizeof(PNT_STRU));//复制缓冲区之间的字节
			PntToDot(dot, point);//Pnt转Dot
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（窗口转数据）
			pnt.x = point.x; //设置点坐标的x
			pnt.y = point.y; //设置点坐标的y
			WritePntToFile(GPntTmpF, GPntNum, pnt); //将点写入临时文件
			PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换
			pnt.x = dot.x;//设置点坐标的x（缩放后）
			pnt.y = dot.y;//设置点坐标的y（缩放后）
			DrawPnt(&dc, pnt); //绘制点
			GPntNum++; //点物理数加1
			GPntLNum++; //点逻辑数加l
			GPntchanged = true; //是否更改标志设置为true
			break;
		case OPERSTATE_DELETE_PNT://当前为删除点状态
			PntToDot(dot, point);//Pnt转Dot
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（窗口转数据）
			DotToPnt(point, dot);//Dot转Pnt（缩放后）
			Findpoint(point, GPntNum, GPntTmpF, GPntNdx);//查找最近点
			if (GPntNdx != -1)//如找到最近点
			{
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);//从临时点文件读点
				pnt.isDel = 1;//删除标记置为1
				UpdatePnt(GPntTmpF, GPntNdx, pnt);//更新该点数据
				dot.x = pnt.x;
				dot.y = pnt.y;
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换（数据转窗口）
				pnt.x = dot.x;
				pnt.y = dot.y;
				DrawPnt(&dc, pnt);//异或模式重绘该点以清除屏幕
				GPntNdx = -1;
				GPntchanged = true;//数据发生更改
				GPntLNum--;//删除一个点，逻辑数减一，但物理储存不变
			}
			break;
		case OPERSTATE_MOVE_PNT://当前为移动点状态
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（窗口转数据）
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt); //从点临时文件读取点
				pnt.x = point.x; //移动后的点坐标x
				pnt.y = point.y; //移动后的点坐标y
				UpdatePnt(GPntTmpF, GPntNdx, pnt); //更新点数据（写入临时文件）
				GPntNdx = -1;
				GPntchanged = true; //数据发生变更
			}
			break;
		case OPERSTATE_UNDELETE_PNT://当前为恢复点状态
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标转换（窗口转坐标）
			DotToPnt(point, dot);
			FindDeletePnt(point, GPntNum, GPntTmpF, GPntNdx);//查找离鼠标最近的已经被删除的点
			if (GPntNdx != -1)
			{
				PNT_STRU pnt;
				ReadTempFileToPnt(GPntTmpF, GPntNdx, pnt);//从临时文件读取所找到的最近的删除的数据到pnt
				pnt.isDel = 0;//设置该点为不删除
				UpdatePnt(GPntTmpF, GPntNdx, pnt);//更新点数据
				dot.x = pnt.x;
				dot.y = pnt.y;
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换（数据转窗口）
				pnt.x = dot.x;
				pnt.y = dot.y;
				DrawPnt(&dc, pnt);//在当前视图再次绘制点，从当前视图擦除要恢复的点
				GPntchanged = true;//点文件发生更改
				GPntNdx = -1;//重新设为-1，以便下次利用
				GPntLNum++;//点逻辑数加一
			}
			break;
		case OPERSTATE_MODIFY_POINT_PARAMETER://当前为修改点
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//窗口转数据坐标
			DotToPnt(point, dot);
			PNT_STRU tempPoint;
			memcpy_s(&tempPoint, sizeof(PNT_STRU), &Findpoint(point, GPntNum, GPntTmpF, GPntNdx), sizeof(PNT_STRU));//查找最近的点
			if (GPntNdx != -1)
			{
				CPointParameterDlg dlg; //点参数设置对话框
				dlg.m_ColorButton.SetColor(tempPoint.color);
				dlg.m_Pattern = tempPoint.pattern;
				if (IDOK == dlg.DoModal())
				{
					COLORREF tempColor = dlg.m_ColorButton.GetColor();
					memcpy_s(&tempPoint.color, sizeof(COLORREF), &tempColor,sizeof(COLORREF));
					tempPoint.pattern = dlg.m_Pattern;
					GPntTmpF->Seek(GPntNdx * sizeof(PNT_STRU), CFile:: begin);
					GPntTmpF->Write(&tempPoint, sizeof(PNT_STRU));//写入点数据
				}
				this->Invalidate();
				GPntchanged = true;
				GPntNdx = -1;
			}
			break;
		default:
			break;
		}
	}
	if (GLinFCreated)//已创建线临时文件
	{
		D_DOT dot;
		int min = 10, distance = 0;
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN://当前为绘制线状态
			if (GTLin.dotNum == 0)
			{
				memcpy_s(&GTLin, sizeof(LIN_NDX_STRU), &GLin, sizeof(LIN_NDX_STRU));//复制缓冲区之间的字节
			}
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换（窗口转数据）
				WriteLinDatToFile(GLinTmpDatF, GLin.datOff, GTLin.dotNum, dot);//将线的点数据写入临时文件中
				GTLin.dotNum++; //线节点数加l
				PntDPtoVP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换（数据转窗口）
				GLPnt.x = (long)dot.x; //设置线段的起点(x)
				GLPnt.y = (long)dot.y;// 设置线段的起点(y)
				GLinChanged = true; //线数据变更
				break;
		case OPERSTATE_DELETE_LIN://当前为删除线状态
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（窗口转数据）
			DotToPnt(point, dot);
			Findline(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);//找最近线
			if (GLinNdx != -1)//找到最近线
			{
				GLinLNum--;//线逻辑数减一
				GLinChanged = true;//线数据更改
				LIN_NDX_STRU TmpLinNdx;
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				ReadTempFileToLinNdx(GLinTmpNdxF,GLinNdx,TmpLinNdx);//从临时线索引文件中读取线索引
				TmpLinNdx.isDel = 1; //设置删除标志为已删除
				UpdateLin(GLinTmpNdxF, GLinNdx , TmpLinNdx); //更新线数据
				for (int i = 0; i < TmpLinNdx.dotNum - 1; i++)
				{
					//从临时线的点数据文件中读取点
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff,i,dot1);
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff,i+1, dot2);
					//坐标系转换（数据转窗口坐标系）
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					//Dot转Pnt
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);//重绘
				}
				GLinNdx = -1;
			}
			break;
		case OPERSTATE_MOVE_LIN://移动线状态
			if (GLinNdx != -1)//找到符合条件的最近的线
			{
				if (GLinLBDPnt.x != -1 && GLinLBDPnt.y != -1)//鼠标左键按下的位置有效
				{
					D_DOT dot1, dot2;
					//Pnt转Dot
					PntToDot(dot1, point);//鼠标当前位置
					PntVPtoDP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换（窗口转数据）
					PntToDot(dot2, GLinLBDPnt);//鼠标上一状态位置
					PntVPtoDP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（窗口转数据）
					//计算偏移量
					double offset_x = dot1.x - dot2.x;
					double offset_y = dot1.y - dot2.y;
					//更新线数据
					UpdateLin(GLinTmpNdxF, GLinTmpDatF, GLinNdx, offset_x, offset_y);
					//将找到线在文件中的位置重新设为-1
					GLinNdx = -1;
					//将x轴偏移量，y轴偏移量重新设为0
					GLinMMOffsetX = 0;
					GLinMMOffsetY = 0;
					//线数据改变
					GLinChanged = true;
				}
			}
			break;
		case OPERSTATE_LINK_LIN://当前操作状态为连接线
			if (GnLine < 2)
			{
				//连接线数小于2，需要再次点击
				LIN_NDX_STRU line;
				D_DOT dot;
				PntToDot(dot, point);
				PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标系转换（窗口转数据）
				DotToPnt(point, dot);
				//查找离鼠标最近的线
				line = Findline(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);
				if (GLinNdx != -1)
				{
					//找到符合条件的最近的线
					GnLine++;
					if (GnLine == 1)//找到第一条线
					{
						GStartLin = line;
						GnStart = GLinNdx;
					}
					else if (GnLine == 2)//找到第二条线
					{
						if (GnStart != GLinNdx)
						{
							GEndLin = line;
							GnEnd = GLinNdx;
						}
						else
						{
							GnLine--;//两次选中同一条线，同一条线不能和自身相连
						}
					}
				}
				if (GnLine != 0)
				{
					D_DOT pt;
					if (GnLine == 1)//选中第一条线，并将其端点画圆标记
					{
						//从临时文件中读取线的起点并将其转为窗口坐标
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, 0, pt);
						PntDPtoVP(pt, GZoom, GZoomOffset_x, GZoomOffset_y);//数据坐标系转窗口坐标系
						dc.Ellipse((long)pt.x - 2, (long)pt.y - 2, (long)pt.x + 2, (long)pt.y + 2);//在端点处画圆
						//从临时文件中读取线的终点并将其转为窗口坐标
						ReadTempFileToLinDat(GLinTmpDatF, GStartLin.datOff, GStartLin.dotNum-1, pt);
						PntDPtoVP(pt, GZoom, GZoomOffset_x, GZoomOffset_y);//数据坐标系转窗口坐标系
						dc.Ellipse((long)pt.x - 2, (long)pt.y - 2, (long)pt.x + 2, (long)pt.y + 2);//在端点处画圆
					}
					else//选中第二条线，连接线
					{
						AlterLindot(GLinTmpDatF, GStartLin, GEndLin, GnStart, GnEnd, GLin.datOff);//改变线的点数据，将连接线的点写入文件中
						AlterStartLin(GLinTmpNdxF, GLin.datOff, GnStart, GStartLin.dotNum + GEndLin.dotNum);//修改第一条线的索引
						AlterEndLin(GLinTmpNdxF, GnEnd);//修改第二条线的索引
						GLin.datOff += (GStartLin.dotNum + GEndLin.dotNum) * sizeof(D_DOT); //连接线索引
						GnLine = 0;
						GLinLNum --;
						GLinChanged = true;
						GnStart = -1;
						GnEnd = -1;
						GLinNdx = -1;
						this->Invalidate();
					}
				}
			}
			break;
		case OPERSTATE_UNDELETE_LIN://当前操作状态为恢复线
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//坐标转换（窗口转坐标）
			DotToPnt(point, dot);
			FindDeleteline(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx);//查找离鼠标最近的已经被删除的线
			if (GLinNdx != -1)
			{
				GLinLNum++;//线逻辑数加一
				GLinChanged = true;//线数据更改
				LIN_NDX_STRU TmpLinNdx;
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				ReadTempFileToLinNdx(GLinTmpNdxF, GLinNdx, TmpLinNdx);//从临时线索引文件中读取线索引
				TmpLinNdx.isDel = 0; //设置删除标志为未删除
				UpdateLin(GLinTmpNdxF, GLinNdx, TmpLinNdx); //更新线数据
				for (int i = 0; i < TmpLinNdx.dotNum - 1; i++)
				{
					//从临时线的点数据文件中读取点
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, TmpLinNdx.datOff, i + 1, dot2);
					//坐标系转换（数据转窗口坐标系）
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					//Dot转Pnt
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					DrawSeg(&dc, TmpLinNdx, pnt1, pnt2);//重绘
				}
				GLinNdx = -1;
			}
			break;
		case OPERSTATE_MODIFY_LINE_PARAMETER://修改线参数状态
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（窗口转数据）
			DotToPnt(point, dot);
			LIN_NDX_STRU tempLin;
			memcpy_s(&tempLin, sizeof(LIN_NDX_STRU), &Findline(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx),sizeof(LIN_NDX_STRU));//查找最近的线
			if (GLinNdx != -1)
			{
				CLinParameterDlg dlg; //线参数设置对话框
				dlg.m_ColorButton.SetColor(tempLin.color);
				dlg.m_Pattern = tempLin.pattern;
				if (IDOK == dlg.DoModal())
				{
					COLORREF tempColor = dlg.m_ColorButton.GetColor();
					memcpy_s(&tempLin.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
					tempLin.pattern = dlg.m_Pattern;
					GLinTmpNdxF->Seek(GLinNdx * sizeof(LIN_NDX_STRU), CFile::begin);
					GLinTmpNdxF->Write(&tempLin, sizeof(LIN_NDX_STRU));//写入线数据
				}
				this->Invalidate();
				GLinChanged = true;
				GLinNdx = -1;
			}
			break;
		case OPERSTATE_LIN_DELETE_PNT://线上删点
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（窗口转数据）
			DotToPnt(point, dot);
			LIN_NDX_STRU tempLine;
			D_DOT tempDot1,tempDot2;
			memcpy_s(&tempLine, sizeof(LIN_NDX_STRU), &Findline(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx), sizeof(LIN_NDX_STRU));//查找最近的线
			if (GLinNdx != -1)
			{
				FindPntOnLin(point, GLinTmpDatF, tempLine, tempPntNum);//查找线上距离鼠标最近的点
				if (tempLine.dotNum < 3)
				{
					MessageBox(L"点数小于3不能删除", L"Message", MB_OK);
				}
				else
				{
					//删除线上的点
					DelPntOnLin(GLinTmpDatF, tempLine, tempPntNum);
				}
				UpdateLin(GLinTmpNdxF, GLinNdx, tempLine); //更新线数据
				this->Invalidate();//刷新窗口重绘
				GLinChanged = true;
				GLinNdx = -1;
			}
			break;
		case OPERSTATE_LIN_ADD_PNT://线上加点
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y); //坐标系转换（窗口转数据）
			DotToPnt(point, dot);
			LIN_NDX_STRU tempLine1;
			PntToDot(dot, point);
			D_DOT dot3, dot4;
			memcpy_s(&tempLine1, sizeof(LIN_NDX_STRU), &Findline(GLinTmpNdxF, GLinTmpDatF, point, GLinNum, GLinNdx), sizeof(LIN_NDX_STRU));//查找最近的线并复制给tempLine1
			if (GLinNdx != -1)
			{
				//根据鼠标位置查找要加点的线
				for (int i = 0; i < tempLine1.dotNum - 1; i++)
				{
					//读取tempLine1的连续两个节点到dot3和dot4，并计算鼠标到这条线段的距离
					ReadTempFileToLinDat(GLinTmpDatF, tempLine1.datOff, i, dot3);
					ReadTempFileToLinDat(GLinTmpDatF, tempLine1.datOff, i + 1, dot4);
					/*PntDPtoVP(dot3, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot4, GZoom, GZoomOffset_x, GZoomOffset_y);*/
					distance = DisPntToSeg(dot3, dot4, dot);//计算鼠标到dot3和dot4节点所连的线段间的距离
					if (distance < min)
					{
						//若鼠标与线段间的距离小于min，更新以下变量
						min = distance;
						dot5 = dot3;
						dot6 = dot4;
						num1 = i;
						num2 = i + 1;
					}
				}
				POINT pnt5, pnt6;
				if (dot5.x != -1 && dot5.y != -1 && dot6.x != -1 && dot6.y != -1)
				{
					
					//Pnt转Dot
					DotToPnt(pnt5, dot5);
					DotToPnt(pnt6, dot6);
					DrawSeg(&dc, tempLine1, pnt5, pnt6);//重绘，擦除原来的线段
					//重新连接线段的两个端点和鼠标
					DrawSeg(&dc, tempLine1, pnt5, point);
					DrawSeg(&dc, tempLine1, pnt6, point);
					tempLine1.dotNum++;//线节点数加一
					UpdateLin(GLinTmpNdxF, GLinNdx, tempLine1);//更新线数据
					//记录下鼠标上一状态点
					GMPnt.x = dot.x;
					GMPnt.y = dot.y;
				}
				GLinFCreated = true;
				GLineNdx = GLinNdx;
				GLinNdx = -1;
			}
			break;
		default:
			break;
		}
		
	}
	if (GRegFCreated)//区临时文件已创建
	{
		D_DOT dot;
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_REG://造区操作
			if (GTReg.dotNum == 0)
			{
				memcpy_s(&GTReg, sizeof(REG_NDX_STRU), &GReg, sizeof(REG_NDX_STRU));//初始化GTReg
			}
			if (GRegCreateStartPnt.x == -1 && GRegCreateStartPnt.y == -1)
			{
				//造线区起点为（-1，-1），则此时鼠标按下并抬起的位置为造线区起点
				GRegCreateStartPnt = point;
			}
			if (GRegCreateMMPnt.x == -1 && GRegCreateMMPnt.y == -1)
			{
				//鼠标移动前一状态为（-1，-1），则此时鼠标按下并抬起的位置为鼠标移动前一状态
				GRegCreateMMPnt = point;
			}
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//窗口转数据
			WriteRegDatToFile(GRegTmpDatF, GReg.datOff, GTReg.dotNum, dot);//将节点数据写入临时文件
			++GTReg.dotNum;//区边界节点数加一
			if (GTReg.dotNum == 2)
			{
				//区节点数小于3个则取消造区操作
				this->Invalidate();//刷新
			}
			GRegChanged = true;//标记区已更改
			break;
		case OPERSTATE_DELETE_REG://当前为删除区操作
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//窗口转数据坐标
			DotToPnt(point, dot);
			FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);//查找区
			if (GRegNdx != -1)
			{
				//找到符合条件的区
				GRegLNum--;//区逻辑数减一
				GRegChanged = true;//区文件当前状态为已更改
				REG_NDX_STRU TmpRegNdx;
				ReadTempFileToRegNdx(GRegTmpNdxF, GRegNdx, TmpRegNdx);//从临时文件中读取将要删除的区的索引到TmpRegNdx
				TmpRegNdx.isDel = 1;//设置删除标记
				UpdateReg(GRegTmpNdxF, GRegNdx, TmpRegNdx);//更新区数据
				D_DOT* dot = new D_DOT[TmpRegNdx.dotNum];
				GRegTmpDatF->Seek(TmpRegNdx.datOff, CFile::begin);
				GRegTmpDatF->Read(dot, TmpRegNdx.dotNum * sizeof(D_DOT));
				for (int i = 0; i < TmpRegNdx.dotNum; ++i)
				{
					//将删除区的点数据从数据坐标转窗口坐标
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
				}
				POINT* pnt = new POINT[TmpRegNdx.dotNum];
				DotToPnt(pnt, dot, TmpRegNdx.dotNum);
				DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum); //重绘(擦除区)
				delete[] pnt;
				delete[] dot;
				GRegNdx = -1;
			}
			break;
		case OPERSTATE_MOVE_REG://移动区操作
			if (GRegNdx != -1)
			{
				//找到符合条件的区才可进行移动区操作
				if (GRegLBDPnt.x != -1 && GRegLBDPnt.y != -1)
				{
					//鼠标左键按下的位置有效
					D_DOT dot1, dot2;
					//获取鼠标现在的位置并由窗口坐标系转数据坐标系
					PntToDot(dot1, point);
					PntVPtoDP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					//获取左键按下的位置并由窗口坐标系转数据坐标系
					PntToDot(dot2, GRegLBDPnt);
					PntVPtoDP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					//计算数据坐标系中鼠标移动的偏移量
					double offset_x = dot1.x - dot2.x;
					double offset_y = dot1.y - dot2.y;
					UpdateReg(GRegTmpNdxF, GRegTmpDatF, GRegNdx, offset_x, offset_y);//更新区数据
					GRegNdx = -1;//将找到的区位于文件中的位置重新设为-1
					//将偏移量重新设为0
					GRegMMOffsetX = 0;
					GRegMMOffsetY = 0;
					//设置当前文件为已更改
					GRegChanged = true;
				}
			}
			break;
		case OPERSTATE_UNDELETE_REG://当前操作状态为恢复区
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//窗口转数据坐标
			DotToPnt(point, dot);
			FindDeleteReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx);//查找区
			if (GRegNdx != -1)
			{
				//找到符合条件的区
				GRegLNum++;//区逻辑数加一
				GRegChanged = true;//区文件当前状态为已更改
				REG_NDX_STRU TmpRegNdx;
				ReadTempFileToRegNdx(GRegTmpNdxF, GRegNdx, TmpRegNdx);//从临时文件中读取将要删除的区的索引到TmpRegNdx
				TmpRegNdx.isDel = 0;//设置删除标记为未删除
				UpdateReg(GRegTmpNdxF, GRegNdx, TmpRegNdx);//更新区数据
				D_DOT* dot = new D_DOT[TmpRegNdx.dotNum];
				GRegTmpDatF->Seek(TmpRegNdx.datOff, CFile::begin);
				GRegTmpDatF->Read(dot, TmpRegNdx.dotNum * sizeof(D_DOT));
				for (int i = 0; i < TmpRegNdx.dotNum; ++i)
				{
					//将删除区的点数据从数据坐标转窗口坐标
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);
				}
				POINT* pnt = new POINT[TmpRegNdx.dotNum];
				DotToPnt(pnt, dot, TmpRegNdx.dotNum);
				DrawReg(&dc, TmpRegNdx, pnt, TmpRegNdx.dotNum); //重绘(擦除区)
				delete[] pnt;
				delete[] dot;
				GRegNdx = -1;
			}
			break;
		case OPERSTATE_MODIFY_REGION_PARAMETER://修改区参数
			PntToDot(dot, point);
			PntVPtoDP(dot, GZoom, GZoomOffset_x, GZoomOffset_y);//窗口转数据坐标
			DotToPnt(point, dot);
			REG_NDX_STRU tempReg;
			memcpy_s(&tempReg, sizeof(REG_NDX_STRU), &FindReg(GRegTmpNdxF, GRegTmpDatF, point, GRegNum, GRegNdx), sizeof(REG_NDX_STRU));//查找最近的区
			if (GRegNdx != -1)
			{
				CRegParameterDlg dlg; //线参数设置对话框
				dlg.m_ColorButton.SetColor(tempReg.color);
				dlg.m_Pattern = tempReg.pattern;
				if (IDOK == dlg.DoModal())
				{
					COLORREF tempColor = dlg.m_ColorButton.GetColor();
					memcpy_s(&tempReg.color, sizeof(COLORREF), &tempColor, sizeof(COLORREF));
					tempReg.pattern = dlg.m_Pattern;
					GRegTmpNdxF->Seek(GRegNdx * sizeof(REG_NDX_STRU), CFile::begin);
					GRegTmpNdxF->Write(&tempReg, sizeof(REG_NDX_STRU));//写入区数据
				}
				this->Invalidate();
				GRegChanged = true;
				GRegNdx = -1;
			}
			break;
		default:
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		RECT client, rect;
		double zoom = 1.0;
		switch (GCurOperState)
		{
		case OPERSTATE_ZOOM_IN:
			GetClientRect(&client);
			if (abs(GZoomLBDPnt.x - GZoomMMPnt.x) <= 10 && abs(GZoomLBDPnt.y - GZoomMMPnt.y) <= 10)//鼠标按下和抬起的位置间距离不大，放大模式为单击放大
			{
				GZoomStyle = 0; //单击放大
			}
			else//否则为拉框放大
			{
				GZoomStyle = 1; //拉框放大
			}
			if (GZoomStyle == 0)//单击放大
			{
				double xO = point.x - (client.right/2.0) + (client.right / 8.0);
				double yO = point.y - (client.bottom/2.0) + (client.bottom/8.0);
				GZoomOffset_x += (xO/GZoom); //偏移向量x
				GZoomOffset_y += (yO/GZoom); //偏移向量y
				GZoom*= 4 /3.0; //缩放系数为4/3
			}
			else//拉框放大
			{
				rect.right = max(point.x, GZoomLBDPnt.x);
				rect.left = min(point.x, GZoomLBDPnt.x);
				rect.bottom = max(point.y, GZoomLBDPnt.y);
				rect.top = min(point.y, GZoomLBDPnt.y);
				modulusZoom(client, rect, zoom);//计算放大倍数
				double x0 = GetCenter(rect).x - (client.right / 2.0) + (client.right *(zoom - 1) / (2.0 * zoom));
				double y0 = GetCenter(rect).y - (client.bottom / 2.0) + (client.bottom * (zoom - 1) / (2.0 * zoom));
				GZoomOffset_x += (x0 / GZoom); //偏移向量
				GZoomOffset_y += (y0 / GZoom);//偏移向量y
				GZoom *= zoom; //缩放系数
				GZoomStyle = 0;
			}
			GZoomLBDPnt = CPoint(-1, -1);
			GZoomMMPnt = CPoint(-1, -1);
			this->Invalidate();
			break;
		case OPERSTATE_ZOOM_OUT://当前为缩小状态
			if (true)
			{
				GetClientRect(&client);
				double x0 = point.x - (client.right / 2.0) - (client.right / 8.0);
				double y0 = point.y - (client.bottom / 2.0) - (client.bottom / 8.0);
				GZoomOffset_x += (x0 / GZoom); //偏移向量x
				GZoomOffset_y += (y0 / GZoom);//偏移向量y
				GZoom *= 3 / 4.0; //缩放系数为3/4
				this->Invalidate();
			}
			break;
		case OPERSTATE_WINDOW_MOVE ://当前为移动操作状态
				GWinMoveLBDPnt = CPoint(-1, -1);//复位移动窗口时左键按下点
				GWinMoveMMPnt = CPoint(-1, -1);//复位移动窗口鼠标移动前状态点位置
				break;
		default:
			break;
		}
	}
	CView::OnLButtonUp(nFlags, point);
}





void CMapEditorView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//点
	if (GPntFCreated)//如点临时文件已创建，则关闭并删除
	{
		if (GPntTmpF->m_hFile != CFile::hFileNull)
		{
			GPntTmpF->Close();
			GPntTmpF->Remove(GPntTmpFName);
		}
	}
	delete GPntTmpF;//删除点临时对象

	//线
	if (GLinFCreated)//如线临时文件已创建，则关闭并删除
	{
		if (GLinTmpDatF->m_hFile != CFile::hFileNull)
		{
			GLinTmpDatF->Close();
			GLinTmpDatF->Remove(GLinTmpDatFName);
		}
		if (GLinTmpNdxF->m_hFile != CFile::hFileNull)
		{
			GLinTmpNdxF->Close();
			GLinTmpNdxF->Remove(GLinTmpNdxFName);
		}
	}
	delete GLinTmpDatF;//删除线临时数据对象
	delete GLinTmpNdxF;//删除线临时索引对象

	//区
	if (GRegFCreated)//如区临时文件已创建，则关闭并删除
	{
		if (GRegTmpDatF->m_hFile != CFile::hFileNull)
		{
			GRegTmpDatF->Close();
			GRegTmpNdxF->Remove(GRegTmpDatFName);
		}
		if (GRegTmpNdxF->m_hFile != CFile::hFileNull)
		{
			GRegTmpNdxF->Close();
			GRegTmpNdxF->Remove(GRegTmpNdxFName);
		}
	}
	delete GRegTmpDatF;//删除区临时数据对象
	delete GRegTmpNdxF;//删除区临时索引对象
}


void CMapEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnMouseMove(nFlags, point);
	if (GPntFCreated)//点临时文件已创建
	{
		switch (GCurOperState)
		{
		case OPERSTATE_MOVE_PNT://移动点操作
			if (GPntNdx != -1)
			{
				CClientDC dc(this); //获得本窗口或当前活动视图
				dc.SetROP2(R2_NOTXORPEN);//设置异或模式画点
				DrawPnt(&dc, GTPnt); //在原位置画，清除原点图形
				GTPnt.x = point.x; //移动点的坐标x
				GTPnt.y = point.y;// 移动点的坐标y
				DrawPnt(&dc, GTPnt); //在新位置画
			}
			break;
		default:
			break;
		}
	}
	if (GLinFCreated)//线临时文件已创建
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_LIN://画线操作
			if (GTLin.dotNum > 0)//线的节点数大于0
			{
				CClientDC dc(this); //获得本窗口或当前活动视图
				dc.SetROP2(R2_NOTXORPEN); //设置异或模式画线
				if (GMPnt.x != -1 && GMPnt.y != -1)
				{
					DrawSeg(&dc, GTLin, GLPnt, GMPnt); //默认样式绘制线段
				}
				GMPnt.x = point.x; //设置鼠标上一状态点(x)
				GMPnt.y = point.y; //设置鼠标上一状态点(y)
				POINT mpoint = { mpoint.x = point.x , mpoint.y = point.y };
				DrawSeg(&dc, GTLin, GLPnt, mpoint); //默认样式绘制线段
			}
			break;
		case OPERSTATE_MOVE_LIN://移线操作
			if (GLinNdx != -1)//找到范围内的符合条件的线
			{
				CClientDC dc(this);//获得本窗口或当前活动视图
				dc.SetROP2(R2_NOTXORPEN); //设置异或模式
				D_DOT dot1, dot2;
				POINT pnt1, pnt2;
				//擦除原来的线
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)
				{
					//从临时线节点数据文件读取两个节点到dot1，dot2
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff,i,dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff,i+1,dot2);
					//坐标系转换（数据转窗口）
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					//Dot转Pnt
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					//节点偏移
					pnt1.x += GLinMMOffsetX;
					pnt1.y += GLinMMOffsetY;
					pnt2.x += GLinMMOffsetX;
					pnt2.y += GLinMMOffsetY;
					//在鼠标上一位置画线
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);
				}
				//计算偏移量
				GLinMMOffsetX = GLinMMOffsetX + point.x - GLinMMPnt.x;
				GLinMMOffsetY = GLinMMOffsetY + point.y - GLinMMPnt.y;
				//在新的位置绘制一条新的线段
				for (int i = 0; i < GLinMMTmpNdx.dotNum - 1; i++)
				{
					//从临时线节点数据文件读取两个节点到dot1，dot2
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i, dot1);
					ReadTempFileToLinDat(GLinTmpDatF, GLinMMTmpNdx.datOff, i + 1, dot2);
					//转换坐标系（数据转窗口）
					PntDPtoVP(dot1, GZoom, GZoomOffset_x, GZoomOffset_y);
					PntDPtoVP(dot2, GZoom, GZoomOffset_x, GZoomOffset_y);
					//Dot转Pnt
					DotToPnt(pnt1, dot1);
					DotToPnt(pnt2, dot2);
					//节点偏移
					pnt1.x += GLinMMOffsetX;
					pnt1.y += GLinMMOffsetY;
					pnt2.x += GLinMMOffsetX;
					pnt2.y += GLinMMOffsetY;
					//在目前鼠标所在位置画线
					DrawSeg(&dc, GLinMMTmpNdx, pnt1, pnt2);
				}
				GLinMMPnt = point;//记录下此时鼠标的位置，作为下一次移动时的上一次位置
			}
			break;
		case OPERSTATE_LIN_ADD_PNT://线上加点
				LIN_NDX_STRU LinNdx;
				POINT pnt5, pnt6;
				if (GLineNdx != -1)
				{
					ReadTempFileToLinNdx(GLinTmpNdxF, GLineNdx, LinNdx);//从临时文件读取线索引
					if (dot5.x != -1 && dot5.y != -1 && dot6.x != -1 && dot6.y != -1)
					{
						/*PntDPtoVP(dot5, GZoom, GZoomOffset_x, GZoomOffset_y);
						PntDPtoVP(dot5, GZoom, GZoomOffset_x, GZoomOffset_y);*/
						DotToPnt(pnt5, dot5);
						DotToPnt(pnt6, dot6);
						CClientDC dc(this); //获得本窗口或当前活动视图
						dc.SetROP2(R2_NOTXORPEN); //设置异或模式画线
						if (GMPnt.x != -1 && GMPnt.y != -1)
						{
							//在上一位置重绘以消除原来的线
							DrawSeg(&dc, LinNdx, pnt5, GMPnt);
							DrawSeg(&dc, LinNdx, pnt6, GMPnt);
							//在现在的位置画线
							DrawSeg(&dc, LinNdx, pnt5, point);
							DrawSeg(&dc, LinNdx, pnt6, point);
							//上一位置坐标更新
							GMPnt.x = point.x;
							GMPnt.y = point.y;
						}
					}
				}
			break;
		default:
			break;
		}
	}
	if (GRegFCreated)//区临时文件已创建
	{
		switch (GCurOperState)
		{
		case OPERSTATE_INPUT_REG://当前操作状态为造区
			if (GRegCreateMMPnt.x != -1 && GRegCreateMMPnt.y != -1)
			{
				CClientDC dc(this);//获得本窗口或当前活动视图
				dc.SetROP2(R2_NOTXORPEN);//设置异或模式
				LIN_NDX_STRU tln = { tln.pattern = GTReg.pattern ,tln.color = GTReg.color }; //设置区参数
				if (GTReg.dotNum == 1)
				{
					DrawSeg(&dc, tln, GRegCreateStartPnt, GRegCreateMMPnt);
					DrawSeg(&dc, tln, GRegCreateStartPnt, point);
				}
				else
				{
					D_DOT* dot = new D_DOT[GTReg.dotNum];
					for (int i = 0; i < GTReg.dotNum; ++i)
					{
						ReadTempFileToRegDat(GRegTmpDatF, GTReg.datOff, i, dot[i]);//从临时文件读取区的点数据
						PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);//将区的点数据坐标转换为窗口坐标
					}
					POINT* pnt = new POINT[GTReg.dotNum + 1];
					DotToPnt(pnt, dot, GTReg.dotNum);
					//异或模式造区
					pnt[GTReg.dotNum] = GRegCreateMMPnt;
					DrawReg(&dc, GTReg, pnt, GTReg.dotNum + 1);//画鼠标前一状态的区
					pnt[GTReg.dotNum] = point;
					DrawReg(&dc, GTReg, pnt, GTReg.dotNum + 1);//画鼠标现在状态画区，消除前一状态的区
					//释放动态内存
					delete[] dot;
					delete[] pnt;
				}
				GRegCreateMMPnt = point;
			}
			break;
		case OPERSTATE_MOVE_REG://移动区操作
			if (GRegNdx != -1)
			{
				CClientDC dc(this);
				dc.SetROP2(R2_NOTXORPEN); //设置异或模式
				D_DOT* dot = new D_DOT[GRegMMTmpNdx.dotNum];
				//擦除原来的区
				for (int i = 0; i < GRegMMTmpNdx.dotNum; i++)
				{
					ReadTempFileToRegDat(GRegTmpDatF, GRegMMTmpNdx.datOff, i, dot[i]);//从临时文件中读取节点数据到dot[i]
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);//数据坐标系转窗口坐标系
					//节点偏移（此时偏移量为0）
					dot[i].x += GRegMMOffsetX;
					dot[i].y += GRegMMOffsetY;
				}
				POINT* pnt = new POINT[GRegMMTmpNdx.dotNum];
				DotToPnt(pnt, dot, GRegMMTmpNdx.dotNum);//所有节点由Dot转Pnt
				DrawReg(&dc, GRegMMTmpNdx, pnt, GRegMMTmpNdx.dotNum);//重绘，注释掉原来的区
				//计算偏移量
				GRegMMOffsetX = GRegMMOffsetX + point.x - GRegMMPnt.x;
				GRegMMOffsetY = GRegMMOffsetY + point.y - GRegMMPnt.y;
				//在新的位置绘制一个新的区
				for (int i = 0; i < GRegMMTmpNdx.dotNum; i++)
				{
					ReadTempFileToRegDat(GRegTmpDatF, GRegMMTmpNdx.datOff, i, dot[i]);//从临时文件读取区节点数据到dot[i]
					PntDPtoVP(dot[i], GZoom, GZoomOffset_x, GZoomOffset_y);//数据坐标系转窗口坐标系
					//节点偏移
					dot[i].x += GRegMMOffsetX;
					dot[i].y += GRegMMOffsetY;
				}
				DotToPnt(pnt, dot, GRegMMTmpNdx.dotNum);//所有节点由Dot转Pnt
				DrawReg(&dc, GRegMMTmpNdx, pnt, GRegMMTmpNdx.dotNum);//在新的位置画新的区
				//释放动态内存
				delete[] dot;
				delete[] pnt;
				GRegMMPnt = point;
			}
			break;
		default:
			break;
		}
	}
	if (GPntFCreated || GLinFCreated || GRegFCreated)
	{
		CClientDC dc(this); //获得本窗口或当前活动视图
		CPen pen(PS_DOT, 1, RGB(0, 0, 0)); /* .......  */
		CPen * oldPen = dc.SelectObject(&pen);
		switch (GCurOperState)
		{
			case OPERSTATE_ZOOM_IN : //当前为放大操作状态
			if (GZoomMMPnt.x != -1 && GZoomMMPnt.y != -1)//鼠标移动前的点有效
			{
				dc.SetROP2(R2_NOTXORPEN); //设置异或模式画线
				//在客户区域绘画矩形
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, GZoomMMPnt.x, GZoomMMPnt.y);
				dc.Rectangle(GZoomLBDPnt.x, GZoomLBDPnt.y, point.x, point.y);
				GZoomMMPnt = point;
				dc.SelectObject(oldPen);
			}
			    break;
			case OPERSTATE_WINDOW_MOVE ://当前为移动操作状态
			if(GWinMoveMMPnt.x != -1 && GWinMoveMMPnt.y != -1)
				{
					CPoint offset(0, 0); //鼠标移动偏移量
					//计算鼠标偏移量
					offset.x = point.x - GWinMoveLBDPnt.x;
					offset.y = point.y - GWinMoveLBDPnt.y;
					//变换放大与缩小时的偏移量
					GZoomOffset_x -= offset.x / GZoom; 
					GZoomOffset_y -= offset.y / GZoom;
					GWinMoveLBDPnt = point;
					this->Invalidate();//使窗口无效并重绘
				}
				break;

			default:
				break;
		}
	}
}


void CMapEditorView::OnUpdateWindowShowPoint(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//显示状态是显示未删除状态并且显示点则将菜单标记选中
	if (GCurShowState == SHOWSTATE_UNDEL && GShowPnt == true)
	{
		pCmdUI->SetCheck(1);//菜单选中标记
	}
	else
	{
		pCmdUI -> SetCheck(0);
	}

}


void CMapEditorView::OnUpdateWindowShowLine(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//显示状态是显示未删除状态并且显示点则将菜单标记选中
	if (GCurShowState == SHOWSTATE_UNDEL && GShowLin == true)
	{
		pCmdUI->SetCheck(1);//菜单选中标记
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CMapEditorView::OnUpdateWindowShowRegion(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//显示状态是显示未删除状态并且显示点则将菜单标记选中
	if (GCurShowState == SHOWSTATE_UNDEL && GShowReg == true)
	{
		pCmdUI->SetCheck(1);//菜单选中标记
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CMapEditorView::OnUpdatePointShowDeleted(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//若当前显示状态是显示删除状态且显示点,菜单标记选中;否则取消
	if(GCurShowState == SHOWSTATE_DEL &&GShowPnt == true)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CMapEditorView::OnUpdateLineShowDeleted(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//若当前显示状态是显示删除状态且显示线,菜单标记选中;否则取消
	if (GCurShowState == SHOWSTATE_DEL && GShowLin == true)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}


void CMapEditorView::OnUpdateRegionShowDeleted(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	//若当前显示状态是显示删除状态且显示区,菜单标记选中;否则取消
	if (GCurShowState == SHOWSTATE_DEL && GShowReg == true)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}
