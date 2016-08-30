#include "eudaq/Configuration.hh"
#include "eudaq/Producer.hh"
#include "eudaq/Logger.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Timer.hh"
#include "eudaq/Utils.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/ExampleHardware.hh"
#include "PIController.h"
#include <iostream>
#include <ostream>
#include <vector>

// A name to identify the raw data format of the events generated
// Modify this to something appropriate for your producer.
static const std::string EVENT_TYPE = "PI";

// Declare a new class that inherits from eudaq::Producer
class PIProducer : public eudaq::Producer {
public:

	// The constructor must call the eudaq::Producer constructor with the name
	// and the runcontrol connection string, and initialize any member variables.
	PIProducer(const std::string & name, const std::string & runcontrol)
		: eudaq::Producer("PI", runcontrol),
		m_run(0), m_ev(0), stopping(false), done(false), started(false) {}



	// This gets called whenever the DAQ is configured
	virtual void OnConfigure(const eudaq::Configuration & config) {
		std::cout << "Configuring: " << config.Name() << std::endl;

		int portnum;
		int BaudRate;
		double positions[4];
		char *szAxis[] = { "0", "0", "0", "0" };
		char *specifiedAxis = "0";
		int sizeSzAxis = 4;
		char *hostname;


		// copy from PIControl.cxx
		try {
			portnum = config.Get("Portnumber", 0);
			std::cout << "portnumber: " << portnum << std::endl;

			BaudRate = config.Get("Baudrate", 0);
			std::cout << "baudrate: " << BaudRate << std::endl;

			//hostname = config.Get("Hostname", "000.000.000.000");
			//std::cout << "Hostname: " << Hostname << std::endl;

			positions[0] = config.Get("move1", -1);
			std::cout << "move stage 1: " << positions[0] << std::endl;

			positions[1] = config.Get("move2", -1);
			std::cout << "move stage 2: " << positions[1] << std::endl;

			positions[2] = config.Get("move3", -1);
			std::cout << "move stage 3: " << positions[2] << std::endl;

			positions[3] = config.Get("move4", -1);
			std::cout << "move stage 4: " << positions[3] << std::endl;

			if (positions[0] != -1){
				szAxis[0] = "1";
			}
			if (positions[1] != -1){
				szAxis[1] = "2";
			}
			if (positions[2] != -1){
				szAxis[2] = "3";
			}
			if (positions[3] != -1){
				szAxis[3] = "4";
			}
			if (positions[0] == 0 && positions[1] == 0 && positions[2] == 0 && positions[3] == 0){
				std::cout << "error no controller acessed, set szaxis to true" << std::endl;
			}
		}
		catch (...) {
			printf("Unknown exception\n");
			SetConnectionState(eudaq::ConnectionState::STATE_ERROR, "Configuration Error");
		}



		// do

		for (int i = 0; i < sizeSzAxis; i++){
			if (szAxis[i] != "0"){
				PI_Controller controller(portnum, BaudRate, szAxis[i]);

				/*attempt to make TCPIP work, doesn't work yet...*/
				//std::cout << hostname << std::endl;
				//int iD = controller.connectTCPIP(hostname);

				std::cout << controller.connectRS232() << "  that was the ID... Just if you maybe wondered..." << "\n";
				if (controller.getID() > -1)
				{
					if (!controller.ReferenceIfNeeded())
					{
						controller.closeConnection("Not referenced, Referencing failed.\n");
						//return FALSE;
					}
					std::cout
						<< "move controller: " << szAxis[i] << "\n"
						<< "from position: " << controller.getPosition() << "\n"
						<< "to position: " << positions[i] << std::endl;
					if (!controller.Move(positions[i]))
					{
						controller.closeConnection("Not referenced, Referencing failed.In move function detected\n");
						//return FALSE;
					}
					else{
						//while (controller.isMoving()){
							std::cout
								<< "is controller moving(1 is yes 0 no )? " << controller.isMoving()
								<< std::endl;
					//		Sleep(300);
						//}
					}
					controller.closeConnection();
				}
				else
				{
					printf("Could not connect to E-871\n");
				}
			}
		}




		// At the end, set the status that will be displayed in the Run Control.
		SetConnectionState(eudaq::ConnectionState::STATE_CONF, "Configured (" + config.Name() + ")");
	}

