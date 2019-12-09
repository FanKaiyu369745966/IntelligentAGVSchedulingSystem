#ifndef GRAPHICRFID_H
#define GRAPHICRFID_H

#include "GraphicMember.h"

typedef class GraphicRFID IconRFID;

class GraphicRFID : public GraphicMember
{
public:
	explicit GraphicRFID(const quint32&);
protected:
	quint32 m_unNo;								/*!< 编号 */
	static quint32 g_unWidth;					/*!< 图标宽 */
	static quint32 g_unHeight;					/*!< 图标高 */
public:
	/*!
	 * @brief 获取编号
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::quint32
	 */
	quint32 GetNo() const { return m_unNo; }

	/*!
	 * @brief 获取图标宽
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::quint32
	 */
	static quint32 GetWidth() { return g_unWidth; }

	/*!
	 * @brief 设置图标宽
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const quint32 & 图标宽
	 */
	static void SetWidth(const quint32& width) { g_unWidth = width; return; }

	/*!
	 * @brief 获取图标高
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::quint32
	 */
	static quint32 GetHeight() { return g_unHeight; }

	/*!
	 * @brief 设置图标高
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const quint32 & 图标高
	 */
	static void SetHeight(const quint32& height) { g_unHeight = height; return; }

	/*!
	 * @brief 绘制图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param QPainter & painter
	 */
	void Draw(QPainter& painter);
};

#endif // GRAPHICRFID_H
