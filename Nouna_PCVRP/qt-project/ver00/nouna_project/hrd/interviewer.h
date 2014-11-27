#ifndef INTERVIEWER_H
#define INTERVIEWER_H

/*
 * An interviewer
 */
struct stInterviewer
{
    unsigned int ID;
    unsigned int nRoutes;

    void print()
    {
        std::cout << "Interviewer: ID=" << ID << "  nRoutes"
                << nRoutes << std::endl;
    }
};
#endif // INTERVIEWER_H
