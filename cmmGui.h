#pragma once

//#include <QtWidgets/QWidget>
//#include "ui_cmmGui.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QTableWidget>
#include <QLineEdit>
#include <QLabel>

#include "attendance.hpp"

//Written by Kevin Kawecki 2018
//
//Function tree
//1
//Ask for todays date: see enterDate()
//	-> store in data structure via setDate() in attendance.hpp
//	-> update history.txt
//2
//Receive a swipe: swipe()
//	-> Check if in data structure
//		-> If not add to structure and update members.txt
//		-> If yes print their name and email
//	-> Update history.txt
//
//That's really it. Seems a bit over whelming but comments should help along the way 
//Feel free to contact: kevin.kawecki@gmail.com

class cmmGui : public QWidget
{
	Q_OBJECT

public:
	cmmGui(QWidget *parent = Q_NULLPTR);

	void fillHistory();

private slots:
	//action after date is entered 
	void enterDate();
	//action for a swipe
	void swipe();
	//action for name 
	void enterName();
	//action for email
	void enterEmail();

private:
	//Ui::cmmGuiClass ui;

	//Gui elements
	QWidget *window;

	QVBoxLayout *mainLayout;

	QHBoxLayout	*dateLayout;
	QLineEdit	*dateBar;
	QLabel		*dateLabel;

	QHBoxLayout		*tableLayout;
	QTableWidget	*peopleHere;
	QTableWidget	*powerRank;

	QHBoxLayout	*nameLayout;
	QLineEdit	*nameBar;
	QLabel		*nameLabel;

	QHBoxLayout	*emailLayout;
	QLineEdit	*emailBar;
	QLabel		*emailLabel;

	QHBoxLayout *swipeLayout;
	QLineEdit	*swipeBar;
	QLabel		*swipeLabel;

	//attendance data structure to keep number associations 
	Attendance	monitor;

	//allows passing from swipe slot to name slot
	std::string tempString;

	//history stream
	std::ofstream historyFile;

	//members stream
	std::ofstream membersFile;

};
