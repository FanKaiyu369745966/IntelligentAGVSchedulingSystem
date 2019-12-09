#ifndef ZIGBEECALLER_H
#define ZIGBEECALLER_H

#include "CallerBase.h"
#include "CallerAttribute.h"
#include <string>
#include <memory>
#include <map>

typedef class ZigbeeCaller ZgbCaller;
typedef std::shared_ptr<ZigbeeCaller> CallerPtr;
typedef std::map<unsigned short, CallerPtr> CallerArray;

class ZigbeeCaller :
	public CallerBase
{
public:
	ZigbeeCaller();
	explicit ZigbeeCaller(const unsigned short& id, std::string addr = "", const unsigned short& = 0, const bool& = true);
	ZigbeeCaller(const ZigbeeCaller&);
protected:
	std::string m_strAddr;			/*!< MAC地址 */
	CallerAttr m_attribute;			/*!< 呼叫器属性 */
public:
	void operator=(const ZigbeeCaller&);
public:
	/*!
	 * @brief 设置地址
	 * @param const std::string& MAC地址
	*/
	void SetAddress(const std::string& addr);

	/*!
	 * @brief 获取地址
	 * @return std::string MAC地址
	*/
	std::string GetAddress() const { return m_strAddr; }

	/*!
	 * @brief 获取属性
	 * @return AgvAttr Agv属性状态;
	*/
	const CallerAttr GetAttribute() const { return m_attribute; }
	CallerAttr* GetAttributePtr() { return &m_attribute; }

	/*!
	 * @brief 更新属性
	 * @param const AgvAttr& 新的属性
	 * @return bool 当属性发生改变时返回true,否则返回false
	*/
	bool UpdateAttribute(const CallerAttr& attr) { return m_attribute.UpdateSignal(attr); }
};

#endif // ZIGBEECALLER_H
