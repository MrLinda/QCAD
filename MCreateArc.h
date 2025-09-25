#pragma once
#include "MCommand.h"

class CArc;  // ����������
class MLine;

class MCreateArc : public MCommand
{
private:
    QPointF m_center;    // Բ�������ĵ�
    QPointF m_begin;     // Բ�������
    QPointF m_end;       // Բ�����յ�
    CArc* m_pArc;        // ��ǰ�����Ļ�����
    MLine* m_pLine1; //��һ��������
    MLine* m_pLine2; //�ڶ���

public:
    // ���캯������������
    MCreateArc(QCADView* pDiagramScene);
    ~MCreateArc();


    int GetType();
    int OnLButtonDown(QMouseEvent* mouseEvent);
    int OnRButtonDown(QMouseEvent* mouseEvent);
    int OnMouseMove(QMouseEvent* mouseEvent);
    int Cancel();
};
#pragma once
