#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <unistd.h>
#include <algorithm>
#include <stdexcept>
//#include <windows.h>
using namespace std;

void clean();
// bool isSeatOccupied();

struct Movie
{
	string id;
	string name;
	string date;
	string time;
	double price;
};

struct Ticket
{
	int id;
	string movie_id;
	vector<int> seats;
	int quantity;
	double price;
};

class MovieList
{
private:
	map<string, Movie> movies;
	int next_id;

public:
	MovieList() : next_id(1)
	{
		srand(time(0));
		addMovie("Interstellar", 10.4, "May 14, 2023", "06:00 PM");
		addMovie("Inception", 12.09, "August 12, 2020", "08:00 AM");
		addMovie("The Dark Knight", 15.0, "September 20, 2009", "04:00 PM");
	}

	void addMovie(string name, double price, string date, string time)
	{
		Movie movie;
		stringstream ss;
		ss << rand();
		movie.id = ss.str();
		movie.name = name;
		movie.price = price;
		movie.date = date;
		movie.time = time;
		movies[movie.id] = movie;
		cout << "\n\t\tSucessfully added. ";
	}

	void editMovie(string id, string name, double price, string date, string time)
	{
		if (movies.find(id) != movies.end())
		{
			movies[id].name = name;
			movies[id].price = price;
			movies[id].date = date;
			movies[id].time = time;
			cout << "\n\t\tSucessfully edited. ";
		}
		else
		{
			cout << "\n\t\tMovie not found! ";
		}
	}

	void deleteMovie(string id)
	{
		if (movies.find(id) != movies.end())
		{
			movies.erase(id);
			cout << "\n\t\tSucessfully deleted. ";
		}
		else
		{
			cout << "\n\t\tMovie not found! ";
		}
	}

	void listMovies()
	{
		clean();
		cout << "\t\t============================================\n";
		cout << "\t\t|               MOVIE LIST                 |\n";
		cout << "\t\t============================================\n";
		for (map<string, Movie>::const_iterator g = movies.begin(); g != movies.end(); ++g)
		{
			cout << "\t\t--------------------------------------------\n";
			cout << "\t\tMovie: " << g->second.name << "\n\t\tID: " << g->second.id << "\n\t\tPrice: " << g->second.price << "\n\t\tDate: " << g->second.date << "\n\t\tTime: " << g->second.time << '\n';
			cout << "\t\t--------------------------------------------\n";
		}
	}

	double getMoviePrice(string id)
	{
		if (movies.find(id) != movies.end())
		{
			return movies[id].price;
		}
		return -1;
	}

	bool isValidMovieID(string id);

	string getMovieName(string id)
	{
		if (movies.find(id) != movies.end())
		{
			return movies[id].name;
		}
		return "";
	}
};

void printWelcomeMessage()
{
	clean();
	system("Color 0A");
	cout << "\t\t=========================================\n";
	cout << "\t\t|       Welcome to \"Filmatic\" by        |\n";
	cout << "\t\t|           Caigas & Gavina             |\n";
	cout << "\t\t=========================================\n\n";
	cout << "\t\tMENU:\n";
	cout << "\t\tA. Show Movies\n";
	cout << "\t\tB. Purchase Tickets\n";
	cout << "\t\tC. Check Sales\n";
	cout << "\t\tD. Exit\n\n";
	cout << "\t\tPlease enter your choice: ";
}

bool MovieList::isValidMovieID(string id)
{
	return movies.find(id) != movies.end();
}

class TicketList
{
private:
	map<int, Ticket> tickets;
	int next_id;
	MovieList &movieList;

public:
	TicketList(MovieList &ml) : next_id(1), movieList(ml) {}

