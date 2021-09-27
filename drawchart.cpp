#include "drawchart.h"

DrawChart::DrawChart(QString chosen, QVector<GitStatistics::Data> datas, QWidget *parent) : QWidget(parent)
{
    setLayout(new QVBoxLayout);

    QVector<QDateTime> dataDates;
    for (int i = 0; i < datas.size(); ++i) {
        dataDates.push_back(QDateTime::fromSecsSinceEpoch(datas[i].epoch));
    }
    if (!dataDates.isEmpty())
        graphicName = QString::number(dataDates[0].date().year());
    else
        graphicName = "Empty";

    QHash<QString, float> weekly;
    weekly = getWeeklyDataStatistics(dataDates);

    if (chosen.count("month") > 0){
        QStringList monthName = chosen.split( "/" );
        drawMonthly(weekly, monthName[1]);
    }else if (chosen == "year" || chosen == "allyear"){
        drawYearly(getMonthlyDataStatistics(weekly));
    }else if (chosen == "period")
    {
        QHash<QString, float> dailyCommit;
        for (int i = 0; i < datas.size(); ++i) {
            dailyCommit[getMonthName(QDateTime::fromSecsSinceEpoch(datas[i].epoch).date().month())]++;
        }
        qDebug() << dailyCommit << datas.size();
        drawYearly(dailyCommit);
    }
    setMaximumWidth(700);
    setMaximumHeight(700);
    setMinimumHeight(600);
    setMinimumWidth(600);

    this->datas = datas;
}

void DrawChart::drawYearly( QHash <QString, float> monthly)
{
    DonutBreakdownChart *donutBreakdown = new DonutBreakdownChart();
    donutBreakdown->setAnimationOptions(QChart::AllAnimations);
    donutBreakdown->setTitle(graphicName);
    donutBreakdown->legend()->setAlignment(Qt::AlignRight);
    for (int i = 1; i < 13; ++i) {
        if (monthly[getMonthName(i)] >= 1){
            QPieSeries *series = new QPieSeries();
            series->setName(getMonthName(i));
            series->append(getMonthName(i), monthly[getMonthName(i)]);
            donutBreakdown->addBreakdownSeries(series, randColor());
        }
    }
    QChartView *chartView = new QChartView(donutBreakdown);
    chartView->setRubberBand(QChartView::HorizontalRubberBand);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout()->addWidget(chartView);
}

void DrawChart::drawMonthly( QHash <QString, float> weekly, QString monthName)
{
    DonutBreakdownChart *donutBreakdown = new DonutBreakdownChart();
    donutBreakdown->setAnimationOptions(QChart::AllAnimations);
    donutBreakdown->setTitle(graphicName);
    donutBreakdown->legend()->setAlignment(Qt::AlignRight);
    QPieSeries *series = new QPieSeries();
    series->setName(monthName);
    for (int i = 0; i < 4; ++i) {
        if (weekly[monthName + "/" + QString::number(i+1)] >= 1)
            series->append(monthName + "/" + QString::number(i+1), weekly[monthName + "/" + QString::number(i+1)]);
    }
    donutBreakdown->addBreakdownSeries(series, randColor());
    QChartView *chartView = new QChartView(donutBreakdown);
    chartView->setRenderHint(QPainter::Antialiasing);
    layout()->addWidget(chartView);
}

QColor DrawChart::randColor()
{
    return QColor(randInt(0,180),randInt(0,180),randInt(0,180));
}

QString DrawChart::getMonthName(int monthNumber)
{
    QStringList monthNames = {"Ocak", "Şubat", "Mart", "Nisan", "Mayıs", "Haziran", "Temmuz", "Ağustos", "Eylül", "Ekim", "Kasım", "Aralık"};
    return monthNames[monthNumber-1];
}

QHash<QString, float> DrawChart::getWeeklyDataStatistics(QVector<QDateTime> datas)
{
    QHash<QString, float> weeks;
    for (int i = 0; i < datas.size(); ++i) {
        if (datas[i].date().day() <= 7)
        {
            weeks[getMonthName(datas[i].date().month()) + "/1"]++;
        }else if (datas[i].date().day() <= 14)
        {
            weeks[getMonthName(datas[i].date().month()) + "/2"]++;
        }else if (datas[i].date().day() <= 21)
        {
            weeks[getMonthName(datas[i].date().month()) + "/3"]++;
        }else {
            weeks[getMonthName(datas[i].date().month()) + "/4"]++;
        }
    }

    for (int i = 1; i < 13; ++i) {
        for (int j = 1; j < 5; ++j) {
            if (weeks[getMonthName(i) + "/" + QString::number(j)] <= 0)
            {
                weeks[getMonthName(i) + "/" + QString::number(j)] = 0.1;
            }
        }
    }
    return weeks;
}
QHash<QString, float> DrawChart::getMonthlyDataStatistics(QHash<QString, float> weekly)
{
    QHash<QString, float> monthly;
    for(int i=0; i < 12; i++)
    {
        for (int j = 0; j < 4; ++j) {
            if (weekly[getMonthName(i + 1) + "/" + QString::number(j + 1)] >= 1)
                monthly[getMonthName(i + 1)] += weekly[getMonthName(i + 1) + "/" + QString::number(j + 1)];
        }
    }
    return monthly;
}

int DrawChart::randInt(int low, int high)
{
    return qrand() % ((high + 1) - low) + low;
}
