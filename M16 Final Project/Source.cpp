/*
* Program Name: M16 Final Project
* Author: Logan Kinman
* Date Last Updated: 5-9-2019
* Purpose: Connect and interact with the business database
*/

#include <iostream>
#include <string>
#include "sqlite3.h"

class Database
{
private:

	int rc;
	sqlite3* mydb = NULL;
	sqlite3_stmt* query = NULL;
	std::string functionChoice;

public:

	void openDatabase()
	{
		rc = sqlite3_open_v2("MidtowneMedicines.db", &mydb, SQLITE_OPEN_READWRITE, "win32");		//open the business database

		if (rc != 0)
		{
			std::cout << "There was an error opening the database..." << std::endl;
		}
		else
		{
			std::cout << "Database opened successfully!\n" << std::endl;
		}
	}

	void closeDatabase()		//function to close the business database
	{
		rc = sqlite3_close_v2(mydb);
	}

	void chooseFunction()				//allows user to choose any of the 4 functions: view, add, update, or delete
	{
		std::cout << "Please enter a function (view, add, update, delete): ";
		std::getline(std::cin, functionChoice);

		while (functionChoice != "view" && functionChoice != "View" && functionChoice != "VIEW" && functionChoice != "add" && functionChoice != "Add" && functionChoice != "ADD" && functionChoice != "update" && functionChoice != "Update" && functionChoice != "UPDATE" && functionChoice != "delete" && functionChoice != "Delete" && functionChoice != "DELETE")
		{
			std::cout << "Please enter 'view', 'add', 'update', or 'delete': ";			//in case user enters unavailable function
			std::getline(std::cin, functionChoice);
		}
		std::cout << " " << std::endl;
	}

	void viewAddUpdateDelete()			//function containing all of the 4 available functions
	{
		std::string tableChoice;
		std::string idChoice;
		std::string rowChoice;
		std::string tempHold;
		const char* sqlStatement;
		std::string rowOptions;
		std::string rowOptionsChoice;
		std::string valueChoice;

		if (functionChoice == "update" || functionChoice == "Update" || functionChoice == "UPDATE")			//dynamic wording for different function choices
		{
			std::cout << "Which table would you like to update?" << std::endl;
		}
		else if (functionChoice == "add" || functionChoice == "Add" || functionChoice == "ADD")
		{
			std::cout << "Which table would you like to add to?" << std::endl;
		}
		else if (functionChoice == "delete" || functionChoice == "Delete" || functionChoice == "DELETE")
		{
			std::cout << "Which table would you like to delete from?" << std::endl;
		}
		else if (functionChoice == "view" || functionChoice == "View" || functionChoice == "VIEW")
		{
			std::cout << "Which table would you like to view?" << std::endl;
		}

		std::cout << "(Choices are: Clock, Doctor, Employee, Midtowne, Patient, Pharmacy, Prescription): ";
		std::getline(std::cin, tableChoice);				//user's choice for which table they want to interact with

		while (tableChoice != "clock" && tableChoice != "Clock" && tableChoice != "CLOCK" && tableChoice != "doctor" &&
			tableChoice != "Doctor" && tableChoice != "DOCTOR" && tableChoice != "employee" && tableChoice != "Employee" &&
			tableChoice != "EMPLOYEE" && tableChoice != "midtowne" && tableChoice != "Midtowne" && tableChoice != "MIDTOWNE" &&
			tableChoice != "patient" && tableChoice != "Patient" && tableChoice != "PATIENT" && tableChoice != "pharmacy" &&
			tableChoice != "Pharmacy" && tableChoice != "PHARMACY" && tableChoice != "prescription" && tableChoice != "Prescription" &&
			tableChoice != "PRESCRIPTION")
		{
			std::cout << "Please enter a specified choice: Clock, Doctor, Employee, Midtowne, Patient, Pharmacy, Prescription: ";
			std::getline(std::cin, tableChoice);
		}

		if (tableChoice == "midtowne" || tableChoice == "Midtowne" || tableChoice == "MIDTOWNE")		//don't want to make the user have to type out midtowne_medicines...midtowne is a bit easier
		{
			tableChoice = "Midtowne_Medicines";
		}

		tempHold = "SELECT count(*) From " + tableChoice;		//holds the SQL Statement in a string
		sqlStatement = tempHold.c_str();			//converts the string to a const char* for use in the prepare_v2 function

		rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);			//counts how many columns are in the selected table...
		rc = sqlite3_step(query);