	bool isSeatOccupied(string movie_id, int seat)
	{
		for (map<int, Ticket>::iterator g = tickets.begin(); g != tickets.end(); ++g)
		{
			if (g->second.movie_id == movie_id)
			{
				for (vector<int>::iterator seatg = g->second.seats.begin(); seatg != g->second.seats.end(); ++seatg)
				{
					if (*seatg == seat)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void addTicket(string movie_id, const vector<int> &seats, int quantity)
	{
		double price = movieList.getMoviePrice(movie_id);
		if (price != -1)
		{
			//
			for (vector<int>::const_iterator seatg = seats.begin(); seatg != seats.end(); ++seatg)
			{
				if (isSeatOccupied(movie_id, *seatg))
				{
					std::cout << "\n\t\tSeat " << *seatg << " is already occupied.\n";
					return;
				}
			}
			//
			Ticket ticket;
			ticket.id = next_id++;
			ticket.movie_id = movie_id;
			ticket.seats = seats;
			ticket.quantity = quantity;
			ticket.price = price * quantity;
			tickets[ticket.id] = ticket;
		}
		else
		{
			cout << "\n\t\tInvalid movie id\n";
		}
	}

	void listTickets()
	{
		cout << "\t\tTICKET BOUGHT: \n";
		cout << "\t\t";
		cout << string(77, '=') << "\n";
		cout << "\t\t| " << setw(15) << left << "Ticket ID"
			 << setw(17) << "Movie"
			 << setw(18) << "Seats"
			 << setw(17) << "Quantity"
			 << setw(6) << "Price"
			 << " |\n";
		cout << "\t\t" << string(77, '=') << "\n";

		for (map<int, Ticket>::const_iterator g = tickets.begin(); g != tickets.end(); ++g)
		{
			cout << "\t\t| " << setw(15) << left << g->second.id
				 << setw(17) << movieList.getMovieName(g->second.movie_id)
				 << "[";

			for (vector<int>::const_iterator seatg = g->second.seats.begin(); seatg != g->second.seats.end(); ++seatg)
			{
				cout << *seatg << "";
				if (seatg != g->second.seats.end() - 1)
				{
					cout << ", ";
				}
				else
				{
					cout << "";
				}
			}
			cout << setw(10) << "]"
				 << setw(17) << g->second.quantity
				 << setw(6) << g->second.price << " |\n";
		}

		cout << "\t\t" << string(77, '=') << "\n";
	}

	void totalSales()
	{
		double total = 0.0;
		if (tickets.empty())
		{
			cout << "\n\t\tNo sales yet";
		}
		else
		{
			clean();

			cout << "\t\t=================================================================\n";
			cout << "\t\t|                          SALES REPORT                         |\n";
			cout << "\t\t=================================================================\n";
			cout << "\t\t| MOVIE              | PRICE      | Quantity      | Total       |\n";
			cout << "\t\t-----------------------------------------------------------------\n";
			for (map<int, Ticket>::const_iterator g = tickets.begin(); g != tickets.end(); ++g)
			{
				//  cout << "\t\t| "<<movieList.getMovieName(g->second.movie_id)<<"           |   "<<g->second.price<<"       |   "<<g->second.quantity<< "           |   "<<g->second.quantity * g->second.price<<"        |\n";
				cout << "\t\t| " << setw(19) << left << movieList.getMovieName(g->second.movie_id) << "|   "
					 << setw(9) << left << g->second.price / g->second.quantity << "|   "
					 << setw(12) << left << g->second.quantity << "|   "
					 << setw(10) << left << g->second.price << "|\n";

				total += g->second.price;
			}

			cout << "\t\t-----------------------------------------------------------------\n";
			cout << "\t\t| Total sales:       |            |               |   " << setw(9) << total << " |\n";
			cout << "\t\t=================================================================\n";
		}
	}
};

void clean()
{
	system("cls");
}

bool tryAgain(string a)
{
	char __a;
	cout << "\n\t\t" << a << " Try again? [Y/N]: ";
	cin >> __a;

	if (__a == 'y' || __a == 'Y')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isValidDate(const string &s)
{
	string monthNames[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
	string monthAbbrNames[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sept", "Oct", "Nov", "Dec"};

	istringstream ss(s);
	string monthStr;
	int monthInt, day, year;
	char delim;

	if (s.find('/') != string::npos || s.find('-') != std::string::npos)
	{
		ss >> monthInt >> delim >> day >> delim >> year;
		return monthInt >= 1 && monthInt <= 12 && day >= 1 && day <= 31 && year >= 1900 && year <= 9999;
	}
	else
	{
		ss >> monthStr >> day;
		ss.ignore(); // ignore comma
		ss >> year;

		for (int i = 0; i < 12; i++)
		{
			if (monthStr == monthNames[i] || monthStr == monthAbbrNames[i])
			{
				return day >= 1 && day <= 31 && year >= 1900 && year <= 9999;
			}
		}
	}

	return false;
}

string convertDateFormat(const string &date)
{
	vector<string> monthNames;
	monthNames.push_back("January");
	monthNames.push_back("February");
	monthNames.push_back("March");
	monthNames.push_back("April");
	monthNames.push_back("May");
	monthNames.push_back("June");
	monthNames.push_back("July");
	monthNames.push_back("August");
	monthNames.push_back("September");
	monthNames.push_back("October");
	monthNames.push_back("November");
	monthNames.push_back("December");

	vector<string> monthAbbrNames;
	monthAbbrNames.push_back("Jan");
	monthAbbrNames.push_back("Feb");
	monthAbbrNames.push_back("Mar");
	monthAbbrNames.push_back("Apr");
	monthAbbrNames.push_back("May");
	monthAbbrNames.push_back("Jun");
	monthAbbrNames.push_back("Jul");
	monthAbbrNames.push_back("Aug");
	monthAbbrNames.push_back("Sept");
	monthAbbrNames.push_back("Oct");
	monthAbbrNames.push_back("Nov");
	monthAbbrNames.push_back("Dec");

	for (int i = 0; i < 12; ++i)
	{
		if (date.find(monthNames[i]) != string::npos)
		{
			return date;
		}
		if (date.find(monthAbbrNames[i]) != string::npos)
		{

			size_t start_pos = date.find(monthAbbrNames[i]);
			if (start_pos == string::npos)
				return date;
			string newDate = date;
			newDate.replace(start_pos, monthAbbrNames[i].length(), monthNames[i]);
			return newDate;
		}
	}

	istringstream ss(date);
	string monthStr, dayStr, yearStr;

	char delimiter = date.find('/') != string::npos ? '/' : '-';

	getline(ss, monthStr, delimiter);
	getline(ss, dayStr, delimiter);
	getline(ss, yearStr);

	int month, day, year;
	istringstream(monthStr) >> month;
	istringstream(dayStr) >> day;
	istringstream(yearStr) >> year;

	ostringstream oss;
	oss << monthNames[month - 1] << " " << day << ", " << year;

	return oss.str();
}

string getTime()
{
	string timeString;
	getline(cin, timeString);

	istringstream iss(timeString);
	int hours, minutes;
	string ampm;
	char colon;

	if (iss >> hours >> colon >> minutes >> ampm && colon == ':' &&
		(ampm == "AM" || ampm == "am" || ampm == "PM" || ampm == "pm") &&
		hours >= 0 && hours <= 12 && minutes >= 0 && minutes <= 59)
	{
		ostringstream oss;
		oss << setfill('0') << setw(2) << hours << ':' << setw(2) << minutes << ' ' << ampm;
		return oss.str();
	}
	else
	{
		return "Invalid";
	}
}

bool isSeatAlreadyEntered(const vector<int> &seats, int seat)
{
	return find(seats.begin(), seats.end(), seat) != seats.end();
}

int main()
{
	MovieList movieList;
	TicketList ticketList(movieList);
	char letchoose;
main_menu:
	try
	{
		printWelcomeMessage();
		cin >> letchoose;
		if (cin.fail())
		{
			throw invalid_argument("Invalid input.");
		}
		switch (letchoose)
		{
		case 'a':
		case 'A':
		{
			string id, name, date, time;
			double price;
			char y = true;
			int menu;
			do
			{
				try
				{
					movieList.listMovies();
					cout << "\n";
					cout << "\t\tPress '1' to Add Movie\n";
					cout << "\t\tPress '2' to Edit Movie\n";
					cout << "\t\tPress '3' to Delete Movie\n";
					cout << "\t\tPress '4' to Go back to the main menu\n";
					cout << "\n";
					cout << "\t\tEnter your choice: ";
					cin >> menu;

					if (cin.fail())
					{

						throw invalid_argument("Invalid input.");
					}
					switch (menu)
					{
					case 1:
					{
						// cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cin.ignore();
						cout << "\n\t\tEnter movie name: ";
						getline(cin, name);
						cout << "\t\tEnter movie price: ";
						cin >> price;
						cin.ignore();
						cout << "\t\tEnter movie date: ";
						getline(cin, date);
						if (!isValidDate(date))
						{
							bool tryme = tryAgain("Invalid date format.");
							if (tryme)
							{
								y = true;
							}
							else
							{
								cout << "\n\t\tGoodbye...";
								y = false;
							}
							break;
						}
						date = convertDateFormat(date);
						cout << "\t\tEnter movie time (HH:MM AM/PM): ";
						time = getTime();
						if (time == "Invalid")
						{
							bool tryme = tryAgain("Invalid time format.");
							if (tryme)
							{
								y = true;
							}
							else
							{
								cout << "\n\t\tGoodbye...";
								y = false;
							}
							break;
						}
						//  getline(cin, time);
						movieList.addMovie(name, price, date, time);
						cout << "refreshing....\n";
						sleep(2);
						break;
					}
					case 2:
					{
					    cin.ignore(1000, '\n');
						cout << "\n\t\tEnter movie id: ";
						getline(cin, id);
						while (!movieList.isValidMovieID(id))
						{
							cout << "\t\tInvalid id input. Please try again.\n";
							cout << "\n\t\tEnter movie id: ";
							cin >> id;
							cin.ignore(1000, '\n');
						}
						
						cout << "\t\tEnter new movie name: ";
						getline(cin, name);
						cout << "\t\tEnter new movie price: ";
						cin >> price;
						cin.ignore();
						cout << "\t\tEnter new movie date: ";
						getline(cin, date);
						if (!isValidDate(date))
						{
							bool tryme = tryAgain("Invalid date format.");
							if (tryme)
							{
								y = true;
							}
							else
							{
								cout << "\n\t\tGoodbye...";
								y = false;
							}
							break;
						}
						date = convertDateFormat(date);
						cout << "\t\tEnter new movie time (HH:MM AM/PM): ";
						time = getTime();
						if (time == "Invalid")
						{
							bool tryme = tryAgain("Invalid time format.");
							if (tryme)
							{
								y = true;
							}
							else
							{
								cout << "\n\t\tGoodbye...";
								y = false;
							}
							break;
						}
						movieList.editMovie(id, name, price, date, time);
						cout << "refreshing....\n";
						sleep(2);
						break;
					}
					case 3:
					{
						cin.ignore();
						cout << "\n\t\tEnter movie id: ";
						getline(cin, id);
						movieList.deleteMovie(id);
						cout << "refreshing....\n";
						sleep(2);
						break;
					}
					case 4:
					{
						goto main_menu;
						break;
					}
					default:
					{
						cout << "\n\t\tInvalid choice, try again.";
					}
					}
				}
				catch (const invalid_argument &e)
				{
					cin.clear();
					cin.ignore(1000, '\n');
					cerr << "\n\t\tCaught exception: " << e.what() << "\n";
					cout << "\t\tPlease try again.\n";
					sleep(1);
				}
			} while (y == true);
			break;
		}
		case 'b':
		case 'B':
		{
			cin.ignore();
			string movie_id;
			vector<int> seats;
			int quantity, attempts = 0;
			string any_key;
			do
			{
				cout << "\n\t\tEnter movie id: ";
				getline(cin, movie_id);

				if (movieList.isValidMovieID(movie_id))
				{
					break;
				}
				else
				{
					cout << "\t\tInvalid movie id, try again.\n";
					attempts++;
				}
			} while (attempts < 3);

			if (attempts == 3)
			{
				cout << "\t\tToo many invalid attempts, returning to main menu.\n";
				sleep(1);
				goto main_menu;
			}
			//	cin.ignore();
			cout << "\t\tEnter number of seats: ";
			cin >> quantity;
			cin.ignore();
		nund:
			try
			{
				for (int i = 0; i < quantity; ++i)
				{
					int seat;
					cout << "\t\tEnter seat " << i + 1 << ": ";
					cin >> seat;
					if (cin.fail())
					{
						cin.clear();
						cin.ignore();
						seats.clear();
						throw invalid_argument("Invalid input.");
					}
					else if (isSeatAlreadyEntered(seats, seat))
					{
						//   seats.clear();
						cout << "\n\t\tSeat is already entered!\n";
						--i;
					}
					else if (ticketList.isSeatOccupied(movie_id, seat))
					{
						cout << "\n\t\tSeat is already occupied.\n";
						// seats.clear();
						--i;
						// goto nund;
					}
					else
					{
						if (seat <= 0)
						{
							cout << "Invalid seat number. Please try again.\n";
							--i;
						}
						else
						{
							seats.push_back(seat);
						}
					}
				}
				ticketList.addTicket(movie_id, seats, quantity);
				cout << "\n";
				ticketList.listTickets();

				cout << "\n\t\tThank you for purchasing. Enter anything to go back: ";
				cin >> any_key;
				goto main_menu;
			}
			catch (const invalid_argument &e)
			{
				cerr << "\n\t\tCaught exception: " << e.what() << "\n";
				cout << "\t\tPlease try again.\n";
				goto nund;
			}
			break;
		}
		case 'c':
		case 'C':
		{
			char __w;
			ticketList.totalSales();
			cout << "\n\t\tWanna go back to main menu? [Y/N]: ";
			cin >> __w;
			if (__w == 'y' || __w == 'Y')
			{
				goto main_menu;
			}
			break;
		}
		case 'd':
		case 'D':
		{
			cout << "\n\t\tGoodbye...";
			break;
		}
		default:
		{
			cin.ignore(1000, '\n');
			bool tryme = tryAgain("You entered an invalid keyword,");
			if (tryme)
			{
				goto main_menu;
			}
			else
			{
				cout << "\n\t\tGoodbye...";
			}
		}
		}
	}
	catch (const char *msg)
	{
		cerr << "Error: " << msg << "\n";
	}

	return 0;
}
