#ifndef GRAPHICSTATION_H
#define GRAPHICSTATION_H

#include "GraphicMember.h"

class GraphicStation : public GraphicMember
{
public:
	explicit GraphicStation(const quint32& unNo, const QString& name = "");
protected:
	quint32 m_unNo;						/*!< 编号 */
	QString m_strName;					/*!< 名称 */
public:
	/*!
	 * @brief 获取编号
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @return QT_NAMESPACE::quint32 编号
	 */
	quint32 GetNo() const { return m_unNo; }

	/*!
	 * @brief 设置名称
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 新名称
	 */
	void SetName(const QString& name) { m_strName = name; return; }

	/*!
	 * @brief 获取名称
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 名称
	 */
	QString GetName() const { return m_strName; }
};

#endif // GRAPHICSTATION_H
