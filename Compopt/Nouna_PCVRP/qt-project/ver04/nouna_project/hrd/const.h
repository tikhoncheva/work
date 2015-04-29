#ifndef CONST_H
#define CONST_H

class constant
{
public:
    static unsigned int shortITime; // 10 min
    static unsigned int  longITime;  // 25 min

    static unsigned int P;           // 3 periods
    static unsigned int nweeks;     // each period contains 16 weeks

    static unsigned int nInterviewers;

    static unsigned int nLongInterviews;   // number of Households with the type 1
    static unsigned int nShortInterviews;  // number of Households with the type 0
    static double summaryLongITime;              // summary long interview times in year

    static double maxTimeDistDry;
    static double maxTimeDistRain;

};

#endif // CONST_H
