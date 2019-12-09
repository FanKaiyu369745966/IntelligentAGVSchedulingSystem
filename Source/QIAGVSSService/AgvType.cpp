#include "AgvType.h"

AgvType::AgvType(const QString& strName, const std::shared_ptr<ProtocolBase>& protocol, const float& fMaxSpeed
	, const float& fMaxWeight, const QString& strBrand, const QString& strVersion)
	: m_strName(strName)
	, m_strBrand(strBrand)
	, m_strVersion(strVersion)
	, m_fMaxSpeed(fMaxSpeed)
	, m_fMaxWeight(fMaxWeight)
	, m_pProtocol(protocol)
	, m_listName(new QString[UCHAR_MAX])
{
	m_listName[0] = QString::fromLocal8Bit("无动作");
	m_listName[UCHAR_MAX - 1] = QString::fromLocal8Bit("移动");
}

AgvType::AgvType(const AgvType& type)
{
	m_strName = type.m_strName;
	m_strBrand = type.m_strBrand;
	m_strVersion = type.m_strVersion;
	m_fMaxSpeed = type.m_fMaxSpeed;
	m_fMaxWeight = type.m_fMaxWeight;
	m_pProtocol = type.m_pProtocol;

	for (int i = 0; i < UCHAR_MAX; ++i)
	{
		m_listName[i] = type.m_listName[i];
	}
}

AgvType::operator bool() const
{
	return (!IsNull());
}

bool AgvType::operator==(const bool& _bool) const
{
	return (!IsNull()) && _bool;
}

bool AgvType::operator!=(const bool& _bool) const
{
	return !((!IsNull()) == _bool);
}

void AgvType::operator=(const AgvType& type)
{
	m_strName = type.m_strName;
	m_strBrand = type.m_strBrand;
	m_strVersion = type.m_strVersion;
	m_fMaxSpeed = type.m_fMaxSpeed;
	m_fMaxWeight = type.m_fMaxWeight;
	m_pProtocol = type.m_pProtocol;

	for (int i = 0; i < UCHAR_MAX; ++i)
	{
		m_listName[i] = type.m_listName[i];
	}

	return;
}

bool AgvType::IsNull() const
{
	if (m_strName.isNull()
		|| m_strName.isEmpty()
		|| m_fMaxSpeed == 0
		|| m_pProtocol == nullptr)
	{
		return true;
	}

	return false;
}

const std::shared_ptr<ProtocolBase> AgvType::GetProtocol() const
{
	return m_pProtocol;
}

QString AgvType::GetActionName(const AgvAction& action) const
{
	AgvAttr::action_t t = static_cast<AgvAttr::action_t>(action);
	if (m_listName[t].isNull() || m_listName[t].isEmpty())
	{
		return QString("%1").arg(t);
	}

	return m_listName[t];
}

AgvAction AgvType::GetActionCode(const QString& name) const
{
	for (int i = 0; i < UCHAR_MAX; ++i)
	{
		if (m_listName[i] == name)
		{
			return static_cast<AgvAction>(i);
		}
	}

	return static_cast<AgvAction>(name.toShort());
}

void AgvType::LoadNameList(const QJsonObject& Json)
{
	if (Json.isEmpty())
	{
		return;
	}

	QStringList listKeys = Json.keys();

	for (int i = 0; i < listKeys.size(); ++i)
	{
		QString key = listKeys.at(i);

		int code = key.toInt();

		if (code >= UCHAR_MAX - 1 || code <= 0)
		{
			continue;
		}

		m_listName[key.toInt()] = Json.value(key).toString();;
	}

	return;
}
