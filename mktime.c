#include <time.h>
#include "Python.h"

const size_t YEARS[] = {0, 31536000, 63072000, 94694400, 126230400, 157766400, 189302400, 220924800, 252460800, 283996800, 315532800, 347155200, 378691200, 410227200, 441763200, 473385600, 504921600, 536457600, 567993600, 599616000, 631152000, 662688000, 694224000, 725846400, 757382400, 788918400, 820454400, 852076800, 883612800, 915148800, 946684800, 978307200, 1009843200, 1041379200, 1072915200, 1104537600, 1136073600, 1167609600, 1199145600, 1230768000, 1262304000, 1293840000, 1325376000, 1356998400, 1388534400, 1420070400, 1451606400, 1483228800, 1514764800, 1546300800, 1577836800, 1609459200, 1640995200, 1672531200, 1704067200, 1735689600, 1767225600, 1798761600, 1830297600, 1861920000, 1893456000, 1924992000, 1956528000, 1988150400, 2019686400, 2051222400, 2082758400, 2114380800, 2145916800};
const size_t MONTHS1[] = {0, 0, 2678400,5097600, 7776000, 10368000, 13046400, 15638400, 18316800, 20995200, 23587200, 26265600, 28857600, 31536000};
const size_t MONTHS2[] = {0, 0, 2678400, 5184000, 7862400, 10454400, 13132800, 15724800, 18403200, 21081600, 23673600, 26352000, 28944000, 31622400};
const size_t DAYS[] = {0, 0, 86400, 172800, 259200, 345600, 432000, 518400, 604800, 691200, 777600, 864000, 950400, 1036800, 1123200, 1209600, 1296000, 1382400, 1468800, 1555200, 1641600, 1728000, 1814400, 1900800, 1987200, 2073600, 2160000, 2246400, 2332800, 2419200, 2505600, 2592000, 2678400};
const size_t HOURS[] = {0, 3600, 7200, 10800, 14400, 18000, 21600, 25200, 28800, 32400, 36000, 39600, 43200, 46800, 50400, 54000, 57600, 61200, 64800, 68400, 72000, 75600, 79200, 82800, 86400};
const size_t MINUTES[] = {0, 60, 120, 180, 240, 300, 360, 420, 480, 540, 600, 660, 720, 780, 840, 900, 960, 1020, 1080, 1140, 1200, 1260, 1320, 1380, 1440, 1500, 1560, 1620, 1680, 1740, 1800, 1860, 1920, 1980, 2040, 2100, 2160, 2220, 2280, 2340, 2400, 2460, 2520, 2580, 2640, 2700, 2760, 2820, 2880, 2940, 3000, 3060, 3120, 3180, 3240, 3300, 3360, 3420, 3480, 3540, 3600};

#define ATOI(digitBuffer, bEnd, num) \
	num = 0, multiplier = 1, bPtr = bEnd - 1;\
	while (bPtr >= digitBuffer) {\
		num += (*bPtr - '0') * multiplier;\
		multiplier *= 10;\
		bPtr--;\
	}

#define PROCESS_TIME(METHOD) \
	size_t index = 0, num, multiplier;\
	PyObject *tuple = PyTuple_New(9);\
	while (*s) {\
		if (*s >= '0' && *s <= '9') {\
			if (d == NULL) d = digitBuffer;\
			if (d >= dEnd) { Py_DECREF(tuple); return NULL; }\
			*d++ = *s;\
			if (s[1] < '0' || s[1] > '9') {\
				*d = 0;\
				ATOI(digitBuffer, d, num);\
				PyTuple_SET_ITEM(tuple, index++, PyInt_FromSize_t(num));\
				if (index >= 9) return tuple;\
				d = NULL;\
			}\
		}\
		s++;\
	}\
	while (index < 9) {\
		PyTuple_SET_ITEM(tuple, index++, PyInt_FromSize_t(0));\
	}\
	return tuple;

static PyObject*
_split_u(PyObject* self, PyObject* arg) {
	Py_UNICODE *src = PyUnicode_AS_UNICODE(arg), *s = src, digitBuffer[32], *d = digitBuffer, *dEnd = digitBuffer + 32, *bPtr;

	PROCESS_TIME(PyInt_FromUnicode);
}

static PyObject*
_split(PyObject* self, PyObject* arg) {
	char *src = PyString_AS_STRING(arg), *s = src, digitBuffer[32], *d = digitBuffer, *dEnd = digitBuffer + 32, *bPtr;

	PROCESS_TIME(PyInt_FromString);
}

#define IS_LEAP(y) ((((y) % 4) == 0 && ((y) % 100) != 0) || ((y) % 400) == 0)

