================================================================================
================================================================================
		  Nouna Optimization Project
================================================================================
================================================================================
Ekaterina Tikhoncheva, ekaterina.tikhoncheva@gmail.com
29.04.2014


My work on this project was consisted in the implementation of a simple GUI for
the project and implementation of an algorithm, that find a feasible solution of
the problem for further optimization.

In this version of the project there are two different versions of an algorithm
for finding a feasible solution (see initialsolution1.h/cpp and 
initialsolution2.h/cpp).


================================================================================
Implementation
================================================================================
Implementation language : C++ 
Used Software		: Qt Creator 3.2.2 base on Qt version 5.3.2 (GCC 4.6.1, 64 bit) 
Additional libraries:	: Qt QCustomPlot library (version 1.2.1)
--------------------------------------------------------------------------------


================================================================================
Overview of the header files
================================================================================

village.h 
  declaration of a structure stVillage that describes one village:
  ID, name, geographical coordinates

household.h
  declaration of a structure stVillage that describes one household:
  ID, name, type (0 or 1), number of Persons, village it belongs to
  and required interview time (computed according to the type)

interviewer.h
  declaration of a structure stInterviewer that describes one interview:
  ID and vectors of his week and daily routes
  declaration of a structure stRoute, that describes a route:
  time of the route (traveling time + time of interviews planned on the route)
  villages, visited on the route
  households, interviewed on the route
  

road.h
  declaration of a structure stRoad that describes one road:
  ID, name, start and end, distance in km, category of speed limit and if it
  is dependent on rain season
  
  additional structure adjmatrix_entry for an entry of distance matrix (in km), that
  is used for computing traveling time between villages

readdate.h
  implementation of three functions for reading input data (list of villages,
  list of households, list of roads) for the problem from corresponding csv-files. 
  
  Currently, all input files are read from directory ./data automatically.

datadef.h
  declaration of the global data structures of the input/output data

distmatrix.h
  declaration of a function, that returns adjacency matrix of the villages
  where each non-zero entry is equal the distance between corresponding
  villages
  
  declaration of a function, that implements the Dijkstra algorithm to compute
  distances between one village and all other villages (in min) given
  weighted adjacency matrix of the villages
  
  
collectdata.h	
  A routine, that calculate distance matrices between villages in km and
  than in min according to the allowed speed on the roads
  It also sets the constants for the problem, such that maximal number of available
  interviewers, time for short and long interviewers.
  
const.h
  declaration of the class constant with the static member variables for
    short and long interview time
    number of periods in year and number of weeks in each period
    maximal number of available interviewers
    number of short interviews and long interviews (depends on input data)
    summary time of all long Interviews (depends on input data)
    maximal traveling time between villages in dry and raining season (depends on input data)
    
plot.h
  declaration of the plotting functions for map and routes
  
report.h
  two functions to invert initial solution in a set of strings (will be used later in
  reportwindow.h/.cpp) and save this set in a file
  
reportwindow.h
  declaration of a new window form to show results of the planning problem. The report window contains
  now a table representation of initial solution (i.e. year schedule of visiting the households:
  when, how long and by whom), list of week and day routes of each interviewer over whole year,
  two tables with traveling time between villages in dry and rain seasons.
  

test.h
  declaration of a class test for testing, if current solution fulfills all constrains.
  Currently, the class includes following tests:
    test1 : for each interviewer check, if his working time in each week of an year does not
	    exceed the norm (i.e. 5 working days with 8 working hours pro day)
    test2 : for each interviewer check, if his working time in each day of an year does not
	    exceed the norm (i.e. 8 hours pro day)	    
    test3 : for each too long interview (interview time exceeds 8 hours) check, if
	    interview time is split correctly in several days: the sum over days should be equal
	    entire interview time	    
    test4 : for each household check, if dates of visits are correct, i.e. 16 weeks between visits
            in different periods and 1 day, if interview was split in several day (by too long interviews)

testwindow.h
  declares new window form to show results of tests running
    
  
mainwindow.h
  declaration of a main window with implementation of responses on the user interaction

main.cpp 
  starting the GUI and setting the constant values defined in const.h

