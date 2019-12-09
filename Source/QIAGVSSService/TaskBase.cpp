#include "TaskBase.h"

TaskBase::TaskBase()
	: m_order("")
	, m_task(0)
	, m_tray("")
	, m_target(nullptr)
	, m_mission("")
	, m_executer(nullptr)
	, m_status(TASK_STA_DEFAULT)
	//, m_publish()
	//, m_execute()
	//, m_finish()
	, m_error(TASK_ERR_DEFAULT)
	, m_log("")
{
}

TaskBase::TaskBase(const QString& no, const int& id, const QString& tray, const WorkerPtr& worker, const QString& mission, const AgvPtr& executer, const QString& status, const QString& error, const QString& log)
	: m_order(no)
	, m_task(id)
	, m_tray(tray)
	, m_target(worker)
	, m_mission(mission)
	, m_executer(executer)
	, m_status(status)
	//, m_publish()
	//, m_execute()
	//, m_finish()
	, m_error(error)
	, m_log(log)
{
}

TaskBase::TaskBase(const TaskBase& task)
{
	m_order = task.m_order;
	m_task = task.m_task;
	m_tray = task.m_tray;
	m_target = task.m_target;
	m_mission = task.m_mission;
	m_executer = task.m_executer;
	m_status = task.m_status;
	//m_publish = task.;
	//m_execute = task.;
	//m_finish = task.;
	m_error = task.m_error;
	m_log = task.m_log;
}

void TaskBase::operator=(const TaskBase& task)
{
	m_order = task.m_order;
	m_task = task.m_task;
	m_tray = task.m_tray;
	m_target = task.m_target;
	m_mission = task.m_mission;
	m_executer = task.m_executer;
	m_status = task.m_status;
	//m_publish = task.;
	//m_execute = task.;
	//m_finish = task.;
	m_error = task.m_error;
	m_log = task.m_log;

	return;
}

bool TaskBase::IsNull() const
{
	if (m_order.isNull()
		|| m_order.isEmpty()
		|| m_task == 0
		|| m_mission.isNull()
		|| m_mission.isEmpty()
		|| m_status.isNull()
		|| m_status.isEmpty())
	{
		return true;
	}

	return false;
}

Order::Order()
	: m_no("")
	, m_tray("")
	, m_executer(nullptr)
	, m_count(-1)
{
}

Order::Order(const QString& no, const QString& tray, const AgvPtr& executer)
	: m_no(no)
	, m_tray(tray)
	, m_executer(executer)
	, m_count(-1)
{
}

Order::Order(const Order& order)
{
	m_no = order.m_no;
	m_tray = order.m_tray;
	m_executer = order.m_executer;
	m_mapTasks = order.m_mapTasks;
	m_list = order.m_list;
	m_count = order.m_count;
}

void Order::operator=(const Order& order)
{
	m_no = order.m_no;
	m_tray = order.m_tray;
	m_executer = order.m_executer;
	m_mapTasks = order.m_mapTasks;
	m_list = order.m_list;
	m_count = order.m_count;
}

bool Order::IsNull() const
{
	if (m_no.isNull()
		|| m_no.isEmpty())
	{
		return true;
	}

	return false;
}

int Order::GetExecuterNo() const
{
	if (m_executer == nullptr)
	{
		return 0;
	}

	return m_executer->GetId();
}

bool Order::Insert(const TaskBase& task)
{
	if (task.m_order != m_no)
	{
		return false;
	}

	m_mapTasks[task.m_task] = task;

	m_list.push_back(task.m_task);

	return true;
}

bool Order::Delete(const int& id)
{
	if (m_mapTasks.find(id) == m_mapTasks.end())
	{
		return false;
	}

	m_mapTasks.erase(m_mapTasks.find(id));

	for (std::list<int>::iterator it = m_list.begin(); it != m_list.end(); ++it)
	{
		if (*it = id)
		{
			it = m_list.erase(it);
			break;
		}
	}

	return true;
}

void Order::Sort()
{
	int len = m_list.size();
	int i, j;

	for (i = 0; i < len - 1; i++)
	{/* 外循环为排序趟数，len个数进行len-1趟 */

		for (j = 0; j < len - 1 - i; j++)
		{ /* 内循环为每趟比较的次数，第i趟比较len-i次 */

			TaskBase cur, next, tmp;
			cur = m_mapTasks[j];
			next = m_mapTasks[j + 1];

			if (cur.m_target == nullptr || next.m_target == nullptr)
			{
				continue;
			}

			if (cur.m_mission == next.m_mission && cur.m_target->GetId() > next.m_target->GetId())
			{/* 相邻元素比较，若逆序则交换（升序为左大于右，降序反之） */
				tmp = cur;
				cur = next;
				next = tmp;
			}
		}
	}

	return;
}

bool Order::Next()
{
	++m_count;

	if (m_count == m_list.size())
	{
		return false;
	}

	return true;
}

void Order::Seed(const int& index)
{
	if (index == -1)
	{
		m_count = m_list.size() - 1;

		return;
	}

	m_count = index;

	return;
}

TaskBase Order::Head() const
{
	return m_mapTasks.at(m_list.front());
}

TaskBase Order::Tail() const
{
	return m_mapTasks.at(m_list.back());
}

TaskBase Order::GetTask() const
{
	int id = 0;

	int index = 0;

	for (std::list<int>::const_iterator it = m_list.begin(); it != m_list.end(); ++it, ++index)
	{
		if (index == m_count)
		{
			id = *it;
			break;
		}
	}

	return m_mapTasks.at(id);
}

TaskBase Order::GetTask(const int& id) const
{
	return m_mapTasks.at(id);
}

bool Order::IsFull() const
{
	if (Tail().m_mission == TASK_MISSION_FINISH)
	{
		return true;
	}

	return false;
}
