
from django.shortcuts import render

from django.http.response import JsonResponse
from rest_framework.response import Response
from rest_framework.parsers import JSONParser 
from rest_framework import status
 
from coordinates.models import Coordinates
from coordinates.serializers import CoordinateSerializer
from rest_framework.decorators import api_view
import subprocess
from subprocess import Popen, PIPE
import pandas as pd
import csv , json
from django.http import HttpResponse


@api_view(['GET','POST'])
def handle_coordinates(request):
    if request.method == 'GET':
        coordinate = Coordinates.objects.all()
         
        coordinate_serializer = CoordinateSerializer(coordinate, many=True)
        return JsonResponse(coordinate_serializer.data, safe=False)
    elif request.method == 'POST':
        # subprocess.call(["g++", "Test.cc"]) # OR gcc for c program
        tutorial_data = JSONParser().parse(request)
        print(tutorial_data["input"])
        p = Popen(['./a.out'], shell=True, stdout=PIPE, stdin=PIPE)


        value = str(tutorial_data["input"]) + '\n'
        value = bytes(value, 'UTF-8')  # Needed in Python 3.
        p.stdin.write(value)
        p.stdin.flush()
        result = p.stdout.readline().strip()
        print(result)
        a_dict = {'output': result}

        csvfile = open('rectangles.csv', 'r')
        jsonfile = open('res.json', 'w')

        fieldnames = ("x1","x2","y1","y2","d")
        reader = csv.DictReader( csvfile)
        
        out = json.dumps(  [ row for row in reader ])
    
        # jsonfile.write(out)
        return JsonResponse(out, safe=False)


 
