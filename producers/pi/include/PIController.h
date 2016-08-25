#include "PI_GCS2_DLL.h"

//#ifndef WIN32
#include <windows.h>
#include <vector>
//#endif


#ifndef PI_Controller_H
#define PI_Controller_H

using namespace std;

//#define

class PI_Controller{
public:
	/*-------------------------------Constructors--------------------------------*/

		PI_Controller(int PortNr,int baudRate,char* axis);
		PI_Controller();
		~PI_Controller();
	
	/*------------------------functions for initialising the controller------------------------*/

	/*to connect the C-863 via RS232, returns iD or some value <0 if failed. for BaudRate check manuel of the controller*/
	int ConnectRS232();
	/*reference the PI stage if needed*/
	bool ReferenceIfNeeded();

	/*--------------------------functions for configuring the PI stage----------------------*/

	/*move the stage to a certain position between 0 and 102*/
	bool Move(const double position);

	int MoveServeralSteps(const std::vector<double> positions);

	bool isMoving();

	void pause(int miliseconds);

	/*---------------------getter and setter functions----------------*/
	
	inline int getID(){
		return iD;
	}

	inline void setID(const int newID){
		iD = newID;
	}

	inline int getBaudRate(){
		return baudRate;
	}

	inline void setBaudRate(const int newBaudRate){
		baudRate = newBaudRate;
	}

	inline int getPortNumber(){
		return portNr;
	}

	inline void setPortNumber(const int portNumber){
		portNr = portNumber;
	}
	
	inline char* getSzAxis(){
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
#endif