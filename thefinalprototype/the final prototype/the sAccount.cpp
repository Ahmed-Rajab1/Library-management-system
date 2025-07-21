#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
using namespace std;

const int maxBooks = 100;	//the max number of books in the library
const int maxBorrowed = 5;	//the maximum number of borrowing books are allowed for the student
const int maxStudents = 100;
int bookCount = 0, studentCount = 0;
int user_id = 0;
/*	to know if the user is a student or an admin
	if (user_id = 0) then the user is the admin
	if (user_id != 0) then the user is a student	*/

struct Date {
	int day;
	int month;
	int year;
};

struct Book {
	string name;
	string edition;
	int id = 0;
	bool isBorrowed = 0; // if the book is borrowed  
	int borrowedBy = 0;  // ID of the student who borrowed that book  
	string author;
	Date publishDate = { 1, 1, 2000 }; // Initialize with default date (e.g., January 1, 2000)  
	string category;
	int pageCount = 0;
} books[maxBooks];	//array of books

struct Admin {
	string user_name;
	string password;
}admin;

struct Student {
	string name;
	int phone = 00000000000;
	int id = 0;
	int favBooks[maxBooks]{};	//array for favorite books is ID
	int favCount = 0;	//counter for favorite books
	int borrowedBooks[maxBorrowed]{};	//array for borrowed books is ID
	int borrowedCount = 0;	//counter for borrowed books
	string password;
}students[maxStudents];

void warning();
void loadBooksInfo();
void loadAdminsInfo();
void loadStudentsInfo();
void loadAll();
void saveBooks();
void saveAdmins();
void saveStudents();
void saveAll();
void DisplayFirstPage();
void studentLoginMenu();
void admin_login();
void logout();
void view_fav_list();
void View_Student_Personal_info_for_student();
void view_book_info(int book_id);
bool is_favorite(int book_id);
void search_by_book_id();
void student_home_page();
void admin_home_page();
void admin_edit_account();
void view_borrowed_books_for_admin();
void AddBook(int new_book_id);
void EditBook();
void DeleteBook();
void modify_order();
void viewBookList();
void displayStudentsList();
void search_for_student_by_ID();
void student_login();
void signup();
void student_edit_acconut();
void delete_account();
void view_student_info_for_admin(int stud_id);
void borrow_book();
void view_borrowed_books();
int  ValidInput(string OutputWhenErrorOccurs);	//Make sure user enters proper input.
string pass();

int main() {
	loadAll();
	//fisrt page
	DisplayFirstPage();
	return 0;
}

//load functions
void loadBooksInfo() {
	ifstream inFile("books.txt");
	if (!inFile.is_open()) {
		cout << "Failed to open BOOKS file\n";
		cout << "Check the file and try again\n";
		exit(0);
	}
	if (inFile.is_open()) {
		while (inFile.good() && bookCount < maxBooks) {
			// order of input data (name / author / category / id / borrowedBy / isBorrowed / pageCount / date)
			int id, borrowedBy, pageCount, day, month, year;
			bool isBorrowed;
			string name, author, category, edition;

			getline(inFile, name);
			getline(inFile, author);
			getline(inFile, category);
			getline(inFile, edition);
			inFile >> id;
			inFile >> borrowedBy;
			inFile >> isBorrowed;
			inFile >> pageCount;
			inFile >> day;
			inFile >> month;
			inFile >> year;
			inFile.ignore(); // Skip newline

			books[id - 1].name = name;
			books[id - 1].author = author;
			books[id - 1].category = category;
			books[id - 1].edition = edition;
			books[id - 1].id = id;
			books[id - 1].borrowedBy = borrowedBy;
			books[id - 1].isBorrowed = isBorrowed;
			books[id - 1].pageCount = pageCount;
			books[id - 1].publishDate.day = day;
			books[id - 1].publishDate.month = month;
			books[id - 1].publishDate.year = year;

			bookCount++;
		}
		inFile.close();
	}
	else  cout << "Failed to open BOOKS file." << endl;
}
void loadAdminsInfo() {
	ifstream inFile("admins.txt");

	if (inFile.is_open()) {
		getline(inFile, admin.user_name);
		getline(inFile, admin.password);
		inFile.close();
	}
	else  cout << "Failed to open ADMIN file." << endl;
}
void loadStudentsInfo() {
	ifstream inFile("students.txt");

	if (inFile.is_open()) {
		while (inFile.good() && studentCount < maxStudents) {
			int id, phone, favCount, borrowedCount;
			string name, password;

			inFile >> std::ws;
			getline(inFile, name);

			inFile >> std::ws;
			getline(inFile, password);

			inFile >> phone >> id >> favCount >> borrowedCount;

			students[studentCount].name = name;
			students[studentCount].phone = phone;
			students[studentCount].password = password;
			students[studentCount].id = id;
			students[studentCount].favCount = favCount;
			students[studentCount].borrowedCount = borrowedCount;

			for (int j = 0; j < favCount; ++j) {
				inFile >> students[studentCount].favBooks[j];
			}

			for (int j = 0; j < borrowedCount; ++j) {
				inFile >> students[studentCount].borrowedBooks[j];
			}

			inFile >> std::ws;
			studentCount++;
		}

		inFile.close();
	}
	else {
		cout << "Failed to open STUDENTS file." << endl;
	}
}
void loadAll() {
	loadBooksInfo();
	loadAdminsInfo();
	loadStudentsInfo();
}

//save functions
void saveBooks() {
	ofstream outFile("books.txt");

	for (int i = 0; i < bookCount; ++i) {
		if (books[i].id == 0) {
			continue;
		}
		outFile << books[i].name << "\n"
			<< books[i].author << "\n"
			<< books[i].category << "\n"
			<< books[i].edition << "\n"
			<< books[i].id << "\n"
			<< books[i].borrowedBy << "\n"
			<< books[i].isBorrowed << "\n"
			<< books[i].pageCount << "\n"
			<< books[i].publishDate.day << "\n"
			<< books[i].publishDate.month << "\n"
			<< books[i].publishDate.year;
		if (i < bookCount - 1) {
			outFile << "\n";
		}
	}
	outFile.close();
}
void saveAdmins() {
	ofstream outFile("admins.txt");
	outFile << admin.user_name << "\n" << admin.password;
	outFile.close();
}
void saveStudents() {
	ofstream outFile("students.txt");

	for (int i = 0; i < studentCount; ++i) {
		if (students[i].id == 0) {
			continue;
		}
		outFile << students[i].name << "\n"
			<< students[i].password << "\n"
			<< students[i].phone << "\n"
			<< students[i].id << "\n"
			<< students[i].favCount << "\n"
			<< students[i].borrowedCount;

		if (students[i].favCount > 0 || students[i].borrowedCount > 0) {
			outFile << "\n";
		}

		for (int j = 0; j < students[i].favCount; ++j) {
			outFile << students[i].favBooks[j];
			if (j < students[i].favCount - 1) {
				outFile << "\n";
			}
		}

		if (students[i].favCount > 0 && students[i].borrowedCount > 0) {
			outFile << "\n";
		}

		for (int j = 0; j < students[i].borrowedCount; ++j) {
			outFile << students[i].borrowedBooks[j];
			if (j < students[i].borrowedCount - 1) {
				outFile << "\n";
			}
		}

		if (i < studentCount - 1) {
			outFile << "\n";
		}
	}
	outFile.close();
}
void saveAll() {
	saveBooks();
	saveAdmins();
	saveStudents();
}

//function of view the first page
void DisplayFirstPage() {
	int choice;
	char confirm;
	cout << "=============================================================\n"
		<< "                       WELCOME TO YOUR                       \n"
		<< "                  LIBRARY MANAGEMENT SYSTEM                  \n"
		<< "=============================================================\n\n";

	//display the warning
	warning();

	while (true) {
		cout << "How will you use the system? \n"
			<< "1. Student\n"
			<< "2. Admin\n"
			<< "3. Exit\n\n"

			<< "Please enter your choice: ";
		choice = ValidInput("\nError!!\nPleace try again: ");

		switch (choice) {

		case 1:
			studentLoginMenu();
			break;

		case 2:
			admin_login();
			break;

		case 3:
			cout << "\nAre you sure you want to exit? (press 'y' to exit and any other button to continue): ";
			cin >> confirm;
			if (confirm == 'y' || confirm == 'Y') {
				cout << "\nThank you for using the Library Management System!\nHave a nice day :D\n";
				saveAll();
				exit(0);
			}
			else {
				continue;
			}
			break;

		default:
			cout << "\nInvalid choice\nPlease try again\n\n";
			break;
		}
	}
}

//function of student login menu
void studentLoginMenu() {
	int choice;

	while (true) {
		cout << "\n=============================================================\n"
			<< "                     Student access menu                     \n"
			<< "=============================================================\n\n"

			<< "The options that you have:\n"
			<< "1. Login\n"
			<< "2. Sign Up\n"
			<< "3. Back to Main Menu\n\n"
			<< "Enter your choice: ";
		choice = ValidInput("Error!!\nPleace try again: ");

		switch (choice) {

		case 1:
			student_login();
			break;

		case 2:
			signup();
			break;

		case 3:
			DisplayFirstPage();
			break;

		default:
			cout << "Invalid choice\nPlease try again\n\n";
			break;
		}
	}
}

