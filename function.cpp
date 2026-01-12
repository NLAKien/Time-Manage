#include "function.h"

void addEvent(vector<Event> &events)
{
    Event newEvent;
    cout << "Nhap ngay (dd/mm/yyyy): ";
    cin >> newEvent.date;
    cout << "Nhap tieu de: ";
    cin.ignore();
    getline(cin, newEvent.title);
    cout << "Nhap mo ta: ";
    getline(cin, newEvent.description);
    cout << "Nhap thoi gian: ";
    getline(cin, newEvent.time);
    cout << "Nhap dia diem: ";
    getline(cin, newEvent.location);

    events.push_back(newEvent);
    cout << "Su kien da duoc them thanh cong!" << endl;

    ofstream file("events.dat", ios::app);
    if (file.is_open())
    {
        file << newEvent.date << "," << newEvent.title << "," << newEvent.description << "," << newEvent.time << "," << newEvent.location << endl;
        file.close();
    }
}

void viewMonthEvents(const vector<Event> &events, const string &month)
{
    cout << "\nDanh sach su kien trong thang " << month << ":\n";
    for (const auto &event : events)
    {
        if (event.date.size() >= 10 && event.date.substr(3, 2) == month)
        {
            cout << "Ngay: " << event.date << " | Tieu de: " << event.title << endl;
        }
    }
}

static bool hasEventOnDate(const vector<Event> &events, int day, int month, int year)
{
    char buf[11];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d", day, month, year);
    string dateStr(buf);
    for (const auto &e : events)
    {
        if (e.date == dateStr)
            return true;
    }
    return false;
}

void viewCalendar(const vector<Event> &events, int month, int year)
{
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    if (leap)
        daysInMonth[2] = 29;

    tm time_in = {0, 0, 0, 1, month - 1, year - 1900};
    time_t first_day = mktime(&time_in);
    tm const *time_out = localtime(&first_day);
    int start_wday = time_out->tm_wday;

    time_t now = time(nullptr);
    tm *lt = localtime(&now);
    int today_d = lt->tm_mday;
    int today_m = lt->tm_mon + 1;
    int today_y = lt->tm_year + 1900;

    int count = 0;
    for (const auto &e : events)
    {
        if (e.date.size() >= 10)
        {
            int mm = stoi(e.date.substr(3, 2));
            int yy = stoi(e.date.substr(6, 4));
            if (mm == month && yy == year)
                ++count;
        }
    }

    cout << "Thang " << month << " co " << count << " su kien" << "\n";
    cout << "\n             " << setfill(' ') << setw(2) << month << "/" << year << "\n";
    cout << setw(3) << 'S' << ' ' << setw(4) << 'M' << ' ' << setw(4) << 'T' << ' ' << setw(4) << 'W' << ' ' << setw(4) << 'T' << ' ' << setw(4) << 'F' << ' ' << setw(4) << 'S' << "\n";

    int day = 1;
    for (int i = 0; i < start_wday; ++i)
        cout << "    ";

    while (day <= daysInMonth[month])
    {
        bool has = hasEventOnDate(events, day, month, year);
        bool isToday = (day == today_d && month == today_m && year == today_y);
        char lead = isToday ? '>' : ' ';
        cout << lead << setw(2) << setfill(' ') << day << (has ? "*" : " ");

        if ((start_wday + day) % 7 == 0)
            cout << "\n";
        else
            cout << " ";

        ++day;
    }
    cout << "\n";
}

void viewDayEvents(const vector<Event> &events, const string &date)
{
    cout << "\nDanh sach su kien cho " << date << ":\n";
    bool found = false;
    for (const auto &e : events)
    {
        if (e.date == date)
        {
            found = true;
            cout << "Tieu de: " << e.title << "\n";
            cout << "Mo ta: " << e.description << "\n";
            cout << "Thoi gian: " << e.time << "\n";
            cout << "Dia diem: " << e.location << "\n";
            cout << "--------------------------\n";
        }
    }
    if (!found)
        cout << "Khong co su kien nao tren ngay nay.\n";
}

void loadEvents(vector<Event> &events)
{
    ifstream file("events.dat");
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        Event event;
        getline(ss, event.date, ',');
        getline(ss, event.title, ',');
        getline(ss, event.description, ',');
        getline(ss, event.time, ',');
        getline(ss, event.location);
        events.push_back(event);
    }
}

