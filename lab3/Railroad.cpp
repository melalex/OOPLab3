#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <list> 
#include "BinTree.h"
#include "Train.h"
#define MAX_LENGTH 50

void get_Train(char *csv_line, BinTree<Train> &_trains)
{
	char buf[33], *_m_Destination = new char[33];
	int counter = 0, buf_iter = 0, csv_line_iter = 0, _m_TrainNumber, _hour, _minute;

	do{
		if (csv_line[csv_line_iter] == ':' || csv_line[csv_line_iter] == ';' || csv_line[csv_line_iter] == '\0')
		{
			buf[buf_iter] = '\0';

			switch (counter)
			{
				case 0:
					_m_TrainNumber = atoi(buf);
					break;

				case 1:
					strcpy(_m_Destination, buf);
					break;

				case 2:
					_hour = atoi(buf);
					break;

				case 3:
					_minute = atoi(buf);
					break;
			}

			csv_line_iter++;
			counter++;

			buf_iter = 0;
		}
		else
		{
			buf[buf_iter] = csv_line[csv_line_iter];
			csv_line_iter++;
			buf_iter++;
		}
	} while (csv_line[csv_line_iter - 1] != '\0');

	_trains.insert(Train(_m_TrainNumber, _m_Destination, _hour, _minute));
}

////////////////////////////////////////////////////////////

void _csv_to_BinTree(char *csv_name, BinTree<Train> &_trains)
{
	std::ifstream csvout(csv_name);

	char csv_line[MAX_LENGTH];
	csvout.getline(csv_line, MAX_LENGTH);

	while (csvout)
	{
		get_Train(csv_line, _trains);
		csvout.getline(csv_line, MAX_LENGTH);
	}

	csvout.close();
}

//----------------------------------------------------------

void print(BinTree<Train> &_trains) 
{
	for (auto it = _trains.begin(); it != _trains.null_iter(); ++it)
		std::cout << *it;
}

//----------------------------------------------------------

Train find(int key, const BinTree<Train> &_trains)
{
	return *_trains.find(Train(key));
}

//----------------------------------------------------------

void remove(int key, BinTree<Train> &_trains)
{
	_trains.remove(find(772, _trains));
}

//----------------------------------------------------------

std::list<Train> *trains_to_Destination(char *_m_Destination, BinTree<Train> &_trains)
{
	std::list<Train> *My_list = new std::list < Train > ;

	for (auto it = _trains.begin(); it != _trains.null_iter(); ++it)
		if (!_stricmp((*it).m_Destination, _m_Destination))
			My_list->push_back(*it);

	return My_list;
}

//----------------------------------------------------------

int main()
{
	BinTree<Train> _trains;
	_csv_to_BinTree("Trains.csv", _trains);
	print(_trains);

	std::cout << std::endl << find(772, _trains) << std::endl;

	std::list<Train> *to_Moscow = trains_to_Destination("Moscow", _trains);

	for (Train &x : *to_Moscow)
		std::cout << x;

	delete to_Moscow;

	std::cout << std::endl;

	remove(772, _trains);
	print(_trains);
	system("PAUSE");
	return 0;
}