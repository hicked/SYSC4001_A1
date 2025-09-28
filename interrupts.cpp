/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

/*
 * Includes added: 
 */
 #include <unordered_map>

#include<interrupts.hpp>

int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);

    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output
    unsigned long upTime = 0;

    /******************ADD YOUR VARIABLES HERE*************************/
    
    std::vector<string>     vectorTableList;
    std::vector<int>        IODeviceDelay;
    auto parsedData = parse_args(argc, argv);
    
    vectorTableList, IODeviceDelay = parsedData[0], parsedData[1];
    
    
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            execution += createOutputString(upTime, duration_intr, "CPU processing");
            upTime += duration_intr;
        }
        else if (activity == "SYSCALL") {
            execution += createOutputString(upTime, 1, "switch to kernel mode");
            upTime += 1;

            execution += createOutputString(upTime, 2, "context saved");
            upTime += 2;

            std::string inputStr = "";
            inputStr += "find vector ";
            inputStr += std::to_string(duration_intr);
            inputStr += " in memory position"
            execution += createOutputString(upTime, 1, "switch to kernel mode");
            upTime += 1;

            std::string ISRAddr = vectorTableList[duration_intr];
            execution += createOutputString(upTime, 1, "obtain ISR address: ");
            upTime += 1;







            
            execution += ",  find vector "
            execution += std::to_string(duration_intr);
            execution += " in memory position ";
            execution += std::to_string(duration_intr*2);           // DOUBLE CHECK THIS !!!!!!!!!!!!!!!!!!
            execution += "\n";
            upTime += 1;

            execution += ", context saved\n";
            upTime += 2;

            //enter kernel mode (1ms)
            //context saved (~10ms) macro this
            //caluclate where in memory ISR start address is (1ms)
            //obtain ISR address (1ms)
            //execute ISR body (depends on activity) - depends on device - expand on this
            //execute IRET, enter user mode
        }
        // else if (activity == "END_IO") {

        // }
        // else {
        //     //invalid input
        //     //append invalid instruction and end program
        // }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}


std::string createOutputString(unsigned long totalTime, int delay, std::string msg) {
    std::string output = "";
    output += std::to_string(totalTime);
    output += ", ";
    output += std::to_string(delay);
    output += ", ";
    output += msg;
    output += "\n";
    return output;
}
