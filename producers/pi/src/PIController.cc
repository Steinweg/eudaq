#include "PIController.h"
#include <iostream>

using namespace std;

PI_Controller::PI_Controller(int portNr, int newBaudRate, char* szAxis){ 
	setPortNumber(portNr); setBaudRate(newBaudRate); setSzAxis(szAxis); 
	//ConnectRS232(); 
	ReferenceIfNeeded();
}

PI_Controller::PI_Controller(){ 
	std::cout << "please enter port number and Baud Rate of your controller.";
	}

PI_Controller::~PI_Controller(){}

int PI_Controller::ConnectRS232(){
	setID(PI_ConnectRS232(getPortNumber(), getBaudRate()));
	return getID();
}

bool PI_Controller::ReferenceIfNeeded(){
	BOOL flag = true;
	if (PI_SVO(getID(), getSzAxis(), &flag) == FALSE)
	{
		//CloseConnectionWithComment(iD, "qIDN failed. Exiting.\n");
		return FALSE;
	}
	BOOL bReferenced;
	BOOL bFlag;
	if (!PI_qFRF(getID(), getSzAxis(), &bReferenced))
		return false;
	if (!bReferenced){
		// if needed,
		// reference the axis using the refence switch
		//printf("Referencing axis %s...\n", axis);
		if (!PI_FPL(getID(), getSzAxis())){ return false; }

		// Wait until the reference move is done.
		bFlag = false;
		while (bFlag != TRUE){
			if (!PI_IsControllerReady(getID(), &bFlag))
				return false;
		}
	}
	return true;
}

bool PI_Controller::Move(const double position){
	if (!PI_MVE(getID(), getSzAxis(), &position)){
		return false;
	}
	else{
		return true;
	}
}

int PI_Controller::MoveServeralSteps(const std::vector<double> positions){
	for (unsigned int i = 0; i < positions.size(); i++){
		if (positions[i] < 0 || positions[i] > 102){
			return i;
		}
		else{
			Move(positions[i]);
			Sleep(10000);
		}
	}
	return -1;

}


/*doesn't work yet... don't know why*/
bool PI_Controller::isMoving(){
	BOOL pbValue = TRUE;
	return PI_IsMoving(getID(),getSzAxis(),&pbValue);
}

void pause(int miliseconds){
	Sleep(miliseconds);
}