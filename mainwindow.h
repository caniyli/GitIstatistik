#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gitstatistics.h"
#include "donutbreakdownchart.h"
#include "drawchart.h"

#include <QWidget>
#include <QString>
#include <QPushButton>
#include <QDebug>
#include <QComboBox>
#include <QDateEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:

	void on_buttonDraw_clicked();

	void on_rbYear_clicked();

	void on_rbMonth_clicked();

	void on_rbPeriod_clicked();

	void on_buttonDelete_clicked();

	void on_rbAllYear_clicked();

	void on_rbAllMonth_clicked();

	void on_cbFileName_currentIndexChanged(const QString &arg1);

	void startTime_dateChanged(const QDate &date);

	void endTime_dateChanged(const QDate &date);

	void on_radioButton_clicked();

private:
	Ui::MainWindow *ui;
	QString getMonthName(int monthNumber);
	int getMonthNumber(QString monthName);
	void updateInfoFrame(int64_t beginTime, int64_t endTime);
	GitStatistics *gs;
	QComboBox *yearCombo;
	QComboBox *monthCombo;
	QDateEdit *startTime;
	QDateEdit *endTime;
	QString chosen;
	QGridLayout *cchartL;
	QLabel *labelFileName;
	QLabel *labelCount;
	QLabel *labelMean;
	QLabel *labelWeekDayMean;
	QLabel *labelFreeDays;
	QLabel *labelWeakDaysFree;
	QLabel *labelMonday;
	QLabel *labelFriday;
	QCalendarWidget *calendar;
	int graphicCounter = 0;
	void createCalendar();
	void createInfoColor();
	void createFrameDate();
	void createInfoLabels();
	void setChartDatas(QString chosen, QVector<GitStatistics::Data> datas);
	void clearFrame(QWidget *widget);
	void fillCalendar();
	QString printTool(QDate date);
	QMap<int64_t, int> setLineSeries();
	void drawLineChart();
	void updateLineChart();
};
#endif // MAINWINDOW_H
