================================================================================
================================================================================
		  Nouna Optimization Project
================================================================================
================================================================================
Ekaterina Tikhoncheva, ekaterina.tikhoncheva@gmail.com
29.04.2014


My work on this project was consisted in the implementation of a simple GUI for
the project and implementation of an algorithm, that find a feasible solution of
the problem, that will be used further for optimization.

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
  declaration of a structure stVillage that describes one village;
  ID, name, geographical coordinates

household.h
  declaration of a structure stVillage that describes one household;
  ID, name, type (0 or 1),number of Persons, village it belongs to
  and required interview time

interviewer.h
  declaration of a structure stInterviewer that describes one interview:

road.h
  declaration of a structure stRoad that describes one road:
  ID, name, start and end, distance in km, category of speed limit and if it
  is dependent on rain season
  
  additional structure for an entry of distance matrix before computing travel
  time between villages
  

readdate.h
  Implementation of three functions for reading input data (list of villages,
  list of households, list of roads) for the problem from corresponding csv-files. 
 
dijkstra.h
  Implementation of the Dijkstra algorithm to compute distances between one
  village and all other villages
 
  
collectdata.h	
  A routine, that calculate distance matrices between villages in km and
  than in min according to the allowed speed on the roads
  It also sets the constants for the problem, such that maximal number of available
  interviewers, time for short and long interviewers.
  
  





================================================================================
initialsolution1.h/cpp
================================================================================

    First approach for route planning and assignment (idea of G.Reinelt):
    
    1) Assignement of week of visits to households 
    
      We consider first only households of the type 1 (special households).
      For each such household it will be decided, in which period of a year 
      (1 year = three periods) it will be visited long (25 min per person).
      In two other periods it will be visited as a normal household (10min).
      
      Then, the households get assigned weeks of visits. 
      First, the week of an interview is selected for long interviews 
      of each household. Than this week is accepted in other two periods for 
      short interviews of the same household.
      
      The relative week number of visit for each household is as a consequence 
      the same in all three periods.
      
      We considere further normal households (type 0). They should be visited 
      three times in a year with the same times. We devide such households in 16
      groups (16=number of weeks in one period) and assign in this way weeks of
      visit for this households in one period. For two other periods the weeks
      from the first period will be just accepted, and hence the relative week 
      number of visit for each normal household is also the same in all three periods.
      
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
    
      For each week the daily routes will be assigned in groups by 5 routes 
      (except perhaps last group) to interviewer. Some interviewer may be stay free
      for some weeks.
      
      
    4) Allow to stay of a night
    
      For each interviewer check if the previous work day in a week ended in the
      same village the naxt day starts with. If so the interviewer can stay
      in this village for a night and must not go back to Nouna and come on the next day again.
      
      NOTE: there is small problem here in current implementation: it can happen,
      that second part of a long interviewer will be taken (see step 3) by
      next interviewer. Than the condition, that an too long interview must be
      continued on the next day will not be fullfilled.

--------------------------------------------------------------------------------

================================================================================
initialsolution1.h/cpp
================================================================================
  Second approach for route planning and assignment:
  
--------------------------------------------------------------------------------








================================================================================
================================================================================