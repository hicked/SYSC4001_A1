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
    
    unsigned long   upTime =                0;
    const int       contextSavResTime =       10;
    const int       ISRActivityTime =       40;

    /******************************************************************/

    // Parse each line of the input trace file
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
                execution += createOutputString(upTime, 1, "ERROR: INVALID VECTOR TABLE INDEX: The I/O device specific drivers may be corrupted.");
                upTime += 1;
            }
            else {
                std::pair<std::string, int> output = intr_boilerplate(upTime, duration_intr, contextSavResTime, vectors);
                execution += output.first;
                upTime = output.second;

                // If there is no delay in device table, we assume that the ISR isn't for an I/O device, and is a regular system call
                // We are assuming there is no low level paralism yet, and therefore the CPU will hang until the I/O devce is finished
                // This is why, for now, we print "polling..." until the device is done before continuing
                // In the future (assignment 2), we will use an interupt schedule to allow for low level parallelism
                // Note: We decided to additionally differentiate between the software and hardware (I/O) ISRs in the output (execution.txt)
                if (duration_intr < delays.size()) {
                    execution += createOutputString(upTime, ISRActivityTime, "running I/O device specific ISR (driver) due to hardware interrupt");
                    upTime += ISRActivityTime;

                    int IODelay = delays[duration_intr];

                    // Printing "." to simulate the CPU polling, or waiting for the I/O device to finish
                    int numDots = (IODelay/50 < 3) ? 3 : IODelay/50;
                    execution += createOutputString(upTime, IODelay, "IO device busy: CPU polling" + std::string(numDots, '.') + "I/O device finished, resuming");
                    upTime += IODelay;
                }
                else {
                    execution += createOutputString(upTime, ISRActivityTime, "running ISR from system call software interrupt");
                    upTime += 1;
                }

                // Restoring context
                execution += createOutputString(upTime, contextSavResTime, "restoring previously saved context");
                upTime += contextSavResTime;

                // IRET (restoring)
                execution += createOutputString(upTime, 1, "running IRET");
                upTime += 1;
            }

        }
        else if (activity == "END_IO") {
            execution += createOutputString(upTime, 1, "ending I/O for device " + std::to_string(duration_intr));
            upTime += 1;
        }
        else {
            // return 1 since it fails (most likely an incorrect trace file)
            execution += createOutputString(upTime, 0, "ERROR: INVALID ACTIVITY. ENDING SIMULATION");
            input_file.close();
            write_output(execution);
            return 1;
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

// Things to thing about for Assignment 2 (interupt scheduler)
// ISR is seperate from actual hardware wait time (in device table)
// store last uptime when system calls for device
// at x milliseconds, stop and check device, then resume? Probably 1ms