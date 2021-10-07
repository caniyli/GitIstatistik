#ifndef GITSTATISTICS_H
#define GITSTATISTICS_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

class GitStatistics
{
public:
	GitStatistics(const QString &filename);
	struct Data{
		QString author;
		QString commitid;
		int64_t epoch;
		QString message;
	};
	struct statisticData
	{
		int commit;
		int day;
		int outsideWeekend;
		int freeDays;
		int outsideWeekendFreeDays;
		int monday;
		int friday;
		QMap<QString, int> dailyCommit;
	};
	QVector<Data> getStatData();
	QVector<Data> getPeriodlyData(QDate beginTime, QDate endTime);
	QStringList getAllYears();
	int64_t getEndTime();
	int64_t getBeginTime();
	statisticData calculateDay(int64_t beginTime, int64_t endTime);

private:
	statisticData dailyData;
	int totalCount;
	QString personName;
	QVector<Data> statData;
	QStringList allYears;
};

#endif // GITSTATISTICS_H
