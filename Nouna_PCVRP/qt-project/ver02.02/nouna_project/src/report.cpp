#include <hrd/report.h>


//-----------------------------------------------------------------------------------------------
/*
 * for each hh print on which day it will be visited and what interview time is required
 */
void saveHH_ITPlan_d(const std::vector<std::vector<std::pair<unsigned int, double> > > _hhITimePlan,
                     const std::string fileName)
{
    std::set<writeFormat1> dataset;
    std::set<writeFormat1> ::iterator it;

    std::ofstream file(fileName.c_str());	// file to open
    assert(file.is_open() && "ERROR saveHHSchedule: File cannot be opened to write");

    for (unsigned int d=0; d<_hhITimePlan.size(); ++d)
        for (unsigned int h=0; h<_hhITimePlan[d].size(); ++h)
        {
            const unsigned hhID = _hhITimePlan[d][h].first;
            const int itime  = _hhITimePlan[d][h].second;

            writeFormat1 searchEntry(hhID + 10001);

            it = dataset.find(searchEntry);

            if (it==dataset.end())
            {   // make new entry
                writeFormat1 writeEntry(hhID + 10001);

                writeEntry.itime = std::to_string(itime);
                writeEntry.day = std::to_string(d + 1) + "("
                        + std::to_string(d/5 + 1) + ")";

                dataset.insert( writeEntry);
            }
            else
            {   // make new entry, which complements the already existing entry
                writeFormat1 writeEntry((*it).hhID);

                writeEntry.itime = (*it).itime + ", " + std::to_string(itime);
                writeEntry.day = (*it).day + ", " + std::to_string(d + 1)
                        + "(" + std::to_string(d/5 + 1) + ")";

                // delete the old one
                dataset.erase(it);
                // insert the new one
                dataset.insert( writeEntry);
            }
        }

    // write set in to the file
    file << "hh_id" << std::setw(20) << "itime" << std::setw(40) << "day"  <<  std::endl;
    for (it=dataset.begin(); it!=dataset.end(); ++it)
        file << (*it).hhID << std::setw(20) << (*it).itime << std::setw(40) << (*it).day  << std::endl;

    file.close();

}

void saveHH_ITPlan_w (const std::vector<std::vector<std::pair<unsigned int, double> > > _hhITimePlan,
                      const std::string fileName)
{
    std::set<writeFormat1> dataset;
    std::set<writeFormat1> ::iterator it;

    std::ofstream file(fileName.c_str());	// file to open
    assert(file.is_open() && "ERROR saveHHSchedule: File cannot be opened to write");

    for (unsigned int w=0; w<_hhITimePlan.size(); ++w)
        for (unsigned int h=0; h<_hhITimePlan[w].size(); ++h)
        {
            const unsigned hhID = _hhITimePlan[w][h].first;
            const int itime  = _hhITimePlan[w][h].second;

            writeFormat1 searchEntry(hhID + 10001);

            it = dataset.find(searchEntry);

            if (it==dataset.end())
            {   // make new entry
                writeFormat1 writeEntry(hhID + 10001);

                writeEntry.itime = std::to_string(itime);
                writeEntry.day = std::to_string(w + 1);

                dataset.insert( writeEntry);
            }
            else
            {   // make new entry, which complements the already existing entry
                writeFormat1 writeEntry((*it).hhID);

                writeEntry.itime = (*it).itime + ", " + std::to_string(itime);
                writeEntry.day = (*it).day + ", " + std::to_string(w + 1);

                // delete the old one
                dataset.erase(it);
                // insert the new one
                dataset.insert( writeEntry);
            }
        }

    // write set in to the file
    file << "hh_id" << std::setw(20) << "itime" << std::setw(40) << "day"  <<  std::endl;
    for (it=dataset.begin(); it!=dataset.end(); ++it)
        file << (*it).hhID << std::setw(20) << (*it).itime << std::setw(40) << (*it).day  << std::endl;

    file.close();

}

//-----------------------------------------------------------------------------------------------

/*
 * for each HH print visiting times in each period,
 *                   days and weeks of visits
 *                   and ID of the interviewers
 */
// _ITimePlan has a day view
std::set<writeFormat2>  saveHHSchedule_dayview_d(const std::vector<stInterviewer> _interviewer,
                                                 std::vector<std::vector<std::pair<unsigned int, double> > > _ITimePlan)
