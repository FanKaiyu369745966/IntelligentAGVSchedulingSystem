#ifndef CHAINTABLE_H
#define CHAINTABLE_H

/*! @brief 双向链表节点 */
template<class T>
struct DNode
{
public:
	T value;
	DNode* prev;	/*!< 指向上一位的指针 */
	DNode* next;	/*!< 指向下一位的指针 */

public:
	DNode();
	DNode(const T& t, DNode* prev, DNode* next);
};

/*!
 * @class DoubleChainTable
 * @brief 双向链表
*/
template<class T>
class DoubleChain
{
public:
	DoubleChain();
	~DoubleChain();
	DoubleChain(const DoubleChain& dChain);

public:
	void operator=(const DoubleChain& dChain);

public:
	/*!
	 * @brief 获取链表大小
	 * @return int 返回链表大小
	*/
	int Size() const { return count; }

	/*!
	 * @brief 判断链表是否为空
	 * @return bool 为空返回true,否则返回false
	*/
	bool IsEmpty() const { return count == 0; }

	/*!
	 * @brief 获取节点
	 * @param int 节点位置
	 * @return T 节点对象
	*/
	T Get(int index) const;

	/*!
	 * @brief 获取首节点
	 * @return T 首节点对象
	*/
	T GetFirst() const;

	/*!
	 * @brief 获取尾节点
	 * @return T 尾节点对象
	*/
	T GetLast() const;

	int FindNode(const T& t) const;

	void Insert(const int& index, const T& t);
	void PushFront(const T& t);
	void PushBack(const T& t);

	void Delete(const int& index);
	void DeleteFirst();
	void DeleteLast();

	void Clear();

private:
	int count;
	DNode<T>* phead;

private:
	DNode<T>* GetNode(int index) const;
};

/*! @brief 单向链表节点 */
template<class T>
struct SNode
{
public:
	T value;
	SNode* next;	/*!< 指向下一位的指针 */

public:
	SNode();
	SNode(const T& t, SNode* next);
};

/*!
 * @class DoubleChainTable
 * @brief 单向链表
*/
template<class T>
class SingleChain
{
public:
	SingleChain();
	~SingleChain();
	SingleChain(const SingleChain& dChain);

public:
	void operator=(const SingleChain& dChain);

public:
	/*!
	 * @brief 获取链表大小
	 * @return int 返回链表大小
	*/
	int Size() const { return count; }

	/*!
	 * @brief 判断链表是否为空
	 * @return bool 为空返回true,否则返回false
	*/
	bool IsEmpty() const { return count == 0; }

	/*!
	 * @brief 获取节点
	 * @param int 节点位置
	 * @return T 节点对象
	*/
	T Get(int index) const;

	/*!
	 * @brief 获取首节点
	 * @return T 首节点对象
	*/
	T GetFirst() const;

	/*!
	 * @brief 获取尾节点
	 * @return T 尾节点对象
	*/
	T GetLast() const;

	int FindNode(const T& t) const;

	void Insert(const int& index, const T& t);
	void PushFront(const T& t);
	void PushBack(const T& t);

	void Delete(const int& index);
	void DeleteFirst();
	void DeleteLast();

	void Clear();

private:
	int count;
	SNode<T>* phead;

private:
	SNode<T>* GetNode(int index) const;
};

#pragma region 双向链表实现
template<class T>
inline DNode<T>::DNode()
	: prev(nullptr)
	, next(nullptr)
{
}

template<class T>
inline DNode<T>::DNode(const T& t, DNode* prev, DNode* next)
{
	this->value = t;
	this->prev = prev;
	this->next = next;
}

template<class T>
inline DoubleChain<T>::DoubleChain()
	:count(0)
{
	phead = new DNode<T>();
	phead->prev = phead->next = phead;
}

template<class T>
inline DoubleChain<T>::~DoubleChain()
{
	// 删除所有的节点
	DNode<T>* ptmp = nullptr;
	DNode<T>* pnode = phead->next;
	while (pnode != phead)
	{
		ptmp = pnode;
		pnode = pnode->next;
		delete ptmp;
		ptmp = nullptr;
	}

	// 删除"表头"
	delete phead;
	phead = nullptr;
	pnode = nullptr;
}