//function of admin log in
void admin_login()
{
	cout << "\n============================================================\n";
	cout << "                     Log in as an admin                     \n";
	cout << "============================================================\n\n";
	string user_name;
	string password;
	char ch;	//to input the password as a stars
	int index = 0;

	do {
		cout << "Please enter the user name (Enter 0 to exit): ";
		cin >> user_name;
		if (user_name == "0") {
			DisplayFirstPage();
		}
		if (user_name != admin.user_name)
		{
			cout << "Wrong user name!\nplease try again\n";
		}
	} while (user_name != admin.user_name);

	do {
		password = "";        // To delete last enter password 

		cout << "Please enter the password: ";

		while (true) {

			ch = _getch();

			if (ch == '\r') break; // Enter
			if (ch == '\b')
			{
				if (index > 0)
					index--;
				password.pop_back();	//if the user press backspace the last letter will remove
				cout << "\b \b";

			}
			else
			{
				password += ch;
				index++;
				cout << "*";
			}

		}

		if (password != admin.password) {
			cout << "\nInvalid password\nplease re-enter the password\n";
		}

	} while (password != admin.password);

	cout << "\nlogin completed successfully\n";
	cout << "Welcome back admin :)\n";
	user_id = 0;
	admin_home_page();
}

//function of edit admin's account 
void admin_edit_account()
{
	string newpassword1;
	string newpassword2;
	string oldpassword;
	int i = 0;
	char ch;
	cout << "\n============================================================\n";
	cout << "                      Edit the account                      \n";
	cout << "============================================================\n\n";
	char cont;
	cout << "The edit options:\n";
	cout << "1. Edit the user name\n";
	cout << "2. Edit the password\n";
	cout << "3. Back to home page\n\n";
	while (true) {
		int choose;
		cout << "Enter the number of what you want to edit: ";
		choose = ValidInput("\nError!!\nPleace try again: ");

		//if the admin wnat to change his user name
		if (choose == 1) {
			cout << "Enter new user name: ";
			cin >> admin.user_name;
			cout << "User name updated successfully :)\n";
		}

		//if the admin wnat to change his password
		else if (choose == 2) {
			do {
				cout << "Enter your old password to create a new one : ";
				while (true) {

					ch = _getch();

					if (ch == '\r') break; // Enter
					if (ch == '\b')
					{
						if (i > 0)
							i--;
						oldpassword.pop_back();	//if the user press backspace the last letter will remove
						cout << "\b \b";

					}
					else
					{
						oldpassword += ch;
						i++;
						cout << "*";
					}

				}
				if (oldpassword == admin.password) {

					cout << "\nEnter the new password: ";
					cin >> newpassword1;
					while (true) {
						cout << "Please re-enter your new password to confirm: ";
						cin >> newpassword2;
						if (newpassword1 == newpassword2) {
							admin.password = newpassword1;
							break;
						}
						else {
							cout << "Error!\nYou enter the password wrong\nPleace try again\n";
						}
					}
				}
				else {
					cout << "\nInvalid old password !\n";

				}
				if (newpassword1.length() < 8) {
					cout << "Password is too short\nPlease enter at least 8 characters\n";
				}
				else {
					break;
				}

			} while (admin.password.length() < 8);
			cout << "Password updated successfully\n";
		}

		//if the user want to return to his home page
		else if (choose == 3) {
			admin_home_page();
		}

		else {
			cout << "Invalid choice\nTry another number\n";
			continue;
		}

		cout << "Do you want to do another edit?\n(Enter 'y' to edit again and any other button to exit): ";
		cin >> cont;
		if (cont == 'y' || cont == 'Y') {
			continue;
		}
		else {
			admin_home_page();
		}
	}
}

//function of log out
void logout()
{
	cout << "Are you sure you want to log out?\n(Enter 'y' to log out and any other button to no): ";
	char logout;
	cin >> logout;
	if (logout == 'y' || logout == 'Y') {
		cout << "\nLogout completed successfully :)\nSee you later\n";
		DisplayFirstPage();
	}
	else {
		if (user_id == 0) {
			admin_home_page();
		}
		else {
			student_home_page();
		}
	}
}

//function of view favorite books list
void view_fav_list() {
	cout << "\n============================================================\n";
	cout << "                ❤The favorites books' list❤                \n";
	cout << "============================================================\n\n";

	cout << "You have " << students[user_id].favCount << " book in your favorite books' list\n\n";

	//output the books in his favorite list
	for (int i = 0; i < students[user_id - 1].favCount; i++) {
		if (students[user_id - 1].favBooks[i] != 0) {
			cout << "[ " << books[students[user_id - 1].favBooks[i]].name << " ]\n(ID: " << students[user_id - 1].favBooks[i] << " )\n";
		}
	}

	cout << "\nThe options that you have:\n";
	cout << "1. Return to your home page\n";
	cout << "2. View an information of a book from your favorite books\n";
	cout << "3. Remove a book from your favorite books\n";

	//output the options and choose one of them
	while (true) {
		//choose an option
		int option;		//the variable of the number of the option that the user want
		cout << "\nEnter the number of the option you want to do: ";
		option = ValidInput("\nError!!\nPleace try again: ");

		//if the user choose return to his home page
		if (option == 1) {
			student_home_page();
		}

		//if the user choose to see a book is information
		else if (option == 2) {
			int id;
			while (true) {
				cout << "Enter the ID of the book you want to see information about (Enter 0 to exit): ";
				id = ValidInput("\nError!!\nPleace try again: ");
				if (id == 0) {
					break;
				}
				for (int i = 0; i < students[user_id - 1].favCount; i++) {
					if (students[user_id - 1].favBooks[i] == id) {
						view_book_info(id);
					}
				}
				cout << "There is no book with this ID in your favorites\nTry another number\n";
			}
			continue;
		}

		//if the user choose to remove a book from his favorite books
		else if (option == 3) {
			int id;
			while (true) {
				cout << "Enter the ID of the book you want to remove from your favorite list (Enter 0 to exit): ";
				id = ValidInput("\nError!!\nPleace try again: ");
				if (id == 0) {
					break;
				}
				for (int i = 0; i < students[user_id - 1].favCount; i++) {
					if (students[user_id - 1].favBooks[i] == id) {
						cout << "You Removed this book from your favorite books💔\n";
						students[user_id - 1].favBooks[i] = 0;

						//decrease number of favorite books
						students[user_id - 1].favCount--;

						//review favorite books list after the edit
						view_fav_list();
					}
				}
				cout << "There is no book with this ID in your favorites\nTry another number\n";
			}
			continue;
		}

		//If the user chooses a number that is not in the options
		else {
			cout << "This choice is not in the available options\n";
			cout << "Try another choice\n";
			continue;
		}
	}
}

//function of view student information
void View_Student_Personal_info_for_student() {
	cout << "\n============================================================\n";
	cout << "                 Your account's information                 \n";
	cout << "============================================================\n\n";
	cout << "Name: " << students[user_id - 1].name << endl;
	cout << "ID:    " << students[user_id - 1].id << "\n";
	cout << "Phone: " << students[user_id - 1].phone << "\n\n";
	if (students[user_id - 1].borrowedCount == 0) {
		cout << "You havn't borrowed any book\n";
	}
	else if (students[user_id - 1].borrowedCount == 1) {
		cout << "You have borrowed only one book\n";
	}
	else {
		cout << "You have borrowed " << students[user_id - 1].borrowedCount << " books\n";
	}

	if (students[user_id - 1].favCount == 0) {
		cout << "You don't have any book in your favorite list\n";
	}
	else if (students[user_id - 1].favCount == 1) {
		cout << "You have only one book in your favorite list\n";
	}
	else {
		cout << "You have " << students[user_id - 1].favCount << " books in your favorite list\n";
	}

	cout << "\nThe option that you have:\n";
	cout << "1. Return to Home Page\n";
	cout << "2. Edit account information\n";
	while (true) {
		int choice;
		cout << "\nEnter your choice: ";
		choice = ValidInput("\nError!!\nPleace try again: ");

		//if the student want to return to his home page
		if (choice == 1) {
			student_home_page();
		}

		//if the student want to edit his account
		else if (choice == 2) {
			student_edit_acconut();
		}
		else {
			cout << "Invalid input\nTry another number\n\n";
			continue;
		}
	}
}

