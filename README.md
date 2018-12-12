# Process_Scheduling
Process Scheduling

This is a project to scheduling the processes inputting from the txt file.

Format in input1 txt file for example:
  
  6   // there are 6 nodes in the input graph.
  
  1 2 // 1 must be done before 2.
  
  1 4 // 1 must be done before 4.
  
  4 3 // 3 must be done before 3.
  
  ...
  
Format in input2 txt file for example:
  
  6   //  there are 6 job to be done.
  
  1 3 //  job 1 needs time 3.
  
  2 4 //  job 2 needs time 4.
  
  ...
  
  
Command line format:

  ./main input1.txt input2.txt output1.txt output2.txt
  
  input1: Graph relationship.
  input2: Time for each job.
  output1: Process scheduling in visual.
  output2: Debugging output, recording done jobs, 1 means done, 0 means not done.
  
Users are able to decide how many processors to assign.
