#ifndef GRAPHICBACKGROUND_H
#define GRAPHICBACKGROUND_H

#include "Graphic.h"
#include <mutex>

typedef	class GraphicBackground GraphicBg;

class GraphicBackground :
	public Graphic
{
public:
	explicit GraphicBackground(const QColor& colorLine = Qt::black, const float& fWidth = 2.0f, const QString& strPath = "./res/image/background.jpg");
protected:
	bool m_bMove;			// 移动标识
	QPoint m_ptPrepoint;	// 起始坐标
	QPoint m_ptLastpoint;	// 终止坐标
	QPoint m_ptLocal;		// 偏移量记录
protected:
	QImage m_pImage;	/*!< 背景图 */
protected:
	std::mutex m_mutexLock;
public:
	/**
	 * @brief setImage  设置背景图
	 * @param strPath   背景图路径
	 */
	void SetImage(const QString& strPath);

	/**
	 * @brief scale     缩放
	 * @param bShrink   true 放大， false 缩小
	 */
	void Scale(bool bShrink = false);

	/**
	 * @brief reset 复位
	 */
	void Reset();

	/**
	 * @brief drag          拖拽
	 * @param ptWinPoint    窗口坐标
	 * @return 鼠标指针形状
	 */
	QCursor Drag(const QPoint& ptWinPoint);

	/**
	 * @brief select        选中
	 * @param ptWinPoint    窗口坐标
	 * @details 鼠标左键点击底图,触发此函数
	 */
	void Select(const QPoint& ptWinPoint);

	/**
	 * @brief cancel 取消拖拽
	 */
	void Cancel();

	/**
	 * @brief confirm 完成拖拽
	 */
	void Confirm();

public:
	/**
	 * @brief drawBg    绘制背景图
	 * @param painter   绘图
	 */
	void Draw(QPainter& painter);
};
#endif // GRAPHICBACKGROUND_H