//function of view the choosen book is information
void view_book_info(int book_id) {

	//if the user is a student check if this book is in his favorite list or not
	bool fav;	//it is out of the if statment because of the scope
	if (user_id != 0) {
		fav = is_favorite(book_id);
	}

	//output book information
	cout << "\n============================================================\n";
	cout << "                  The books is information                  \n";
	cout << "============================================================\n\n";
	cout << "~Book name: " << books[book_id - 1].name << endl;
	cout << "~Book ID: " << book_id << endl;
	cout << "~Author's name: " << books[book_id - 1].author << endl;
	cout << "~Book category: " << books[book_id - 1].category << endl;
	cout << "~Book Edition: " << books[book_id - 1].edition << endl;
	cout << "~Book publication date(y/m/d): " << books[book_id - 1].publishDate.year << '/' << books[book_id - 1].publishDate.month << '/' << books[book_id - 1].publishDate.day << endl;
	cout << "~Number of book pages: " << books[book_id - 1].pageCount << endl;

	//if the user is the admin
	if (user_id == 0) {

		//show if this book is borrowed or not
		if (books[book_id - 1].isBorrowed) {
			cout << "*This book is borrowed by " << students[books[book_id - 1].borrowedBy - 1].name << " and his ID is " << books[book_id - 1].borrowedBy << endl;
		}
		else {
			cout << "*This book is in your library.\n";
		}

	}

	//if the user is a student
	else {

		//show if this book is borrowed or not
		if (books[book_id - 1].isBorrowed) {
			if (user_id == books[book_id - 1].borrowedBy) {
				cout << "You borrowed this book :)\n";
			}
			else {
				cout << "This book has been borrowed\n";
			}
		}
		else {
			cout << "This book is available\n";
		}

		//show of this book is in favorite list or not
		if (fav) {
			cout << "This book is in your favorite list\n";
		}
		else {
			cout << "This book is not in your favorite list\n";
		}

	}

	//output the options and choose one of them
	cout << "\nThe options that you have:\n";
	cout << "1. Return to your home page\n";
	cout << "2. Return to books list\n";

	//if the user is a student
	if (user_id != 0) {

		//add or remove from favorite list options
		if (fav) {
			cout << "3. Remove this book from your favorite list\n";
		}
		else {
			cout << "3. Add this book to your favorite list\n";
		}

		//borrow or return this book option
		if (books[book_id - 1].borrowedBy == user_id) {
			cout << "4. Return this book\n";
		}

		if (!books[book_id - 1].isBorrowed) {
			cout << "4. borrowed this book\n";
		}
	}


	while (true) {
		cout << "Enter the number of the option that you want to do: ";
		int option = ValidInput("\nError!!\nPleace try again: ");
		if (user_id == 0) {
			//if the user choose return to his home page
			if (option == 1) {
				admin_home_page();
			}

			//if the user choose return to the books list
			else if (option == 2) {
				viewBookList();
			}

			else {
				cout << "Invalid input\nPleace try again\n";
				continue;
			}
		}
		else {
			//if the user choose return to his home page
			if (option == 1) {
				student_home_page();
			}

			//if the user choose return to the books list
			else if (option == 2) {
				viewBookList();
			}

			else if (option == 3) {
				if (fav) {
					//the student want to remove this book from his favorite list
					cout << "You Removed this book from your favorite books💔\n";

					//remove book ID from favorite books in student structure
					for (int i = 0; i < maxBooks; i++) {
						if (students[user_id - 1].favBooks[i] == book_id) {
							students[user_id - 1].favBooks[i] = 0;
							break;
						}
					}

					//decrease number of favorite books
					students[user_id - 1].favCount--;

					//Review book is information after the edit
					view_book_info(book_id);
				}
				else {
					//the student want to add this book to his favorite list
					cout << "You added this book to your favorite books❤\n";

					//add book ID to favorite books in student structure
					for (int i = 0; i < maxBooks; i++) {
						if (students[user_id - 1].favBooks[i] == 0) {
							students[user_id - 1].favBooks[i] = book_id;
							break;
						}
					}

					//increase number of favorite books
					students[user_id - 1].favCount++;

					//Review book is information after the edit
					view_book_info(book_id);
				}
			}

			else if (option == 4) {
				if (books[book_id - 1].borrowedBy == user_id) {
					//the student want to return this book
					cout << "You returned this book to the library successfully :)\n";

					//make this book unborrowed
					books[book_id - 1].isBorrowed = false;

					//Remove the ID of the student how return the book
					books[book_id - 1].borrowedBy = 0;

					//remove book id from borrowed books in student structure
					for (int i = 0; i < maxBorrowed; i++) {
						if (students[user_id - 1].borrowedBooks[i] == book_id) {
							students[user_id - 1].borrowedBooks[i] = 0;
							break;
						}
					}

					//decrease number of borrowed books
					students[user_id - 1].borrowedCount--;

					//Review book is information after the edit
					view_book_info(book_id);
				}

				else if (!books[book_id - 1].isBorrowed) {
					if (students[user_id - 1].borrowedCount < maxBorrowed) {
						//the student want to borrow this book and he have a place
						cout << "You borrowed this book from the library successfully\nhave a nice reading time :)\n";

						//make this book borrowed
						books[book_id - 1].isBorrowed = true;

						//but the id of the student who borrow teh book
						books[book_id - 1].borrowedBy = user_id;

						//add book id to borrowed books in student structure
						for (int i = 0; i < maxBorrowed; i++) {
							if (students[user_id - 1].borrowedBooks[i] == 0) {
								students[user_id - 1].borrowedBooks[i] = book_id;
								break;
							}
						}

						//increase number of borrowed books
						students[user_id - 1].borrowedCount++;

						//Review book is information after the edit
						view_book_info(book_id);
					}
					else {
						//The student has no place to borrow another book.
						cout << "You reached the maximum number of borrowed books\n";
						cout << "Pleace return a book if you want to borrowed another one\n";
						//Review the options
						continue;
					}
				}
				else if (books[book_id - 1].borrowedBy != user_id) {
					cout << "This choice is not in the available options\n";
					cout << "Try another choice\n";
					continue;
				}
			}
			else {
				cout << "This choice is not in the available options\n";
				cout << "Try another choice\n";
				continue;
			}
		}
	}
}

//this function check if this book is in the favorite list of this student or not
bool is_favorite(int book_id) {
	bool fav = false;
	for (int i = 0; i < maxBooks; i++) {
		if (students[user_id - 1].favBooks[i] == book_id) {
			fav = true;
			break;
		}
	}
	return fav;
}

//function of search for a book by ID
void search_by_book_id() {
	int id;		//the id of the book that the user looking for
	bool found = false;		//if this book found or not
	char again;		//if the user want to search again or no
	cout << "\n============================================================\n";
	cout << "                Search for a book by it's ID                \n";
	cout << "============================================================\n\n";
	do {
		found = false;
		while (true) {
			cout << "Enter the id of the book you are looking for (enter 0 to exit): ";
			id = ValidInput("\nError!!\nPleace try again: ");
			if (id == 0) {
				if (user_id == 0) {
					admin_home_page();
				}
				else {
					student_home_page();
				}
			}
			else if (id < 1 || id > maxBooks) {
				cout << "This number is out of library IDs range\ntry another number\n";
			}
			else {
				//search for this id
				for (int i = 0; i < maxBooks; i++) {
					//if this id is found and in use
					if (id == books[id - 1].id) {
						found = true;
						break;
					}
				}
				break;
			}
		}

		//if this book is exist
		if (found) {
			view_book_info(id);
		}

		//if this book is not exist
		else {
			cout << "soory!\nThere is no book with this id\n";
		}

		//if the user want to search again
		cout << "Do you want to search again (press 'y' to search again and ant other button to exit): ";
		cin >> again;
	} while (again == 'y' || again == 'Y');
	//if the user don't want to search again
	if (user_id == 0) {
		admin_home_page();
	}
	else {
		student_home_page();
	}
}

