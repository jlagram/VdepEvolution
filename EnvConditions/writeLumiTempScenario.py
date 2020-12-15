#!/usr/bin/python

from array import array
import os,sys,fnmatch
from ROOT import *
import time
from datetime import datetime
from datetime import timedelta
from numpy import median


def tempInYear(year):
    if year_int>=2010 and year_int <=2012:
        return 4
    if year_int>=2015 and year_int <=2017:
        return -15
    if year_int>=2018:
        return -20
    return -99

##############################################################################

#Simulation start and end dates:
startYear,startMonth,startDay = 2010,3,30#2010,3,30
endYear,endMonth,endDay = 2020,12,31

###############################################################################

startDate=datetime(startYear,startMonth,startDay, 1)
#start=startDate.timestamp()
endDate=datetime(endYear,endMonth,endDay,1)
#end=endDate.timestamp()

print "START TIME:", startDate.strftime('%d/%m/%y')
print "END TIME:", endDate.strftime('%d/%m/%y')
print "       time ranging over", endDate.toordinal()-startDate.toordinal()+1, "days"

#Reading lumi file
datesLumi = []
lumi   = {}
print "READING LUMI ..."
fLumi = open('./Luminosity/Lumi.txt', 'rU')
lumilines = fLumi.readlines()
fLumi.close()
for line in lumilines: #append lumi
    data = line.strip().split()
    date = datetime.fromtimestamp(int(data[1]))
    datesLumi.append(int(date.strftime('%y%m%d')))
    lumi[datesLumi[-1]] = float(data[2])
print "       found lumi information for", len(datesLumi), "days"

#Reading PLC readings
fPLC_TSil={}
PLClines_TSil={}
print "READING TSIL FILES ..."
fPLC_TSil[1] = open('./Temperature/Data/TIB_minus_1.1.1_TLIQ1_2009-2020.csv', 'rU')
fPLC_TSil[2] = open('./Temperature/Data//TIB_minus_1.1.1_TLIQ2_2009-2020.csv', 'rU')
fPLC_TSil[3] = open('./Temperature/Data//TOB_plus_1.1.1.2_TLIQ_2009-2020-modif.csv', 'rU')
fPLC_TSil[4] = open('./Temperature/Data//TOB_plus_1.1.1.3_TLIQ_2009-2020.csv', 'rU')
for file in fPLC_TSil.keys():
    PLClines_TSil[file] = fPLC_TSil[file].readlines()
    fPLC_TSil[file].close()


#initialize dictionaries
dates = []
PLCtempsTSil = {}
PLCtemptimesTSil = {}
isTrackerSD = {}
TrackerSDTempAvgTAir = {}
TrackerSDTempAvgTSil = {}
iterDate = startDate
while(iterDate<=endDate):
    date = int(iterDate.strftime('%y%m%d'))
    dates.append(date)
    PLCtempsTSil[dates[-1]] = {}
    PLCtemptimesTSil[dates[-1]] = {}
    TrackerSDTempAvgTSil[dates[-1]] = {}
    for file in PLClines_TSil.keys():
        PLCtempsTSil[dates[-1]][file] = []
        PLCtemptimesTSil[dates[-1]][file] = []
    iterDate += timedelta(days=1)


print "APPENDING TSIL READINGS FOR EACH DAY ..."
datesTemp = []
for file in PLClines_TSil.keys():
    for line in PLClines_TSil[file]: #append TSil readings for each day to average them later
        data = line.strip().split('"')
        if len(data)<4: continue
        if not data[1].strip().isdigit(): continue
        datenow = datetime.fromtimestamp(int(data[1].strip()[:-3]))
        date = int(datenow.strftime('%y%m%d'))
        if date not in dates:  continue
        datesTemp.append(date)
        PLCtempsTSil[date][file].append(float(data[3].strip()))
        PLCtemptimesTSil[date][file].append(int(data[1].strip()[:-3]))
print "       found temperature information for", len(datesTemp), "days"


print "AVERAGING TEMPERATURE AND GETTING S-D DAYS ..."
for i in range(len(dates)):
    date = dates[i]
    avgTempTSil = {}
    counterTSil = {}
    for file in PLClines_TSil.keys():
        avgTempTSil[file] = 0.
        counterTSil[file] = 0
        if date in datesTemp:
            for temp in PLCtempsTSil[date][file]:
                if temp<30. and temp>-25.:
                    avgTempTSil[file]+=temp
                    counterTSil[file]+=1
        if counterTSil[file]!=0: TrackerSDTempAvgTSil[date][file]=avgTempTSil[file]/counterTSil[file]
        else:
            day = datetime(date/10000+2000, (date%10000)/100, date%100, 1)
            dayBefore = day - timedelta(days=1)
            try: TrackerSDTempAvgTSil[date][file]=TrackerSDTempAvgTSil[int(dayBefore.strftime('%y%m%d'))][file]
            except: TrackerSDTempAvgTSil[date][file]=13
