/**
 *
 * @file interrupts.cpp
 * @author Antoine Hickey
 *
 */

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
    
    unsigned long   upTime =            0;
    const int       contextSaveTime =   10;
    const int       sysCallHandleTime = 1;
    
    /******************************************************************/

    //STATE ASSUMPTIONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //total ISR + device time ex. 40 + 200
    //CPU polls in meantime - no interrupt manager
    //DON'T say CPU polling for x time - unrealisitc


    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        auto [activity, duration_intr] = parse_trace(trace);

        /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "CPU") {
            execution += std::to_string(upTime) 
                        + ", " 
                        + std::to_string(duration_intr)
                        + ", CPU processing\n";

            upTime += duration_intr;
        }
        else if (activity == "SYSCALL") {

            if (duration_intr >= vectors.size()) {
                execution += createOutputString(upTime, 1, "INVALID INDEX");
                upTime += 1;
            }
            else {
                std::pair<std::string, int> output = intr_boilerplate(upTime, duration_intr, contextSaveTime, vectors);
                execution += output.first;
                upTime = output.second;

                // If there is no delay in device table, we assume that the ISR isn't for an I/O device, and is a regular system call
                try {
                    int ISRDelay = delays.at(duration_intr); // Use .at() for bounds checking
                    execution += createOutputString(upTime, ISRDelay, "running I/O device specific ISR from hardware interrupt");
                    upTime += ISRDelay;
                }
                catch (const std::out_of_range&) { // Catch out_of_range exception
                    execution += createOutputString(upTime, sysCallHandleTime, "running system call from software interrupt");
                    upTime += 1;
                }

                // IRET
                execution += createOutputString(upTime, 1, "Running IRET");
                upTime += 1;
            }

            //low level parallelism?????????????????????
            //ISR is seperate from actual hardware wait time (in device table)
            //store last uptime when system calls for device
            //at x milliseconds, stop and check device, then resume?????
            //

        }
        else if (activity == "END_IO") {
            execution += createOutputString(upTime, 1, "Ending I/O for device " + std::to_string(duration_intr));
            upTime += 1;
        }
        else {
            execution += createOutputString(upTime, 0, "INVALID ACTIVITY");
        }

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

