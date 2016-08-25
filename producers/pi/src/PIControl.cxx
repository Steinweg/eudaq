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

void ReportError(int iD)
{
	int err = PI_GetError(iD);
	char szErrMsg[300];
	if (PI_TranslateError(err, szErrMsg, 299))
	{
		printf("Error %d occured: %s\n", err, szErrMsg);
	}
}

void CloseConnectionWithComment(int iD, const char* comment)
{
	printf(comment);
	ReportError(iD);
	PI_CloseConnection(iD);
	_getch();
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	int portnum;
	int BandRate;
	double positions[4];
	char *szAxis[] = { "0", "0", "0", "0" };
	int sizeSzAxis = 4;
	eudaq::OptionParser op(
		"PI stage Control Utility", "1.1",
		"A comand-line tool for controlling the PI stage");
	eudaq::Option<int> baudrate(op, "b", "baudrate", 0, "baudrate",
		"The baudrate to get connection");
	eudaq::Option<int> portnumber(op, "p", "portnumber", 0, "portnumber",
		"The portnumber used for connecting the controller");

	/*questionably hard coded, but that way i wanted to make sure the user can only adress stages that exist*/
	eudaq::Option<double> move1(op, "m1", "move1", -1, "moves stage on axis1 to position",
		"moves stage on axis1 to position, entered by user, -1 is the default in which case the stage isn't moved at all");
	eudaq::Option<double> move2(op, "m2", "move2", -1, "moves stage on axis2 to position",
		"moves stage on axis2 to position, entered by user, -1 is the default in which case the stage isn't moved at all");
	eudaq::Option<double> move3(op, "m3", "move3", -1, "moves stage on axis3 to position",
		"moves stage on axis3 to position, entered by user, -1 is the default in which case the stage isn't moved at all");
	eudaq::Option<double> move4(op, "m4", "move4", -1, "moves stage on axis4 to position",
		"moves stage on axis4 to position entered by user, -1 is the default in which case the stage isn't moved at all");

	try {
		op.Parse(argv);
		portnum = portnumber.Value();
		BandRate = baudrate.Value();
		if (move1.Value() != -1){
			szAxis[0] = "1";
			positions[0] = move1.Value();
		}
		if (move2.Value() != -1){
			szAxis[1] = "2";
			positions[1] = move2.Value();
		}
		if (move3.Value() != -1){
			szAxis[2] = "3";
			positions[2] = move3.Value();
		}
		if (move4.Value() != -1){
			szAxis[3] = "4";
			positions[3] = move4.Value();
		}
		if (move1.Value() == 0 && move2.Value() == 0 && move3.Value() == 0 && move4.Value() == 0){
			std::cout << "error no controller acessed, set szaxis to true" << std::endl;
		}
		for (int i = 0; i < 4; i++){
			std::cout << szAxis[i] << std::endl;
		}
	}
	catch (...) {
		return op.HandleMainException();
	}
	char szBuffer[10000];
	int buffersize = 10000;
	char szFilter[6] = "C-867";
	std::cout << PI_EnumerateUSB(szBuffer, buffersize, szFilter) << std::endl;
	for (int i = 0; i < sizeSzAxis; i++){
		if (szAxis[i] != "0"){
			PI_Controller controller(portnum, BandRate, szAxis[i]);
			int iD = controller.ConnectRS232();
			std::cout << iD << "  that was the ID... Just if you maybe wondered..." << "\n";
			const BOOL flag = true;
			if (iD > -1)
			{
				if (!controller.ReferenceIfNeeded())
				{
					CloseConnectionWithComment(iD, "Not referenced, Referencing failed.\n");
					return FALSE;
				}
				if (!controller.Move(positions[i]))
				{
					CloseConnectionWithComment(iD, "Not referenced, Referencing failed.\n");
					return FALSE;
				}
				else{
					std::cout
						<< "move controller: " << szAxis[i] << "\n"
						<< "to position: " << positions[i]
						<< std::endl;
				}
				PI_CloseConnection(iD);
			}
			else
			{
				printf("Could not connect to E-871\n");
			}
		}
	}
	return 0;
}

