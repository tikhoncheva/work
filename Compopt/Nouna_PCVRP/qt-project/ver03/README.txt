--------------------------------------------------------------------------------------------------------------------
Ver 01

+-  Sort interviews time in decreasing order
    Plan first lang interviews and then fill the gaps with short ones
    
+- Use week plan insted of a day plan


+ allow interviewers to stay at the end of the day to overnight in the village they currently are
--------------------------------------------------------------------------------------------------------------------
Ver 02

  Corrected erros:
  + type0 households require 10 min independent on number of persons
  + type1 households must be visited only one time as long interviews (25 min pro person), other two times as short interviews (10 min)
  
  New approach:
    1. distribute long interviews uniformly in 3 periods
    2. plan visiting weeks for long interviewers in each period, full free time with short interviews
       adopt week plan of the first period in other two periods
    3. make week plan for each interviewer (first period only) TODO
      adopt week plans from first period for the second and the third	TODO
    4. make day plans	TODO
    
    
Ver02.01

  New approach:
    1. Plan long interview times as in previous version
    2. Plan households for each day independent on the interviewer
    3. Asssign interviewrs to day plans

    
    ToDo: better sorting of villages/hh
          allow to stay over night
          split too longs interviews
          
 Old approach (week assignments):
   do not close bins(days)
   
 Common problem: split too long interviews
 
 
 Ver02.02:
 
    The approach to build dauly routes and assign them afterwards to interviewer seems to work worth than the approach with building week routes, assigning 
    them to households and than separating them in daily routes
    
    The approach with week assignemnts (initialsolution2.h) works, but interviewer are not equaly busy with inteviews. For example, Interviewer 1, 2 and 3 are mainly used in year/ month/ week. All other are working only a few days a week
    
    + allow to stay over the night
    + split to long interviews in two days 
    
 --------------------------------------------------------------------------------------------------------------------
       
 Ver03
    New approach for route planning and assignment (idea of G.Reinelt):
    1) Assignement of week of visits to households 
    
      We consider first only households of the type 1 (special households).
      For each such household it will be decided, in which period of a year (1 year = three periods) it will be visited long (25 min per person). In two other periods it will be visited as a normal household (10min). Then, the households get assigned weeks of visits. First, the week of an interview
      is selected for long interviews of each household. Than this week is accepted in other two periods for short interview of the same household.
      The relative week number of visit for each household is as a consequence the same in all three periods.
      
      We considere further normal households (type 0). They should be visited three times in a year with the same times. We devide such households in 16
      groups (16=number of weeks in one period) and assign in this way weeks of visit for this households in one period. For two other periods the weeks from the first period will be just accepted, and hence the relative week number of visit for each normal household is also the same in all three periods.
      
    2) Daily routes between households, that should be visited on the same week
    
      Given the result of the previous step we construct a set of daily routes for each week of a year. This routes should fulfill time requirements
      for one work day: maximal working time is 8 hours, each route should start and end in the capital (Nouna).
      
      NOTE: the number of daily routes for one week cannot exceed number of available interviewers times 5 (number of working day in a week).
      
      NOTE: Splitting of long interviewers in several days is implemented already in this step and working better than doing it afterwards as in ver02.2
      
    3) Assign daily plans to available interviewers
    
      For each week the daily routes will be assigned in groups by 5 routes (except perhaps last group) to interviewer. Some interviewer may be stay free
      for some weeks.
      
      
    4) Allow to stay of a night
    
      For each interviewer check it the previous work day in a week ended in the same village the naxt day starts with, than the interviewer can stay
      in this village for a night and must not go back to Nouna and come on the next day again.
      
      
      
    Additionaly:
    + round entries of the dist matrices to integer numbers. That means, that we consider minutes as a smallest time entity
    + test-class to test, if the different constrains on the model are satisfied or not.
    