================================================================================
initialsolution1.h/cpp
================================================================================

    First approach for route planning and assignment (idea of G.Reinelt):
    
    1) Assignment of week of visits to households 
    
      We consider first only households of the type 1 (special households).
      For each such household it will be decided, in which period of a year 
      (1 year = three periods) it will be visited long (25 min per person).
      In two other periods it will be visited as a normal household (10min).
      
      Then, the households get assigned weeks of visits. 
      First, the week of an interview is selected for long interviews 
      of each household, so that the average long interview time in each
      period is roughly the same. Than this week is accepted in other two periods for 
      short interviews of the same household.
      
      The relative week number of visit for each household is as a consequence 
      the same in all three periods.
      
      We consider further normal households (type 0). They should be visited 
      three times in a year with the same times. We divide such households in 16
      groups (16=number of weeks in one period) and assign in this way weeks of
      visit for this households in one period. For two other periods the weeks
      will be set the same as they are in the first period. The relative week 
      number of visits for each normal household is hence the same in all three periods.
      
    2) Daily routes between households, that should be visited on the same week
    
      Given the result of the previous step we construct a set of daily routes 
      for each week of a year.
      This routes should fulfill time requirements for one work day:
	maximal working time is 8 hours,
	each route should start and end in the capital (Nouna).
      
      NOTE: the number of daily routes for one week cannot exceed number of 
      available interviewers times 5 (number of working day in a week).
      
      NOTE: Splitting of long interviewers in several days is implemented already
      in this step and working better than doing it afterwards as in initialsolution2.h
      
    3) Assign daily plans to available interviewers
    
      For each week the daily routes will be assigned in groups by 5 (5 working
      days on a week) to interviewer. Some interviewer may be stay free
      for some weeks.
      
      
    4) Allow to stay of a night
    
      For each interviewer check if the previous work day in a week ended in the
      same village the next day starts with. If so, the interviewer can stay
      in this village for a night and must not go back to Nouna and come on the next day again.
      
-------------------------------------------------------------------------------      
      NOTE: 
       - It can happen, that second part of a long interview (after splitting)
      will be taken (see step 3) by next interviewer. Than the condition, that
      too long interviews must be continued after interruption on the next day,
      will not be full filled.
       - It is also only guarantied, that household will be visited on the same
      week in each period, but not on the same day!
--------------------------------------------------------------------------------

================================================================================
initialsolution2.h/cpp
================================================================================
  Second approach for route planning and assignment:
  
   
    1) Assignment of week of visits to households 
    
      We consider first only households of the type 1 (special households).
      For each such household it will be decided, in which period of a year 
      (1 year = three periods) it will be visited long (25 min per person).
      In two other periods it will be visited as a normal household (10min).
      
      Then, the households get assigned weeks of visits. 
      First, the week of an interview is selected for long interviews 
      of each household, so that the average long interview time in each
      period is roughly the same. Than this week is accepted in other two
      periods for short interviews of the same household.
      
      The relative week number of visit for each household is as a consequence 
      the same in all three periods.
      
      We consider further normal households (type 0). They should be visited 
      three times in a year with the same times. We assign to each short interview
      recursively a week 1 to 16 in the first period. For each household the week selected
      in the first period will be accepted in the second and third period.The relative week 
      number of visits for each normal household is hence the same in all three periods.
      
    2) Create week plans for each interviewer 
    
      We consider one week of the year after another. For each week we construct
      K routes, where K is the number of available interviewers.
      
      The working time in each route should not exceed 5(days)*8 hours.
      
      For each week the K constructed routes are assigned to K interviewers.
      
    3) Create day plans for each interviewer
    
      Given result of the previous step, we divide a week route of each interviewer
      into 5 day routes, so that the working time in each day route should not
      exceed 8 hours. That will not be always possible because of too long interviews.
      So the next two steps should improve the result of this step.
    
    4) Split too long interviews (interview time > 8 hours)
      
      We go through each day route of all interviewers and try to split too long
      interviews in several days.
      For each found too long interview we look if it is better to start this
      interview one day before or continue on the next day.
      
    5) Allow to stay of a night
    
      Same as it was described in the first approach

-------------------------------------------------------------------------------      
      NOTE: 
       - The splitting of too long interviews is not working so good, as
      in the first approach. From the other side, here it will be guarantied, 
      that split interview will take place on the sequential days 
      and will be done by the same interviewer.
       - It is again only guarantied, that household will be visited on the same
      week in each period, but not on the same day!

--------------------------------------------------------------------------------



================================================================================
================================================================================