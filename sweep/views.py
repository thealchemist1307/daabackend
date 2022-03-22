from django.shortcuts import render

# Create your views here.

from django.shortcuts import render

from django.http.response import JsonResponse
from rest_framework.response import Response
from rest_framework.parsers import JSONParser 
from rest_framework import status
 
from rest_framework.decorators import api_view
import subprocess
from subprocess import Popen, PIPE, check_output
import pandas as pd
import csv , json
from django.http import HttpResponse
import os

@api_view(['GET','POST'])
def handle_coordinates_sweep(request):
    if request.method == 'GET':
        coordinate = Coordinates.objects.all()
        coordinate_serializer = CoordinateSerializer(coordinate, many=True)
        return JsonResponse(coordinate_serializer.data, safe=False)
    elif request.method == 'POST':
        # subprocess.call(["g++", "Test.cc"]) # OR gcc for c program
        tutorial_data = JSONParser().parse(request)
        value = tutorial_data["input"]
        value = str(value)
        #print("Done")
        print(tutorial_data["input"])
        data, temp = os.pipe()
        os.write(temp, bytes(value, "utf-8"))
        os.close(temp) 
        p = check_output("sweep.exe", shell=True, stdin=data)
        
        #value = str(tutorial_data["input"]) + '\n'
        #value = bytes(value, 'UTF-8')  # Needed in Python 3.
        #p.stdin.write(value)
        #p.stdin.flush()
        #result = p.stdout.readline().strip()
        #output=str(result)


        csvfile = open('Lines.csv', 'r')        
        reader = csv.DictReader(csvfile)
        array=[]
        #result = {}
        for row in reader:
            array.append(row)

        # p = Popen(['./contour.out'], shell=True, stdout=PIPE, stdin=PIPE)

        # value = str(tutorial_data["input"]) + '\n'
        # value = bytes(value, 'UTF-8')  # Needed in Python 3.
        # p.stdin.write(value)
        # p.stdin.flush()
        # result = p.stdout.readline().strip()

        intersectcsv = open('Points.csv', 'r')
        
        intersect = csv.DictReader(intersectcsv)
        iarray=[]
        for row in intersect:
            iarray.append(row)

        
        dict={"coords":array, "output":iarray}
        out = json.dumps(  dict )

        # jsonfile.write(out)
        return JsonResponse(out, safe=False)


 
