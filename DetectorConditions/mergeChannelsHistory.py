#!/usr/bin/python3

import time
from datetime import datetime, date, timedelta
from numpy import median
import sys

def read_LVfile(filename, dates, LV):
    fsd = open(filename, 'r')
    SDlines = fsd.readlines()
    fsd.close()
    for line in SDlines:
        data = line.strip().split()
        curDate = datetime.strptime(data[0], '%Y%m%d')
        dates.append(int(curDate.strftime('%Y%m%d')))
        LV[dates[-1]] = float(data[1])
        #LV[dates[-1]] = round(float(data[1]),2)

    return

#########################################################

# default values of args
type = 'LV'
files = []
outputfile = 'output.txt'

# Getting args
if len(sys.argv)<2:
    print('Usage:', sys.argv[0], 'type')
    print('       type is LV or HV')
    exit()

if len(sys.argv)>1:
    type = sys.argv[1]
    if type != 'LV' and type != 'HV':
        print('type must be LV or HV')
        exit()

## LV
if type == 'LV':
    files = ['LV-Info/FractionOn/LV-history-TIB.txt', 'LV-Info/FractionOn/LV-history-TOB.txt', 'LV-Info/FractionOn/LV-history-TID.txt', 'LV-Info/FractionOn/LV-history-TEC.txt']
    output = 'LV-history-tracker.txt'
## HV
if type == 'HV':
    files = ['HV-Info/FractionOn/HV-history-TIB.txt',  'HV-Info/FractionOn/HV-history-TOB.txt',  'HV-Info/FractionOn/HV-history-TID.txt', 'HV-Info/FractionOn/HV-history-TEC.txt']
    output = 'HV-history-tracker.txt'


dates = []
LVs = []

for i in range(0, len(files)):
    dates.append([])
    LVs.append({})
    read_LVfile(files[i], dates[i], LVs[i])
    print('Found', len(dates[i]), ' days in file', files[i])

# Computing and saving median of channel values for each da
fout = open(output, 'w')
sameval = []
for date in dates[0]:
    val = []
    val.append(LVs[0][date])
    for i in range(1, len(files)):
        if date not in dates[i]:
            print('date ', date, ' missing in file', files[i])
        else:
            val.append(LVs[i][date])
    med = median(val)
    sameval.append(val.count(med))
    if max(val)-min(val) > 0.5 and val.count(med)<3:
        print(date, val, len(val), min(val), max(val), med, val.count(med))
    fout.write(str(date)+' '+'{:.3f}'.format(med)+'\n')
fout.close()

print('0 times: ', sameval.count(0))
print('1 times: ', sameval.count(1))
print('2 times: ', sameval.count(2))
print('3 times: ', sameval.count(3))
print('4 times: ', sameval.count(4))
print('5 times: ', sameval.count(5))
print('6 times: ', sameval.count(6))
