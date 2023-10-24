
#define DomainAssert(expression, message)\
    if (expression) {throw domain_error(message);}


void CheckDateTimeValidity(const DateTime& dt) {
    const auto& year = dt.year;
    const auto& month = dt.mont;
    const auto& day = dt.day;
    const auto& hour = dt.hour;

    DomainAssert(year < 1, "year is too small"s);
    DomainAssert(year > 9999, "year is too big"s);
    DomainAssert(month < 1, "month is too small"s);
    DomainAssert(month > 12, "month is too big"s);

    const bool is_leap_year = (year % 4 == 0) && !(year % 100 == 0 && year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    DomainAssert(day < 1, "day is too small"s);
    DomainAssert(day > month_lengths[month - 1], "day is too big"s);
    DomainAssert(hour < 0, "hour is too small"s);
    DomainAssert(hour > 23, "hour is too big"s);
    DomainAssert(minute < 0, "minute is too small"s);
    DomainAssert(minute > 59, "minute is too big"s);
    DomainAssert(second < 0, "second is too small"s);
    DomainAssert(second > 59, "second is too big"s);
}
