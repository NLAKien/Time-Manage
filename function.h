#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <cstdio>

using namespace std;

struct Event
{
    string date;
    string title;
    string description;
    string time;
    string location;
};

// Hàm thêm sự kiện mới
void addEvent(vector<Event> &events);
// Hàm xem sk trong tháng
void viewMonthEvents(const vector<Event> &events, const string &month);
// Xem lich thang
void viewCalendar(const vector<Event> &events, int month, int year);
// Xem su kien trong ngay
void viewDayEvents(const vector<Event> &events, const string &date);
// Đọc sk
void loadEvents(vector<Event> &events);
// Hàm tìm kiếm sk
void searchEvent(const vector<Event> &events, const string &keyword);
// Hàm chinh sửa sk
void editEvent(vector<Event> &events, const string &date);
// Hàm xóa sk
void deleteEvent(vector<Event> &events, const string &date);
// Hiển thị nhắc nhở sắp tới
void showUpcomingReminders(const vector<Event> &events, int daysAhead);
// Hàm lưu sự kiện vào file
void saveEvents(const vector<Event> &events);
// Xuất báo cáo tháng (filePath có default)
void exportMonthlyReport(const vector<Event> &events, int month, int year, const string &filePath = "report.txt");

#endif