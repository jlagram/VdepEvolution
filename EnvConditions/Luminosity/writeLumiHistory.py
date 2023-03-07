import os,sys
import time
from datetime import datetime
from datetime import timedelta

lumiDir='lumi'
outFile='Lumi_temp.txt'

startYear,startMonth,startDay = 2010,3,31
endYear,endMonth,endDay = 2022,12,31

startDate=datetime(startYear,startMonth,startDay, 1)
endDate=datetime(endYear,endMonth,endDay,1)

print("START TIME:", startDate.strftime('%d/%m/%y'))
print("END TIME:", endDate.strftime('%d/%m/%y'))

#initialize dictionaries
dates = []
LumiOfPeriod = {}
LumiOfYear = {}
iterDate = startDate
while(iterDate<=endDate):
    date = int(iterDate.strftime('%y%m%d'))
    dates.append(date)
    LumiOfPeriod[dates[-1]] = 0.
    LumiOfYear[iterDate.year] = 0.
    iterDate += timedelta(days=1)


print('READING INPUT FILES')
dummyTime = datetime(2000, 1, 1, 1)
for date in sorted(LumiOfPeriod.keys()):
    print(date)
    year = date/10000
    month = date%10000/100
    day = date%100
    lumiFile=lumiDir+'/lumi_'+str(date)+'.txt'
    try:
        f = open(lumiFile, 'r')
    except IOError:
        print('Error: no file', lumiFile, '. Skipping day.')
        continue
    for line in f.readlines():
        data = line.strip().split('|')
        #print len(data)
        if len(data)==8:
            if data[3].replace(' ','').isdigit():
                strTime = data[2]
                #print(strTime, data[5])
                time = dummyTime
                try:
                    time = datetime.strptime(strTime, ' %m/%d/%y %H:%M:%S ')
                except ValueError:
                    print('Wrong time format:', strTime)
                if time !=dummyTime:
                    date = int(time.strftime('%y%m%d'))
                    if date in LumiOfPeriod.keys():
                        lumi = float(data[5])*0.001#change of lumi unit in 2022
                        LumiOfPeriod[date]+=lumi
                        LumiOfYear[time.year]+=lumi
                    else:
                        print('day', date, 'not in time range')
    f.close()

print("STORING INFO")
outFile = open(outFile, 'w')
for date in sorted(LumiOfPeriod.keys()):
    year = int(date/10000)
    month = int(date%10000/100)
    day = int(date%100)
    #print date, year, month, day
    time = datetime(2000+year, month, day, 1)
    timestamp = int((time - datetime(1970, 1, 1)).total_seconds())
    strToWrite = time.strftime('%d/%m/%y')+'\t'+str(timestamp)+'\t{:.3e}'.format(LumiOfPeriod[date])
    print(strToWrite)
    outFile.write(strToWrite+'\n')
outFile.close()

print("SUMMARY:")
for year in sorted(LumiOfYear.keys()):
    print(year, LumiOfYear[year], 'ub-1')
