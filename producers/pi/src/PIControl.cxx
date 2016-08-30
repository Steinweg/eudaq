// StaticLoadSample_DataRecorder.cpp : 
// Sample program to demonstrate:

// 1. Usage of Lib file to link dll statically in MS VC++
//   - make sure the linker finds the PI_GCS2_DLL.lib
//   - make sure the PI_GCS2_DLL.dll is accessible at program launch
//   - Borland C++: use the implib.exe tool to generate the tlb file
// 2. Basic open loop and closed loop motion, referencing
// 3. Usage of the data recorder with asynchronous reading
//
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <string>
#include "eudaq/OptionParser.hh"
#include "PIController.h"
#include "eudaq/Utils.hh"




int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	int portnum;
	int BandRate;
	double positions[4];
	char *szAxis[] = { "0", "0", "0", "0", };
	char *specifiedAxis = "0";
	int sizeSzAxis = 4;
	char *hostname;

	eudaq::OptionParser op(
		"PI stage Control Utility", "1.1",
		"A comand-line tool for controlling the PI stage");
	eudaq::Option<int> baudrate(op, "b", "baudrate", 0, "baudrate",
		"The baudrate to get connection");
	eudaq::Option<int> portnumber(op, "p", "portnumber", 0, "portnumber",
		"The portnumber used for connecting the controller");
	eudaq::Option<std::string> hostnameOption(op, "ho", "hostname", "000.000.000", "hostname",
		"give a hostname to connect via TCP/IP");
	eudaq::Option<double> move1(op, "m1", "move1", -1, "moves stage on axis1 to position",
		"moves stage on axis1 to position, entered by user, -1 is the default in which case the stage isn't moved at all");
	eudaq::Option<double> move2(op, "m2", "move2", -1, "moves stage on axis2 to position",
		"moves stage on axis2 to position, entered by user, -1 is the default in which case the stage isn't moved at all");
	eudaq::Option<double> move3(op, "m3", "move3", -1, "moves stage on axis3 to position",
		"moves stage on axis3 to position, entered by user, -1 is the default in which case the stage isn't moved at all");
	eudaq::Option<double> move4(op, "m4", "move4", -1, "moves stage on axis4 to position",
		"moves stage on axis4 to position entered by user, -1 is the default in which case the stage isn't moved at all");
	eudaq::Option<std::vector<double> > moves(op, "m", "moves", "values", ";",
		"position vector, steps given will be moved to when szAxis is spezified");
	eudaq::Option<std::string> specifiedSzAxis(op, "sz", "szAxis", "-1", "hvuyiyviyvuyv", 
		"the axis you want to adress with the move command");

	try {
		op.Parse(argv);
		portnum = portnumber.Value();
		BandRate = baudrate.Value();
		hostname = const_cast<char*>(hostnameOption.Value().c_str());
		if (specifiedSzAxis.Value() != "-1"){
			specifiedAxis = const_cast<char*>(specifiedSzAxis.Value().c_str());
		}
		if (move1.Value() != -1){
			szAxis[1] = "1";
			positions[1] = move1.Value();
		}
		if (move2.Value() != -1){
			szAxis[2] = "2";
			positions[2] = move2.Value();
		}
		if (move3.Value() != -1){
			szAxis[3] = "3";
			positions[3] = move3.Value();
		}
		if (move4.Value() != -1){
			szAxis[4] = "4";
			positions[4] = move4.Value();
		}
		if (move1.Value() == 0 && move2.Value() == 0 && move3.Value() == 0 && move4.Value() == 0){
			std::cout << "error no controller acessed, set szaxis to true" << std::endl;
		}
	}
	catch (...) {
		return op.HandleMainException();
	}
	
	/*this is the poor attempt to make usb connection work... Doesn't work*/
//	char szBuffer[10000];
//	int buffersize = 10000;
//	char szFilter[6] = "C-867";
//	std::cout << PI_EnumerateUSB(szBuffer, buffersize, szFilter) << std::endl;
	
	if (specifiedAxis != "0"){
		PI_Controller controller(portnum, BandRate, specifiedAxis);
		std::cout << "connect to controller, iD: " << controller.connectRS232() << std::endl;
		if (controller.getID() > -1){
			if (!controller.ReferenceIfNeeded()){
				controller.closeConnection("Not referenced, Referencing failed.\n");
				return FALSE;
			}
			std::cout << "move controller: " << szAxis << "\n";
			if (!controller.MoveServeralSteps(moves.Value(), 1000)){
				controller.closeConnection("error occured while trying to move controller\n");
					return FALSE;
			}
			else{

			}
			controller.closeConnection();
		}
		else{
			std::cout << "couldn't connect to controller" << std::endl;
		}
	}

	for (int i = 0; i < sizeSzAxis; i++){
		if (szAxis[i] != "0"){
			PI_Controller controller(portnum, BandRate, szAxis[i]);

			/*attempt to make TCPIP work, doesn't work yet...*/
			//std::cout << hostname << std::endl;
			//int iD = controller.connectTCPIP(hostname);

			std::cout << controller.connectRS232() << "  that was the ID... Just if you maybe wondered..." << "\n";
			const BOOL flag = true;
			std::cout
				<< "move controller: " << szAxis[i] << "\n"
				<< "from position: " << controller.getPosition() << "\n"
				<< "to position: " << positions[i] << std::endl;
			if (controller.getID() > -1)
			{
				if (!controller.ReferenceIfNeeded())
				{
					controller.closeConnection("Not referenced, Referencing failed.\n");
					return FALSE;
				}
				std::cout 
					<< "move controller: " << szAxis[i] << "\n"
					<< "from position: " << controller.getPosition() << "\n"
					<< "to position: " << positions[i] << std::endl;
				if (!controller.Move(positions[i]))
				{
					controller.closeConnection("Not referenced, Referencing failed.In move function detected\n");
					return FALSE;
				}
				else{
					std::cout
						<< "is controller moving(1 is yes 0 no )? " << controller.isMoving()
						<< std::endl;
				}
				controller.closeConnection();
			}
			else
			{
				printf("Could not connect to E-871\n");
			}
		}
	}
	return 0;
}

