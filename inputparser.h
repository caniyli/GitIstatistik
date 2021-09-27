#ifndef INPUTPARSER_H
#define INPUTPARSER_H

#include <QString>
#include <QDateTime>
#include <QVector>


class inputParser
{
public:
    inputParser();
    struct Data{
      QString author;
      QString commitid;
      QDateTime date;
      QString message;
    };
    QVector<Data> allDatas;
    void setData(QVector<Data> allDatas);
    QVector<Data> getWeeklyData(int year);
    QVector<Data> getMountlyData(int year);
    QVector<Data> getPeriodData(QDate beginTime, QDate endTime);
};

#endif // INPUTPARSER_H
