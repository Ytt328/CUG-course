#pragma once
//�������ݽṹ



#ifndef MYDATATYPE_H
#define MYDATATYPE_H
#include "pch.h"

//������ݽṹ
typedef struct {
	double x;//��λ����x
	double y;//��λ����y
	COLORREF color;//����ɫ
	int pattern;//��ͼ����
	char isDel;//�Ƿ�ɾ��
}PNT_STRU;

//�ļ��汾�ṹ
typedef struct {
	char flag[3]; //��־��PNT LIN REG
	int version; //10�������Ϊ1.0 �汾
} VERSION;


//�������ṹ
typedef struct {
	char isDel; //�Ƿ�ɾ��
	COLORREF color; //����ɫ
	int pattern; //���ͣ� �ţ�
	long dotNum; //�߽ڵ���
	long datOff; //�߽ڵ��������ݴ洢λ��
}LIN_NDX_STRU;


//�ߺ����Ľڵ����ݽṹ
typedef struct {
	double x; //�ڵ�x����
	double y; //�ڵ�y����
} D_DOT;


//�������ṹ
typedef struct {
	char isDel; //�Ƿ�ɾ��
	COLORREF color; //����ɫ
	int pattern; //ͼ�����ţ�
	long dotNum; //�߽�ڵ���
	long datOff; //�߽�ڵ����ݴ洢λ��
}REG_NDX_STRU;

#endif
