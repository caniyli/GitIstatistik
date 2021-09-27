#ifndef DRAWCHART_H
#define DRAWCHART_H

#include "donutbreakdownchart.h"
#include "gitstatistics.h"

#include <QWidget>
#include <QtCharts>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>

class DrawChart : public QWidget
{
	Q_OBJECT
public:
	explicit DrawChart(QString chosen, QVector<GitStatistics::Data> datas, QWidget *parent = nullptr);

signals:

private:
	QColor randColor();
	QString getMonthName(int monthNumber);
	QHash<QString, float> getWeeklyDataStatistics(QVector<QDateTime> datas);
	QHash<QString, float> getMonthlyDataStatistics(QHash<QString, float> weekly);
	void drawYearly(QHash<QString, float> weekly);
	void drawMonthly(QHash<QString, float> weekly, QString MonthName);
	int randInt(int low, int high);
	QString graphicName;
	QVector<GitStatistics::Data> datas;

};

#endif // DRAWCHART_H
