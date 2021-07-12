import matplotlib.pyplot as plt
import pandas as pd
import pathlib
from datetime import datetime
import sys
import time

try:
    # Load data
    filenameSFC = sys.argv[1]
    infileSFC = './SFCDATA/' + filenameSFC + '.csv'
    dataSFC = pd.read_csv(infileSFC)
    filenameSlow = sys.argv[2]
    infileSlow = './SlowDATA/' + filenameSlow + '.csv'
    dataSlow = pd.read_csv(infileSlow)
    timedur = -int(sys.argv[3])
    sl = slice(timedur,None)
    mode = sys.argv[4]
    plt.ioff()
    plt.rcParams["font.size"] = 20
    while True:
        # SFC plot        
        # Get last-modified datetime
        p = pathlib.Path(infileSFC)
        last_modified = datetime.fromtimestamp(p.stat().st_mtime).strftime("%Y/%m/%d %H:%M:%S")
            
        if mode == "SFC":
            figSFC = plt.figure(figsize=(20,17))
            axSC = figSFC.add_subplot(3,1,1)
            axSV = figSFC.add_subplot(3,1,2)
            axSP = figSFC.add_subplot(3,1,3)
            axSC.set_xlabel('Time (s)')
            axSV.set_xlabel('Time (s)')
            axSP.set_xlabel('Time (s)')
            axSC.set_ylabel('Current (nA)')
            axSC.set_yscale('log')
            axSV.set_ylabel('Voltage (V)')
            axSP.set_ylabel('Pressure (Pa)')
            axSP.set_yscale('log')
            figSFC.tight_layout()
            plt.grid(which='major',color='black',linestyle='-')
            plt.grid(which='minor',color='black',linestyle='dotted')
            axSC.set_title('SFCDATA Log (Last Update: %s)'%(last_modified))
            axSC.plot(data['Time (s)'][sl], data['Current'][sl],'-',label='Current')
            axSC.legend(bbox_to_anchor=(1.02,1),loc='upper left',borderaxespad=0,fontsize=14)
            axSV.plot(data['Time (s)'][sl], data['FC up'][sl],'-',label='FC up')
            axSV.plot(data['Time (s)'][sl], data['FC center'][sl],'-',label='FC center')
            axSV.plot(data['Time (s)'][sl], data['FC down'][sl],'-',label='FC down')
            axSV.plot(data['Time (s)'][sl], data['Au target'][sl],'-',label='VA')
            axSV.plot(data['Time (s)'][sl], data['Takefune Left'][sl],'-',label='VCL')
            axSV.plot(data['Time (s)'][sl], data['Takefune Right'][sl],'-',label='VCR')
            axSV.plot(data['Time (s)'][sl], data['Extraction'][sl],'-',label='Extraction')
            axSV.plot(data['Time (s)'][sl], data['Deflection Up'][sl],'-',label='Deflection Up')
            axSV.plot(data['Time (s)'][sl], data['Deflection Down'][sl],'-',label='Deflection Down')
            axSV.plot(data['Time (s)'][sl], data['Drift 1'][sl],'-',label='Drift 1')
            axSV.plot(data['Time (s)'][sl], data['Einzel Lens'][sl],'-',label='Einzel Lens')
            axSV.plot(data['Time (s)'][sl], data['Drift 2'][sl],'-',label='Drift 2')
            axSV.legend(bbox_to_anchor=(1.02,1),loc='upper left',borderaxespad=0,fontsize=14)
            axSP.plot(data['Time (s)'][sl], data['CH1'][sl],'-',label='CH1')
            axSP.plot(data['Time (s)'][sl], data['CH2'][sl],'-',label='CH2')
            axSP.legend(bbox_to_anchor=(1.02,1),loc='upper left',borderaxespad=0,fontsize=14)
            plt.yscale('log')
        elif mode == "PBT":
            figSFC = plt.figure(figsize=(20,17))
            axSC = figSFC.add_subplot(3,1,1)
            axSV = figSFC.add_subplot(3,1,2)
            axSP = figSFC.add_subplot(3,1,3)
            axSC.set_xlabel('Time (s)')
            axSV.set_xlabel('Time (s)')
            axSP.set_xlabel('Time (s)')
            axSC.set_ylabel('Current (nA)')
            axSC.set_yscale('log')
            axSV.set_ylabel('Voltage (V)')
            axSP.set_ylabel('Pressure (Pa)')
            axSP.set_yscale('log')
            figSFC.tight_layout()
            plt.grid(which='major',color='black',linestyle='-')
            plt.grid(which='minor',color='black',linestyle='dotted')
            axSC.set_title('Primary Beam Tuning Log (Last Update: %s)'%(last_modified))
            axSC.plot(data['Time (s)'][sl], data['Current'][sl],'-',label='Current')
            axSC.legend(bbox_to_anchor=(1.02,1),loc='upper left',borderaxespad=0,fontsize=14)
            axSP.plot(data['Time (s)'][sl], data['CH1'][sl],'-',label='CH1')
            axSP.plot(data['Time (s)'][sl], data['CH2'][sl],'-',label='CH2')
            axSP.legend(bbox_to_anchor=(1.02,1),loc='upper left',borderaxespad=0,fontsize=14)
            plt.yscale('log')
        elif mode == "VAC":
            figSFC = plt.figure(figsize=(17,10))
            plt.title('SFCDATA Vacuum Gauge (Last Update: %s)'%(last_modified))
            plt.plot(data['Time (s)'][sl], data['C1 - CH 1 (Pa)'][sl],'-',label='CH 1')
            plt.plot(data['Time (s)'][sl], data['C1 - CH 2 (Pa)'][sl],'-',label='CH 2')
            plt.legend(bbox_to_anchor=(1.02,1),loc='upper left',borderaxespad=0,fontsize=14)
            plt.xlabel('Time (s)')
            plt.ylabel('Pressure (Pa)')
            plt.grid(which='major',color='black',linestyle='-')
            plt.grid(which='minor',color='black',linestyle='dotted')
            plt.yscale('log')

        # Draw and save figure
        figSFC.canvas.draw()
        figSFC.savefig('./SFCDATA/'+filenameSFC+'.png',dpi=300,bbox_inches="tight")
        figSFC.canvas.flush_events()
        figSFC.clear()
        plt.clf()
        plt.close(figSFC)

        # Slow plot
        # Get last-modified datetime
        p = pathlib.Path(infileSlow)
        last_modified = datetime.fromtimestamp(p.stat().st_mtime).strftime("%Y/%m/%d %H:%M:%S")
        
        # Plot
        figSlow = plt.figure(figsize=(17,10))
        plt.title('SlowDATA Vacuum Gauge (Last Update: %s)'%(last_modified))
        plt.plot(data['Time (s)'][sl], data['C1 - CH 1 (Pa)'][sl],'-',label='CH 1')
        plt.plot(data['Time (s)'][sl], data['C1 - CH 2 (Pa)'][sl],'-',label='CH 2')
        plt.legend(bbox_to_anchor=(1.02,1),loc='upper left',borderaxespad=0,fontsize=14)
        plt.xlabel('Time (s)')
        plt.ylabel('Pressure (Pa)')
        plt.grid(which='major',color='black',linestyle='-')
        plt.grid(which='minor',color='black',linestyle='dotted')
        plt.yscale('log')
       
        # Draw and save figure
        figSlow.canvas.draw()
        figSlow.savefig('./SlowDATA/'+filenameSlow+'.png',dpi=100,bbox_inches="tight")
        figSlow.canvas.flush_events()
        figSlow.clear()
        plt.clf()
        plt.close(figSlow)

        # Wait for 5 seconds
        print("\r",end='')
        print('Plotting data; LAST UPDATE: %s (Type Ctrl+C to finish...)'%(datetime.now()),end='',flush=True)
        time.sleep(5)

except KeyboardInterrupt:
    print('\nFINISHED PLOT UPDATE')