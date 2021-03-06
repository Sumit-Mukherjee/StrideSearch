#include "StrideSearchEventList.h"
#include <sstream>
#include <set>

namespace StrideSearch {

EventList::EventList() : events(std::vector<std::shared_ptr<Event>>()) {};

EventList::EventList(const std::vector<std::shared_ptr<Event>>& eventVec) : events(eventVec) {};

EventList::EventList(const std::vector<std::vector<std::shared_ptr<Event>>>& eventVecs) {
    for (index_type i = 0; i < eventVecs.size(); ++i) {
        for (index_type j = 0; j < eventVecs[i].size(); ++j) {
            events.push_back(eventVecs[i][j]);
        }
    }
}

void EventList::extend(const EventList& other) {
    for (index_type i = 0; i < other.size(); ++i)
        events.push_back(other.events[i]);
}

scalar_type EventList::minSeparationDistance() const {
    scalar_type result = 2.0 * PI * EARTH_RADIUS_KM;
    for (index_type i = 0; i < events.size(); ++i) {
        for (index_type j = i + 1; j < events.size(); ++j) {
            const scalar_type dist = sphereDistance(events[i]->latLon, events[j]->latLon);
            if (dist < result)
                result = dist;
        }
    }
    return result;
}

scalar_type EventList::maxSeparationDistance() const {
    scalar_type result = 0.0;
    for (index_type i = 0; i < events.size(); ++i) {
        for (index_type j = i + 1; j < events.size(); ++j) {
            const scalar_type dist = sphereDistance(events[i]->latLon, events[j]->latLon);
            if (dist > result)
                result = dist;
        }
    }
    return result;
}

std::string EventList::infoString(const int tablevel, const bool printAll) const {
    std::ostringstream ss;
    std::string tabstr;
    for (int i = 0; i < tablevel; ++i)
        tabstr += "\t";
    ss << tabstr << "EventList record:\n";
    ss << tabstr << "\tnEvents = " << size() << std::endl;
    if (printAll) {
        for (index_type i = 0; i < events.size(); ++i)
            ss << events[i]->infoString(tablevel + 1);
    }
    ss << "--------------------------------------" << std::endl;
    return ss.str();
}


void EventList::removeDuplicates(const scalar_type distThreshold) {
    std::vector<bool> duplicates(events.size(), false);
    index_type dupCount = 0;
    for (index_type i = 0; i < events.size(); ++i) {
        if (!duplicates[i]) {
            for (index_type j = i + 1; j < events.size(); ++j) {
                if (!duplicates[j]) {
                    if (events[i]->isDuplicate(*events[j])) {
                        duplicates[j] = true;
                        dupCount += 1;
                    }
                }
            }
        }
    }
    for (index_type i = 0; i < events.size(); ++i) {
        if (!duplicates[i]) {
            for (index_type j = i+1; j < events.size(); ++j) {
                if (!duplicates[j]) {
                    if (events[i]->isRedundant(*events[j], distThreshold)) {
                        dupCount += 1;
                        if (events[i] < events[j])
                            duplicates[i] = true;
                        else
                            duplicates[j] = true;
                    }
                }
            }
        }
    }
    if (dupCount > 0) {
        std::vector<std::shared_ptr<Event>> newEvents;
        for (index_type i = 0; i < events.size(); ++i) {
            if (!duplicates[i]) {
                newEvents.push_back(events[i]);
            }
        }
        events = newEvents;
    }   
}

std::map<DateTime, std::vector<Event>> EventList::separateByDate() const {
    std::map<DateTime, std::vector<Event>> result;
    for (index_type i = 0; i < events.size(); ++i) {
        result[events[i]->datetime].push_back(*events[i]);    
    }
    return result;
}

void EventList::writeASCIIFormatTSTORMS(std::ostream& os) const{
    const DateTime* dt = events[0]->getDateTime();
    os << dt->day << " " << dt->month << " " << dt->year << " " << size() << " " << dt->hour << std::endl;
    for (int i=0; i<events.size(); ++i) {
        os << events[i]->tstormsEntry();
    }
}

void EventList::consolidateRelatedEvents(const scalar_type distThreshold) {
    std::vector<bool> alreadyUsed(events.size(), false);
    for (index_type i = 0; i < events.size(); ++i) {
        if (!alreadyUsed[i]) {
            std::set<std::string> descSet;
            descSet.insert(events[i]->desc);
            for (index_type j = i + 1; j < events.size(); ++j) {
                if (!alreadyUsed[j]) {
                    if (events[i]->isNear(*events[j], distThreshold) && events[i]->datetime == events[j]->datetime) {
                        //
                        //  event i and event j are close enough that they are related
                        //
                        if (descSet.count(events[j]->desc) == 0) {
                            //
                            //  event i is not related to an event of type j already
                            //
                            events[i]->addRelated(events[j]);
                            descSet.insert(events[j]->desc);
                        }
                        else {
                            //
                            //  event i is related to an event of type j already; pick the most intense one, discard the other
                            //
                            if (events[i]->desc == events[j]->desc) {
                                if ( *events[i] < *events[j] ) {
                                    events[i]->latLon = events[j]->latLon;
                                    events[i]->val = events[j]->val;
                                    events[i]->dataIndex = events[j]->dataIndex;
                                    
                                }
                            }
                            else {
                                for (index_type k = 0; k < events[i]->relatedEvents.size(); ++k) {
                                    if (events[i]->relatedEvents[k]->desc == events[j]->desc) {
                                        if ( *(events[i]->relatedEvents[k]) < *events[j] ) {
                                            events[i]->relatedEvents[k]->latLon = events[j]->latLon;
                                            events[i]->relatedEvents[k]->dataIndex = events[j]->dataIndex;
                                            events[i]->relatedEvents[k]->val = events[j]->val;
                                        }
                                    }
                                }
                            }
                        }
                        alreadyUsed[j] = true;
                    }   
                }
            }
        }
    }
    std::vector<std::shared_ptr<Event> > newEvents;
    for (index_type i = 0; i < events.size(); ++i) {
        if (!alreadyUsed[i]) {
            newEvents.push_back(events[i]);
        }
    }
    events = newEvents;
}

void EventList::requireCollocation(const IDCriterion* crit1, const IDCriterion* crit2, const scalar_type distThreshold) {
    std::vector<bool> isCollocated(events.size(), false);
    for (index_type i =0; i < events.size(); ++i) {
        isCollocated[i] = events[i]->isCollocated(crit1, crit2, distThreshold);
    }
    std::vector<std::shared_ptr<Event>> newEvents;
    for (index_type i= 0; i < events.size(); ++i) {
        if (isCollocated[i]) {
            newEvents.push_back(events[i]);
        }
    }
    events = newEvents;
}

}
