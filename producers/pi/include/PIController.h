#include "PI_GCS2_DLL.h"

//#ifndef WIN32
#include <windows.h>
#include <vector>
//#endif


#ifndef PI_Controller_H
#define PI_Controller_H

//#define

class PI_Controller{
public:
	/*-------------------------------Constructors--------------------------------*/

		PI_Controller(int PortNr,int baudRate,char* axis);
		PI_Controller();
		~PI_Controller();
	
	/*------------------------functions for initialising the controller------------------------*/

	/*to connect the C-863 via RS232, returns iD or some value <0 if failed. for BaudRate check manuel of the controller*/
	int connectRS232();
	/*reference the PI stage if needed*/
	bool ReferenceIfNeeded();

	int connectTCPIP(const char* hostname);

	void closeConnection(std::string comment = "");

	void reportError();

	/*--------------------------functions for configuring the PI stage----------------------*/

	/*move the stage to a certain position between 0 and 102*/
	bool Move(const double position);

	int MoveServeralSteps(const std::vector<double> positions, int pauseTime);

	bool isMoving();

	double getPosition();

	void pause(int miliseconds);

	/*---------------------getter and setter functions----------------*/
	
	inline int getID() const{
		return iD;
	}

	inline void setID(const int newID){
		iD = newID;
	}

	inline int getBaudRate() const{
		return baudRate;
	}

	inline void setBaudRate(const int newBaudRate){
		baudRate = newBaudRate;
	}

	inline int getPortNumber() const{
		return portNr;
	}

	inline void setPortNumber(const int portNumber){
		portNr = portNumber;
	}
	
	inline char* getSzAxis() const{
		return szAxis;
	}

	inline void setSzAxis(char* axis){
		szAxis = axis;
	}

private:
	int baudRate;
	int iD;
	int portNr;
	char* szAxis;
};

/*
std::ostream& operator<<(std::ostream& os, const PI_Controller& controller) {
	return os << " baudRate: " << controller.getBaudRate() << " ID: " << controller.getID() << " portnumber: " << controller.getPortNumber() << std::endl;
	//<< " szAxis: " << controller.getSzAxis() << std::endl;
}
*/

#endif