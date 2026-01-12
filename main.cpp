#include "function.h"
#include <ctime>

int main()
{
    vector<Event> events;
    loadEvents(events);

    // Hiển thị nhắc nhở sự kiện sắp tới trong 2 ngày
    // showUpcomingReminders(events, 2);

    saveEvents(events);
    time_t now_report = time(nullptr);
    tm *lt_report = localtime(&now_report);
    int cur_month = lt_report->tm_mon + 1;
    int cur_year = lt_report->tm_year + 1900;
    exportMonthlyReport(events, cur_month, cur_year);
    int choice;
    do
    {
        time_t now = time(nullptr);
        tm *lt = localtime(&now);
        char todayBuf[11];
        snprintf(todayBuf, sizeof(todayBuf), "%02d/%02d/%04d", lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
        cout << "\nHom nay: " << todayBuf << "\n";
        int cur_month = lt->tm_mon + 1;
        int cur_year = lt->tm_year + 1900;
        int month_count = 0;
        for (const auto &e : events)
        {
            if (e.date.size() >= 10)
            {
                int mm = stoi(e.date.substr(3, 2));
                int yy = stoi(e.date.substr(6, 4));
                if (mm == cur_month && yy == cur_year)
                    ++month_count;
            }
        }
        cout << "Thang " << cur_month << " co " << month_count << " su kien" << "\n";
        // Hiển thị nhắc nhở sự kiện sắp tới trong 2 ngày
        showUpcomingReminders(events, 2);

        cout << "\n1. Them su kien moi\n2. Xem lich thang\n3. Xem su kien trong ngay\n4. Tim kiem su kien\n5. Chinh sua su kien\n6. Xoa su kien\n7. Thoat\n";
        cout << "Nhap lua chon: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            addEvent(events);
            break;
        case 2:
        {
            int month, year;
            cout << "Nhap thang (mm): ";
            cin >> month;
            cout << "Nhap nam (yyyy): ";
            cin >> year;
            viewCalendar(events, month, year);

            cout << "\nNhap ngay (dd/mm/yyyy) de xem chi tiet (nhap 0 de bo qua): ";
            string date;
            cin >> date;
            if (date != "0")
                viewDayEvents(events, date);
        }
        break;
        case 3:
        {
            string date;
            cout << "Nhap ngay (dd/mm/yyyy): ";
            cin >> date;
            viewDayEvents(events, date);
        }
        break;
        case 4:
        {
            string keyword;
            cout << "Nhap tu khoa tim kiem: ";
            cin >> keyword;
            searchEvent(events, keyword);
        }
        break;
        case 5:
        {
            string date;
            cout << "Nhap ngay su kien can chinh sua (dd/mm/yyyy): ";
            cin >> date;
            editEvent(events, date);
            saveEvents(events);
        }
        break;
        case 6:
        {
            string date;
            cout << "Nhap ngay su kien can xoa (dd/mm/yyyy): ";
            cin >> date;
            deleteEvent(events, date);
            saveEvents(events);
        }
        break;
        case 7:
            cout << "Thoat chuong trinh.\n";
            break;
        default:
            cout << "Lua chon khong hop le. Vui long chon lai!\n";
        }
    } while (choice != 7);

    return 0;
}
