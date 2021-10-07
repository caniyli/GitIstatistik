#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->frameDate->hide();
	createFrameDate();
	createInfoColor();
	createInfoLabels();
	createInfoLabels_2();
	ui->cbFileName_2->hide();
	ui->infoFrame_2->hide();
	QStringList texts = directory.entryList(QStringList() << "*.txt" << "*.txt",QDir::Files);
	ui->cbFileName->addItems(texts);
	createCalendar();
	connect(startTime, &QDateEdit::dateChanged, this, &MainWindow::startTime_dateChanged);
	connect(endTime, &QDateEdit::dateChanged, this, &MainWindow::endTime_dateChanged);
	updateMonthCombo();
	connect(yearCombo, &QComboBox::currentTextChanged, this, &MainWindow::updateMonthCombo);
}

void MainWindow::on_buttonDraw_clicked()
{
	if (chosen == "year"){
	QString date_string_on_db = yearCombo->currentText();
	QDate Date = QDate::fromString(date_string_on_db,"yyyy");
		setChartDatas(chosen, gsMap[ui->cbFileName->currentText()]->getPeriodlyData(Date, Date.addYears(1)));
	}else if (chosen == "month"){
	QString date_string_on_db = monthCombo->currentText() + "/" + yearCombo->currentText();
	QDate Date = QDate::fromString(date_string_on_db,"MMM/yyyy");
		setChartDatas(chosen + "/" +monthCombo->currentText(),
								gsMap[ui->cbFileName->currentText()]->getPeriodlyData(Date, Date.addMonths(1)));
	}else if (chosen == "allyear"){
		QStringList allYear = gsMap[ui->cbFileName->currentText()]->getAllYears();
		for (int i = 0; i <allYear.size() ; ++i) {
			QDate Date = QDate::fromString(allYear[i],"yyyy");
			setChartDatas(chosen, gsMap[ui->cbFileName->currentText()]->getPeriodlyData(Date, Date.addYears(1)));
		}
	}else if (chosen == "allmonth"){
		for (int i = 0; i < monthCombo->count(); ++i){
			QString date_string_on_db = monthCombo->itemText(i) + "/" + yearCombo->currentText();
			QDate Date = QDate::fromString(date_string_on_db,"MMM/yyyy");
			setChartDatas(chosen + "/" + monthCombo->itemText(i), 
							gsMap[ui->cbFileName->currentText()]->getPeriodlyData(Date, Date.addMonths(1)));
		}
	}else if (chosen == "lineChart")
	{
		drawLineChart();
	}else if (chosen == "compare"){
		QStringList allYear_1 = gsMap[ui->cbFileName->currentText()]->getAllYears();
		QStringList allYear_2 = gsMap[ui->cbFileName_2->currentText()]->getAllYears();
		if (allYear_1.size() >= allYear_2.size()) {
			for (int i = 0; i <allYear_1.size(); ++i) {
				QDate Date = QDate::fromString(allYear_1[i],"yyyy");
				setChartDatas(chosen, gsMap[ui->cbFileName->currentText()]->getPeriodlyData(Date, Date.addYears(1)));
				setChartDatas(chosen, gsMap[ui->cbFileName_2->currentText()]->getPeriodlyData(Date, Date.addYears(1)));
			}
		}else {
			for (int i = 0; i <allYear_2.size(); ++i) {
				QDate Date = QDate::fromString(allYear_2[i],"yyyy");
				setChartDatas(chosen, gsMap[ui->cbFileName->currentText()]->getPeriodlyData(Date, Date.addYears(1)));
				setChartDatas(chosen, gsMap[ui->cbFileName_2->currentText()]->getPeriodlyData(Date, Date.addYears(1)));
			}
		ui->infoFrame_2->show();
		updateInfoFrame_2(gsMap[ui->cbFileName_2->currentText()]->getBeginTime(), gsMap[ui->cbFileName_2->currentText()]->getEndTime()+24 * 60 * 60);
		}
	}
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_rbYear_clicked()
{
	ui->stackedWidget->setCurrentIndex(index[ui->cbFileName->currentText()]);
	ui->frameDate->show();
	ui->infoFrame_2->hide();
	yearCombo->show();
	monthCombo->hide();
	startTime->hide();
	endTime->hide();
	ui->cbFileName_2->hide();
	this->chosen = "year";
}

void MainWindow::on_rbMonth_clicked()
{
	ui->stackedWidget->setCurrentIndex(index[ui->cbFileName->currentText()]);
	ui->frameDate->show();
	ui->infoFrame_2->hide();
	startTime->hide();
	endTime->hide();
	yearCombo->show();
	monthCombo->show();
	ui->cbFileName_2->hide();
	this->chosen = "month";
}

void MainWindow::on_rbPeriod_clicked()
{
	ui->stackedWidget->setCurrentIndex(0);
	fillCalendar();
	ui->frameDate->show();
	ui->infoFrame_2->hide();
	startTime->show();
	endTime->show();
	yearCombo->hide();
	monthCombo->hide();
	ui->cbFileName_2->hide();
}

void MainWindow::on_buttonDelete_clicked()
{
	clearFrame(mapFrame[ui->cbFileName->currentText()]);
	graphicCounter[ui->cbFileName->currentText()] = 0;
}

void MainWindow::on_rbAllYear_clicked()
{
	ui->stackedWidget->setCurrentIndex(index[ui->cbFileName->currentText()]);
	ui->frameDate->hide();
	ui->infoFrame_2->hide();
	ui->cbFileName_2->hide();
	this->chosen = "allyear";
}

void MainWindow::on_rbAllMonth_clicked()
{
	ui->stackedWidget->setCurrentIndex(index[ui->cbFileName->currentText()]);
	ui->frameDate->show();
	ui->infoFrame_2->hide();
	yearCombo->show();
	monthCombo->hide();
	startTime->hide();
	endTime->hide();
	ui->cbFileName_2->hide();
	this->chosen = "allmonth";
}

void MainWindow::updateInfoFrame(int64_t beginTime, int64_t endTime)
{
	GitStatistics::statisticData dailyData = gsMap[ui->cbFileName->currentText()]->calculateDay(beginTime, endTime + (24 * 60 * 60));
	labelFileName->setText(ui->cbFileName->currentText());
	labelCount->setText("Commit Sayısı:\n" + QString::number(dailyData.commit));
	labelMean->setText("Günlük Commit Ortalaması:\n" + QString::number((float) dailyData.commit / (float) dailyData.day));
	labelWeekDayMean->setText("Haftaiçi Commit Ortalaması:\n" + QString::number((float) dailyData.commit / (float) dailyData.outsideWeekend));
	labelFreeDays->setText("Commit Atılmamış Gün Sayısı:\n" + QString::number(dailyData.freeDays));
	labelWeakDaysFree->setText("Haftaiçi Commit Atılmamış Gün Sayısı:\n" + QString::number(dailyData.outsideWeekendFreeDays));
	labelMonday->setText("Pazartesi Atılan Commit Sayısı:\n" + QString::number(dailyData.monday));
	labelFriday->setText("Cuma Atılan Commit Sayısı:\n" + QString::number(dailyData.friday));
}

void MainWindow::updateInfoFrame_2(int64_t beginTime, int64_t endTime)
{
	GitStatistics::statisticData dailyData = gsMap[ui->cbFileName_2->currentText()]->calculateDay(beginTime, endTime + (24 * 60 * 60));
	labelFileName_2->setText(ui->cbFileName_2->currentText());
	labelCount_2->setText("Commit Sayısı:\n" + QString::number(dailyData.commit));
	labelMean_2->setText("Günlük Commit Ortalaması:\n" + QString::number((float) dailyData.commit / (float) dailyData.day));
	labelWeekDayMean_2->setText("Haftaiçi Commit Ortalaması:\n" + QString::number((float) dailyData.commit / (float) dailyData.outsideWeekend));
	labelFreeDays_2->setText("Commit Atılmamış Gün Sayısı:\n" + QString::number(dailyData.freeDays));
	labelWeakDaysFree_2->setText("Haftaiçi Commit Atılmamış Gün:\n" + QString::number(dailyData.outsideWeekendFreeDays));
	labelMonday_2->setText("Pazartesi Atılan Commit Sayısı:\n" + QString::number(dailyData.monday));
	labelFriday_2->setText("Cuma Atılan Commit Sayısı:\n" + QString::number(dailyData.friday));
}
void MainWindow::on_cbFileName_currentIndexChanged(const QString &arg1)
{
	if (ui->cbFileName->count() != 0) {
		if (gsMap.count(arg1) == 0){
			gsMap[arg1] = new GitStatistics(directory.absolutePath() + "/" + arg1);
			QWidget *firstPageWidget = new QWidget;
			firstPageWidget->setLayout(new QVBoxLayout());
			QScrollArea *area = new QScrollArea();
			firstPageWidget->layout()->addWidget(area);
			QFrame *frame = new QFrame;
			frame->setBackgroundRole(QPalette::Dark);
			mapFrame[arg1] = frame;
			QGridLayout *layout = new QGridLayout;
			frame->setLayout(layout);
			area->setWidget(frame);
			area->setWidgetResizable(true);
			ui->stackedWidget->insertWidget(mapLayout.size()+2,firstPageWidget);
			mapLayout[arg1] = layout;
			index[arg1] = mapLayout.size() + 1;
		}
		ui->stackedWidget->setCurrentIndex(index[arg1]);
		endTime->setDate(QDateTime::fromSecsSinceEpoch(gsMap[arg1]->getEndTime()).date());
		startTime->setDate(QDateTime::fromSecsSinceEpoch(gsMap[arg1]->getBeginTime()).date());
		yearCombo->clear();
		yearCombo->addItems(gsMap[arg1]->getAllYears());
		updateInfoFrame(QDateTime(startTime->date(), QTime(0,0,0)).toSecsSinceEpoch(),QDateTime(endTime->date(), QTime(0,0,0)).toSecsSinceEpoch());
	}
}

void MainWindow::createCalendar()
{
	calendar = new QCalendarWidget;
	ui->frameCalendarView->layout()->addWidget(calendar);
	calendar->setGridVisible(true);
	calendar->setMinimumDate(QDateTime::fromSecsSinceEpoch(gsMap[ui->cbFileName->currentText()]->getBeginTime()).date());
	calendar->setMaximumDate(QDateTime::fromSecsSinceEpoch(gsMap[ui->cbFileName->currentText()]->getEndTime()).date());
}

void MainWindow::fillCalendar()
{
	QMap<QString, int> dailyData = gsMap[ui->cbFileName->currentText()]->calculateDay(QDateTime(startTime->date(), QTime(0,0,0)).toSecsSinceEpoch(),
													QDateTime(endTime->date().addDays(1), QTime(0,0,0)).toSecsSinceEpoch()).dailyCommit;
	QTextCharFormat fmt;
	calendar->setMinimumDate(QDateTime::fromSecsSinceEpoch(gsMap[ui->cbFileName->currentText()]->getBeginTime()).date());
	calendar->setMaximumDate(QDateTime::fromSecsSinceEpoch(gsMap[ui->cbFileName->currentText()]->getEndTime()).date());
	for (int i = 0; i < dailyData.size(); ++i) {
		if (dailyData[dailyData.keys()[i]] > 0 && dailyData[dailyData.keys()[i]] < 2)
			fmt.setBackground(Qt::green);
		if (dailyData[dailyData.keys()[i]] >= 2 && dailyData[dailyData.keys()[i]] < 5)
			fmt.setBackground(Qt::yellow);
		if (dailyData[dailyData.keys()[i]] >=5)
			fmt.setBackground(Qt::red);
		fmt.setToolTip(printTool(QDate::fromString(dailyData.keys()[i])));
		calendar->setDateTextFormat(QDate::fromString(dailyData.keys()[i]), fmt);
	}
}

void MainWindow::startTime_dateChanged(const QDate &date)
{
	calendar->setMinimumDate(date);
	endTime->setDate(calendar->maximumDate());
	updateInfoFrame(QDateTime(startTime->date(), QTime(0,0,0)).toSecsSinceEpoch(),
					QDateTime(endTime->date(), QTime(0,0,0)).toSecsSinceEpoch());
}

void MainWindow::endTime_dateChanged(const QDate &date)
{
	calendar->setMaximumDate(date);
	startTime->setDate(calendar->minimumDate());
	updateInfoFrame(QDateTime(startTime->date(), QTime(0,0,0)).toSecsSinceEpoch()
					,QDateTime(endTime->date(), QTime(0,0,0)).toSecsSinceEpoch());
}

QString MainWindow::printTool(QDate date)
{
	QString commits;
	QVector<GitStatistics::Data> data = gsMap[ui->cbFileName->currentText()]->getPeriodlyData(date, date.addDays(1));
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

void MainWindow::createInfoLabels_2()
{
	labelFileName_2 = new QLabel();
	labelCount_2 = new QLabel();
	labelMean_2 = new QLabel();
	labelWeekDayMean_2 = new QLabel();
	labelFreeDays_2 = new QLabel();
	labelWeakDaysFree_2 = new QLabel();
	labelMonday_2 = new QLabel();
	labelFriday_2 = new QLabel();
	ui->infoFrame_2->layout()->addWidget(labelFileName_2);
	ui->infoFrame_2->layout()->addWidget(labelCount_2);
	ui->infoFrame_2->layout()->addWidget(labelMean_2);
	ui->infoFrame_2->layout()->addWidget(labelWeekDayMean_2);
	ui->infoFrame_2->layout()->addWidget(labelFreeDays_2);
	ui->infoFrame_2->layout()->addWidget(labelWeakDaysFree_2);
	ui->infoFrame_2->layout()->addWidget(labelMonday_2);
	ui->infoFrame_2->layout()->addWidget(labelFriday_2);
}

void MainWindow::setChartDatas(QString chosen, QVector<GitStatistics::Data> datas)
{
	int row, col;
	row = graphicCounter[ui->cbFileName->currentText()] / 2;
	col = graphicCounter[ui->cbFileName->currentText()] % 2;
	QFrame *f = new QFrame();
	f->setLayout(new QVBoxLayout);
	DrawChart *c = new DrawChart(chosen, datas);
	f->layout()->addWidget(c);
	mapLayout[ui->cbFileName->currentText()]->addWidget(f, row ,col);
	graphicCounter[ui->cbFileName->currentText()]++;
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

QMap<int64_t, int> MainWindow::setLineSeries()
{
	QMap<QString, int> dailyData = gsMap[ui->cbFileName->currentText()]->calculateDay(QDateTime(startTime->date(), QTime(0,0,0)).toSecsSinceEpoch(),
													QDateTime(endTime->date().addDays(1), QTime(0,0,0)).toSecsSinceEpoch()).dailyCommit;
	QMap<int64_t, int> epochDailyData;
	for (int i = 0; i < dailyData.size(); ++i) {
		epochDailyData[QDateTime(QDate::fromString(dailyData.keys()[i])).toMSecsSinceEpoch()] = dailyData[dailyData.keys()[i]];
	}
	return epochDailyData;
}

void MainWindow::on_radioButton_clicked()
{
	ui->stackedWidget->setCurrentIndex(1);
	ui->frameDate->show();
	ui->infoFrame_2->hide();
	startTime->show();
	endTime->show();
	yearCombo->hide();
	monthCombo->hide();
	ui->cbFileName_2->hide();
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

void MainWindow::on_pbFilePath_clicked()
{
	 QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
											directory.absolutePath(),
											QFileDialog::ShowDirsOnly
											| QFileDialog::DontResolveSymlinks);
	directory.setPath(dir);
	QStringList texts = directory.entryList(QStringList() << "*.txt" << "*.txt",QDir::Files);
	ui->cbFileName->clear();
	ui->cbFileName->addItems(texts);
}

void MainWindow::updateMonthCombo()
{
	QMap<QString, int> dailyCommit = gsMap[ui->cbFileName->currentText()]->calculateDay(gsMap[ui->cbFileName->currentText()]->getBeginTime()
																						, gsMap[ui->cbFileName->currentText()]->getEndTime()+24*60*60).dailyCommit;
	QMap<QString, int> month;
	monthCombo->clear();
	for (int i = 0; i < dailyCommit.size(); ++i) {
		QStringList pieces = dailyCommit.keys()[i].split(" ");
		if(yearCombo->currentText() == pieces[3]){
			month[pieces[1]]++;
		}
	}
	monthCombo->addItems(month.keys());
}

void MainWindow::on_rbCompare_clicked()
{
	ui->stackedWidget->setCurrentIndex(index[ui->cbFileName->currentText()]);
	ui->cbFileName_2->clear();
	QStringList texts = directory.entryList(QStringList() << "*.txt" << "*.txt",QDir::Files);
	ui->cbFileName_2->addItems(texts);
	ui->cbFileName_2->show();
	ui->frameDate->hide();
	startTime->hide();
	endTime->hide();
	yearCombo->hide();
	monthCombo->hide();
	this->chosen = "compare";
}

void MainWindow::on_cbFileName_2_currentIndexChanged(const QString &arg1)
{
	if (ui->cbFileName_2->count() != 0) {
		if (gsMap.count(arg1) == 0)
			gsMap[arg1] = new GitStatistics(directory.absolutePath() + "/" + arg1);
	}
}
