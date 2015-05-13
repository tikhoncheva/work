#include <hrd/report.h>



//-----------------------------------------------------------------------------------------------
// for each HH print visiting times in each period, days and weeks of visits and ID of the interviewers
//
// _ITimePlan has a week view
//-----------------------------------------------------------------------------------------------
std::set<rowEntries> HHSchedule_weekview_forTable(const std::vector<stInterviewer> _interviewer,
                                                  std::vector<std::vector<std::pair<unsigned int, double> > > _ITimePlan) //                    const std::string fileName)
{
    std::set<rowEntries> dataset;
    std::set<rowEntries> ::iterator it;

    int itime;

    std::vector<std::vector<std::pair<unsigned int, double> > > copyITimePlan; // copy of _ITimePlan

    copyITimePlan.resize(_ITimePlan.size());
    for (unsigned int w=0; w<_ITimePlan.size(); ++w)
        for (unsigned int p=0; p<_ITimePlan[w].size(); ++p )
        {
            copyITimePlan[w].push_back(std::make_pair(_ITimePlan[w][p].first,_ITimePlan[w][p].second));
        }

    for (unsigned int i=0; i<_interviewer.size(); ++i)
        for (unsigned int d=0; d< _interviewer[i].routes_days.size(); ++d)
            for (unsigned int h=0; h<_interviewer[i].routes_days[d].households.size(); ++h)
            {
                const unsigned hhID = _interviewer[i].routes_days[d].households[h];

                // find planed interview time for current hh on the day d
                std::vector<std::pair<unsigned int, double> >::iterator timeplan_it;

                timeplan_it = std::find_if(copyITimePlan[d/5].begin(),copyITimePlan[d/5].end(),
                        [hhID] (std::pair<unsigned int, double> weekday)
                {return weekday.first == hhID;});
                itime = round( (*timeplan_it).second);

                rowEntries searchEntry;
                searchEntry.hhID = hhID + 10001;

                it = dataset.find(searchEntry);

                if (it==dataset.end())
                {   // make new entry
                    rowEntries saveEntry;
                    saveEntry.hhID = hhID + 10001;
                    saveEntry.itime = std::to_string(itime);
                    saveEntry.day = std::to_string(d + 1) + "("
                            + std::to_string(d/5 + 1) + ")";
                    saveEntry.Interviewer = std::to_string(i + 1);

                    dataset.insert( saveEntry);
                }
                else
                {   // make new entry, which complements the already existing entry
                    rowEntries saveEntry;

                    saveEntry.hhID = (*it).hhID;
                    saveEntry.itime = (*it).itime + ", " + std::to_string(itime);
                    saveEntry.day = (*it).day + ", " + std::to_string(d + 1)
                            + "(" + std::to_string(d/5 + 1) + ")";
                    saveEntry.Interviewer = (*it).Interviewer + ", " + std::to_string(i + 1);
                    // delete the old one
                    dataset.erase(it);
                    // insert the new one
                    dataset.insert( saveEntry);
                }

                (*timeplan_it).first = 99999;

            }

    return dataset;
}

void  saveHHSchedule_weekview(const std::set<rowEntries> HHSchedule,
                              const std::string fileName)
{
    std::ofstream file(fileName.c_str());	// file to open
    assert(file.is_open() && "ERROR saveHHSchedule_weekview: File cannot be opened to write");

    file << "hh_id" << std::setw(20) << "itime" << std::setw(40)
         << "day(weeks)"  << std::setw(20)  << "Interviewer ID" << std::endl;
    file << std::endl;

    for (std::set<rowEntries> ::iterator
         it=HHSchedule.begin(); it!=HHSchedule.end(); ++it)
    {
        file << (*it).hhID << std::setw(20) << (*it).itime << std::setw(40)
             << (*it).day  << std::setw(20) << (*it).Interviewer << std::endl;
    }

    file.close();
}