template<class T>
inline DoubleChain<T>::DoubleChain(const DoubleChain& dChain)
	:count(0)
{
	phead = new DNode<T>();
	phead->prev = phead->next = phead;

	for (int i = 0; i < dChain.Size(); ++i)
	{
		PushBack(dChain.Get(i));
	}
}

template<class T>
inline void DoubleChain<T>::operator=(const DoubleChain& dChain)
{
	Clear();

	for (int i = 0; i < dChain.Size(); ++i)
	{
		PushBack(dChain.Get(i));
	}
}

template<class T>
inline DNode<T>* DoubleChain<T>::GetNode(int index) const
{
	// 判断参数有效性
	if (index < 0 || index >= count)
	{
		return nullptr;
	}

	// 正向查找
	if (index <= count / 2)
	{
		int i = 0;
		DNode<T>* pindex = phead->next;
		while (i++ < index)
		{
			pindex = pindex->next;
		}

		return pindex;
	}

	// 反向查找
	int j = 0;
	int rindex = count - index - 1;
	DNode<T>* prindex = phead->prev;
	while (j++ < rindex)
	{
		prindex = prindex->prev;
	}

	return prindex;
}

template<class T>
inline T DoubleChain<T>::Get(int index) const
{
	DNode<T>* p = GetNode(index);

	if (p == nullptr)
	{
		return T();
	}

	return p->value;
}

template<class T>
inline T DoubleChain<T>::GetFirst() const
{
	DNode<T>* p = GetNode(0);

	if (p == nullptr)
	{
		return T();
	}

	return p->value;
}

template<class T>
inline T DoubleChain<T>::GetLast() const
{
	DNode<T>* p = GetNode(count - 1);

	if (p == nullptr)
	{
		return T();
	}

	return p->value;
}

template<class T>
inline int DoubleChain<T>::FindNode(const T& t) const
{
	DNode<T>* pindex = phead->next;

	for (int i = 0; i < count; ++i, pindex = pindex->next)
	{
		if (pindex->value == t)
		{
			return i;
		}
	}

	return -1;
}

template<class T>
inline void DoubleChain<T>::Insert(const int& index, const T& t)
{
	if (index == 0)
	{
		return PushFront(t);
	}

	DNode<T>* pindex = GetNode(index);	/*!< 插入点 */
	DNode<T>* pnode = new DNode<T>(t, pindex->prev, pindex);	/*!< 新节点 */
	pindex->prev->next = pnode;
	pindex->prev = pnode;
	count++;

	return;
}

template<class T>
inline void DoubleChain<T>::PushFront(const T& t)
{
	DNode<T>* pnode = new DNode<T>(t, phead, phead->next);
	phead->next->prev = pnode;
	phead->next = pnode;
	count++;

	return;
}

template<class T>
inline void DoubleChain<T>::PushBack(const T& t)
{
	DNode<T>* pnode = new DNode<T>(t, phead->prev, phead);
	phead->prev->next = pnode;
	phead->prev = pnode;
	count++;

	return;
}

template<class T>
inline void DoubleChain<T>::Delete(const int& index)
{
	DNode<T>* pindex = GetNode(index);
	pindex->next->prev = pindex->prev;
	pindex->prev->next = pindex->next;
	delete pindex;
	count--;

	return;
}

template<class T>
inline void DoubleChain<T>::DeleteFirst()
{
	return Delete(0);
}

template<class T>
inline void DoubleChain<T>::DeleteLast()
{
	return Delete(count - 1);
}

template<class T>
inline void DoubleChain<T>::Clear()
{
	// 删除所有的节点
	DNode<T>* ptmp = nullptr;
	DNode<T>* pnode = phead->next;
	while (pnode != phead)
	{
		ptmp = pnode;
		pnode = pnode->next;
		delete ptmp;
		ptmp = nullptr;
	}

	phead->prev = phead->next = phead;
	count = 0;
}

#pragma endregion

#pragma region 单向链表实现
template<class T>
inline SNode<T>::SNode()
	: next(nullptr)
{
}

template<class T>
inline SNode<T>::SNode(const T& t, SNode* next)
{
	this->value = t;
	this->next = next;
}