	// This gets called whenever a new run is started
	// It receives the new run number as a parameter
/*	virtual void OnStartRun(unsigned param) {
		m_run = param;
		m_ev = 0;

		std::cout << "Start Run: " << m_run << std::endl;

		// It must send a BORE to the Data Collector
		eudaq::RawDataEvent bore(eudaq::RawDataEvent::BORE(EVENT_TYPE, m_run));
		// You can set tags on the BORE that will be saved in the data file
		// and can be used later to help decoding
		bore.SetTag("EXAMPLE", eudaq::to_string(m_exampleparam));
		// Send the event to the Data Collector
		SendEvent(bore);

		// At the end, set the status that will be displayed in the Run Control.
		SetStatus(eudaq::Status::LVL_OK, "Running");
		started = true;
	}

	// This gets called whenever a run is stopped
	virtual void OnStopRun() {
		std::cout << "Stopping Run" << std::endl;
		started = false;
		// Set a flag to signal to the polling loop that the run is over
		stopping = true;

		// wait until all events have been read out from the hardware
		while (stopping) {
			eudaq::mSleep(20);
		}

		// Send an EORE after all the real events have been sent
		// You can also set tags on it (as with the BORE) if necessary
		SendEvent(eudaq::RawDataEvent::EORE("Test", m_run, ++m_ev));
	}

	// This gets called when the Run Control is terminating,
	// we should also exit.
	*/
	virtual void OnTerminate() {
		std::cout << "Terminating..." << std::endl;
		done = true;
	}
	
	// This is just an example, adapt it to your hardware
	void ReadoutLoop() {
		// Loop until Run Control tells us to terminate
		while (!done) {
			/*if (!hardware.EventsPending()) {
				// No events are pending, so check if the run is stopping
				if (stopping) {
					// if so, signal that there are no events left
					stopping = false;
				}
				// Now sleep for a bit, to prevent chewing up all the CPU
				eudaq::mSleep(20);
				// Then restart the loop
				continue;
			}*/
			if (!started)
			{
				// Now sleep for a bit, to prevent chewing up all the CPU
				eudaq::mSleep(20);
				// Then restart the loop
				continue;
			}
			// If we get here, there must be data to read out
			// Create a RawDataEvent to contain the event data to be sent
			eudaq::RawDataEvent ev(EVENT_TYPE, m_run, m_ev);

			for (unsigned plane = 0; plane < hardware.NumSensors(); ++plane) {
				// Read out a block of raw data from the hardware
				std::vector<unsigned char> buffer = hardware.ReadSensor(plane);
				// Each data block has an ID that is used for ordering the planes later
				// If there are multiple sensors, they should be numbered incrementally

				// Add the block of raw data to the event
				ev.AddBlock(plane, buffer);
			}
			hardware.CompletedEvent();
			// Send the event to the Data Collector      
			SendEvent(ev);
			// Now increment the event number
			m_ev++;
		}
	}
	
	
private:
	// This is just a dummy class representing the hardware
	// It here basically that the example code will compile
	// but it also generates example raw data to help illustrate the decoder
	eudaq::ExampleHardware hardware;
	unsigned m_run, m_ev, m_exampleparam;
	bool stopping, done, started;
	
};

// The main function that will create a Producer instance and run it
int main(int /*argc*/, const char ** argv) {
	// You can use the OptionParser to get command-line arguments
	// then they will automatically be described in the help (-h) option
/*	eudaq::OptionParser op("PIProducer", "1.0",
		"Just an example, modify it to suit your own needs");
	eudaq::Option<std::string> rctrl(op, "r", "runcontrol",
		"tcp://localhost:44000", "address",
		"The address of the RunControl.");
	eudaq::Option<std::string> level(op, "l", "log-level", "NONE", "level",
		"The minimum level for displaying log messages locally");
	eudaq::Option<std::string> name(op, "n", "name", "Example", "string",
		"The name of this Producer"); */
	eudaq::OptionParser op("EUDAQ Example Producer", "1.0",
		"Just an example, modify it to suit your own needs");
	eudaq::Option<std::string> rctrl(op, "r", "runcontrol",
		"tcp://localhost:44000", "address",
		"The address of the RunControl.");
	eudaq::Option<std::string> level(op, "l", "log-level", "NONE", "level",
		"The minimum level for displaying log messages locally");
	eudaq::Option<std::string> name(op, "n", "name", "Example", "string",
		"The name of this Producer");
	try {
		// This will look through the command-line arguments and set the options
		op.Parse(argv);
		// Set the Log level for displaying messages based on command-line
		EUDAQ_LOG_LEVEL(level.Value());
		// Create a producer
		PIProducer producer(name.Value(), rctrl.Value());
		//std::cout << producer.portnumber << " , " << producer.baudrate << std::endl;
		//producer.move(move1)
		// And set it running...
		producer.ReadoutLoop();
		// When the readout loop terminates, it is time to go
		std::cout << "Quitting" << std::endl;
	}
	catch (...) {
		// This does some basic error handling of common exceptions
		return op.HandleMainException();
	}
	return 0;
}
