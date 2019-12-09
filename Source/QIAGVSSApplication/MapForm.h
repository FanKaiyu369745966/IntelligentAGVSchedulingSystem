#ifndef MAPFORM_H
#define MAPFORM_H

#include <QWidget>
#include <QDebug>
#include <QPainter>
#include <QMap>
#include <QSqlQuery>
#include <QMenu>
#include <QFile>
#include <QMouseEvent>
#include <QStyleOption>
#include <QSqlDatabase>
#include "graphic/GraphicMap.h"
#include "MsgBoxEx.h"


namespace Ui {
	class MapForm;
}

static const int RETCODE_RESTART = 773;

class MapForm : public QWidget
{
	Q_OBJECT

public:
	explicit MapForm(const QSqlDatabase& db, QWidget* parent = nullptr);
	~MapForm();

private:
	Ui::MapForm* ui;
	QSqlDatabase m_db;
	std::shared_ptr<GraphicMap> m_pMap;

protected:
	void paintEvent(QPaintEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
signals:
	void onUpdateMapRelateTable();

private slots:
	void onUpdateMapMenuEvent();
	void onBtnOkClicked();
	void onBtnCancelClicked();
	void onCursorChange();
public slots:
	// 显示AGV
	void onShowAgv(quint32 unNo, quint32 unCurMark, bool bAuto, bool bRun, bool bUnVol, bool bObs, bool bError);

	// 隐藏AGV
	void onHideAgv(quint32 unNo);

	// 更新呼叫器信号
	void onUpdateCallerSignal(quint32, bool, bool, bool, bool);

	// 更新呼叫器启用状态
	void onUpdateCallerEnable(int, bool);
public:
	/*!
	 * @brief 更新地图关联表
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 地图文件路径
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool UpdateMapRelateTable(const QString&);
private:
	/*!
	 * @brief 更新地图关联表
	 *
	 * 通过XML文件读取
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 地图文件路径
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool UpdateMapRelateTable(const QDomDocument&);

	/*!
	 * @brief 更新地图关联表
	 *
	 * 通过JSON文件读取
	 * @date 2019/11/19
	 * @author FanKaiyu
	 * @param const QString & 地图文件路径
	 * @return bool 读取成功返回true,否则返回false
	 */
	bool UpdateMapRelateTable(const QJsonDocument&);
	bool UpdateAGV(const QDomElement&);
	bool UpdateAGV(const QJsonObject&);
	bool UpdateRFID(const QDomElement&);
	bool UpdateRFID(const QJsonObject&);
	bool UpdateStation(const QDomElement&);
	bool UpdateStation(const QJsonObject&);
	bool UpdateRest(const QDomElement&);
	bool UpdateRest(const QJsonObject&);
	bool UpdateCharger(const QDomElement&);
	bool UpdateCharger(const QJsonObject&);
	bool UpdateTrack(const QDomElement&);
	bool UpdateTrack(const QJsonArray&);
	bool UpdateCaller(const QDomElement&);
	bool UpdateCaller(const QJsonObject&);
	bool UpdateAGVType(const QDomElement&);
	bool UpdateAGVType(const QJsonObject&);
	bool UpdateRestQueue(const QDomElement&);
	bool UpdateRestQueue(const QJsonObject&);
	bool UpdateTraffic(const QDomElement&);
	bool UpdateTraffic(const QJsonObject&);
};

#endif // MAPFORM_H
