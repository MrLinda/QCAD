#pragma once

#include <QVector>
class CCommand;
//命令序列，用于记录创建、修改图元实体的操作过程
//主程序启动时，新建；主程序退出时，删除（清楚全部内存）
// 
/*
MCommand对象改进：
成员数据增加：实体指针，操作参数（创建不需要）；
成员函数增加：Undo(), Redo();
*/

//每次操作(创建、修改或删除）时，需根据m_curPos重整当前序列，然后将操作加入序列；
// Undo时，m_curPos--并调用命令元素内的实体的Undo()；
// Redo时，m_curPos++并调用命令元素内的实体的Redo()；
//


/*
*  命令操作类型      Undo                        Redo
*  创建实体          移除实体出显示列表          将实体再加入显示列表
*  移动/旋转/镜像    反向操作，回原来位置        执行操作（需记录操作参数和实体集）
*  删除              将实体再加入显示            移除实体出显示列表
*  其他（如字体、线型、颜色）  都需要定义其操作命令，参数和实体集，对应实现Undo/redo
*/

class CCommandQrue  //应该 queue
{
private:
	unsigned int m_curPos; //记录当前位置，用于Undo/Redo
	QVector<CCommand*> m_vCommands;  //全部的操作序列

public:
	CCommandQrue();
	~CCommandQrue();

public:
	void Undo(); //撤销
	void Redo(); //重做
	
	void AddCommands(CCommand* pCmd);  //增加命令操作
	void AdjustCommands();   //整理序列，将多余的序列去除，可能需要删除实体
	void DeleteAll();        //删除全部命令序列，真正删除实体内存
};