#define PROCESS_MKTIME \
	size_t index = 0, num, multiplier;\
	while (*s) {\
		if (*s >= '0' && *s <= '9') {\
			if (d == NULL) d = digitBuffer;\
			if (d >= dEnd) { return NULL; }\
			*d++ = *s;\
			if (s[1] < '0' || s[1] > '9') {\
				*d = 0;\
				ATOI(digitBuffer, d, num);\
				tbuf[index++] = num; \
				if (index >= 6) break;\
				d = NULL;\
			}\
		}\
		s++;\
	}\
	if (*tbuf < 1970 || *tbuf > 2038 || tbuf[1] > 13 || tbuf[2] > 32 || tbuf[3] > 24 || tbuf[4] > 60 || tbuf[5] > 60) \
		return NULL;\
	timestamp = YEARS[*tbuf - 1970] + (IS_LEAP(*tbuf) ? MONTHS2[tbuf[1]] : MONTHS1[tbuf[1]]) + \
		DAYS[tbuf[2]] + HOURS[tbuf[3]] + MINUTES[tbuf[4]] + tbuf[5];\
	return PyInt_FromSize_t(timestamp);

static PyObject*
_mktime(PyObject* self, PyObject* arg) {
	char *src = PyString_AS_STRING(arg), *s = src, digitBuffer[32], *d = digitBuffer, *dEnd = digitBuffer + 32, *bPtr;
	size_t tbuf[] = {0, 0, 0, 0, 0, 0}, timestamp = 0;

	PROCESS_MKTIME;
}

static PyObject*
_mktime_u(PyObject* self, PyObject* arg) {
	Py_UNICODE *src = PyUnicode_AS_UNICODE(arg), *s = src, digitBuffer[32], *d = digitBuffer, *dEnd = digitBuffer + 32, *bPtr;
	size_t tbuf[] = {0, 0, 0, 0, 0, 0}, timestamp = 0;

	PROCESS_MKTIME;
}

static PyObject*
_mktime_tuple(PyObject* self, PyObject* arg) {
	if (!PyTuple_CheckExact(arg)) return NULL;
	PyTupleObject* tuple = (PyTupleObject*) arg;
	size_t year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0, size = tuple->ob_size, timestamp = 0;

	if (size > 0) year = PyInt_AsSsize_t(tuple->ob_item[0]);
	if (size > 1) month = PyInt_AsSsize_t(tuple->ob_item[1]);
	if (size > 2) day = PyInt_AsSsize_t(tuple->ob_item[2]);
	if (size > 3) hour = PyInt_AsSsize_t(tuple->ob_item[3]);
	if (size > 4) minute = PyInt_AsSsize_t(tuple->ob_item[4]);
	if (size > 5) second = PyInt_AsSsize_t(tuple->ob_item[5]);

	if (year < 1970 || year > 2038 || month > 13 || day > 32 || hour > 24 || minute > 60 || second > 60) {
		Py_RETURN_NONE;
	}

	timestamp = YEARS[year - 1970] + (IS_LEAP(year) ? MONTHS2[month] : MONTHS1[month]) +
		DAYS[day] + HOURS[hour] + MINUTES[minute] + second;

	return PyInt_FromSize_t(timestamp);
}

static PyObject*
_mktime_ymd(PyObject* self, PyObject* arg) {
	if (!PyUnicode_CheckExact(arg)) return NULL;

	Py_UNICODE *src = PyUnicode_AS_UNICODE(arg), *bPtr;
	size_t year = 0, month = 0, day = 0, timestamp = 0, multiplier, size = PyUnicode_GET_SIZE(arg);

	if (size >= 4) { ATOI(src, src + 4, year); }
	if (size >= 6) { ATOI(src + 4, src + 6, month); }
	if (size >= 8) { ATOI(src + 6, src + 8, day); }

	if (year < 1970 || year > 2038 || month > 13 || day > 32) {
		Py_RETURN_NONE;
	}

	timestamp = YEARS[year - 1970] + (IS_LEAP(year) ? MONTHS2[month] : MONTHS1[month]) + DAYS[day];

	return PyInt_FromSize_t(timestamp);
}

static PyObject*
_time(PyObject* self) {
	return PyInt_FromLong(time(NULL));
}

static PyMethodDef exports[] = {
	{"split_u", (PyCFunction)_split_u, METH_O, "Split unicode time string to time tuple"},
	{"split", (PyCFunction)_split, METH_O, "Split time string to time tuple"},
	{"mktime_u", (PyCFunction)_mktime_u, METH_O, "Split unicode time string to timestamp"},
	{"mktime", (PyCFunction)_mktime, METH_O, "Split time string to timestamp"},
	{"mktime_tuple", (PyCFunction)_mktime_tuple, METH_O, "Split time tuple to timestamp"},
	{"mktime_ymd", (PyCFunction)_mktime_ymd, METH_O, "Split Ymd time unicode string to timestamp"},
	{"time", (PyCFunction)_time, METH_NOARGS, "time as int"},
	{NULL, NULL}
};

PyMODINIT_FUNC
initmktime(void) {
	Py_InitModule("mktime", exports);
}
