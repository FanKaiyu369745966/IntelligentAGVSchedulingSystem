#ifndef GRAPHICRESTSTATION_H
#define GRAPHICRESTSTATION_H

#include "GraphicStation.h"

typedef class GraphicRestStation IconRS;

class GraphicRestStation :
	public GraphicStation
{
public:
	explicit GraphicRestStation(const quint32& unNo, const QString& name = "");
protected:
	static quint32 g_unWidth;	/*!< 图标宽 */
	static quint32 g_unHeight;	/*!< 图标高 */
	static QImage g_icon;		/*!< 图标 */
public:
	/*!
	 * @brief 设置图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetIcon(const QString& path);

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
public:
	/*!
	 * @brief 绘制图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param QPainter & painter
	 */
	void Draw(QPainter& painter);
};

#endif // GRAPHICRESTSTATION_H
