#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	cchartL = new QGridLayout();
	ui->chartFrame->setLayout(cchartL);
	ui->frameDate->hide();

	createFrameDate();
	createCalendar();
	createInfoColor();
	createInfoLabels();
	QDir directory ( "/home/mert/projects/projects/build-DonutChart-Desktop-Debug");
	QStringList texts = directory.entryList(QStringList() << "*.txt" << "*.txt",QDir::Files);
	ui->cbFileName->addItems(texts);
	ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_buttonDraw_clicked()
{
	if (chosen == "year"){
		setChartDatas(chosen, gs->getYearlyData(yearCombo->currentText().toInt()));
	}else if (chosen == "month"){
		setChartDatas(chosen + "/" +monthCombo->currentText(),
					gs->getMontlyData(yearCombo->currentText().toInt(), getMonthNumber(monthCombo->currentText())));
	}else if (chosen == "allyear"){
		QStringList allYear = gs->getAllYears();
		for (int i = 0; i <allYear.size() ; ++i) {
			setChartDatas(chosen, gs->getYearlyData(allYear[i].toInt()));
		}
	}else if (chosen == "allmonth"){
		for (int i = 0; i < 12 ; ++i)
			if (!gs->getMontlyData(yearCombo->currentText().toInt(), i + 1).isEmpty()){
				setChartDatas(chosen + "/" +getMonthName(i + 1), gs->getMontlyData(yearCombo->currentText().toInt(), i + 1));
			}
	}else if(chosen == "lineChart")
	{
		drawLineChart();
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_rbYear_clicked()
{
	ui->stackedWidget->setCurrentIndex(0);
	ui->frameDate->show();
	yearCombo->show();
	monthCombo->hide();
	startTime->hide();
	endTime->hide();
	this->chosen = "year";
}

void MainWindow::on_rbMonth_clicked()
{
	ui->stackedWidget->setCurrentIndex(0);
	ui->frameDate->show();
	startTime->hide();
	endTime->hide();
	yearCombo->show();
	monthCombo->show();
	this->chosen = "month";
}

void MainWindow::on_rbPeriod_clicked()
{
	ui->stackedWidget->setCurrentIndex(1);
	fillCalendar();
	ui->frameDate->show();
	startTime->show();
	endTime->show();
	yearCombo->hide();
	monthCombo->hide();
}

void MainWindow::on_buttonDelete_clicked()
{
	clearFrame(ui->chartFrame);
	graphicCounter = 0;
}

void MainWindow::on_rbAllYear_clicked()
{
	ui->stackedWidget->setCurrentIndex(0);
	ui->frameDate->hide();
	this->chosen = "allyear";
}

void MainWindow::on_rbAllMonth_clicked()
{
	ui->stackedWidget->setCurrentIndex(0);
	ui->frameDate->show();
	yearCombo->show();
	monthCombo->hide();
	startTime->hide();
	endTime->hide();
	this->chosen = "allmonth";
}

void MainWindow::updateInfoFrame(int64_t beginTime, int64_t endTime)
{
	GitStatistics::statisticData dailyData = gs->calculateDay(beginTime, endTime);
	labelFileName->setText(ui->cbFileName->currentText());
	labelCount->setText("Commit Sayısı:\n" + QString::number(dailyData.commit));
	labelMean->setText("Günlük Commit Ortalaması:\n" + QString::number((float) dailyData.commit / (float) dailyData.day));
	labelWeekDayMean->setText("Haftaiçi Commit Ortalaması:\n" + QString::number((float) dailyData.commit / (float) dailyData.outsideWeekend));
	labelFreeDays->setText("Commit Atılmamış Gün Sayısı:\n" + QString::number(dailyData.freeDays));
	labelWeakDaysFree->setText("Haftaiçi Commit Atılmamış Gün Sayısı:\n" + QString::number(dailyData.outsideWeekendFreeDays));
	labelMonday->setText("Pazartesi Atılan Commit Sayısı:\n" + QString::number(dailyData.monday));
	labelFriday->setText("Cuma Atılan Commit Sayısı:\n" + QString::number(dailyData.friday));

}

void MainWindow::on_cbFileName_currentIndexChanged(const QString &arg1)
{
	gs = new GitStatistics(arg1);
	endTime->setDate(QDateTime::fromSecsSinceEpoch(gs->getEndTime()).date());
	startTime->setDate(QDateTime::fromSecsSinceEpoch(gs->getBeginTime()).date());
	yearCombo->clear();
	yearCombo->addItems(gs->getAllYears());
	drawLineChart();
}

void MainWindow::createCalendar()
{
	calendar = new QCalendarWidget;
	ui->frameCalendarView->layout()->addWidget(calendar);
	calendar->setGridVisible(true);
	connect(startTime, &QDateEdit::dateChanged, this, &MainWindow::startTime_dateChanged);
	connect(endTime, &QDateEdit::dateChanged, this, &MainWindow::endTime_dateChanged);
}

void MainWindow::fillCalendar()
{
	QMap<QString, int> dailyData = gs->calculateDay(QDateTime(startTime->date(), QTime(0,0,0)).toSecsSinceEpoch(),
													QDateTime(endTime->date(), QTime(0,0,0)).toSecsSinceEpoch()+24*60*60).dailyCommit;
	QTextCharFormat fmt;
	calendar->setMinimumDate(QDateTime::fromSecsSinceEpoch(gs->getBeginTime()).date());
	calendar->setMaximumDate(QDateTime::fromSecsSinceEpoch(gs->getEndTime()).date());
	for (int i = 0; i < dailyData.size(); ++i) {
		if (dailyData[dailyData.keys()[i]] > 0 && dailyData[dailyData.keys()[i]] < 2)
			fmt.setBackground(Qt::green);
		if (dailyData[dailyData.keys()[i]] >= 2 && dailyData[dailyData.keys()[i]] < 5)
			fmt.setBackground(Qt::yellow);
		if (dailyData[dailyData.keys()[i]] >=5)
			fmt.setBackground(Qt::red);
		QStringList list;
		fmt.setToolTip(printTool(QDate::fromString(dailyData.keys()[i])));
		calendar->setDateTextFormat(QDate::fromString(dailyData.keys()[i]), fmt);
	}
}

void MainWindow::startTime_dateChanged(const QDate &date)
{
	calendar->setMinimumDate(date);
	endTime->setDate(calendar->maximumDate());
	updateInfoFrame(QDateTime(startTime->date(), QTime(0,0,0)).toSecsSinceEpoch(),QDateTime(endTime->date(), QTime(0,0,0)).toSecsSinceEpoch());
}

void MainWindow::endTime_dateChanged(const QDate &date)
{
	calendar->setMaximumDate(date);
	startTime->setDate(calendar->minimumDate());
	updateInfoFrame(QDateTime(startTime->date(), QTime(0,0,0)).toSecsSinceEpoch(),QDateTime(endTime->date(), QTime(0,0,0)).toSecsSinceEpoch());
}

QString MainWindow::printTool(QDate date)
{
	QString commits;
	QVector<GitStatistics::Data> data = gs->getPeriodlyData(date, date.addDays(1));
	for (int i = 0; i < data.size(); ++i) {
		commits = commits + data[i].commitid + data[i].author + data[i].message + "\n";
	}
	return commits;
}


void MainWindow::createFrameDate()
{
	yearCombo = new QComboBox();
	monthCombo = new QComboBox();
	startTime = new QDateEdit();
	endTime = new QDateEdit();
	if (!ui->frameDate->layout())
		ui->frameDate->setLayout(new QVBoxLayout);
	ui->frameDate->layout()->addWidget(yearCombo);
	ui->frameDate->layout()->addWidget(monthCombo);
	ui->frameDate->layout()->addWidget(startTime);
	ui->frameDate->layout()->addWidget(endTime);
	for (int i = 1; i < 13; ++i) {
		monthCombo->addItem(getMonthName(i));
	}
}

void MainWindow::createInfoLabels()
{
	labelFileName = new QLabel();
	labelCount = new QLabel();
	labelMean = new QLabel();
	labelWeekDayMean = new QLabel();
	labelFreeDays = new QLabel();
	labelWeakDaysFree = new QLabel();
	labelMonday = new QLabel();
	labelFriday = new QLabel();
	ui->infoFrame->layout()->addWidget(labelFileName);
	ui->infoFrame->layout()->addWidget(labelCount);
	ui->infoFrame->layout()->addWidget(labelMean);
	ui->infoFrame->layout()->addWidget(labelWeekDayMean);
	ui->infoFrame->layout()->addWidget(labelFreeDays);
	ui->infoFrame->layout()->addWidget(labelWeakDaysFree);
	ui->infoFrame->layout()->addWidget(labelMonday);
	ui->infoFrame->layout()->addWidget(labelFriday);
}

void MainWindow::setChartDatas(QString chosen, QVector<GitStatistics::Data> datas)
{
	int row, col;
	row = graphicCounter / 2;
	col = graphicCounter % 2;
	QFrame *f = new QFrame();
	f->setLayout(new QVBoxLayout);
	DrawChart *c = new DrawChart(chosen, datas);
	f->layout()->addWidget(c);
	cchartL->addWidget(f, row ,col);
	graphicCounter++;

}

void MainWindow::clearFrame(QWidget *widget)
{
	QLayoutItem *wItem;
	while ((wItem = widget->layout()->takeAt(0)) != 0)
	{
		if (wItem->widget())
			wItem->widget()->setParent(NULL);
		delete wItem;
	}
}

QString MainWindow::getMonthName(int monthNumber)
{
	QStringList monthNames = {"Ocak", "Şubat", "Mart", "Nisan", "Mayıs", "Haziran", "Temmuz", "Ağustos", "Eylül", "Ekim", "Kasım", "Aralık"};
	return monthNames[monthNumber-1];
}

int MainWindow::getMonthNumber(QString monthName)
{
	QStringList monthNames = {"Ocak", "Şubat", "Mart", "Nisan", "Mayıs", "Haziran", "Temmuz", "Ağustos", "Eylül", "Ekim", "Kasım", "Aralık"};
	for (int i = 0; i < monthNames.size(); ++i) {
		if (monthName == monthNames[i])
			return i+1;
	}
	return 1;
}

QMap<int64_t, int> MainWindow::setLineSeries()
{
	QMap<QString, int> dailyData = gs->calculateDay(QDateTime(startTime->date(), QTime(0,0,0)).toSecsSinceEpoch(),
													QDateTime(endTime->date(), QTime(0,0,0)).toSecsSinceEpoch()+24*60*60).dailyCommit;
	QMap<int64_t, int> epochDailyData;
	QList<int64_t> list;
	for (int i = 0; i < dailyData.size(); ++i) {
		epochDailyData[QDateTime(QDate::fromString(dailyData.keys()[i])).toMSecsSinceEpoch()] = dailyData[dailyData.keys()[i]];
	}
	return epochDailyData;
}

void MainWindow::on_radioButton_clicked()
{
	ui->stackedWidget->setCurrentIndex(2);
	ui->frameDate->show();
	startTime->show();
	endTime->show();
	yearCombo->hide();
	monthCombo->hide();
	chosen = "lineChart";
}

void MainWindow::createInfoColor()
{
	ui->frameGreen->setStyleSheet("background-color: rgb(113, 230, 100)");
	ui->frameGreen->setLayout(new QVBoxLayout());
	ui->frameGreen->layout()->addWidget(new QLabel(">0"));

	ui->frameYellow->setStyleSheet("background-color: rgb(249, 255, 60)");
	ui->frameYellow->setLayout(new QVBoxLayout());
	ui->frameYellow->layout()->addWidget(new QLabel("2-4"));

	ui->frameRed->setStyleSheet("background-color: rgb(225, 71, 47)");
	ui->frameRed->setLayout(new QVBoxLayout());
	ui->frameRed->layout()->addWidget(new QLabel(">4"));
}

void MainWindow::drawLineChart()
{
	QMap<int64_t, int> dailyData = setLineSeries();
	QLineSeries *series = new QLineSeries;

	for (int i = 0; i < dailyData.size(); ++i) {
		series->append(dailyData.keys()[i], dailyData[dailyData.keys()[i]]);
	}

	QChart *chart = new QChart();
	chart->addSeries(series);
	chart->legend()->hide();
	chart->setTitle("İstatistik Çizgi Grafiği");

	QDateTimeAxis *axisX = new QDateTimeAxis;
	axisX->setTickCount(10);
	axisX->setFormat("MMM yyyy");
	axisX->setTitleText("Date");
	chart->addAxis(axisX, Qt::AlignBottom);
	series->attachAxis(axisX);

	QValueAxis *axisY = new QValueAxis;
	axisY->setLabelFormat("%i");
	axisY->setTitleText("Commit Sayısı");
	chart->addAxis(axisY, Qt::AlignLeft);
	series->attachAxis(axisY);

	QChartView *chartView = new QChartView(chart);
	chartView->setRenderHint(QPainter::Antialiasing);
	clearFrame(ui->frameLineChart);
	ui->frameLineChart->layout()->addWidget(chartView);
}
