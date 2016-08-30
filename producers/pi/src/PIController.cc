#include "PIController.h"
#include <iostream>
#include <string>

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

int PI_Controller::connectRS232(){
	setID(PI_ConnectRS232(getPortNumber(), getBaudRate()));
	return getID();
}

bool PI_Controller::ReferenceIfNeeded(){
	BOOL flag = true;
	if (PI_SVO(getID(), getSzAxis(), &flag) == FALSE)
	{
		return false;
	}
	BOOL bReferenced;
	BOOL bFlag;
	if (!PI_qFRF(getID(), getSzAxis(), &bReferenced)){ return false; }
	if (!bReferenced){
		// if needed,
		// reference the axis using the refence switch
		if (!PI_FNL(getID(), getSzAxis())){ return FALSE; }
		// Wait until the reference move is done.
		bFlag = FALSE;
		while (bFlag != TRUE){
			if (!PI_IsControllerReady(getID(), &bFlag))
				return false;
		}
	}
	return true;
}

int PI_Controller::connectTCPIP(const char* hostname){
	std::cout << hostname;
	int info = PI_ConnectTCPIP(hostname, getPortNumber());
	return info;
}

void PI_Controller::closeConnection(std::string comment){
	if (getID() < 0){
		if (comment != ""){
			std::cout << comment << std::endl;
		}
		reportError();
	}
	PI_CloseConnection(getID());
}

void PI_Controller::reportError()
{
	int err = PI_GetError(getID());
	char szErrMsg[300];
	if (PI_TranslateError(err, szErrMsg, 299))
	{
		printf("Error %d occured: %s\n", err, szErrMsg);
	}
}

/*not that the values this function returns so far don't realy make any sense...*/
double PI_Controller::getPosition(){
	double position[1];
	bool returnValue = PI_qPOS(getID(), getSzAxis(), position);
	if (!returnValue){
		std::cout << "couldn't find the controllers position." << endl;
		return -1;
	}
	else{ return position[0]; }
}

bool PI_Controller::Move(const double position){
	if (!PI_MVE(getID(), getSzAxis(), &position)){
		return false;
	}
	else{
		return true;
	}
}

int PI_Controller::MoveServeralSteps(const std::vector<double> positions, int pauseTime){
	for (unsigned int i = 0; i < positions.size(); i++){
		if (positions[i] < 0 ){
			return i;
		}
		else{
			Move(positions[i]);
			while (isMoving()){}
			pause(pauseTime);
		}
	}
	return -1;

}

bool PI_Controller::isMoving(){
	BOOL pbValue = TRUE;
	if (PI_IsMoving(getID(), getSzAxis(), &pbValue)){
		return pbValue;
	}
	else{
		std::cout << "Error in isMoving function in PIController class! function PI_IsMoving didn't return expected value(true)" << std::endl;
		return false;
	}
}

void PI_Controller::pause(int miliseconds){
	Sleep(miliseconds);
}
