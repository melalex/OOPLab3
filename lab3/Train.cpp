#define _CRT_SECURE_NO_WARNINGS

#include "Train.h"

#pragma region Train

Train::Train(int _m_TrainNumber, char *_m_Destination, int _hour, int _minute)
	: m_TrainNumber(_m_TrainNumber), m_Destination(_m_Destination), m_DepartureTime(_hour, _minute)
{}

Train::Train(int _m_TrainNumber) : m_TrainNumber(_m_TrainNumber), m_Destination(nullptr), m_DepartureTime()
{}

Train::Train(Train const& copy)
{
	m_TrainNumber = copy.m_TrainNumber;
	m_DepartureTime = copy.m_DepartureTime;

	m_Destination = new char[strlen(copy.m_Destination) + 1];

	strcpy(m_Destination, copy.m_Destination);
}


Train::~Train()
{
	delete[] m_Destination;
}

//----------------------------------------------------------

Train &Train::operator=(Train &copy)
{
	m_TrainNumber = copy.m_TrainNumber;
	m_DepartureTime = copy.m_DepartureTime;

	delete[] m_Destination;

	m_Destination = new char[strlen(copy.m_Destination) + 1];

	strcpy(m_Destination, copy.m_Destination);

	return *this;
}

//----------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const Train &dt)
{
	os << "Train number " << dt.m_TrainNumber << " to " << dt.m_Destination
		<< " on " << dt.m_DepartureTime.hour << ":" << dt.m_DepartureTime.minute << std::endl;

	return os;
}

//----------------------------------------------------------

bool Train::operator< (Train &other) const
{
	return m_TrainNumber < other.m_TrainNumber;
}

bool Train::operator>(Train &other) const
{
	return m_TrainNumber > other.m_TrainNumber;
}


bool Train::operator<= (Train &other) const
{
	return m_TrainNumber <= other.m_TrainNumber;
}


bool Train::operator>= (Train &other) const
{
	return m_TrainNumber >= other.m_TrainNumber;
}


bool Train::operator== (Train &other) const
{
	return m_TrainNumber == other.m_TrainNumber;
}


bool Train::operator!= (Train &other) const
{
	return m_TrainNumber != other.m_TrainNumber;
}

#pragma endregion

Train::Time::Time(int _hour, int _minute) : hour(_hour), minute(_minute)
{}

Train::Time::Time() : hour(0), minute(0)
{}

