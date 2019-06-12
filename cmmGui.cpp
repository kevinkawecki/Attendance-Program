#include "cmmGui.h"

#include <QHeaderView>

cmmGui::cmmGui(QWidget *parent)
	: QWidget(parent)
{
	//ui.setupUi(this);

	window = new QWidget;// (this);

	peopleHere = new QTableWidget(0, 1, window);
	powerRank  = new QTableWidget(0, 2, window);

	//label and stretch left table
	peopleHere->setHorizontalHeaderItem(0, new QTableWidgetItem("This Meeting"));
	peopleHere->horizontalHeader()->setStretchLastSection(true);

	//label and stretch right table
	powerRank->setHorizontalHeaderItem(0, new QTableWidgetItem("Attended"));
	powerRank->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	powerRank->horizontalHeader()->setStretchLastSection(true);
	//powerRank->horizontalHeader()->sortIndicatorOrder();

	//date bar init 
	dateLabel = new QLabel("Todays Date:");
	dateBar	  = new QLineEdit();
	dateBar->setReadOnly(false);
	connect(dateBar, SIGNAL(editingFinished()), this, SLOT(enterDate()));

	//name bar init 
	nameLabel = new QLabel("Name:");
	nameBar	  = new QLineEdit();
	nameBar->setReadOnly(true);
	connect(nameBar, SIGNAL(editingFinished()), this, SLOT(enterName()));

	//email bar init
	emailLabel = new QLabel("Email:");
	emailBar = new QLineEdit();
	emailBar->setReadOnly(true);
	//connect(nameBar, SIGNAL(editingFinished()), this, SLOT(enterName()));

	//swipe bar init
	swipeLabel = new QLabel("Swipe Here:");
	swipeBar   = new QLineEdit();
	swipeBar->setReadOnly(true);
	connect(swipeBar, SIGNAL(editingFinished()), this, SLOT(swipe()));

	//layout management (add to things in the order that they are displayed)
	dateLayout = new QHBoxLayout;
	dateLayout->addWidget(dateLabel);
	dateLayout->addWidget(dateBar);

	tableLayout = new QHBoxLayout;
	tableLayout->addWidget(peopleHere);
	tableLayout->addWidget(powerRank);

	nameLayout = new QHBoxLayout;
	nameLayout->addWidget(nameLabel);
	nameLayout->addWidget(nameBar);

	emailLayout = new QHBoxLayout;
	emailLayout->addWidget(emailLabel);
	emailLayout->addWidget(emailBar);

	swipeLayout = new QHBoxLayout;
	swipeLayout->addWidget(swipeLabel);
	swipeLayout->addWidget(swipeBar);

	mainLayout = new QVBoxLayout;
	mainLayout->addLayout(dateLayout);
	mainLayout->addLayout(tableLayout);
	mainLayout->addLayout(nameLayout);
	mainLayout->addLayout(emailLayout);
	mainLayout->addLayout(swipeLayout);

	//init window size and layout
	window->setLayout(mainLayout);
	window->setMinimumSize(1500, 1000);

	window->show();
	//this->show();

	//init data structure
	monitor.initNameAndNum();

	//fill power rank 
	fillHistory();

	//show example 
	dateBar->setText("MM/DD");
	dateBar->selectAll();
	dateBar->setFocus();
	
}

void cmmGui::enterDate()
{
	static bool doubleCheck = true;		//outputs twice to history file for some reason // fixed

	std::string date = dateBar->text().toStdString();
	if (monitor.setDate(date))
	{
		//prevent future date edits
		dateBar->setReadOnly(true);

		//make swipe next
		swipeBar->setReadOnly(false);
		swipeBar->setText("Swipe Here");
		swipeBar->selectAll();
		swipeBar->setFocus();

		//update history date
		if (doubleCheck == true)
		{
			historyFile.open("history.txt", std::ios::app);
			historyFile << monitor.getDate() << std::endl;
			historyFile.close();
			doubleCheck = false;
		}
	}
	else
	{
		//reset text
		dateBar->setText("MM/DD");
		dateBar->selectAll();
		dateBar->setFocus();
	}
}