//function of student's home page
void student_home_page() {
	cout << "\n============================================================\n";
	cout << "                       Your home page                       \n";
	cout << "============================================================\n";

	cout << "\nThe option that you have:\n";
	cout << "1. View books list\n";
	cout << "2. View borrowed books and return a book\n";
	cout << "3. Borrow a book\n";
	cout << "4. View favorite books' list\n";
	cout << "5. Modify an order\n";
	cout << "6. Search for a book\n";
	cout << "7. View account's information\n";
	cout << "8. Edit your account\n";
	cout << "9. Log out\n";
	cout << "10. Delete your account\n";
	cout << "11. Close the program\n";

	while (true) {
		int option;
		cout << "\nEnter the number of the option that you want to do:";
		option = ValidInput("Error!!\nPleace try again: ");

		//if the student want to see books' list
		if (option == 1) {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0) {
					viewBookList();
				}
			}
			cout << "Sorry!\nThere is no books in the library\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the student want to see the books that he borrowed
		else if (option == 2) {
			if (students[user_id - 1].borrowedCount == 0) {
				cout << "Soory!\nYou don't have any borrowed books for now\n";
				cout << "Enter any number to continue: ";
				int num = ValidInput("\nError!!\nPleace try again: ");
				continue;
			}
			else {
				view_borrowed_books();
			}
		}

		//if the student want to borrowed a book
		else if (option == 3) {
			bool borrowed_place = false;
			bool is_there_available = false;
			if (students[user_id - 1].borrowedCount != maxBorrowed) {
				borrowed_place = true;
			}
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0 && !books[i].isBorrowed) {
					is_there_available = true;
				}
			}

			if (borrowed_place && is_there_available) {
				borrow_book();
			}
			else if (!borrowed_place && is_there_available) {
				cout << "Sorry!\nYou reached the maximum of borrowed books\nYou can't borrowed any other book\n";
				cout << "Enter any number to continue: ";
				int num = ValidInput("\nError!!\nPleace try again: ");
				continue;
			}
			else if (borrowed_place && !is_there_available) {
				cout << "Sorry!\nThere is no available books in the library\n";
				cout << "Enter any number to continue: ";
				int num = ValidInput("\nError!!\nPleace try again: ");
				continue;
			}
			else if (!borrowed_place && !is_there_available) {
				cout << "Sorroy!\nThere is no available books and you reached the maximum of borrowed books\n";
				cout << "Enter any number to continue: ";
				int num = ValidInput("\nError!!\nPleace try again: ");
				continue;
			}
		}

		//if the student want to see his favorite books
		else if (option == 4) {
			if (students[user_id - 1].favCount == 0) {
				cout << "Sorroy!\nThere is no books in your favorite list yet\n";
				cout << "Enter any number to continue: ";
				int num = ValidInput("\nError!!\nPleace try again: ");
				continue;
			}
			else {
				view_fav_list();
			}
		}

		//if the student want to modify an order
		else if (option == 5) {
			bool is_there_borrowed = false;
			bool is_there_available = false;
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0 && books[i].isBorrowed) {
					is_there_available = true;
					break;
				}
			}
			if (students[user_id - 1].borrowedCount > 0) {
				is_there_borrowed = true;
			}
			if (is_there_borrowed && is_there_available) {
				modify_order();
			}
			else if (is_there_borrowed && !is_there_available) {
				cout << "Sorry!\nThere is no available books to modify with\n";
				cout << "Enter any number to continue: ";
				int num = ValidInput("\nError!!\nPleace try again: ");
				continue;
			}
			else if (!is_there_borrowed && is_there_available) {
				cout << "Sorry!\nYou don't have borrowed any book to modify it\n";
				cout << "Enter any number to continue: ";
				int num = ValidInput("\nError!!\nPleace try again: ");
				continue;
			}
			else {
				cout << "Sorry!\nYou don't have borrowed any book to modify it and there is no available books to modify with\n";
				cout << "Enter any number to continue: ";
				int num = ValidInput("\nError!!\nPleace try again: ");
				continue;
			}
		}

		//if the student want to search for a book
		else if (option == 6) {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0) {
					search_by_book_id();
				}
			}
			cout << "Sorry!\nThere is no books to search for\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the student want to see his account is information
		else if (option == 7) {
			View_Student_Personal_info_for_student();
		}

		//if the student want to edit his account
		else if (option == 8) {
			student_edit_acconut();
		}

		//if the student want to log out
		else if (option == 9) {
			logout();
		}

		//if the student want to delete his account
		else if (option == 10) {
			delete_account();
		}

		//if the student want to close the program
		else if (option == 11) {
			char out;
			cout << "Are you sure that you want to exit. You will be logged out automatically\n(press 'y' to exit and any other button to no): ";
			cin >> out;
			if (out == 'y' || out == 'Y') {
				cout << "\nThank you for using the library management system\nHave a nice day :D\n";
				saveAll();
				exit(0);
			}
			else {
				continue;
			}
		}

		//if the student enter a wrong input
		else {
			cout << "This choice is not in the available options\n";
			cout << "Try another choice\n";
			continue;
		}
	}
}

//function of admin's home page
void admin_home_page() {
	cout << "\n===========================================================\n";
	cout << "                     Admin's home page                     \n";
	cout << "===========================================================\n";

	cout << "\nThe options that you have:\n";
	cout << "1. View books' list\n";
	cout << "2. View students' list\n";
	cout << "3. Add a new book\n";
	cout << "4. Edit a book's information\n";
	cout << "5. Delete a book\n";
	cout << "6. Search for a book by it's ID\n";
	cout << "7. Search for a student by it's ID\n";
	cout << "8. View borrowed books' list\n";
	cout << "9. Edit your account\n";
	cout << "10. Log out\n";
	cout << "11. Close the program\n";

	while (true) {
		int option;
		cout << "\nEnter the number of the option that you want to do: ";
		option = ValidInput("Error!!\nPleace try again: ");

		//if the admin choose to see books' list
		if (option == 1) {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0) {
					viewBookList();
				}
			}
			cout << "Soory!\nThere is no books in your library yet\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the admin want to see students' list
		else if (option == 2) {
			for (int i = 0; i < maxStudents; i++) {
				if (students[i].id != 0) {
					displayStudentsList();
				}
			}
			cout << "Sorry!\nThere is no students in the library for now\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the admin want to add a new book
		else if (option == 3) {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id == 0) {
					AddBook(i + 1);
				}
			}
			cout << "Sorry!\nThere is no place for the new book in the library\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the admin want to edit a book's information
		else if (option == 4) {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0) {
					EditBook();
				}
			}
			cout << "Sorry!\nThere is no books to edit in the library\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the admin want to delete a book
		else if (option == 5) {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0) {
					DeleteBook();
				}
			}
			cout << "Sorry!\nThere is no books to delete in the library\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the admin want to search for a book
		else if (option == 6) {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0) {
					search_by_book_id();
				}
			}
			cout << "Sorry!\nThere is no books to search for in the library\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the admin want to search for a student by his ID
		else if (option == 7) {
			for (int i = 0; i < maxStudents; i++) {
				if (students[i].id != 0) {
					search_for_student_by_ID();
				}
			}
			cout << "Sorry!\nThere is no students in the library for now\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the admin want to see the borrowed books' list
		else if (option == 8) {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0 && books[i].isBorrowed) {
					view_borrowed_books_for_admin();
				}
			}
			cout << "Sorry!\nThere is no borrowed books\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the admin want to edit his account
		else if (option == 9) {
			admin_edit_account();
		}

		//if the admin want to log out
		else if (option == 10) {
			logout();
		}

		//if the admin want to close the program
		else if (option == 11) {
			char out;
			cout << "Are you sure that you want to exit. You will be logged out automatically\n(press 'y' to exit and any other button to no): ";
			cin >> out;
			if (out == 'y' || out == 'Y') {
				cout << "\nThank you for using the library management system\nHave a nice day :D\n";
				saveAll();
				exit(0);
			}
			else {
				continue;
			}
		}

		//if the admin enter a wrong input
		else {
			cout << "This choice is not in the available options\n";
			cout << "Try another choice\n";
			continue;
		}
	}
}

//function of view borrowed books to the admin
void view_borrowed_books_for_admin()
{
	cout << "\n============================================================\n";
	cout << "                   List of borrowed books                   \n";
	cout << "============================================================\n\n";

	for (int i = 0; i < maxBooks; i++) {
		if (books[i].id != 0 && books[i].isBorrowed) {
			cout << "[ " << books[i].name << " ]  (ID: " << books[i].id << " )\n";
			cout << "~>Borrowed By " << students[books[i].borrowedBy - 1].name << " and his ID is " << books[i].borrowedBy << "\n\n";
		}
	}

	cout << "The options that you have:\n";
	cout << "1. view a book's information\n";
	cout << "2. Back to home page\n";
	while (true) {
		int num;
		cout << "Enter the number of the option that you want to do: ";
		num = ValidInput("\nError!!\nPleace try again: ");

		//if the admin want to see a book's information
		if (num == 1) {
			while (true) {
				int id;
				cout << "Enter the ID of the book you want to see it's information (Enter 0 to exit): ";
				id = ValidInput("\nError!!\nPleace try again: ");
				if (id != 0) {
					for (int i = 0; i < maxBooks; i++)
					{
						if (books[i].isBorrowed && books[i].id == id)
						{
							view_book_info(id);
						}
					}
					cout << "There is no borrowed book with this ID\nTry another ID\n";
				}
				else {
					break;
				}
			}
			continue;
		}

		//if the admin want to return to his home page
		else if (num == 2) {
			admin_home_page();
		}
		else {
			cout << "Invalid option\nTry another number\n";
			continue;
		}
	}
}

