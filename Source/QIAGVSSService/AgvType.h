/*!
 * @copyright (C), 2014-2019, Harbin brain robot technology co. LTD.
 *
 * @contact www.brainchina.com.cn
 *
 * @file AgvType.h
 *
 * @brief 描述AGV类型的属性的文件
 *
 * @date 2019/11/16
 *
 * @author FanKaiyu
 *
 * @version 1.0
 */
#ifndef AGVTYPE_H
#define AGVTYPE_H

#include <QString>
#include <QJsonObject>
#include <memory>
#include <map>
#include "Protocol.h"
#include "AgvAttribute.h"

using namespace std;

class AgvType;
typedef std::shared_ptr<AgvType> AgvTypePtr;
typedef std::map<QString, AgvTypePtr> AgvTypeArray;

/*!
 * @class AgvType
 * @brief 描述AGV类型的属性以及功能的类
*/
class AgvType
{
protected:
	QString m_strName;				/*!< 名称 */

public:
	QString m_strBrand;				/*!< 品牌 */
	QString m_strVersion;			/*!< 类型 */
	float m_fMaxSpeed;					/*!< 最大速度 */
	float m_fMaxWeight;					/*!< 最大载重量 */

protected:
	std::shared_ptr<ProtocolBase> m_pProtocol;	/*!< 使用的通信协议 */
	std::unique_ptr<QString[]> m_listName;		/*!< 动作名称列表 */

public:
	/*!
	 * @brief AgvType
	 *
	 * AgvType::AgvType
	 * @date 2019/11/22
	 * @author FanKaiyu
	 * @param const std::string & 类型名称
	 * @param const std::shared_ptr<ProtocolBase> & 指向通信协议对象的智能指针
	 * @param const float & 最大速度
	 * @param const float & 最大载重
	 * @param const std::string & 品牌
	 * @param const std::string & 型号
	 * @return
	 */
	explicit AgvType(const QString&, const std::shared_ptr<ProtocolBase>&, const float&,
		const float& = 0.0f, const QString & = "", const QString & = "");

	AgvType(const AgvType& type);

public:
	operator bool() const;
	bool operator==(const bool& _bool) const;
	bool operator!=(const bool& _bool) const;
	void operator=(const AgvType& type);

public:
	bool IsNull() const;

	/*!
	 * @brief 获取AGV的通信协议对象
	 * @date 2019/11/16
	 * @author FanKaiyu
	 * @return const std::shared_ptr<ProtocolBase> 通信协议对象指针
	 */
	const std::shared_ptr<ProtocolBase> GetProtocol() const;

	/*!
	 * @brief 获取动作名称
	 * @param const AgvAttr::action_t& 获取动作码
	 * @return QString 动作名称
	*/
	QString GetActionName(const AgvAction& action) const;

	/*!
	 * @brief 获取动作名称对应的动作码
	 * @param const QString& 动作名称
	 * @return const AgvAttr::action_t& 动作码
	*/
	AgvAction GetActionCode(const QString& name) const;

	/*!
	 * @brief 加载动作名称列表
	 * @param const QJsonArray& 列表配置的Json字符串
	*/
	void LoadNameList(const QJsonObject& Json);

	/*!
	 * @brief 获取类型名称
	 * @return string 类型名称
	*/
	QString GetName() const { return m_strName; }

	float GetMaxSpeed() const { return m_fMaxSpeed; }

	float GetMaxWeight() const { return m_fMaxWeight; }

	QString GetBrand() const { return m_strBrand; }

	QString GetVersion() const { return m_strVersion; }
};

#endif // AGVTYPE_h