static bool parseDateToTimeT(const string &s, time_t &out)
{
    if (s.size() < 10)
        return false;
    try
    {
        int dd = stoi(s.substr(0, 2));
        int mm = stoi(s.substr(3, 2));
        int yy = stoi(s.substr(6, 4));
        std::tm tm = {};
        tm.tm_mday = dd;
        tm.tm_mon = mm - 1;
        tm.tm_year = yy - 1900;
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        time_t t = mktime(&tm);
        if (t == -1)
            return false;
        out = t;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void sortEventsByDate(vector<Event> &events)
{
    stable_sort(events.begin(), events.end(), [](const Event &a, const Event &b)
                {
        time_t ta = 0, tb = 0;
        if (!parseDateToTimeT(a.date, ta)) ta = 0;
        if (!parseDateToTimeT(b.date, tb)) tb = 0;
        if (ta != tb) return ta < tb;
        return a.time < b.time; });
}

string getEventCategory(const Event &e)
{
    string loc = e.location;
    std::transform(loc.begin(), loc.end(), loc.begin(), [](unsigned char c)
                   { return std::tolower(c); });
    if (loc.find("school") != string::npos)
        return "Hoc tap";
    if (loc.find("home") != string::npos)
        return "Ca nhan";
    return "Cong viec";
}

void showUpcomingReminders(const vector<Event> &events, int daysAhead)
{
    time_t now = time(nullptr);
    tm today_tm = *localtime(&now);
    today_tm.tm_hour = 0;
    today_tm.tm_min = 0;
    today_tm.tm_sec = 0;
    time_t t_today = mktime(&today_tm);
    time_t t_cutoff = t_today + (time_t)daysAhead * 24 * 3600;

    cout << "\n=== Su kien trong " << daysAhead << " ngay sap toi ===\n";

    vector<pair<time_t, Event>> upcoming;
    for (const auto &e : events)
    {
        time_t t_ev = 0;
        if (!parseDateToTimeT(e.date, t_ev))
            continue;
        if (t_ev >= t_today && t_ev <= t_cutoff)
            upcoming.push_back({t_ev, e});
    }

    if (upcoming.empty())
    {
        cout << "Khong co su kien trong " << daysAhead << " ngay sap toi.\n";
        cout << "----------------------------------------------\n";
        return;
    }

    sort(upcoming.begin(), upcoming.end(), [](const pair<time_t, Event> &a, const pair<time_t, Event> &b)
         { return a.first < b.first; });

    for (const auto &p : upcoming)
    {
        int days = (int)((p.first - t_today) / (24 * 3600));
        string when;
        if (days == 0)
            when = "(Today)";
        else if (days == 1)
            when = "(Tomorrow)";
        else
            when = "(" + to_string(days) + " days)";
        tm *ld = localtime(&p.first);
        char buf[11];
        snprintf(buf, sizeof(buf), "%02d/%02d/%04d", ld->tm_mday, ld->tm_mon + 1, ld->tm_year + 1900);
        cout << buf << " " << when << " - " << p.second.title << " at " << p.second.time << ", " << p.second.location << " | Loai: " << getEventCategory(p.second) << "\n";
    }

    cout << "----------------------------------------------\n";
}

void searchEvent(const vector<Event> &events, const string &keyword)
{
    cout << "\nKet qua tim kiem su kien voi tu khoa: " << keyword << "\n";
    vector<Event> matches;
    for (const auto &event : events)
    {
        if (event.title.find(keyword) != string::npos || event.description.find(keyword) != string::npos)
            matches.push_back(event);
    }
    sortEventsByDate(matches);
    for (const auto &event : matches)
    {
        cout << "Ngay: " << event.date << " | Tieu de: " << event.title << " | Loai: " << getEventCategory(event) << endl;
    }
    if (matches.empty())
        cout << "Khong tim thay ket qua nao.\n";
}

void editEvent(vector<Event> &events, const string &date)
{
    for (auto &event : events)
    {
        if (event.date == date)
        {
            cout << "Nhap tieu de moi: ";
            cin.ignore();
            getline(cin, event.title);
            cout << "Nhap mo ta moi: ";
            getline(cin, event.description);
            cout << "Nhap thoi gian moi: ";
            getline(cin, event.time);
            cout << "Nhap dia diem moi: ";
            getline(cin, event.location);
            cout << "Su kien da duoc cap nhat!\n";
            return;
        }
    }
    cout << "Khong tim thay su kien voi ngay " << date << endl;
}

void deleteEvent(vector<Event> &events, const string &date)
{
    for (auto it = events.begin(); it != events.end(); ++it)
    {
        if (it->date == date)
        {
            events.erase(it);
            cout << "Su kien da duoc xoa!\n";
            return;
        }
    }
    cout << "Khong tim thay su kien voi ngay " << date << endl;
}

void saveEvents(const vector<Event> &events)
{
    ofstream file("events.dat");
    for (const auto &event : events)
    {
        file << event.date << "," << event.title << "," << event.description << "," << event.time << "," << event.location << endl;
    }
}

void exportMonthlyReport(const vector<Event> &events, int month, int year, const string &filePath)
{
    vector<Event> list;
    for (const auto &e : events)
    {
        if (e.date.size() >= 10)
        {
            int mm = 0, yy = 0;
            try
            {
                mm = stoi(e.date.substr(3, 2));
                yy = stoi(e.date.substr(6, 4));
            }
            catch (...)
            {
                continue;
            }
            if (mm == month && yy == year)
                list.push_back(e);
        }
    }
    sortEventsByDate(list);

    ofstream out(filePath);
    if (!out.is_open())
    {
        cout << "Khong the mo " << filePath << " de ghi report.\n";
        return;
    }

    out << "Report for " << setw(2) << setfill('0') << month << "/" << year << "\n";
    out << string(30, '=') << "\n\n";
    out << "Events in " << setw(2) << setfill('0') << month << "/" << year << " (" << list.size() << " total):\n\n";

    for (const auto &e : list)
    {
        out << e.date << " - " << e.title << "\n";
        out << "    Time: " << e.time << "\n";
        out << "    Location: " << e.location << "\n";
        out << "    Category: " << getEventCategory(e) << "\n";
        out << "    Description: " << e.description << "\n\n";
    }

    out << "--- End ---\n";
    out.close();
}