//function of add a new book only for the admin
void AddBook(int new_book_id)
{
	while (true)
	{
		char AnotherAddition;
		cout << "\n============================================================\n";
		cout << "                       Add a new book                       \n";
		cout << "============================================================\n\n";
		//From here We collect all the data about the book.
		cout << "Enter the book's name: ";
		cin.ignore();
		getline(cin, books[new_book_id - 1].name);

		cout << "Enter the book's edition: ";
		getline(cin, books[new_book_id - 1].edition);

		cout << "Enter the book's author: ";
		getline(cin, books[new_book_id - 1].author);

		cout << "Enter the book's category: ";
		getline(cin, books[new_book_id - 1].category);

		cout << "Enter the publish date:\n";
		//Conditions to make sure Publish date is valid.
		while (true)
		{
			cout << "Day (1-31) : ";
			books[new_book_id - 1].publishDate.day = ValidInput("Error!! Please enter a day from (1-31): "); //make sure the input is a number.

			if (books[new_book_id - 1].publishDate.day < 1 || books[new_book_id - 1].publishDate.day > 31)
				cout << "Error!! The publish day must be between (1-31)\n";
			else
				break;
		}

		while (true)
		{
			cout << "Month (1-12) : ";
			books[new_book_id - 1].publishDate.month = ValidInput("Error!! Please enter a month from (1-12): ");

			if (books[new_book_id - 1].publishDate.month < 1 || books[new_book_id - 1].publishDate.month > 12)
				cout << "Error!! The publish month must be between (1-12)\n";
			else
				break;
		}

		while (true)
		{
			cout << "Year : ";
			books[new_book_id - 1].publishDate.year = ValidInput("Error!! Please enter the year in numbers : ");

			if (books[new_book_id - 1].publishDate.year < 0 || books[new_book_id - 1].publishDate.year > 2025)
				cout << "Error!! This publish year is invalid\n";
			else
				break;
		}

		while (true)
		{
			cout << "Enter the page count: ";
			books[new_book_id - 1].pageCount = ValidInput("Error!! Invalid input , Please enter number of pages : ");

			if (books[new_book_id - 1].pageCount < 1)
				cout << "Error!! Page count can't be less than 1";
			else
				break;
		}
		cout << "The new book has been created successfully :)\n";
		cout << "The new book's id is " << new_book_id << endl;
		books[new_book_id - 1].id = new_book_id;
		books[new_book_id - 1].isBorrowed = false;
		books[new_book_id - 1].borrowedBy = 0;
		bookCount++;

		//Will user want to add another book?
		cout << "Do you want to add another book? ('y' for new book or Any other button for no): \n";
		cin >> AnotherAddition;

		if (AnotherAddition == 'y' || AnotherAddition == 'Y') {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id == 0) {
					AddBook(i + 1);
				}
			}
			cout << "Sorroy!\nThere is no more place in the library for a new book\n";
			cout << "Enter any number to back to your home page: ";
			int back;
			cin >> back;
			break;
		}
		else {
			break;
		}
	}
	admin_home_page();
}

//function of edit a book is information
void EditBook()
{
	while (true)
	{
		cout << "\n=============================================================\n";
		cout << "                         Edit a book                         \n";
		cout << "=============================================================\n\n";
		int ID;
		cout << "Enter the ID of the book that will be edited (enter 0 to exit): ";
		ID = ValidInput("Error!! Please try again: ");
		if (ID == 0) {
			admin_home_page();
		}
		if (ID < 1 || ID > maxBooks)        //check the book exists.
		{
			cout << "This ID is out of the library's ID range.\n";
			continue;
		}
		if (books[ID - 1].id == 0)      //If ID is zero this book isn't in use.
		{
			cout << "There is so book with this ID to edit\n";
			continue;
		}

		else
			while (true)
			{
				int Editchoice = 0; char AnotherEdit;
				cout << "Choose what you want to edit :\n";
				cout << "1. Edit Book's name\n2. Edit Book's edition\n3. Edit Book's author\n4. Edit Book's publish date\n5. Edit Book's category\n6. Edit Book's page count\n";
				Editchoice = ValidInput("Error!! Invalid value\n");
				if (Editchoice > 6 || Editchoice < 1)
				{
					cout << "Error!! The number you entered isn't from the choices given\n"; continue;
				}
				else if (Editchoice == 1)
				{
					cout << "Current book name: " << books[ID - 1].name << endl;
					cout << "Enter the new name: ";
					cin.ignore();
					getline(cin, books[ID - 1].name);

				}
				else if (Editchoice == 2)
				{
					cout << "Current Edition: " << books[ID - 1].edition << endl;
					cout << "Enter the new edition: ";
					cin.ignore();
					getline(cin, books[ID - 1].edition);
				}
				else if (Editchoice == 3)
				{
					cout << "Current Author: " << books[ID - 1].author << endl;
					cout << "Enter the new author: ";
					cin.ignore();
					getline(cin, books[ID - 1].author);
				}
				else if (Editchoice == 4)
				{
					cout << "Current Day : " << books[ID - 1].publishDate.day << endl;
					cout << "Current Month : " << books[ID - 1].publishDate.month << endl;
					cout << "Current Year : " << books[ID - 1].publishDate.year << "\n\n";
					cout << "Enter the new Publish Date:\n";
					while (true)
					{
						cout << "Day (1-31) : ";
						books[ID - 1].publishDate.day = ValidInput("Error!! Please enter a day from (1-31): ");
						if (books[ID - 1].publishDate.day < 1 || books[ID - 1].publishDate.day > 31)
							cout << "Error!! The publish day must be between (1-31)\n";
						else
							break;
					}

					while (true)
					{
						cout << "Month (1-12) : ";
						books[ID - 1].publishDate.month = ValidInput("Error!! Please enter a month from (1-12): ");
						if (books[ID - 1].publishDate.month < 1 || books[ID - 1].publishDate.month > 12)
							cout << "Error!! The publish month must be between (1-12)\n";
						else
							break;
					}

					while (true)
					{
						cout << "Year : ";
						books[ID - 1].publishDate.year = ValidInput("Error!! Please enter the year in numbers : ");
						if (books[ID - 1].publishDate.year < 0 || books[ID - 1].publishDate.year > 2025)
							cout << "Error!! This publish year is invalid\n";
						else
							break;
					}
				}
				else if (Editchoice == 5)
				{
					cout << "Current Category: " << books[ID - 1].category << endl;
					cout << "Enter the new category: ";
					cin.ignore();
					getline(cin, books[ID - 1].category);
				}
				else
				{
					cout << "Current Page count: " << books[ID - 1].pageCount << endl;
					cout << "Enter the new page count: ";
					books[ID - 1].pageCount = ValidInput("Error!! Invalid input , Please enter number of pages : ");
				}
				cout << "Do you want to edit anything else in this book? ('y' for new edit or Any other button for no more edits): ";
				cin >> AnotherEdit;
				if (AnotherEdit == 'y' || AnotherEdit == 'Y')
					continue;
				else
					break;
			}
		int other;
		cout << "Do you want to edit any other book ? ('y' for new edit or Any other button for no more edit): ";
		cin >> other;
		if (other == 'y' || other == 'Y') {
			continue;
		}
		else {
			break;
		}
	}
	admin_home_page();
}

//function of delete a book
void DeleteBook()
{
	while (true)
	{
		cout << "\n=============================================================\n";
		cout << "                        Delete a book                        \n";
		cout << "=============================================================\n\n";
		int ID;
		char YesorNo, AnotherDelete;
		cout << "Enter the book's ID that you want to delete: ";

		ID = ValidInput("Error!!\nPlease Try again: ");

		if (ID < 1 || ID > maxBooks)               //Check ID in range.
		{
			cout << "This ID is out of the library's ID range\n";
			continue;
		}

		if (books[ID - 1].id == 0)                //if ID equals zero it means there is no book in this index of the array.
		{
			cout << "This book doesn't exist\n";
			continue;
		}
		else
		{
			cout << "Book name : " << books[ID - 1].name << endl;
			cout << "Are you sure you want to delete this book? ('y' for yes or any other button for no): ";
			cin >> YesorNo;
			if (YesorNo == 'Y' || YesorNo == 'y')
			{
				books[ID - 1].id = 0;	//Setting the book's ID that will be deleted to zero.

				//Deleting the rest of the book's info.
				books[ID - 1].name.erase();
				books[ID - 1].edition.erase();
				books[ID - 1].isBorrowed = false;
				books[ID - 1].author.erase();
				books[ID - 1].borrowedBy = 0;
				books[ID - 1].publishDate.day = 0;
				books[ID - 1].publishDate.month = 0;
				books[ID - 1].publishDate.year = 0;
				books[ID - 1].category.erase();
				books[ID - 1].pageCount = 0;
				bookCount--;
				cout << "Book has been deleted successfully\n\n";

				cout << "Do you want to delete another book? ('y' for yes or any other button for no): ";
				cin >> AnotherDelete;

				if (AnotherDelete == 'Y' || AnotherDelete == 'y') {
					for (int i = 0; i < maxBooks; i++) {
						if (books[i].id != 0) {
							DeleteBook();
						}
					}
					cout << "Sorry!\nThere is no more books to delete in the library\n";
					cout << "Enter any number to back to your home page: ";
					int back;
					cin >> back;
					admin_home_page();
				}
				else
					break;

			}
			else
			{
				cout << "Do you want to delete another book? ('y' for yes or any other button for no): ";
				cin >> AnotherDelete;

				if (AnotherDelete == 'Y' || AnotherDelete == 'y') {
					for (int i = 0; i < maxBooks; i++) {
						if (books[i].id != 0) {
							DeleteBook();
						}
					}
					cout << "Sorry!\nThere is no more books in the library to delete\n";
					cout << "Enter any number to back to your home page: ";
					int num;
					cin >> num;
					break;
				}
				else
					break;
			}
		}
	}
	admin_home_page();
}

