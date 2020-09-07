/*
 * CMOS Real-time Clock
 * SKELETON IMPLEMENTATION -- TO BE FILLED IN FOR TASK (3)
 */

/*
 * STUDENT NUMBER: s1704037
 */
#include <infos/drivers/timer/rtc.h>
#include "arch/x86/pio.h"
#define CURRENT_YEAR        2020

using namespace infos::drivers;
using namespace infos::drivers::timer;
using namespace infos::arch::x86;

class CMOSRTC : public RTC {
public:
	static const DeviceClass CMOSRTCDeviceClass;

	const DeviceClass& device_class() const override
	{
		return CMOSRTCDeviceClass;
	}

	/**
	 * Interrogates the RTC to read the current date & time.
	 * @param tp Populates the tp structure with the current data & time, as
	 * given by the CMOS RTC device.
	 */


    int update_in_progress_flag() {
        __outb(0x70, 0x0A);
        return (__inb(0x71) & 0x80);
    }

    unsigned short get_RTC_register(int reg) {
        __outb(0x70, reg);
        return __inb(0x71);
    }

	void read_timepoint(RTCTimePoint& tp) override
	{
		// FILL IN THIS METHOD - WRITE HELPER METHODS IF NECESSARY
        unsigned short previous_second;
        unsigned short previous_minute;
        unsigned short previous_hour;
        unsigned short previous_day;
        unsigned short previous_month;
        unsigned short previous_year;
        unsigned short registerB;


        while (update_in_progress_flag());                // Ensure an update isn't in progress
        tp.seconds = get_RTC_register(0x00);
        tp.minutes = get_RTC_register(0x02);
        tp.hours = get_RTC_register(0x04);
        tp.day_of_month = get_RTC_register(0x07);
        tp.month = get_RTC_register(0x08);
        tp.year = get_RTC_register(0x09);

        do {
            previous_second = tp.seconds;
            previous_minute = tp.minutes;
            previous_hour = tp.hours;
            previous_day = tp.day_of_month;
            previous_month = tp.month;
            previous_year = tp.year;

            while (update_in_progress_flag());
            tp.seconds = get_RTC_register(0x00);
            tp.minutes = get_RTC_register(0x02);
            tp.hours = get_RTC_register(0x04);
            tp.day_of_month = get_RTC_register(0x07);
            tp.month = get_RTC_register(0x08);
            tp.year = get_RTC_register(0x09);

        } while(
                (previous_second != tp.seconds)   ||
                (previous_minute != tp.minutes)   ||
                (previous_hour != tp.hours)       ||
                (previous_day != tp.day_of_month) ||
                (previous_month != tp.month)      ||
                (previous_year != tp.year)
                );

        registerB = get_RTC_register(0x0B);

        // Convert Binary Coded Decimal to binary if necessary

        if (!(registerB & 0x04)) {
            tp.seconds = ((tp.seconds/16) * 10) + (tp.seconds & 0x0F);
            tp.minutes = ((tp.minutes/16) * 10) + (tp.minutes & 0x0F);
            tp.hours = ((((tp.hours & 0x70)/16) * 10) + (tp.hours & 0x0F)) | (tp.hours & 0x80);
            tp.day_of_month = ((tp.day_of_month/16) * 10) + (tp.day_of_month & 0x0F);
            tp.month = ((tp.month/16) * 10) + (tp.month & 0x0F);
            tp.year = ((tp.year/16) * 10) + (tp.year & 0x0F);

        }

        // Convert to 24 hour time

        if (!(registerB & 0x02) && (tp.hours & 0x80)) {
            tp.hours = ((tp.hours & 0x7F) + 12) % 24;
        }

        // Get 4-digit year

            tp.year += (CURRENT_YEAR/100) * 100;
            if(tp.year < CURRENT_YEAR)
                tp.year += 100;


	}
};

const DeviceClass CMOSRTC::CMOSRTCDeviceClass(RTC::RTCDeviceClass, "cmos-rtc");

RegisterDevice(CMOSRTC);