		int rowcount = sqlite3_column_int(query, 0);			//...and stores that number in rowcount

		sqlite3_finalize(query);

		idChoice = tableChoice;

		if (tableChoice == "doctor" || tableChoice == "Doctor" || tableChoice == "DOCTOR")			//dynamically assign value to idChoice variable that matches specific table's primary key
		{
			idChoice = "Doc";
			tableChoice = "doctor";
		}
		else if (tableChoice == "clock" || tableChoice == "Clock" || tableChoice == "CLOCK")
		{
			tableChoice = "clock";
		}
		else if (tableChoice == "patient" || tableChoice == "Patient" || tableChoice == "PATIENT")
		{
			tableChoice = "patient";
		}
		else if (tableChoice == "pharmacy" || tableChoice == "Pharmacy" || tableChoice == "PHARMACY")
		{
			tableChoice = "pharmacy";
		}
		else if (tableChoice == "employee" || tableChoice == "Employee" || tableChoice == "EMPLOYEE")
		{
			idChoice = "Emp";
			tableChoice = "employee";
		}
		else if (tableChoice == "Midtowne_Medicines")
		{
			idChoice = "Mid";
		}
		else if (tableChoice == "prescription" || tableChoice == "Prescription" || tableChoice == "PRESCRIPTION")
		{
			idChoice = "Prescrip";
			tableChoice = "prescription";
		}

		tempHold = "SELECT " + idChoice + "_ID From " + tableChoice;
		sqlStatement = tempHold.c_str();

		rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);

		std::cout << "\n" << tableChoice << " ID" << std::endl;
		for (int i = 0; i < rowcount; ++i)
		{
			rc = sqlite3_step(query);
			std::cout << sqlite3_column_text(query, 0) << std::endl;			//prints column of primary keys to the screen
		}
		sqlite3_finalize(query);

		if (tableChoice == "clock")					//used to output titles of elements in a row
		{
			rowOptions = "Mid_ID, Emp_ID, Clock_ID, Clock_TimeIn, Clock_TimeOut";
		}
		else if (tableChoice == "doctor")
		{
			rowOptions = "Mid_ID, Emp_ID, Prescrip_ID, Doc_ID";
		}
		else if (tableChoice == "employee")
		{
			rowOptions = "Mid_ID, Emp_ID, Emp_FName, Emp_LName, Emp_Address, Emp_Phone";
		}
		else if (tableChoice == "Midtowne_Medicines")
		{
			rowOptions = "Mid_ID, Mid_Location, Mid_Hours";
		}
		else if (tableChoice == "patient")
		{
			rowOptions = "Mid_ID, Patient_ID, Patient_FName, Patient_LName, Patient_Age, Patient_Phone, Patient_ReasonForVisit";
		}
		else if (tableChoice == "pharmacy")
		{
			rowOptions = "Mid_ID, Pharmacy_ID, Pharmacy_FullStock";
		}
		else if (tableChoice == "prescription")
		{
			rowOptions = "Mid_ID, Patient_ID, Prescrip_ID, Prescrip_Name, Prescrip_Dosage, Prescrip_Instructions";
		}

		if (functionChoice == "update" || functionChoice == "Update" || functionChoice == "UPDATE")		//if the user chooses the update function
		{
			std::cout << "\nPlease select a Primary Key from " << tableChoice << std::endl;
			std::getline(std::cin, rowChoice);
			std::cout << " " << std::endl;

			tempHold = "Select * FROM " + tableChoice + " WHERE " + idChoice + "_ID = " + rowChoice;		//select a particular row
			sqlStatement = tempHold.c_str();

			rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);
			rc = sqlite3_step(query);
			for (int i = 0; i < sqlite3_data_count(query); ++i)
			{
				std::cout << sqlite3_column_text(query, i) << " | ";				//prints the selected row to the screen
			}
			sqlite3_finalize(query);
			std::cout << " " << std::endl;


			std::cout << "\nWhich element would you like to update? (" << rowOptions << "): ";		//select a particular element in row
			std::getline(std::cin, rowOptionsChoice);

			std::cout << "What would you like to change the value to? ";		//user enters new value
			std::getline(std::cin, valueChoice);

			tempHold = "UPDATE " + tableChoice + " SET " + rowOptionsChoice + " = '" + valueChoice + "' WHERE " + idChoice + "_ID = " + rowChoice;
			sqlStatement = tempHold.c_str();

			rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);		//updates element of row to user's input
			rc = sqlite3_step(query);
			sqlite3_finalize(query);

			std::cout << "\nRow updated!" << std::endl;

		}
		else if (functionChoice == "add" || functionChoice == "Add" || functionChoice == "ADD")		//adding a new row to table
		{
			std::string elementvalue1;		//corresponding names of values in table
			std::string elementvalue2;
			std::string elementvalue3;
			std::string elementvalue4;
			std::string elementvalue5;
			std::string elementvalue6;
			std::string elementvalue7;

			std::string valueChoice1;		//new user values to correspond with names in table
			std::string valueChoice2;
			std::string valueChoice3;
			std::string valueChoice4;
			std::string valueChoice5;
			std::string valueChoice6;
			std::string valueChoice7;

			std::cout << "\nPlease individually enter values to the following elements: (" << rowOptions << "): " << std::endl;		//enter values for new row

			if (tableChoice == "clock")
			{
				elementvalue1 = "Mid_ID";
				elementvalue2 = "Emp_ID";
				elementvalue3 = "Clock_ID";
				elementvalue4 = "Clock_TimeIn";
				elementvalue5 = "Clock_TimeOut";

				std::cout << "Value 1: ";				//user's new values to add to row
				std::getline(std::cin, valueChoice1);
				std::cout << "Value 2: ";
				std::getline(std::cin, valueChoice2);
				std::cout << "Value 3: ";
				std::getline(std::cin, valueChoice3);
				std::cout << "Value 4: ";
				std::getline(std::cin, valueChoice4);
				std::cout << "Value 5: ";
				std::getline(std::cin, valueChoice5);

				tempHold = "INSERT INTO " + tableChoice + " (" + elementvalue1 + "," + elementvalue2 + "," + elementvalue3 + "," + elementvalue4 + "," + elementvalue5 + ") VALUES ('" + valueChoice1 + "','" + valueChoice2 + "','" + valueChoice3 + "','" + valueChoice4 + "','" + valueChoice5 + "')";
				sqlStatement = tempHold.c_str();

				rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);		//adds new row to specified table
				rc = sqlite3_step(query);
				sqlite3_finalize(query);

			}
			else if (tableChoice == "doctor")
			{
				elementvalue1 = "Mid_ID";
				elementvalue2 = "Emp_ID";
				elementvalue3 = "Prescrip_ID";
				elementvalue4 = "Doc_ID";

				std::cout << "Value 1: ";
				std::getline(std::cin, valueChoice1);
				std::cout << "Value 2: ";
				std::getline(std::cin, valueChoice2);
				std::cout << "Value 3: ";
				std::getline(std::cin, valueChoice3);
				std::cout << "Value 4: ";
				std::getline(std::cin, valueChoice4);

				tempHold = "INSERT INTO " + tableChoice + " (" + elementvalue1 + "," + elementvalue2 + "," + elementvalue3 + "," + elementvalue4 + ") VALUES ('" + valueChoice1 + "','" + valueChoice2 + "','" + valueChoice3 + "','" + valueChoice4 + "')";
				sqlStatement = tempHold.c_str();

				rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);		//adds new row to specified table
				rc = sqlite3_step(query);
				sqlite3_finalize(query);
			}
			else if (tableChoice == "employee")
			{
				elementvalue1 = "Mid_ID";
				elementvalue2 = "Emp_ID";
				elementvalue3 = "Emp_FName";
				elementvalue4 = "Emp_LName";
				elementvalue5 = "Emp_Address";
				elementvalue6 = "Emp_Phone";

				std::cout << "Value 1: ";
				std::getline(std::cin, valueChoice1);
				std::cout << "Value 2: ";
				std::getline(std::cin, valueChoice2);
				std::cout << "Value 3: ";
				std::getline(std::cin, valueChoice3);
				std::cout << "Value 4: ";
				std::getline(std::cin, valueChoice4);
				std::cout << "Value 5: ";
				std::getline(std::cin, valueChoice5);
				std::cout << "Value 6: ";
				std::getline(std::cin, valueChoice6);

				tempHold = "INSERT INTO " + tableChoice + " (" + elementvalue1 + "," + elementvalue2 + "," + elementvalue3 + "," + elementvalue4 + "," + elementvalue5 + "," + elementvalue6 + ") VALUES ('" + valueChoice1 + "','" + valueChoice2 + "','" + valueChoice3 + "','" + valueChoice4 + "','" + valueChoice5 + "','" + valueChoice6 + "')";
				sqlStatement = tempHold.c_str();

				rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);		//adds new row to specified table
				rc = sqlite3_step(query);
				sqlite3_finalize(query);
			}
			else if (tableChoice == "Midtowne_Medicines")
			{
				elementvalue1 = "Mid_ID";
				elementvalue2 = "Mid_Location";
				elementvalue3 = "Mid_Hours";

				std::cout << "Value 1: ";
				std::getline(std::cin, valueChoice1);
				std::cout << "Value 2: ";
				std::getline(std::cin, valueChoice2);
				std::cout << "Value 3: ";
				std::getline(std::cin, valueChoice3);

				tempHold = "INSERT INTO " + tableChoice + " (" + elementvalue1 + "," + elementvalue2 + "," + elementvalue3 + ") VALUES ('" + valueChoice1 + "','" + valueChoice2 + "','" + valueChoice3 + "')";
				sqlStatement = tempHold.c_str();

				rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);		//adds new row to specified table
				rc = sqlite3_step(query);
				sqlite3_finalize(query);
			}
			else if (tableChoice == "patient")
			{
				elementvalue1 = "Mid_ID";
				elementvalue2 = "Patient_ID";
				elementvalue3 = "Patient_FName";
				elementvalue4 = "Patient_LName";
				elementvalue5 = "Patient_Age";
				elementvalue6 = "Patient_Phone";
				elementvalue7 = "Patient_ReasonForVisit";

				std::cout << "Value 1: ";
				std::getline(std::cin, valueChoice1);
				std::cout << "Value 2: ";
				std::getline(std::cin, valueChoice2);
				std::cout << "Value 3: ";
				std::getline(std::cin, valueChoice3);
				std::cout << "Value 4: ";
				std::getline(std::cin, valueChoice4);
				std::cout << "Value 5: ";
				std::getline(std::cin, valueChoice5);
				std::cout << "Value 6: ";
				std::getline(std::cin, valueChoice6);
				std::cout << "Value 7: ";
				std::getline(std::cin, valueChoice7);

				tempHold = "INSERT INTO " + tableChoice + " (" + elementvalue1 + "," + elementvalue2 + "," + elementvalue3 + "," + elementvalue4 + "," + elementvalue5 + "," + elementvalue6 + "," + elementvalue7 + ") VALUES ('" + valueChoice1 + "','" + valueChoice2 + "','" + valueChoice3 + "','" + valueChoice4 + "','" + valueChoice5 + "','" + valueChoice6 + "','" + valueChoice7 + "')";
				sqlStatement = tempHold.c_str();

				rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);		//adds new row to specified table
				rc = sqlite3_step(query);
				sqlite3_finalize(query);
			}
			else if (tableChoice == "pharmacy")
			{
				elementvalue1 = "Mid_ID";
				elementvalue2 = "Pharmacy_ID";
				elementvalue3 = "Pharmacy_FullStock";

				std::cout << "Value 1: ";
				std::getline(std::cin, valueChoice1);
				std::cout << "Value 2: ";
				std::getline(std::cin, valueChoice2);
				std::cout << "Value 3: ";
				std::getline(std::cin, valueChoice3);

				tempHold = "INSERT INTO " + tableChoice + " (" + elementvalue1 + "," + elementvalue2 + "," + elementvalue3 + ") VALUES ('" + valueChoice1 + "','" + valueChoice2 + "','" + valueChoice3 + "')";
				sqlStatement = tempHold.c_str();

				rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);		//adds new row to specified table
				rc = sqlite3_step(query);
				sqlite3_finalize(query);
			}
			else if (tableChoice == "prescription")
			{
				elementvalue1 = "Mid_ID";
				elementvalue2 = "Patient_ID";
				elementvalue3 = "Prescrip_ID";
				elementvalue4 = "Prescrip_Name";
				elementvalue5 = "Prescrip_Dosage";
				elementvalue6 = "Prescrip_Instructions";

				std::cout << "Value 1: ";
				std::getline(std::cin, valueChoice1);
				std::cout << "Value 2: ";
				std::getline(std::cin, valueChoice2);
				std::cout << "Value 3: ";
				std::getline(std::cin, valueChoice3);
				std::cout << "Value 4: ";
				std::getline(std::cin, valueChoice4);
				std::cout << "Value 5: ";
				std::getline(std::cin, valueChoice5);
				std::cout << "Value 6: ";
				std::getline(std::cin, valueChoice6);

				tempHold = "INSERT INTO " + tableChoice + " (" + elementvalue1 + "," + elementvalue2 + "," + elementvalue3 + "," + elementvalue4 + "," + elementvalue5 + "," + elementvalue6 + ") VALUES ('" + valueChoice1 + "','" + valueChoice2 + "','" + valueChoice3 + "','" + valueChoice4 + "','" + valueChoice5 + "','" + valueChoice6 + "')";
				sqlStatement = tempHold.c_str();

				rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);		//adds new row to specified table
				rc = sqlite3_step(query);
				sqlite3_finalize(query);
			}

			std::cout << "\nRow has been added!" << std::endl;

		}
		else if (functionChoice == "delete" || functionChoice == "Delete" || functionChoice == "DELETE")		//delete a row from table
		{
			char yesNo;

			std::cout << "\nPlease select a Primary Key from " << tableChoice << std::endl;		//select row user wants to delete
			std::getline(std::cin, rowChoice);
			std::cout << " " << std::endl;

			tempHold = "Select * FROM " + tableChoice + " WHERE " + idChoice + "_ID = " + rowChoice;
			sqlStatement = tempHold.c_str();

			rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);
			rc = sqlite3_step(query);
			for (int i = 0; i < sqlite3_data_count(query); ++i)
			{
				std::cout << sqlite3_column_text(query, i) << " | ";				//prints the selected row to the screen
			}
			sqlite3_finalize(query);
			std::cout << " " << std::endl;

			std::cout << "Are you sure you want to delete this row? (y/n): ";			//makes certain user really wants to delete row
			std::cin >> yesNo;

			while (yesNo != 'y' && yesNo != 'Y' && yesNo != 'n' && yesNo != 'N')
			{
				std::cout << "Please enter either 'y' or 'n': ";
				std::cin >> yesNo;
			}

			if (yesNo == 'y' || yesNo == 'Y')
			{
				tempHold = "DELETE FROM " + tableChoice + " WHERE " + idChoice + "_ID = " + rowChoice;
				sqlStatement = tempHold.c_str();

				rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);		//deletes row from specified table
				rc = sqlite3_step(query);
				sqlite3_finalize(query);

				std::cout << "\nRow has been deleted!" << std::endl;
			}
		}
		else if (functionChoice == "view" || functionChoice == "View" || functionChoice == "VIEW")			//select row from table
		{
			std::cout << "\nPlease select a Primary Key from " << tableChoice << std::endl;
			std::getline(std::cin, rowChoice);
			std::cout << " " << std::endl;

			tempHold = "Select * FROM " + tableChoice + " WHERE " + idChoice + "_ID = " + rowChoice;
			sqlStatement = tempHold.c_str();

			rc = sqlite3_prepare_v2(mydb, sqlStatement, -1, &query, NULL);
			rc = sqlite3_step(query);
			for (int i = 0; i < sqlite3_data_count(query); ++i)
			{
				std::cout << sqlite3_column_text(query, i) << " | ";				//prints the selected row to the screen
			}
			sqlite3_finalize(query);
			std::cout << " " << std::endl;
		}
	}

};

int main()
{
	char menuYesNo = 'y';

	Database myDatabase;			//instanciating the Database class
	myDatabase.openDatabase();

	do
	{
		myDatabase.chooseFunction();
		myDatabase.viewAddUpdateDelete();			//perform any of the 4 operations
		std::cout << "\nWould you like to perform another operation? (y/n): ";
		std::cin >> menuYesNo;
		std::cin.ignore();				//ignore the newline so that the loop doesn't mess with further input
		
		while (menuYesNo != 'y' && menuYesNo != 'Y' && menuYesNo != 'n' && menuYesNo != 'N')
		{
			std::cout << "Please enter either 'y' or 'n': ";
			std::cin >> menuYesNo;
		}
		std::cout << " " << std::endl;
	} while (menuYesNo == 'y' || menuYesNo == 'Y');

	myDatabase.closeDatabase();			//closing the database before the program finishes

	return 0;
}