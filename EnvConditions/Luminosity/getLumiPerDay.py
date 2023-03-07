import os,sys
import time
from datetime import datetime
from datetime import timedelta

startYear,startMonth,startDay = 2021,10,15
endYear,endMonth,endDay = 2021,10,31

startDate=datetime(startYear,startMonth,startDay, 1)
endDate=datetime(endYear,endMonth,endDay,1)

print "START TIME:", startDate.strftime('%d/%m/%y')
print "END TIME:", endDate.strftime('%d/%m/%y')

# launch query for each day
iterDate = startDate
while(iterDate<=endDate):
    year = iterDate.year-2000
    print iterDate.strftime('%y/%m/%d')
    query = 'brilcalc lumi -c web --begin "'+iterDate.strftime('%m/%d/%y')+' 00:00:00" --end "'+iterDate.strftime('%m/%d/%y')+' 23:59:59" > lumi/lumi_'+iterDate.strftime('%y%m%d')+'.txt'
    # Needed to split lumi of runs over 2 days
    print query
    os.system(query)
    iterDate += timedelta(days=1)
