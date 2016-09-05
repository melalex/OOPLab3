#pragma once
#include <iostream>

struct Train
{
	Train(int _m_TrainNumber, char *_m_Destination, int _hour, int _minute);
	Train(int _m_TrainNumber);
	Train(Train const& copy);
	~Train();

	Train &operator=(Train &copy);

	friend std::ostream& operator<<(std::ostream& os, const Train &dt);

	int m_TrainNumber;
	char *m_Destination;

	struct Time {
		Time();
		Time(int _hour, int _minute);

		int hour;
		int minute;
	} m_DepartureTime;

	bool operator< (Train &other) const;
	bool operator> (Train &other) const;

	bool operator<= (Train &other) const;
	bool operator>= (Train &other) const;

	bool operator== (Train &other) const;
	bool operator!= (Train &other) const;
};