void cmmGui::fillHistory()
{
	powerRank->setSortingEnabled(false);
	powerRank->setRowCount(0);

	//step through entire map of history
	for (auto it = monitor.nameToNumber.cbegin();
		it != monitor.nameToNumber.cend(); ++it)
	{
		//push back rank table
		powerRank->insertRow(powerRank->rowCount());

		//set history content
		//add attendance
		QTableWidgetItem *item = new QTableWidgetItem(Qt::DisplayRole);
		item->setData(Qt::DisplayRole, monitor.getDays(it->first));
		powerRank->setItem(powerRank->rowCount() - 1, 0, item);
		//add name
		powerRank->setItem(powerRank->rowCount() - 1, 1,
			new QTableWidgetItem(QString::fromStdString(monitor.getName(it->first))));
	}

	powerRank->setSortingEnabled(true);

	//update member file
	membersFile.open("members.txt");
	//step through entire member list and attendance
	for (auto it = monitor.nameToNumber.cbegin();
		it != monitor.nameToNumber.cend(); ++it)
		membersFile << it->first << " : " << it->second << " : "
		<< monitor.getDays(it->first) << std::endl;
	membersFile.close();
}

void cmmGui::swipe()
{
	//take in string 
	std::string idNum = swipeBar->text().toStdString();

	//check for valid swipe 
	if ((idNum[0] != ';') || (idNum.size() < 10) || (idNum == "Swipe Here"))
	{
		//swipeBar->setText("Swipe Here");
		//swipeBar->selectAll();
		nameBar->setFocus();
		return;
	}

	//parse out id number
	idNum.erase(idNum.begin());
	idNum.erase(idNum.begin() + 9, idNum.end());

	//check for name
	if (monitor.inTheSystem(idNum))
	{
		//clear swipe text
		swipeBar->clear();

		//show name 
		nameBar->setText(QString::fromStdString(monitor.getName(idNum)));

		//check if here already
		if (monitor.addToToday(idNum))
		{
			//increment attendance
			monitor.incrementDay(idNum);

			//open and update history file
			historyFile.open("history.txt", std::ios::app);
			historyFile << "\t" << idNum << " : " << monitor.getName(idNum) << std::endl;
			historyFile.close();

			//add to gui list
			peopleHere->insertRow(peopleHere->rowCount());
			peopleHere->setItem(peopleHere->rowCount() - 1, 0,
				new QTableWidgetItem(QString::fromStdString(monitor.getName(idNum))));

			//update power rank
			fillHistory();
		}
	}
	else
	{
		//allow name to be used
		nameBar->setReadOnly(false);
		//block future swipes until name entered
		swipeBar->setReadOnly(true);

		//clear number
		swipeBar->clear();

		//show name format
		nameBar->setText("First Last");

		//set cursor to name bar
		nameBar->setFocus();

		//get name ready to clear
		nameBar->selectAll();

		//store number for next slot
		tempString = idNum;
	}

}

void cmmGui::enterName()
{
	//grab name entered
	std::string name = nameBar->text().toStdString();

	//prevent accidental clicking 
	if (name == "First Last")
	{
		nameBar->setText("First Last");
		nameBar->setFocus();
		nameBar->selectAll();
		return;
	}
		

	//prevent from running twice (this happens here for some reason)
	if (!monitor.inTheSystem(tempString))
	{
		//send to the data structure 
		std::pair<std::string, std::string> in(tempString, name);
		monitor.addNameNum(in);
		monitor.addToToday(tempString);
		monitor.incrementDay(tempString);

		//open and update history file
		historyFile.open("history.txt", std::ios::app);
		historyFile << "\t" << tempString << " : " << monitor.getName(tempString) << std::endl;
		historyFile.close();

		//add to gui list
		peopleHere->insertRow(peopleHere->rowCount());
		peopleHere->setItem(peopleHere->rowCount() - 1, 0,
			new QTableWidgetItem(QString::fromStdString(monitor.getName(tempString))));

		//update power rank
		fillHistory();

		//update use
		nameBar->setReadOnly(true);
		swipeBar->setReadOnly(false);

		//send back to swipe
		swipeBar->setText("Swipe Here");
		swipeBar->selectAll();
		swipeBar->setFocus();
	}
}

void cmmGui::enterEmail()
{

}
