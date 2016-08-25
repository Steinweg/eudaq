#include <vector>
#include <string>
#include <array>
#include "PIController.h"

#ifndef SIMPLEPARSER_H
#define SIMPLEPARSER_H

class SimpleParser{
public:
	/*constructors, destructor*/
	SimpleParser(int baudRate, int portnumber, char *szaxis = "1", int numberDevices = 1);
	~SimpleParser();
	/*options for the parser*/
	const std::string info = "-i";
	const std::string help = "-h";
	const std::string move = "-m";
	const std::string position = "-p";
	const std::string baudrate = "-b";
	const std::string delet = "-d";
	const std::string options[7] = {};
	void getHelp();
	//inline std::array<std::string, 4> getControllerNames(){
		//return controllerNames;
	//}

private:
	/*create controller returns false if controller is allredy existend, or name or parameters are invalit*/
	bool createController(std::string, int portnum=0, int baudr=0, char* szaxis = "1");
	/*delete controller returns false if controller is not existent*/
	bool deleteController(std::string);
	std::string controllerNames[4] = {};
	PI_Controller controllers[4] = {};
	/*needed to find PIcontroller*/
	int findController(std::string);
};

#endif