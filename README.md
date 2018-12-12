# Process_Scheduling
Process Scheduling

This is a project to scheduling the processes inputting from the txt file.

Format in input txt file for example:
  
  6   // there are 6 nodes in the input graph.
  
  1 2 // 1 must be done before 2.
  
  1 4 // 1 must be done before 4.
  
  4 3 // 3 must be done before 3.
  
  ...
  
Command line format:

  ./main input_file_name.txt output1.txt output2.txt
  
  output1: Process scheduling in visual.
  output2: Debugging output, recording done jobs, 1 means done, 0 means not done.
  
Users are able to decide how many processors to assign.