//function of modify an order
void modify_order() {
	char cont;
	int old_index = -1;	//old book is index in the borrowed books array
	int old_id;	//old book is ID
	while (true) {
		cout << "\n=============================================================\n";
		cout << "                       Modify an order                       \n";
		cout << "=============================================================\n\n";
		//view the borrowed books
		cout << "The books that you have borrowed:\n\n";
		for (int i = 0; i < maxBorrowed; i++) {
			if (students[user_id - 1].borrowedBooks[i] != 0) {
				cout << "[ " << books[students[user_id - 1].borrowedBooks[i] - 1].name << " ]\n(ID: " << students[user_id - 1].borrowedBooks[i] << " )\n";
			}
		}

		//choose a book to change
		while (true) {
			int id;
			cout << "\nEnter the ID of the book you want to change (Enter 0 to exit): ";
			id = ValidInput("\nError!!\nPleace try again: ");
			if (id == 0) {
				student_home_page();
			}
			else if (id < 1 || id > maxBooks) {
				cout << "This ID is out of library IDs rang\nTry another ID\n";
				continue;
			}
			else {
				for (int i = 0; i < maxBorrowed; i++) {
					if (students[user_id - 1].borrowedBooks[i] == id) {
						old_index = i;
						old_id = id;
					}
				}
				if (old_index == -1) {
					cout << "There is no borrowed book with this ID\nTry another ID\n";
					continue;
				}
				else {
					break;
				}
			}
		}

		//view the avaliable books
		cout << "\nThe avaliable books:\n";
		for (int i = 0; i < maxBooks; i++) {
			if (!books[i].isBorrowed && books[i].id != 0) {
				cout << "[ " << books[i].name << " ]\n(ID: " << books[i].id << " )\n";
			}
		}
		bool done = false;
		//choose a book to change with the old one
		while (true) {
			int choose;
			cout << "\nEnter the ID of the book you want to borrow: ";
			choose = ValidInput("\nError!!\nPleace try again: ");
			if (choose > maxBooks || choose < 1) {
				cout << "This ID is out of library IDs rang\nTry another ID\n";
			}
			else {
				for (int i = 0; i < maxBooks; i++) {
					if (books[i].id == choose) {
						//make the choosen book borrowed by the current student and the old one unborrowed
						students[user_id - 1].borrowedBooks[old_index] = choose;
						books[old_id - 1].isBorrowed = false;
						books[old_id - 1].borrowedBy = 0;
						books[choose - 1].isBorrowed = true;
						books[choose - 1].borrowedBy = user_id;
						done = true;
					}
				}
				if (done) {
					break;
				}
				else {
					cout << "The ID is not exist\nTry another ID\n";
					continue;
				}
			}
		}
		cout << "The modify has been done successfully :)\n";

		//check if the student want to modify another order
		cout << "Do you want to modify another order (press 'y' to modify and any other button to exit): ";
		cin >> cont;
		if (cont == 'y' || cont == 'Y') {
			continue;
		}
		else {
			break;
		}
	}
	student_home_page();
}

//function of view books list
void viewBookList() {
	cout << "\n============================================================\n";
	cout << "                         Books list                         \n";
	cout << "============================================================\n";
	for (int i = 0; i < maxBooks; ++i) {
		if (books[i].id != 0) {
			cout << "[ " << books[i].name << " ]\n(ID: " << books[i].id << " )  ";
			cout << "(" << (books[i].isBorrowed ? "borrowed" : "available") << ")\n\n";
		}
	}
	cout << "The options that you have:\n";
	cout << "1. Search for a book by his ID\n";
	cout << "2. View a book is information\n";
	cout << "3. Back to the home page\n";
	while (true) {
		int choose;
		cout << "Enter the number of the option you want to do: ";
		choose = ValidInput("\nError!!\nPleace try again: ");

		//if the user want to search for a book
		if (choose == 1) {
			for (int i = 0; i < maxBooks; i++) {
				if (books[i].id != 0) {
					search_by_book_id();
				}
			}
			cout << "Sorry!\nThere is no books to search for\n";
			cout << "Enter any number to continue: ";
			int num = ValidInput("\nError!!\nPleace try again: ");
			continue;
		}

		//if the user want to see a book is information
		else if (choose == 2) {
			while (true) {
				int id;
				cout << "Enter the ID of the book you want to see it's information: ";
				id = ValidInput("\nError!!\nPleace try again: ");
				for (int i = 0; i < maxBooks; i++) {
					if (books[i].id == id) {
						view_book_info(id);
					}
				}
				cout << "There is no book with this ID\npleace try again\n";
				continue;
			}
		}

		//if the user want to return to his home page
		else if (choose == 3) {
			if (user_id == 0) {
				admin_home_page();
			}
			else {
				student_home_page();
			}
		}
		else {
			cout << "This choice is not exist\nTry another number\n";
			continue;
		}
	}
}

//function of view students list
void displayStudentsList() {
	cout << "\n===========================================================\n";
	cout << "                       Students list                       \n";
	cout << "===========================================================\n\n";
	cout << "Total students: " << studentCount << "\n\n";
	for (int i = 0; i < maxStudents; i++) {
		if (students[i].id != 0) {
			cout << "[ " << students[i].name << " ] (ID: " << students[i].id << " )\n";
		}
	}
	cout << "\nThe options that you have:\n";
	cout << "1. View a student's information\n";
	cout << "2. search for a student by his ID\n";
	cout << "3. Back to the home page\n";

	while (true) {
		int choose;
		cout << "\nEnter the number of the option that you want to do: ";
		choose = ValidInput("\nError!!\nPleace try again: ");

		//if the admin want to see a student's information
		if (choose == 1) {
			while (true) {
				int id;
				cout << "Enter the ID of the student from the list: ";
				id = ValidInput("\nError!!\nPleace try again: ");
				for (int i = 0; i < maxStudents; i++) {
					if (students[i].id == id) {
						view_student_info_for_admin(id);
					}
				}
				cout << "There is no student with this ID\nPleace try again\n";
				continue;
			}
		}

		//if the admin want to search for a student
		else if (choose == 2) {
			search_for_student_by_ID();
		}

		//if the admin want to go to the home page
		else if (choose == 3) {
			admin_home_page();
		}
		else {
			cout << "This option number is not exist\nTry another number\n";
			continue;
		}
	}
}

//function of search for a student by his id
void search_for_student_by_ID()
{
	while (true)
	{
		cout << "\n============================================================\n";
		cout << "                    Search for a student                    \n";
		cout << "============================================================\n\n";
		int ID;
		char choice;
		cout << "Enter the student's ID you are searching for (Enter 0 to exit): ";
		ID = ValidInput("\nError!!\nPleace try again: ");
		if (ID == 0) {
			admin_home_page();
		}
		if (ID < 1 || ID > maxStudents)
		{
			cout << "Error!!\nthis id is out of range\n";
			continue;
		}
		for (int i = 0; i < maxStudents; i++)
		{
			if (students[i].id == ID)
			{
				view_student_info_for_admin(ID);
			}
		}
		cout << "Sorry!\nNo student found with this ID\n";
		cout << "Do you want to search for another student? ('y' for yes or any other button for no)\n";
		cin >> choice;
		if (choice == 'y' || choice == 'Y')
			continue;
		else
			admin_home_page();
	}
}

//function of student log in
void student_login() {
	char extra; // to dont let user enter characters after ID;
	int index{}; // index for the student place in array
	bool access = false;
	int ID;
	string password;
	char answer;
	cout << "\n============================================================\n";
	cout << "                       Student log in                       \n";
	cout << "============================================================\n\n";
	do {
		cout << "Enter your ID and password \n";
		cout << "ID: ";

		while (true) {
			cin >> ID;
			if (cin.fail() || ID > maxStudents || ID < 1) {
				cin.clear();
				cin.ignore(10000, '\n');

				cout << "Invalid input\nPlease try again\n";
				cout << "Enter your ID: ";
			}
			else if (cin.get(extra) && extra != '\n') {                 // for dont letting user enter another characters like : 123eds
				cin.ignore(10000, '\n');								// for dont letting user enter another characters like : 123eds
				cout << "Invalid input\nPlease enter only numbers\n";	// for dont letting user enter another characters like : 123eds
				cout << "Enter your ID: ";								// for dont letting user enter another characters like : 123eds
			}
			else
				break;

		}
		cout << "Password: ";
		password = pass();

		for (int i = 0; i < maxStudents; i++) {
			//searching for student name
			if (ID == students[i].id) {
				if (password == students[i].password) {
					access = true;
					index = i;
				}
			}
		}
		if (access)
			break;

		cout << "Wrong ID or password\nTry again? (Enter 'y' for yes and any other button for no): \n";
		cin >> answer;
		if (answer != 'y' && answer != 'Y') {
			DisplayFirstPage();
		}
	} while (answer == 'y' || answer == 'Y');

	cout << "Welcome back " << students[index].name << " :)\n";
	user_id = students[index].id;
	student_home_page();
}

