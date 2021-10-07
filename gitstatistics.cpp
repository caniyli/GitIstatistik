#include "gitstatistics.h"

GitStatistics::GitStatistics(const QString &filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", file.errorString());
    }

    QTextStream in(&file);
    QVector<QStringList> datas;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split("\t");
        datas.append(fields);
    }
    int temp;
    for (int i = 0; i < datas.size(); ++i) {
        QStringList fields = datas[i][2].split(" ");
        if (fields[0].toInt() < 1262338881)
            continue;
        Data data ;
        int date = QDateTime::fromSecsSinceEpoch(fields[0].toInt()).date().year();
        if(temp != date)
        {
            allYears.append(QString::number(date));
            temp =date;
        }
        data.epoch = fields[0].toInt();
        data.author = datas[i][1];
        data.commitid = datas[i][0];
        data.message = datas[i][3];
        statData.append(data);
    }
    file.close();
}

QVector<GitStatistics::Data> GitStatistics::getStatData()
{
    return statData;
}

QVector<GitStatistics::Data> GitStatistics::getPeriodlyData(QDate beginTime, QDate endTime)
{
    QVector<Data> periodlyData;
    int64_t beginEpoch = QDateTime(beginTime, QTime(0,0,0)).toSecsSinceEpoch();
    int64_t endEpoch = QDateTime(endTime, QTime(0,0,0)).toSecsSinceEpoch();
    QMap<QString, int> dailyCommit;
    for (int i = 0; i < statData.size(); ++i) {
        if (statData[i].epoch >= beginEpoch && statData[i].epoch <= endEpoch){
            periodlyData.push_back(statData[i]);
            dailyCommit[QDateTime::fromSecsSinceEpoch(statData[i].epoch).date().toString()]++;
        }
    }
    dailyData.dailyCommit = dailyCommit;
    return periodlyData;
}

QStringList GitStatistics::getAllYears()
{
    QMap<QString,int> map;
    for (int i = 0; i < allYears.size(); ++i) {
        map[allYears[allYears.size()-i-1]]++;
    }
    return map.keys();
}

int64_t GitStatistics::getBeginTime()
{
    int64_t temp = statData[0].epoch;
    for (int i = 0; i < statData.size(); ++i) {
        if (statData[i].epoch < temp)
            temp = statData[i].epoch;
    }
    return temp;
}

int64_t GitStatistics::getEndTime()
{
    int64_t temp = statData[0].epoch;
    for (int i = 0; i < statData.size(); ++i) {
        if (statData[i].epoch > temp)
            temp = statData[i].epoch;
    }
    return temp;
}

GitStatistics::statisticData GitStatistics::calculateDay(int64_t beginTime, int64_t endTime){
    int day = (endTime - beginTime) / (24*60*60);
    int outsideWeekend = day;
    int freeDay = 0;
    int outsideWeekendFreeDays = 0;
    int weekendFreeDays = 0;
    int monday = 0;
    int friday = 0;
    QVector<Data> periodlyData = getPeriodlyData(QDateTime::fromSecsSinceEpoch(beginTime).date(), QDateTime::fromSecsSinceEpoch(endTime).date());
    dailyData.commit = periodlyData.size();

    freeDay = day - dailyData.dailyCommit.size();

    for (int i = 0; i < dailyData.dailyCommit.size(); ++i) {
        QStringList days = dailyData.dailyCommit.keys()[i].split(" ");
        if (days[0] == "Cmt" || days[0] == "Paz"){
            weekendFreeDays++;
        }else if (days[0] == "Pzt"){
            monday = monday + dailyData.dailyCommit[dailyData.dailyCommit.keys()[i]];
        }else if (days[0] == "Cum"){
            friday = friday + dailyData.dailyCommit[dailyData.dailyCommit.keys()[i]];
        }
    }
    outsideWeekendFreeDays = freeDay - weekendFreeDays;
	if(endTime > beginTime)
		for (int64_t i = beginTime; i < endTime; i = i + 86400) {
			QDate date = QDateTime::fromSecsSinceEpoch(i).date();
			QStringList pieces = date.toString().split( " " );
			if (pieces[0] == "Cmt" || pieces[0] == "Paz"){
				outsideWeekend--;
			}
		}

    dailyData.commit = periodlyData.size();
    dailyData.day = day;
    dailyData.outsideWeekend = outsideWeekend;
    dailyData.freeDays = freeDay;
    dailyData.outsideWeekendFreeDays = outsideWeekendFreeDays;
    dailyData.monday = monday;
    dailyData.friday = friday;

    return dailyData;
}
