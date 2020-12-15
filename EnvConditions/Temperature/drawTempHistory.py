#!/usr/bin/python

from array import array
import os,sys,fnmatch
from ROOT import *
import time
from datetime import datetime
from datetime import timedelta
from numpy import median

#Simulation start and end dates:
startYear,startMonth,startDay = 2013,1,1
endYear,endMonth,endDay = 2013,12,31

startDate=datetime(startYear,startMonth,startDay, 1)
endDate=datetime(endYear,endMonth,endDay,1)

print "START TIME:", startDate.strftime('%d/%m/%y')
print "END TIME:", endDate.strftime('%d/%m/%y')
print "       time ranging over", endDate.toordinal()-startDate.toordinal()+1, "days"

#Reading PLC readings
fPLC_TSil={}
PLClines_TSil={}
print "READING TSIL FILES ..."
fPLC_TSil[1] = open('./Data/TIB_minus_1.1.1_TLIQ1_2009-2020.csv', 'rU')
fPLC_TSil[2] = open('./Data/TIB_minus_1.1.1_TLIQ2_2009-2020.csv', 'rU')
fPLC_TSil[3] = open('./Data/TOB_plus_1.1.1.2_TLIQ_2009-2020-modif.csv', 'rU')
fPLC_TSil[4] = open('./Data/TOB_plus_1.1.1.3_TLIQ_2009-2020.csv', 'rU')


for file in fPLC_TSil.keys():
    PLClines_TSil[file] = fPLC_TSil[file].readlines()
    fPLC_TSil[file].close()

print "APPENDING TSIL READINGS FOR EACH DAY ..."
datesTempForGraph = []
PLCtempsTSilForGraph = {}
PLCtemptimesTSilForGraph = {}
for file in PLClines_TSil.keys():
    PLCtempsTSilForGraph[file] = []
    PLCtemptimesTSilForGraph[file] = []
for file in PLClines_TSil.keys():
    for line in PLClines_TSil[file]: #append TSil readings for each day to average them later
        data = line.strip().split('"')
        if len(data)<4: continue
        if not data[1].strip().isdigit(): continue
        datenow = datetime.fromtimestamp(int(data[1].strip()[:-3]))
        #date = int(datenow.strftime('%y%m%d'))
        date = datenow.toordinal()
        datesTempForGraph.append(date)
        #if file==1: print date, data[3].strip()
        PLCtempsTSilForGraph[file].append(float(data[3].strip()))
        PLCtemptimesTSilForGraph[file].append(date)
print "       found temperature information for", len(datesTempForGraph), "days"

c = TCanvas("c","",1200,400)
graphs = {}
graph_avg = TGraph()
tref = datetime(1970, 01, 01)
for file in PLClines_TSil.keys():
    graphs[file] = TGraph()
    ipt=0
    for ip in range(0,len(PLCtempsTSilForGraph[file])):
        if PLCtemptimesTSilForGraph[file][ip] > startDate.toordinal() and PLCtemptimesTSilForGraph[file][ip] < endDate.toordinal() and PLCtempsTSilForGraph[file][ip]>-25 and PLCtempsTSilForGraph[file][ip]<30:
            graphs[file].SetPoint(ipt, (PLCtemptimesTSilForGraph[file][ip]-tref.toordinal())*86400, PLCtempsTSilForGraph[file][ip])
            ipt+=1
    #if file==1: print ip, PLCtemptimesTSilForGraph[file][ip], PLCtempsTSilForGraph[file][ip]
    graphs[file].SetMarkerStyle(20+int(file))
    graphs[file].SetMarkerColor(int(file))
    graphs[file].SetLineColor(int(file))


##################################################################################################
# Copy-paste of writeLumiTempScenario.py

#initialize dictionaries
dates = []
PLCtempsTSil = {}
PLCtemptimesTSil = {}
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


print "AVERAGING TEMPERATURE OVER DAY ..."
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


print "AVERAGING TEMPERATURE OVER FILES ..."
ipt=0
for i in range(len(dates)):
        today  = dates[i]
        today_date = datetime(today/10000+2000, (today%10000)/100, today%100, 1)
        year=today/10000+2000
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
                nval = len(PLClines_TSil)
                tempavg /= nval
                tempmedian = median(values)
                    
                if tempdiff<-2 or tempdiff>2 :
                    print ' Temperature difference '+str(today)+' '+str(tempdiff)+' '+str(tempmedian)
                    for file in PLClines_TSil.keys(): print TrackerSDTempAvgTSil[today][file]
                    print '  median:', tempmedian
                    tempnow=tempmedian
                else:
                    tempnow=tempavg
                if year>=2010 and year <=2012:
                    if abs(tempnow-4)<2.: tempnow=4
                if year>=2015 and year <=2017:
                    if abs(tempnow+15)<1.5: tempnow=-15
                if year>=2018:
                    if abs(tempnow+20)<1.5: tempnow=-20
                graph_avg.SetPoint(ipt ,(today_date.toordinal()-tref.toordinal())*86400, tempnow)
                #print ipt, today, tempnow
                ipt+=1


for file in PLClines_TSil.keys():
    if file==1:
        graphs[file].Draw('APL')
        graphs[file].GetXaxis().SetTimeDisplay(1)
        graphs[file].GetXaxis().SetTimeFormat("%F1970-01-01 00:00:00")
    else:
        graphs[file].Draw('PL')
        print file, graphs[file].GetMarkerColor()
graph_avg.SetMarkerStyle(20)
graph_avg.SetMarkerSize(0.5)
graph_avg.SetMarkerColor(6)
graph_avg.SetLineColor(6)
graph_avg.Draw('PL')

c.Print('temp_history.png')

for file in fPLC_TSil.keys():
    print file