template<class T>
inline SingleChain<T>::SingleChain()
	:count(0)
{
	phead = new SNode<T>();
	phead->next = nullptr;
}

template<class T>
inline SingleChain<T>::~SingleChain()
{
	// 删除所有的节点
	SNode<T>* ptmp = nullptr;
	SNode<T>* pnode = phead->next;
	while (pnode != nullptr)
	{
		ptmp = pnode;
		pnode = pnode->next;
		delete ptmp;
		ptmp = nullptr;
	}

	// 删除"表头"
	delete phead;
	phead = nullptr;
	pnode = nullptr;
}

template<class T>
inline SingleChain<T>::SingleChain(const SingleChain& dChain)
	:count(0)
{
	phead = new SNode<T>();
	phead->next = nullptr;

	for (int i = 0; i < dChain.Size(); ++i)
	{
		PushBack(dChain.Get(i));
	}
}

template<class T>
inline void SingleChain<T>::operator=(const SingleChain& dChain)
{
	Clear();

	for (int i = 0; i < dChain.Size(); ++i)
	{
		PushBack(dChain.Get(i));
	}
}

template<class T>
inline SNode<T>* SingleChain<T>::GetNode(int index) const
{
	// 判断参数有效性
	if (index < 0 || index >= count)
	{
		return nullptr;
	}

	// 正向查找
	int i = 0;
	SNode<T>* pindex = phead->next;
	while (i++ < index)
	{
		pindex = pindex->next;
	}

	return pindex;
}

template<class T>
inline T SingleChain<T>::Get(int index) const
{
	return GetNode(index)->value;
}

template<class T>
inline T SingleChain<T>::GetFirst() const
{
	return GetNode(0)->value;
}

template<class T>
inline T SingleChain<T>::GetLast() const
{
	return  GetNode(count - 1)->value;
}

template<class T>
inline int SingleChain<T>::FindNode(const T& t) const
{
	SNode<T>* pindex = phead->next;

	for (int i = 0; i < count; ++i, pindex = pindex->next)
	{
		if (pindex->value == t)
		{
			return i;
		}
	}

	return -1;
}

template<class T>
inline void SingleChain<T>::Insert(const int& index, const T& t)
{
	if (index == 0)
	{
		return PushFront(t);
	}

	SNode<T>* pindex = GetNode(index);			/*!< 插入点 */
	SNode<T>* pprev = GetNode(index - 1);		/*!< 插入点上一个节点 */
	SNode<T>* pnode = new SNode<T>(t, pindex);	/*!< 新节点 */
	pprev->next = pnode;
	count++;

	return;
}

template<class T>
inline void SingleChain<T>::PushFront(const T& t)
{
	SNode<T>* pnode = new SNode<T>(t, phead->next);
	phead->next = pnode;
	count++;

	return;
}

template<class T>
inline void SingleChain<T>::PushBack(const T& t)
{
	if (count == 0)
	{
		return PushFront(t);
	}

	SNode<T>* pindex = GetNode(count - 1);		/*!< 插入点 */
	SNode<T>* pnode = new SNode<T>(t, nullptr);
	pindex->next = pnode;
	count++;

	return;
}

template<class T>
inline void SingleChain<T>::Delete(const int& index)
{
	DNode<T>* pindex = GetNode(index);

	if (index == 0)
	{
		phead->next = pindex->next;
	}
	else
	{
		SNode<T>* pprev = GetNode(index - 1);			/*!< 插入点上一个节点 */
		pprev->next = pindex->next;
	}

	delete pindex;
	count--;

	return;
}

template<class T>
inline void SingleChain<T>::DeleteFirst()
{
	return Delete(0);
}

template<class T>
inline void SingleChain<T>::DeleteLast()
{
	return Delete(count - 1);
}

template<class T>
inline void SingleChain<T>::Clear()
{
	// 删除所有的节点
	DNode<T>* ptmp = nullptr;
	DNode<T>* pnode = phead->next;
	while (pnode != nullptr)
	{
		ptmp = pnode;
		pnode = pnode->next;
		delete ptmp;
		ptmp = nullptr;
	}

	phead->next = nullptr;
	count = 0;
}
#pragma endregion

#endif // CHAINTABLE_H
