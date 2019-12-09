#ifndef GRAPHICMAP_H
#define GRAPHICMAP_H

#include <QWidget>
#include <QMutex>
#include <QMap>
#include <QSqlQuery>
#include <QDebug>
#include <QMutex>
#include <QtXml>
#include <QSqlDatabase>
#include <memory>
#include "GraphicBackground.h"
#include "GraphicAGV.h"
#include "GraphicRFID.h"
#include "GraphicTrack.h"
#include "GraphicWorkStation.h"
#include "GraphicRestStation.h"
#include "GraphicChargeStation.h"
#include "GraphicCaller.h"

enum BUTTONCTRL
{
	BUTTONCTRL_LDOWN, // 鼠标左键按下
	BUTTONCTRL_LUP, // 鼠标左键抬起
	BUTTONCTRL_DL, // 双击鼠标左键
	BUTTONCTRL_RDOWN, // 鼠标右键按下
	BUTTONCTRL_RUP, // 鼠标右键抬起
	BUTTONCTRL_DR, // 双击鼠标右键
	BUTTONCTRL_MDOWN, // 鼠标中键按下
	BUTTONCTRL_MUP, // 鼠标中键抬起
	BUTTONCTRL_DM, // 双击鼠标中键
};

class GraphicMap : public QWidget
{
public:
	explicit GraphicMap(const QSqlDatabase& db, QWidget* parent = 0);
	~GraphicMap();
private:
	std::shared_ptr<GraphicBg> m_pBg;							/*!< 地图指针 */
	QMap < quint32, std::shared_ptr<IconAGV>> m_mapAGVs;		/*!< AGV指针集合 */
	QMap < quint32, std::shared_ptr<IconRFID>> m_mapRFIDs;		/*!< RFID地标卡指针集合 */
	QMap < quint32, std::shared_ptr<IconWS>> m_mapStations;		/*!< 工作站指针集合 */
	QMap < quint32, std::shared_ptr<IconRS>> m_mapRests;		/*!< 待机站指针集合 */
	QMap < quint32, std::shared_ptr<IconCS>> m_mapChargers;		/*!< 充电站指针集合 */
	QMap < quint32, std::shared_ptr<IconCaller>> m_mapCaller;	/*!< 呼叫器指针集合 */
	QList< std::shared_ptr<IconTrack>> m_listTracks;			/*!< 磁轨指针列表 */
private:
	QSqlDatabase m_db;
	QMutex m_mutex;		/*!< 互斥锁 */
public:
	/**
	 * @brief drawMap   绘制地图
	 * @param painter   绘图
	 */
	void drawMap(QPainter& painter);

	/**
	 * @brief select    选中
	 * @param nButton   鼠标键
	 * @param point     坐标
	 * @return  true 选中， false 未选中
	 */
	bool select(const BUTTONCTRL& nButton, const QPoint& point);

	/**
	 * @brief confirm   提交
	 * @param nButton   鼠标键
	 * @param point     坐标
	 * @return true 提交， false 未提交
	 */
	bool confirm(const BUTTONCTRL& nButton, const QPoint& point);

	/**
	 * @brief wheel 缩放
	 * @param bDire true 放大， false 缩小
	 */
	void wheel(const bool& bDire);

	/**
	 * @brief drag          拖拽
	 * @param ptWinPoint    坐标
	 * @return  鼠标光标
	 */
	QCursor drag(QPoint ptWinPoint);

	/**
	 * @brief cancel    取消拖拽
	 */
	void cancel();

	/**
	 * @brief size  视图更新
	 * @param rect  更新范围
	 */
	void size(const QRect& rect);

	/**
	 * @brief showAgv           显示AGV
	 * @param unNo              AGV编号
	 * @param strType           AGV类型
	 * @param strMove           可运动方向
	 * @param unCurMark         当前地标卡
	 * @param bRun              模式标识
	 * @param bRun              移动标识
	 * @param bUnVol            电量不足标识
	 * @param bObs              避障标识
	 * @param bError            异常标识
	 */
	void showAgv(const quint32& unNo, const quint32& unCurMark, const bool& bAuto, const bool& bRun, const bool& bUnVol, const bool& bObs, const bool& bError);

	/**
	 * @brief hideAgv           隐藏AGV
	 * @param unNo              AGV编号
	 */
	void hideAgv(const quint32& unNo);

	void updateCallerSignal(const quint32&, const bool&, const bool&, const bool&, const bool&);

	void updateCallerEnable(const quint32&, const bool&);

	/**
	 * @brief loadMapElement  加载地图元素信息
	 */
	void loadMapElement();

private:
	/**
	 * @brief 清除地图元素信息
	 */
	void clearMapElement();
};
#endif // GRAPHICMAP_H
