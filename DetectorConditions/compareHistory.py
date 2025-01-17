#!/usr/bin/python3

import time
from datetime import datetime, date, timedelta
import sys

def read_LVfile(filename, dates, LV):
    fsd = open(filename, 'r')
    SDlines = fsd.readlines()
    fsd.close()
    for line in SDlines:
        data = line.strip().split()
        curDate = datetime.strptime(data[0], '%Y%m%d')
        dates.append(int(curDate.strftime('%y%m%d')))
        LV[dates[-1]] = float(data[1])
        
    return

#########################################################@

thresh = 0.05

# Getting args
if len(sys.argv)<3:
    print('Usage:', sys.argv[0], ' file1 file2 [thresh]')
    exit()
else:
    file1 = sys.argv[1]
    file2 = sys.argv[2]

if len(sys.argv)>3:
    thresh = float(sys.argv[3])

## Comparing time fractions of 2 different inputs

datesLV1 = []
LV1 = {}
read_LVfile(file1, datesLV1, LV1)
datesLV2 = []
LV2 = {}
read_LVfile(file2, datesLV2, LV2)


print('Found', len(datesLV1), ' days in file', file1)
print('Found', len(datesLV2), ' days in file', file2)

for date in datesLV1:
    if date not in datesLV2:
        print('date ', date, ' not in both files')
    else:
        diff = LV2[date] - LV1[date]
        if abs(diff) > thresh:
        #if diff > thresh:
            print(date, LV1[date], LV2[date])
