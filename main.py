import serial
import pandas as pd
import matplotlib
import numpy as np
from matplotlib import pyplot as plt
seria=serial.Serial(port='COM4',baudrate=9600)
data=pd.DataFrame(columns=['Humidity','Temperature','Flame','Time'])

port={
    'Humidity':[],
    'Temperature':[],
    'Time':[],
    'Flame':[]
}
i=0
data.to_csv('data.csv')
while True:
    try:
        port = {
            'Humidity': [],
            'Temperature': [],
            'Time': [],
            'Flame':[]
        }
        ser=seria.readline(1000)
        serStr=str(ser,"UTF-8")
        port['Humidity']=serStr[0:5]
        port['Temperature']=serStr[6:10]
        port['Time']=serStr[13:23]
        port['Flame']=serStr[12:13]
        data.loc[len(data.index)]=[port['Humidity'],port['Temperature'],port['Flame'],port['Time']]
        data.to_csv('new_data.csv')
    except KeyboardInterrupt:
        print(data)
        s=data.index


        temp_analysis = data.sort_values(by=['Temperature'])
        humi_analysis = data.sort_values(by=['Humidity'])
        data.to_csv('new_data.csv')
        print("Following data show metrics for Temperature")
        print("The warmest Temperature:"+str(data.Temperature[data.Temperature.idxmax()]))
        print("The 5 top")
        print(temp_analysis.tail())
        print("The coldest Temperature" + str(data.Temperature[data.Temperature.idxmin()]))
        print("The 5 lowest")
        print(temp_analysis.head())
        print("Following data show metrics for Temperature")
        print("The higher Humidity" + str(data.Humidity[data.Humidity.idxmax()]))
        print("The 5 top")
        print(humi_analysis.tail())
        print("The 5 lower Humidity" + str(data.Humidity[data.Humidity.idxmin()]))
        print("The 5 lowest")
        print(humi_analysis.head())
        hum=[]
        temp=[]
        flame=[]
        t=np.arange(0,len(data.Humidity),1)
        for i in range(0,len(data.Humidity)):
            hum.append(float(data.Humidity.loc[i]))
            temp.append(float(data.Temperature.loc[i]))
            flame.append(int(data.Flame.loc[i]))
        plt.plot(t,temp)
        plt.xlabel('Seconds(s)')
        plt.ylabel('Temperature(C)')
        plt.title('Temperature Graph')
        plt.savefig('Temperature_Graph')
        plt.grid(True)
        plt.show()
        plt.plot(t,hum)
        plt.xlabel('Seconds(s)')
        plt.ylabel('Humidity(%)')
        plt.title('Humidity Graph')
        plt.savefig('Humidity_Graph')
        plt.grid(True)
        plt.show()
        plt.plot(t,flame)
        plt.xlabel('Seconds(s)')
        plt.ylabel('Flame Pulse')
        plt.title('Flame Detection')
        plt.savefig('Flame_Graph')
        plt.grid(True)
        plt.show()
