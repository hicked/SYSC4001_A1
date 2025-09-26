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

    /******************ADD YOUR VARIABLES HERE*************************/
    
    //activity, (vector table, device table)
    std::unordered_map<std::string, std::vector<std::string>> actions;
    
    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            //appened CPU, time, "CPU processing" to output
            
        }
        else if (activity == "SYSCALL") {
            //enter kernel mode (1ms)
            //context saved (~10ms) macro this
            //caluclate where in memory ISR start address is (1ms)
            //obtain ISR address (1ms)
            //execute ISR body (depends on activity) - depends on device - expand on this
            //execute IRET, enter user mode
        }
        else if (activity == "END_IO") {

        }
        else {
            //invalid input
            //append invalid instruction and end program
        }


        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}