//function of student sign up
void signup() {
	bool created = false;
	int index{};
	char extra; // to dont let user enter characters after ID;
	cout << "\n===========================================================\n";
	cout << "                      Student sign up                      \n";
	cout << "===========================================================\n";

	cout << "Enter your name, password and mobile number\n";
	for (int i = 0; i < maxStudents; i++) {
		if (students[i].id == 0) {
			cout << "Name: ";
			getline(cin, students[i].name); //getting the spaces
			while (students[i].name == "") {

				cout << "Invalid input\nPlease enter your name: ";
				getline(cin, students[i].name);
			}



			while (true) {

				cout << "password: ";
				getline(cin, students[i].password);


				if (students[i].password == "") {
					cin.clear();
					cin.ignore(10000, '\n');

					cout << "Invalid input\nPlease try again\n";
					cout << "Please enter your password: ";

				}
				else if (students[i].password.length() < 8) // the minimym size is 8
					cout << "Invalid password\nMust be at least 8 characters\n";
				else
					break;
			}


			cout << "mobile number: ";

			while (true) {
				cin >> students[i].phone;
				if (cin.fail() || students[i].phone > 9999999999 || students[i].phone < 999999999) {// (99999999999)11 numer for egyptian phone number
					cin.clear();
					cin.ignore(99999999999, '\n');

					cout << "Invalid input\nPlease enter only a mobile number (+20) egyptian country code\n";
					cout << "Enter your mobile number: ";
				}
				else if (cin.get(extra) && extra != '\n') {                 // for dont letting user enter another characters like : 123eds
					cin.ignore(10000, '\n');								// for dont letting user enter another characters like : 123eds
					cout << "Invalid input\nPlease enter only a mobile number (+20) egyptian country code\n";	// for dont letting user enter another characters like : 123eds
					cout << "Enter your mobile number: ";					// for dont letting user enter another characters like : 123eds
				}
				else
					break;

			}
			students[i].id = i + 1; // adding ID
			index = i; // saving the index
			created = true;
			break;
		}
	}
	if (created) {
		cout << "Welcome " << students[index].name << " :)\nYour ID is " << students[index].id << endl;
		cout << "Pleace save your ID because you should enter it when you log in\n";
		user_id = students[index].id;
		studentCount++;
		student_home_page();
	}
	else {
		cout << "Sorry!\nThe library is full of students :(\n";
		cout << "Enter any number to back: ";
		int num;
		num = ValidInput("Error!!\nPlease enter a number: ");
		DisplayFirstPage();
	}
}

//function of edit student's account
void student_edit_acconut()
{
	// the same logic of login:
	char extra;
	int index;
	bool access = false;
	int ID;
	string password;
	char answer;
	cout << "\n===========================================================\n";
	cout << "                     Edit your account                     \n";
	cout << "===========================================================\n\n";
	do {
		cout << "Enter your ID and password\n";
		cout << "ID: ";

		while (true) {
			cin >> ID;
			if (cin.fail() || ID > maxStudents || ID < 1) {
				cin.clear();
				cin.ignore(10000, '\n');

				cout << "Invalid input, please try again : \n";
				cout << "Enter your ID and password " << endl << "Choise : ";
			}
			else if (cin.get(extra) && extra != '\n') {                 // for dont letting user enter another characters like : 123eds
				cin.ignore(10000, '\n');								// for dont letting user enter another characters like : 123eds
				cout << "Invalid input\nPlease enter only numbers\n";	// for dont letting user enter another characters like : 123eds
				cout << "Enter your ID: ";								// for dont letting user enter another characters like : 123eds
			}
			else
				break;

		}
		cout << "Password: ";
		password = pass();

		for (int i = 0; i < maxStudents; i++) {
			//searching for student name
			if (ID == students[i].id) {
				if (password == students[i].password) {
					access = true;
					index = i;
				}
			}
		}
		if (access)
			break;

		cout << "Wrong ID or password\ntry again? (Enter 'y' for yes and any other button for no) : \n";
		cin >> answer;
	} while (answer == 'y' || answer == 'Y');

	if (access) {
		do {
			cout << " What do you want to edit ? \n" << "1. Name\n2. Password\n3. Mobile number\n4. Exit" << endl << "Choise :";// using switch case
			int input = ValidInput("Error!!\nPleace try again: ");
			if (input == 1) {
				cout << "Enter the new name\n";
				getline(cin, students[index].name);
				while (true) {

					if (students[index].name == "") {// checking if the name exists
						cout << "Name: ";
						getline(cin, students[index].name); //getting the spaces
						while (students[index].name == "") {

							cout << "Invalid input\nPlease enter your name: ";
							getline(cin, students[index].name);
						}
					}
					else
						break;
				}
				cout << "Edit saved!\n";
				break;
			}
			else if (input == 2) {
				cout << "Enter the new password\n";
				getline(cin, students[index].password);
				while (true) {

					if (students[index].password == "") {
						cout << "Password: ";
						getline(cin, students[index].password);
						while (students[index].password == "") {

							cout << "Invalid input\nPlease enter your Passward: ";
							getline(cin, students[index].password);

						}
					}
					else
						break;

				}
				cout << "Edit saved!\n";
				break;
			}
			else if (input == 3) {
				cout << "Enter your new mobile number: ";

				while (true) {
					cin >> students[index].phone;
					if (cin.fail() || students[index].phone > 99999999999 || students[index].phone < 1) {// (99999999999)11 numer for egyptian phone number
						cin.clear();
						cin.ignore(99999999999, '\n');

						cout << "Invalid input, please try again: \n";
						cout << "Enter your mobile number";
					}
					else if (cin.get(extra) && extra != '\n') {                 // for dont letting user enter another characters like : 123eds
						cin.ignore(10000, '\n');								 // for dont letting user enter another characters like : 123eds
						cout << "Invalid input\nPlease enter only numbers\n"; // for dont letting user enter another characters like : 123eds
						cout << "Enter your mobile number: ";					 // for dont letting user enter another characters like : 123eds
					}
					else
						break;

				}
				cout << "Edit saved!\n";
				break;
			}
			else if (input == 4) {
				student_home_page();
			}
			else {
				cout << "This is not in the options\nPleace try another number\n";
				continue;
			}

			cout << "Do you want to edit something else?\n(Enter 'y' for yes and any other button for no): ";
			cin >> answer;
		} while (answer == 'y' || answer == 'Y');
	}
	else {
		student_home_page();
	}
	student_home_page();
}

//function of delete student's account
void delete_account()
{ // same of login logic
	char extra;
	int index;
	bool access = false;
	int ID;
	string password;
	char answer;
	cout << "\n===========================================================\n";
	cout << "                    Delete your account                    \n";
	cout << "===========================================================\n\n";
	do {
		cout << "Enter your ID and password\n";
		cout << "ID: ";

		while (true) {
			cin >> ID;
			if (cin.fail() || ID > 10000 || ID < 1) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Invalid input\nPlease try again: \n";
			}
			else if (cin.get(extra) && extra != '\n') {                 // for dont letting user enter another characters like : 123eds
				cin.ignore(10000, '\n');								// for dont letting user enter another characters like : 123eds
				cout << "Invalid input\nplease enter only numbers: ";	// for dont letting user enter another characters like : 123eds
			}
			else
				break;

		}
		cout << "Password: ";
		password = pass();
		for (int i = 0; i < 10000; i++) {
			//searching for student name
			if (ID == students[i].id) {
				if (password == students[i].password) {
					access = true;
					index = i;
				}
			}
		}
		if (access)
			break;

		cout << "Wrong ID or password\ntry again? (Enter 'y' for yes and any other button for no): ";
		cin >> answer;
	} while (answer == 'y' || answer == 'Y');

	if (access) {
		cout << "Are you sure you want to delete your accout?\n(Enter 'y' for yes and any other button for no): ";
		cin >> answer;
		if (answer == 'y' || answer == 'Y')
		{// return the defult values
			students[index].name = "";
			students[index].password = "";
			students[index].id = 0;
			students[index].phone = 00000000000;

			cout << "Account deleted successfully!\nGood bye :(\n";
			studentCount--;
			DisplayFirstPage();
		}
		else
			student_home_page();
	}
	else {
		student_home_page();
	}
}

//function of view student's information for the admin
void view_student_info_for_admin(int stud_id) {
	int choice;
	cout << "\n=============================================================\n";
	cout << "                    Student's information                    \n";
	cout << "=============================================================\n\n";
	cout << "Name:  " << students[stud_id - 1].name << endl;
	cout << "ID:    " << students[stud_id - 1].id << "\n";
	cout << "Phone: " << students[stud_id - 1].phone << "\n\n";

	cout << "Borrowed books count: " << students[stud_id - 1].borrowedCount << "\n";
	if (students[stud_id - 1].borrowedCount != 0) {
		for (int i = 0; i < maxBorrowed; i++) {
			if (students[stud_id - 1].borrowedBooks[i] != 0) {
				cout << "[ " << books[students[stud_id - 1].borrowedBooks[i] - 1].name << " ]\n";
				cout << "(ID: " << students[stud_id - 1].borrowedBooks[i] << " )\n\n";
			}
		}
	}

	cout << "The options that you have:\n";
	cout << "1. Return to Home Page\n";
	cout << "2. Go to Students List\n";
	cout << "Enter your choice: ";

	while (true) {
		choice = ValidInput("Error!!\nPleace try again: ");

		//if the admin want to back to his home page
		if (choice == 1) {
			admin_home_page();
		}

		//if the admin want to go to the students' list
		else if (choice == 2) {
			displayStudentsList();
		}

		else {
			cout << "Invalid choice\nPlease try again\n\n";
			continue;
		}
	}
}

