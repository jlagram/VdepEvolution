#!/usr/bin/python3

import time
from datetime import datetime, date, timedelta
import sys


# default values of args
type = 'LV'
inputfile = ''
outputfile = 'history.txt'

# Getting args
if len(sys.argv)<3:
    print('Usage:', sys.argv[0], 'type inputfile [outputfile]')
    print('       type is LV or HV')
    exit()

if len(sys.argv)>1:
    type = sys.argv[1]
    if type != 'LV' and type != 'HV':
        print('type must be LV or HV')
        exit()
if len(sys.argv)>2:
    inputfile = sys.argv[2]
if len(sys.argv)>3:
    outputfile = sys.argv[3]



# Reading voltage readings
print("READING VOLTAGE FILE ...")
fLV = open(inputfile, 'r')
LVlines = fLV.readlines()
fLV.close()

# threshold to remove odd values
HighThresh = 1.3
if type=='HV': HighThresh = 500
# threshold to consider channel ON
OnThresh = 0.6
if type=='HV': OnThresh = 150


LV = {} # time: LV_value
for line in LVlines:
    data = line.strip().split('"')
    if len(data)<4: continue
    if not data[1].strip().isdigit(): continue
    if float(data[3]) > HighThresh:
        print('SKIPPING ODD VALUE:', data)
        continue
    datenow = datetime.fromtimestamp(int(data[1].strip()[:-3]))
    #date = datenow.toordinal()
    day = int(datenow.strftime('%Y%m%d'))
    if day not in LV: LV[day] = {}
    time = float(data[1].strip()[:-3])
    LV[day].update( {time : float(data[3].strip())} )
    #print('# ', datenow.strftime('%Y%m%d'), data[1], data[3] , time, float(data[3]))
    print('# ', datenow.strftime('%Y%m%d'), int(time), ':.3f'.format(float(data[3])))
print("       found LV information for", len(LV), "days")


# Writing output file with time fraction On
fout = open(outputfile, 'w')
LVon = {}
statebefore=0
sorted_days = sorted(LV.keys())
day = sorted_days[0]
previous_date=datetime(int(day/10000), int(day%10000/100), int(day%100)).date()

for day in sorted_days:
    
    current_date = datetime(int(day/10000), int(day%10000/100), int(day%100)).date()
    # taking care of missing days
    if (current_date-previous_date).total_seconds()>86400:
        ndays=int((current_date-previous_date).total_seconds()/86400)
        previous_date+=timedelta(days=1)
        while previous_date<current_date:
            if statebefore<OnThresh:
                fout.write(previous_date.strftime('%Y%m%d')+' 0\n')
            else: fout.write(previous_date.strftime('%Y%m%d')+' 1\n')
            previous_date+=timedelta(days=1)

    if day not in LVon: LVon[day] = 0
    if len(LV[day])==0: continue
    # get time for beginning of the day with last available status
    datetimevalue = datetime.fromtimestamp(list(LV[day].keys())[0])
    startOfDay = datetimevalue.replace(hour=0, minute=0, second=0)
    # loop over times with state infos for the day, skip first one just added
    times = sorted(LV[day].keys())
    print(day, len(times))
    for i in range(0, len(times)):
        datetimevalue = datetime.fromtimestamp(times[i])
        if i==0:
            if statebefore>OnThresh: LVon[day] += (datetimevalue - startOfDay).total_seconds()
            print('   ', i, datetimevalue, statebefore, (datetimevalue - startOfDay).total_seconds(), LVon[day])
        else:
            previous_datetimevalue = datetime.fromtimestamp(times[i-1])
            if LV[day][times[i-1]]>OnThresh: LVon[day] += (datetimevalue - previous_datetimevalue).total_seconds()
            print('   ', i, datetimevalue, LV[day][times[i-1]], (datetimevalue - previous_datetimevalue).total_seconds(), LVon[day])
    # take care of remaining time in day
    endOfDay = datetimevalue.replace(hour=23, minute=59, second=59) + timedelta(seconds=1)
    previous_datetimevalue = datetime.fromtimestamp(times[-1])
    statebefore=LV[day][times[-1]]
    if statebefore>OnThresh: LVon[day] += (endOfDay - previous_datetimevalue).total_seconds()
    print('   ', i, endOfDay, statebefore, (endOfDay - previous_datetimevalue).total_seconds(), LVon[day])
    LVon[day] /= 86400
    print('  fraction:', '{:.3f}'.format(LVon[day]))
    
    # Saving states
    fout.write(str(day)+' '+'{:.3f}'.format(LVon[day])+'\n')

    previous_date=current_date

fout.close()