//                    const std::string fileName)
{
    std::set<writeFormat2> dataset;
    std::set<writeFormat2> ::iterator it;
    std::vector<std::pair<unsigned int, double> >::iterator timeplan_it;
    int itime;

    //    std::ofstream file(fileName.c_str());	// file to open
    //    assert(file.is_open() && "ERROR saveHHSchedule: File cannot be opened to write");


    for (unsigned int i=0; i<_interviewer.size(); ++i)
        for (unsigned int d=0; d< _interviewer[i].routes_days.size(); ++d)
            for (unsigned int h=0; h<_interviewer[i].routes_days[d].households.size(); ++h)
            {
                const unsigned hhID = _interviewer[i].routes_days[d].households[h];

                // find planed interview time for current hh on the day d
                timeplan_it = std::find_if(_ITimePlan[d].begin(),_ITimePlan[d].end(),
                                           [hhID] (std::pair<unsigned int, double> time)
                {return time.first == hhID;});
                itime = round( (*timeplan_it).second);

                writeFormat2 searchEntry;
                searchEntry.hhID = hhID + 10001;

                it = dataset.find(searchEntry);

                if (it==dataset.end())
                {   // make new entry
                    writeFormat2 writeEntry;
                    writeEntry.hhID = hhID + 10001;
                    writeEntry.itime = std::to_string(itime);
                    writeEntry.day = std::to_string(d + 1) + "("
                            + std::to_string(d/5 + 1) + ")";
                    writeEntry.Interviewer = std::to_string(i + 1);

                    dataset.insert( writeEntry);
                }
                else
                {   // make new entry, which complements the already existing entry
                    writeFormat2 writeEntry;

                    writeEntry.hhID = (*it).hhID;
                    writeEntry.itime = (*it).itime + ", " + std::to_string(itime);
                    writeEntry.day = (*it).day + ", " + std::to_string(d + 1)
                            + "(" + std::to_string(d/5 + 1) + ")";
                    writeEntry.Interviewer = (*it).Interviewer + ", " + std::to_string(i + 1);
                    // delete the old one
                    dataset.erase(it);
                    // insert the new one
                    dataset.insert( writeEntry);
                }
            }

    //    // write set in to the file
    //    file << "hh_id" << std::setw(20) << "itime" << std::setw(40)
    //         << "day"  << std::setw(20)  << "Interviewer ID" << std::endl;

    //    for (it=dataset.begin(); it!=dataset.end(); ++it)
    //        file << (*it).hhID << std::setw(20) << (*it).itime << std::setw(40)
    //             << (*it).day  << std::setw(20) << (*it).Interviewer << std::endl;

    //    file.close();
    return dataset;
}

// _ITimePlan has a week view
std::set<writeFormat2> saveHHSchedule_weekview_d(const std::vector<stInterviewer> _interviewer,
                                                 std::vector<std::vector<std::pair<unsigned int, double> > > _ITimePlan) //                    const std::string fileName)
{
    std::set<writeFormat2> dataset;
    std::set<writeFormat2> ::iterator it;
    std::vector<std::pair<unsigned int, double> >::iterator timeplan_it;

    int itime;

    std::vector<std::vector<std::pair<unsigned int, double> > > copyITimePlan; // copy of _ITimePlan
    copyITimePlan.resize(_ITimePlan.size());
    for (unsigned int w=0; w<_ITimePlan.size(); ++w)
        for (unsigned int p=0; p<_ITimePlan[w].size(); ++p )
            copyITimePlan[w].push_back(std::make_pair(_ITimePlan[w][p].first,_ITimePlan[w][p].second));


    //    std::ofstream file(fileName.c_str());	// file to open
    //    assert(file.is_open() && "ERROR saveHHSchedule: File cannot be opened to write");


    for (unsigned int i=0; i<_interviewer.size(); ++i)
        for (unsigned int d=0; d< _interviewer[i].routes_days.size(); ++d)
            for (unsigned int h=0; h<_interviewer[i].routes_days[d].households.size(); ++h)
            {
                const unsigned hhID = _interviewer[i].routes_days[d].households[h];
                // find planed interview time for current hh on the day d


                timeplan_it = std::find_if(copyITimePlan[d/5].begin(),copyITimePlan[d/5].end(),
                                           [hhID] (std::pair<unsigned int, double> weekday)
                                                {return weekday.first == hhID;});
                itime = round( (*timeplan_it).second);
                copyITimePlan[d/5].erase(timeplan_it);

                writeFormat2 searchEntry;
                searchEntry.hhID = hhID + 10001;

                it = dataset.find(searchEntry);

                if (it==dataset.end())
                {   // make new entry
                    writeFormat2 writeEntry;
                    writeEntry.hhID = hhID + 10001;
                    writeEntry.itime = std::to_string(itime);
                    writeEntry.day = std::to_string(d + 1) + "("
                            + std::to_string(d/5 + 1) + ")";
                    writeEntry.Interviewer = std::to_string(i + 1);

                    dataset.insert( writeEntry);
                }
                else
                {   // make new entry, which complements the already existing entry
                    writeFormat2 writeEntry;

                    writeEntry.hhID = (*it).hhID;
                    writeEntry.itime = (*it).itime + ", " + std::to_string(itime);
                    writeEntry.day = (*it).day + ", " + std::to_string(d + 1)
                            + "(" + std::to_string(d/5 + 1) + ")";
                    writeEntry.Interviewer = (*it).Interviewer + ", " + std::to_string(i + 1);
                    // delete the old one
                    dataset.erase(it);
                    // insert the new one
                    dataset.insert( writeEntry);
                }

            }

    //    // write set in to the file
    //    file << "hh_id" << std::setw(20) << "itime" << std::setw(40)
    //         << "day"  << std::setw(20)  << "Interviewer ID" << std::endl;

    //    for (it=dataset.begin(); it!=dataset.end(); ++it)
    //        file << (*it).hhID << std::setw(20) << (*it).itime << std::setw(40)
    //             << (*it).day  << std::setw(20) << (*it).Interviewer << std::endl;

    //    file.close();
    return dataset;
}

