# import network
import requests
import time
import os
import argparse

parser = argparse.ArgumentParser(description='Diesel Data Logger')
parser.add_argument("--reps", type=int, default=0)
parser.add_argument("--delay", type=int, default=30)

args = parser.parse_args()
 
repetitions = args.reps
delay = args.delay

processid = str(os.getpid())
print ("Process ID: " + str(os.getpid()))

killpidphp = "<?php exec(\"kill -9 " + processid + "\"); exec(\"rm -f 'killpid.php'\"); exec(\"rm -f 'pid.php'\");?>Killed " + processid + '<p><a href=\"/diesellogger/\">Home</a></p>'
killpidfile = "killpid.php"
with open(killpidfile, 'w') as f:
    print(killpidphp, end="\n", file=f)

pidphp = "<?php echo (" + processid + ");?>"
processidfile = "pid.php"
with open(processidfile, 'w') as f:
    print(pidphp, end="\n", file=f)

# repetitions = int(input("How many repetitions? (0 = indefinite): "))
# delay = int(input("Enter time delay between repetitions in seconds: "))

outfile = "dieseldata.csv"
esp32IP = "192.168.1.98"

def retrieveandsave(i):
    distance = str(requests.get("http://" + esp32IP + "/d/distance").content)
    distance = distance.replace("b","")
    distance = distance.replace("'","")
    distance = str(float(distance))

    motioncount = str(requests.get("http://" + esp32IP + "/d/motioncount").content)
    motioncount = motioncount.replace("b","")
    motioncount = motioncount.replace("'","")
    motioncount = str(int(round(float(motioncount))))

    outstring = (str(time.time()) +","+ distance +","+ motioncount)
    with open(outfile, 'a') as f:
        print(outstring, end="\n", file=f)

    print("Saved line " + str(i))
    time.sleep(delay)

i = 1
while repetitions == 0:
    if i == 1:
        print ("Continuing indefinitely")
    retrieveandsave(i)
    i += 1
else:
    print ("Recording "+str(repetitions)+" events at "+str(delay)+" seconds spacing")
    for j in range(repetitions):
        retrieveandsave(j+1)