#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "gitstatistics.h"
#include "donutbreakdownchart.h"
#include "drawchart.h"

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

	void on_pbFilePath_clicked();

	void on_rbCompare_clicked();
	
	void on_cbFileName_2_currentIndexChanged(const QString &arg1);
	
private:
	Ui::MainWindow *ui;
	void updateInfoFrame(int64_t beginTime, int64_t endTime);
	void updateInfoFrame_2(int64_t beginTime, int64_t endTime);
	QMap<QString, GitStatistics *> gsMap;
	QComboBox *yearCombo;
	QComboBox *monthCombo;
	QDateEdit *startTime;
	QDateEdit *endTime;
	QString chosen;
	QLabel *labelFileName;
	QLabel *labelCount;
	QLabel *labelMean;
	QLabel *labelWeekDayMean;
	QLabel *labelFreeDays;
	QLabel *labelWeakDaysFree;
	QLabel *labelMonday;
	QLabel *labelFriday;
	QLabel *labelFileName_2;
	QLabel *labelCount_2;
	QLabel *labelMean_2;
	QLabel *labelWeekDayMean_2;
	QLabel *labelFreeDays_2;
	QLabel *labelWeakDaysFree_2;
	QLabel *labelMonday_2;
	QLabel *labelFriday_2;
	QCalendarWidget *calendar;
	void createCalendar();
	void createInfoColor();
	void createFrameDate();
	void createInfoLabels();
	void createInfoLabels_2();
	void setChartDatas(QString chosen, QVector<GitStatistics::Data> datas);
	void clearFrame(QWidget *widget);
	void fillCalendar();
	QString printTool(QDate date);
	QMap<int64_t, int> setLineSeries();
	void drawLineChart();
	QDir directory;
	void updateMonthCombo();
	QMap<QString, QGridLayout *> mapLayout;
	QMap<QString, QFrame *> mapFrame;
	QMap<QString, int> index;
	QMap<QString, int> graphicCounter;
};
#endif // MAINWINDOW_H
