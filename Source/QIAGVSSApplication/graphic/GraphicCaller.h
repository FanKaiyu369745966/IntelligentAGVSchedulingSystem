#ifndef GRAPHICCALLER_H
#define GRAPHICCALLER_H

#include "GraphicMember.h"

typedef	class GraphicCaller IconCaller;

class GraphicCaller :
	public GraphicMember
{
public:
	explicit GraphicCaller(const quint32&);
protected:
	quint32 m_unNo;				/*!< 编号 */
protected:
	static quint32 g_unWidth;   /*!< 图标宽 */
	static quint32 g_unHeight;  /*!< 图标高 */
protected:
	static QImage g_enableIcon;	/*!< 呼叫器启用图标 */
	static QImage g_disableIcon;/*!< 呼叫器禁用图标 */
	static QImage g_screamIcon;	/*!< 急停信号图标 */
	static QImage g_callIcon;	/*!< 呼叫信号图标 */
	static QImage g_passIcon;	/*!< 放行信号图标 */
	static QImage g_cancelIcon;	/*!< 取消信号图标 */
protected:
	bool m_bEnable;				/*!< 启用标识 */
	bool m_bSigScream;			/*!< 急停信号 */
	bool m_bSigCall;			/*!< 呼叫器信号 */
	bool m_bSigCancel;			/*!< 取消信号 */
	bool m_bSigPass;			/*!< 放行信号 */
public:
	/*!
	 * @brief 获取编号
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::quint32
	 */
	quint32 GetNo() const { return m_unNo; }

	/*!
	 * @brief 设置启用图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetEnableIcon(const QString& path);

	/*!
	 * @brief 设置禁用图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetDisableIcon(const QString& path);

	/*!
	 * @brief 设置急停信号图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetScreamIcon(const QString& path);

	/*!
	 * @brief 设置呼叫信号图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetCallIcon(const QString& path);

	/*!
	 * @brief 设置放行信号图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetPassIcon(const QString& path);

	/*!
	 * @brief 设置取消信号图标
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 图标路径
	 */
	static void SetCancelIcon(const QString& path);

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
	 * @brief 设置信号
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @param const bool & 急停信号
	 * @param const bool & 呼叫信号
	 * @param const bool & 放行信号
	 * @param const bool & 取消信号
	 */
	void SetSignal(const bool& scream, const bool& call, const bool& pass, const bool& cancel);

	/*!
	 * @brief 设置启用状态
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @param const bool & 急停信号
	 * @param const bool & 呼叫信号
	 * @param const bool & 放行信号
	 * @param const bool & 取消信号
	 */
	void SetEnable(const bool& enable);

	/*!
	 * @brief 绘制图标
	 * @date 2019/11/20
	 * @author FanKaiyu
	 * @param QPainter & painter
	 */
	void Draw(QPainter& painter);
};

#endif // GRAPHICCALLER_H
