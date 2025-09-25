Student 1: Antoine Hickey
Stundet 2: Enzo Chen

Below is an explanation of what this program does, as well as the answers to the questions from Part I.

> Insert description here (to be done during part two)

a) [0.1 mark] Explain, in detail, the complete Interrupt mechanism, starting from an external signal until 
completion. Differentiate clearly what part of the process is carried out by hardware components, and what 
is done by software. [Student 1] 

Below is a sequence of of hardware and software steps that are involved in the full interupt mechanism for I/O deviced servicing.
- The interrupt is enabled by a set of sensor(s) and/or software within the I/O device (this could be for when it's finished printing, reading, or doing some other task depending on the I/O device).
- This signal is then stored within the I/O devices internal IR (interrupt register).
- It then travels through a physical connection (copper wire, cable, port, etc.)
- To the IOP IR (Input/Output Processor Interrupt Register). This can be compared to a 1401 processor from the early staged of computers as it is a less powerful computer that is used as a middleman for communicating between I/O devices and the main CPU.
- The interrupt signal is then sent through the internal Interrupt Databus, where it wakes up the CPU (if it was previously asleep), and stored the signal within the CPUs IR (Interrupt Register).
- Because of the way the CPU's architecture is set up, this register (IR) is checked at the end of each fetch execute cycle. Therefore, after finishing it's execution, or waking up, it will recognize that an interrupt signal has been sent.
- Then, the CPU will use the data that is within the IR to find which interrupt handler to use by looking through it's vector table, which then points to a set of drivers stored in the OS/Monitor. This will branch to that specific devices interrupt handler, before continuing with the instruction it was doing previously. 

-In reality it is a little more complicated than this: Firstly, software is used to determine if an interrupt has already occured, before sending one of it's own. If this is the case, it must wait until it is able to send the signal. Also, we will see that for later iterations, sometimes the CPU might wait until more than just the end of each cycle since this can give the interrupt handler (from within the vector table) permissions it is not supposed to have, including possibly modifying the kernel.

b) [0.1 marks] Explain, in detail, what a System Call is, give at least three examples of known system calls. 
Additionally, explain how system Calls are related to Interrupts and explain how the Interrupt hardware 
mechanism is used to implement System Calls. [Student 2] 

A sytstem call is a request sent by a user program to ask the OS to perform tasks which are normally reserved for the OS.
There are six main categories of system calls:
- Process control
- File management
- Device management
- Information maintenance
- Communications
- Protection

Three examples of system calls are:
1. Reading from a file (File management)
2. Get current time/date (Information maintenance)
3. Allocate memory (Process control)

System calls are related to interupts because they can create a software interrupt, known as a "trap" or "exception". 
The system call provides an interrupt vector, which passes control to a system-call service routine.
The system call also sets the CPU state to kernel mode, by setting the mode bit accordingly.
The kernel identifies which system call was invoked, then executes it, and finally gives control back to the program.
This process of using a software interrupt, providing an interrupt vector, changing the mode bit, and executing a service routine uses the same hardware as interrupts to execute a system call.

 
c) [0.1 marks] In class, we showed a simple pseudocode of an output driver for a printer. This driver 
included two generic statements:  
i. check if the printer is OK 
ii. print (LF, CR) 
Discuss in detail all the steps that the printer must carry out for each of these two items (Student 1 should 
submit answer ii., and Student 2 should submit answer i.). 

i. To check if the printer is OK, the CPU should:
- Check if the printer is powered on and connected
- Check if busy (already printing something)
- Check if the printer is jammed
- Check if the printer has paper
- Check if the printer has ink

ii. LF (Line Feed):
- Spin a motor to move the print head down by one line OR
    - Spin a motor to move the paper up by one line

CR (Carriage Return):
- Spin a motor to move the print head back to the left side of the page

d) [0.4 marks] Explain briefly how the off-line operation works in batch OS. Discuss advantages and 
disadvantages of this approach. 
 
 
e) [0.4 mark] Batch Operating Systems used special cards to automate processing and to identify the jobs to 
be done. A new job started by using a special card that contained a command,  starting with $, like:

For instance, the $FORTRAN card would indicate to start executing the FORTRAN compiler and compile 
the program in the cards and generate an executable. $LOAD loads the executable, and $RUN starts the 
execution. 
 
i. [0.2 marks] Explain what would happen if a programmer wrote a driver and forgot to parse 
the “$” in the cards read. How do we prevent that error? 
 
ii. [0.2 marks] Explain what would happen if, in the middle of the execution of the program (i.e., 
after executing the program using $RUN), we have a card that has the text “$END” at the 
beginning of the card. What should the Operating System do in that case? 
 
 
f) [0.2 marks] Write examples of four privileged instructions and explain what they do and why they are 
privileged (each student should submit an answer for two instructions, separately, by the first 
deadline). 
- Switch to kernel mode

- Interrupt management
If you can change the interrupt management, you can force any interrupt to go through while in the kernel space, which gives the interrupt handler elevated privileges.

- I/O control
- Timer management


  
g) [0.4 marks] A simple Batch OS includes the four components discussed in class: 
  
Suppose that you have to run a program whose executable is stored in a tape. The command $LOAD 
TAPE1: will activate the loader and will load the first file found in TAPE1: into main memory (the 
executable is stored in the User Area of main memory). The $RUN card will start the execution of the 
program. 
 
Explain what will happen when you have the two cards below in your deck, one after the other: 
 
$LOAD TAPE1: 

$RUN 
 
You must provide a detailed analysis of the execution sequence triggered by the two cards, clearly 
identifying the routines illustrated in the figure above. Your explanation should specify which routines are 
executed, the order in which they occur, the timing of each, and their respective functions—step by step. In 
your response, include the following: 
   
i. A clear identification and description of the routines involved, with direct reference to the figure.   
 
ii. A detailed explanation of the execution order and how the routines interact.   
 
iii. A step-by-step breakdown of what each routine performs during its execution. 
 
 
h) [0.3 marks] Consider the following program: 
 
 
 Loop 284 times { 
 x = read_card();  
 name = find_student_Last_Name (x);  // 0.5s 
 print(name, printer);  
 GPA = find_student_marks_and_average(x); // 0.4s 
 print(GPA, printer); 
} 
 
Reading a card takes 1 second, printing anything takes 1.5 seconds. When using basic timing I/O, we add 
an error of 30% for card reading and 20% for printing. Interrupt latency is 0.1 seconds. 
 
For each of the following cases: create a Gantt diagram which includes all actions described above as well 
as the times when the CPU is busy/not busy, calculate the time for one cycle and the time for entire 
program execution, and finally briefly discuss the results obtained. 

