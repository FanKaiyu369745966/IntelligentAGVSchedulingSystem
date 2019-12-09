#ifndef LEDBASE_H
#define LEDBASE_H

class LedBase
{
public:
	LedBase();
	explicit LedBase(const unsigned short&, const unsigned short&, const unsigned short&);
protected:
	unsigned short m_usId;		/*!< 编号 */
	unsigned short m_usWidth;	/*!< 宽 */
	unsigned short m_usHeight;	/*!< 高 */
public:
	unsigned short GetId() const { return m_usId; }
	unsigned short Width() const { return m_usWidth; }
	unsigned short Height() const { return m_usHeight; }
	void SetWidth(const unsigned short& width) { m_usWidth = width; return; }
	void SetHeight(const unsigned short& height) { m_usHeight = height; return; }
};

#endif // LEDBASE_H
