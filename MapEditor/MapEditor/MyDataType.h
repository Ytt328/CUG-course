#pragma once
//定义数据结构



#ifndef MYDATATYPE_H
#define MYDATATYPE_H
#include "pch.h"

//点的数据结构
typedef struct {
	double x;//点位坐标x
	double y;//点位坐标y
	COLORREF color;//点颜色
	int pattern;//点图案号
	char isDel;//是否被删除
}PNT_STRU;

//文件版本结构
typedef struct {
	char flag[3]; //标志符PNT LIN REG
	int version; //10，可理解为1.0 版本
} VERSION;


//线索引结构
typedef struct {
	char isDel; //是否被删除
	COLORREF color; //线颜色
	int pattern; //线型（ 号）
	long dotNum; //线节点数
	long datOff; //线节点坐标数据存储位置
}LIN_NDX_STRU;


//线和区的节点数据结构
typedef struct {
	double x; //节点x坐标
	double y; //节点y坐标
} D_DOT;


//区索引结构
typedef struct {
	char isDel; //是否被删除
	COLORREF color; //区颜色
	int pattern; //图案（号）
	long dotNum; //边界节点数
	long datOff; //边界节点数据存储位置
}REG_NDX_STRU;

#endif