//function of borrow a book
void borrow_book()
{
	cout << "\n===========================================================\n";
	cout << "                       Borrow a book                       \n";
	cout << "===========================================================\n\n";

	cout << "The Available Books:\n";
	// List available books 
	for (int i = 0; i < bookCount; i++)
	{
		if (!books[i].isBorrowed && books[i].id != 0)
		{
			cout << "[ " << books[i].name << " ]\n(ID: " << books[i].id << " )\n\n";
		}
	}

	while (true)
	{
		// User options
		cout << "\nThe options that you have:\n";
		cout << "1. Borrow a Book\n";
		cout << "2. Return to Homepage\n";

		int choice;
		bool done = false;
		cout << "\nEnter the number of the option that you want to do: ";
		choice = ValidInput("\nError!!\nPleace try again: ");

		//if the user want to borrow a book
		if (choice == 1)
		{
			while (true) {
				bool borrowed_place = false;
				bool is_there_available = false;

				if (students[user_id - 1].borrowedCount != maxBorrowed) {
					borrowed_place = true;
				}

				for (int i = 0; i < maxBooks; i++) {
					if (books[i].id != 0 && !books[i].isBorrowed) {
						is_there_available = true;
					}
				}

				if (borrowed_place && is_there_available) {
					cout << "\nEnter ID of the book you want to borrow: ";
					int bookid;
					bookid = ValidInput("\nError!!\nPleace try again: ");

					if (bookid > maxBooks || bookid < 1) {
						cout << "This id is out of library IDs range\nPleace try again\n";
						continue;
					}
					else {

						for (int i = 0; i < maxBooks; i++) {
							if (books[i].id == bookid) {

								// Update book to be borrowed
								books[i].isBorrowed = true;
								books[i].borrowedBy = user_id;
								for (int j = 0; j < maxBorrowed; j++)
								{
									if (students[user_id - 1].borrowedBooks[j] == 0)
									{
										students[user_id - 1].borrowedBooks[j] = bookid;
										students[user_id - 1].borrowedCount++;
										break;
									}
								}
								done = true;

							}
						}

						if (done) {
							cout << "You borrowed this book successfully\nHave a nice read :)\n";
							cout << "\nDo you want to borrow another book? (Enter 'y' to borrow and any other button to no): ";
							char cont;
							cin >> cont;
							if (cont == 'y' || cont == 'Y') {
								bool borrowed_place = false;
								bool is_there_available = false;

								if (students[user_id - 1].borrowedCount != maxBorrowed) {
									borrowed_place = true;
								}

								for (int i = 0; i < maxBooks; i++) {
									if (books[i].id != 0 && !books[i].isBorrowed) {
										is_there_available = true;
									}
								}

								if (borrowed_place && is_there_available) {
									continue;
								}
								else if (!borrowed_place && is_there_available) {
									cout << "Sorry!\nYou reached the maximum of borrowed books\nYou can't borrowed any other book\n";
									cout << "Enter any number to continue: ";
									int num = ValidInput("\nError!!\nPleace try again: ");
									break;
								}
								else if (borrowed_place && !is_there_available) {
									cout << "Sorry!\nThere is no available books in the library\n";
									cout << "Enter any number to continue: ";
									int num = ValidInput("\nError!!\nPleace try again: ");
									break;
								}
								else if (!borrowed_place && !is_there_available) {
									cout << "Sorroy!\nThere is no available books and you reached the maximum of borrowed books\n";
									cout << "Enter any number to continue: ";
									int num = ValidInput("\nError!!\nPleace try again: ");
									break;
								}
							}
							else {
								break;
							}
						}
						else {
							cout << "Wrong ID\nPlace try again\n";
							continue;
						}

					}
				}
				else if (!borrowed_place && is_there_available) {
					cout << "Sorry!\nYou reached the maximum of borrowed books\nYou can't borrowed any other book\n";
					cout << "Enter any number to continue: ";
					int num = ValidInput("\nError!!\nPleace try again: ");
					break;
				}
				else if (borrowed_place && !is_there_available) {
					cout << "Sorry!\nThere is no available books in the library\n";
					cout << "Enter any number to continue: ";
					int num = ValidInput("\nError!!\nPleace try again: ");
					break;
				}
				else if (!borrowed_place && !is_there_available) {
					cout << "Sorroy!\nThere is no available books and you reached the maximum of borrowed books\n";
					cout << "Enter any number to continue: ";
					int num = ValidInput("\nError!!\nPleace try again: ");
					break;
				}
			}
		}

		//if the user want to back to his home page
		else if (choice == 2)
		{
			student_home_page();
		}
		else
		{
			cout << "Invalid choice\nPleace try again\n";
			continue;
		}
	}
}

//function of view borrowed books and return a book
void view_borrowed_books()
{
	cout << "\n===========================================================\n";
	cout << "                    Borrowed books list                    \n";
	cout << "===========================================================\n\n";

	cout << "Your Borrowed Books:\n";

	for (int i = 0; i < maxBorrowed; i++)
	{
		if (students[user_id - 1].borrowedBooks[i] != 0) {
			cout << "[ " << books[students[user_id - 1].borrowedBooks[i]].name << " ]\n";
			cout << "(ID: " << students[user_id - 1].borrowedBooks[i] << " )\n\n";
		}
	}

	cout << "The options that you have:\n";
	cout << "1. Return a Book\n";
	cout << "2. View a Book's Information\n";
	cout << "3. Return to your home page\n";

	while (true) {
		cout << "\nEnter the number of the option that you want to do: ";
		int choice = ValidInput("\nError!!\nPleace try again: ");

		if (choice == 1)
		{
			bool done = false;
			while (true) {
				cout << "Enter the Book ID to want to return: ";
				int bookid = ValidInput("\nError!!\nPleace try again: ");
				if (bookid > maxBooks || bookid < 1) {
					cout << "Error!\nThis ID is out of library IDs range\nPleace try again\n";
					continue;
				}
				else {
					for (int i = 0; i < maxBorrowed; i++)
					{
						if (students[user_id - 1].borrowedBooks[i] == bookid)
						{
							students[user_id - 1].borrowedBooks[i] = 0;
							students[user_id - 1].borrowedCount--;
							books[bookid - 1].isBorrowed = false;
							books[bookid - 1].borrowedBy = 0;
							cout << "You return this book successfully :)\n";
							cout << "Enter any number to continue: ";
							int num = ValidInput("\nError!!\nPleace try again: ");
							//re-view the list after the edit
							view_borrowed_books();
						}
					}
					cout << "Sorroy!\nThere is no book with this id\ntry again? (Enter 'y' for yes and any other book for no): ";
					char again;
					cin >> again;
					if (again == 'y' || again == 'Y') {
						continue;
					}
					else {
						break;
					}
				}
			}
		}
		else if (choice == 2)
		{
			while (true) {
				cout << "Enter the ID of the book that you want to see it's information: ";
				int id = ValidInput("\nError!!\nPleace try again: ");
				if (id > maxBooks || id < 1) {
					cout << "Error!\nThis ID is out of library IDs range\nPleace try again\n";
					continue;
				}
				else {
					for (int i = 0; i < maxBorrowed; i++) {
						if (students[user_id - 1].borrowedBooks[i] == id) {
							view_book_info(id);
						}
					}
					cout << "Sorry!\nThere is no book with this id\ntry again? (Enter 'y' for yes and any other book for no): ";
					char again;
					cin >> again;
					if (again == 'y' || again == 'Y') {
						continue;
					}
					else {
						break;
					}
				}
			}
		}
		else if (choice == 3)
		{
			student_home_page();
		}
		else
		{
			cout << "Invalid choice\nPleace try again\n";
			continue;
		}
	}
}

//creating hidden passwords
string pass()
{
	string password;
	char temp;
	while (true) {
		temp = _getch(); // for input char into string without Enter , (_ before getch beacouse the first one was in old compiler)
		if (temp == 13)
			// 13 is the ENTER button in ASCII code
			break;
		else if (temp == 8 && !password.empty()) // 8 is backspace in ascii code and .empty checks if string is empty
		{
			password.pop_back();// to delete the last characater of a string
			cout << "\b \b";	// to delete the last characater of a string

		}
		else if (temp != 8) {
			password += temp;// add new char to the string
			cout << "*";

		}



	}

	cout << endl;
	return password;

}

//Make sure user enters proper input
int ValidInput(string OutputWhenErrorOccurs)//Make sure user enters proper input. 
{
	char extra;
	int input;
	while (true) {
		if (!(cin >> input))
		{
			cin.clear();
			cin.ignore(1000, '\n');
			cout << OutputWhenErrorOccurs;
		}
		else if (cin.get(extra) && extra != '\n') {                 // for dont letting user enter another characters like : 123eds
			cin.ignore(10000, '\n');								// for dont letting user enter another characters like : 123eds
			cout << "Invalid input\nplease enter only numbers: ";	// for dont letting user enter another characters like : 123eds
		}
		else
			break;

	}
	
	return input;
}

//function of warning
void warning() {
	cout << "Imporant warning:\n";
	cout << "If you close the program suddenly or enter something wrong (such as entering text instead of numbers)\n";
	cout << "all your progress will be erased and what you have done since you opened the program will not be saved\n";
	cout << "If you want to save your progress, you must close the program by selecting the close option inside it\n";
	cout << "Thank you for reading the warning\n\n";
}