print "       interpolated/averaged temperature information for", len(TrackerSDTempAvgTSil), "days"


# Collision energy
sqrts = {'2010':7, '2011':7, '2012':8, '2013':2.6 ,'2014':0, '2015':13 ,'2016':13, '2017':13, '2018':13, '2019':0, '2020':0}


c = TCanvas("c","",1200,400)
graph_temp = TGraph()
graph_lumi = TGraph()
graph_repl = TGraph()
tref = datetime(1970, 01, 01)

#prepare the simulation input files
totDays = 0
sdDays  = 0
year = '2010'
previous_year = '2010'
iday = 1
idayInYear = 89 # when starting the 31/03/10
ipt=0
print "WRITING INPUT FILE ..."
with open('LumiPerDay.txt','w') as fout:
    #for i in range((end-start)/86400+1): #(start,end,86400)
    iterDate = startDate
    while(iterDate<=endDate):
        today = int(iterDate.strftime('%y%m%d'))
        year = iterDate.strftime('%Y')
        year_int = int(year)
        if year != previous_year:
            idayInYear=1
            fout.write('# Starting new year: '+year+'\n')
        luminow=0.
        if today in datesLumi:
            luminow=lumi[today]
        tempnow=0.
        values = []
        if today in TrackerSDTempAvgTSil.keys():
            if len(TrackerSDTempAvgTSil[today])>1:
                min=99.
                max=-99.
                tempavg = 0.
                for file in PLClines_TSil.keys():
                    if TrackerSDTempAvgTSil[today][file]<min : min = TrackerSDTempAvgTSil[today][file]
                    if TrackerSDTempAvgTSil[today][file]>max : max = TrackerSDTempAvgTSil[today][file]
                    values.append(TrackerSDTempAvgTSil[today][file])
                    tempavg += TrackerSDTempAvgTSil[today][file]
                tempdiff = max-min
                tempavg /= len(PLClines_TSil)
                tempmedian = median(values)
                if tempdiff<-2 or tempdiff>2 :
                    print ' Temperature difference '+str(today)+' '+str(tempdiff)+' '+str(tempmedian)
                    for file in PLClines_TSil.keys(): print TrackerSDTempAvgTSil[today][file]
                    tempnow=tempmedian
                else:
                    tempnow=tempavg
                if year_int>=2010 and year_int <=2012:
                    if abs(tempnow-4)<2.: tempnow=4
                if year_int>=2015 and year_int <=2017:
                    if abs(tempnow+15)<1.5: tempnow=-15
                if year_int>=2018:
                    if abs(tempnow+20)<1.5: tempnow=-20
    
        sqrtsnow=0
        if luminow!=0:
            sqrtsnow = sqrts[year]
        #strToWrite = str(today)+'\t'+str(timenow)+'\t'+str(luminow)+'\t'+str(tempnow)+'\n'
        #strToWrite = str(today)+'\t'+str(iday)+'\t'+str(idayInYear)+'\t'+str(sqrtsnow)+'\t'+str(luminow/1000000.)+'\t'+str(tempnow)+'\n'
        strToWrite = str(today)+'\t'+str(iday)+'\t'+str(idayInYear)+'\t'+str(sqrtsnow)+'\t'+str(luminow/1000000.)+'\t'+str(tempnow)+'\n'
        if luminow>0 and tempnow>tempInYear(year)+5:
            print 'WARNING : replacing high temperature during collision in day, ', today, ': ', tempnow, 'oC - ', luminow, 'ub-1'
            strToWrite = str(today)+'\t'+str(iday)+'\t'+str(idayInYear)+'\t'+str(sqrtsnow)+'\t'+str(luminow/1000000.)+'\t'+str(tempInYear(year))+'\n'
            graph_repl.SetPoint(ipt, iday*86400, tempInYear(year))
            ipt+=1
        graph_temp.SetPoint(iday, iday*86400, tempnow) #(today-tref.toordinal())*86400
        graph_lumi.SetPoint(iday, iday*86400, luminow/50000.)
        #sdDays+=1
        totDays+=1
        fout.write(strToWrite)
        previous_year = year
        iterDate += timedelta(days=1)
        iday+=1
        idayInYear+=1
print "      ",sdDays,"days were shut-down out of",totDays,"days"

graph_temp.SetMarkerStyle(21)
graph_temp.SetMarkerColor(6)
graph_temp.SetLineColor(6)
graph_temp.GetXaxis().SetTimeDisplay(1)
graph_temp.GetXaxis().SetTimeFormat("%F2010-03-30 00:00:00")
graph_temp.Draw('APL')
graph_repl.SetMarkerStyle(21)
graph_repl.SetMarkerColor(2)
graph_repl.Draw('P')
graph_lumi.SetMarkerStyle(20)
graph_lumi.SetMarkerColor(4)
graph_lumi.SetLineColor(4)
graph_lumi.Draw('P')
c.Print('scenario.png')