std::set<writeFormat2>  saveHHSchedule2_w(const std::vector<stInterviewer> _interviewer,
                                          std::vector<std::vector<std::pair<unsigned int, double> > > _ITimePlan,
                                          const std::string fileName)
{
    std::set<writeFormat2> dataset;
    std::set<writeFormat2> ::iterator it;
    std::vector<std::pair<unsigned int, double> >::iterator timeplan_it;
    int itime;

    std::ofstream file(fileName.c_str());	// file to open
    assert(file.is_open() && "ERROR saveHHSchedule: File cannot be opened to write");


    for (unsigned int i=0; i<_interviewer.size(); ++i)
        for (unsigned int w=0; w< _interviewer[i].routes_weeks.size(); ++w)
            for (unsigned int h=0; h<_interviewer[i].routes_weeks[w].households.size(); ++h)
            {
                const unsigned hhID = _interviewer[i].routes_weeks[w].households[h];

                // find planed interview time for current hh on the day d
                timeplan_it = std::find_if(_ITimePlan[w].begin(),_ITimePlan[w].end(),
                                           [hhID] (std::pair<unsigned int, double> time)
                {return time.first == hhID;});
                itime = (*timeplan_it).second;

                writeFormat2 searchEntry;
                searchEntry.hhID = hhID + 10001;

                it = dataset.find(searchEntry);

                if (it==dataset.end())
                {   // make new entry
                    writeFormat2 writeEntry;
                    writeEntry.hhID = hhID + 10001;
                    writeEntry.itime = std::to_string(itime);
                    writeEntry.day = std::to_string(w + 1);
                    writeEntry.Interviewer = std::to_string(i + 1);

                    dataset.insert( writeEntry);
                }
                else
                {   // make new entry, which complements the already existing entry
                    writeFormat2 writeEntry;

                    writeEntry.hhID = (*it).hhID;
                    writeEntry.itime = (*it).itime + ", " + std::to_string(itime);
                    writeEntry.day = (*it).day + ", " + std::to_string(w + 1);
                    writeEntry.Interviewer = (*it).Interviewer + ", " + std::to_string(i + 1);
                    // delete the old one
                    dataset.erase(it);
                    // insert the new one
                    dataset.insert( writeEntry);
                }
            }

    // write set in to the file
    file << "hh_id" << std::setw(20) << "itime" << std::setw(40)
         << "week"  << std::setw(20)  << "Interviewer ID" << std::endl;

    for (it=dataset.begin(); it!=dataset.end(); ++it)
        file << (*it).hhID << std::setw(20) << (*it).itime << std::setw(40)
             << (*it).day  << std::setw(20) << (*it).Interviewer << std::endl;

    file.close();
    return dataset;
}
